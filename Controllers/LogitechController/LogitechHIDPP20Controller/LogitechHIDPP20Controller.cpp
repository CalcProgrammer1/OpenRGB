/*---------------------------------------------------------*\
| LogitechHIDPP20Controller.cpp                             |
|                                                           |
|   Unified Logitech HID++ 2.0 controller implementation    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <thread>
#include <chrono>
#include <set>
#include <vector>
#include <map>
#include "LogitechHIDPP20Controller.h"
#include "LogitechHIDPP20ReceiverWatcher.h"
#include "RGBController_LogitechHIDPP20.h"
#include "LogManager.h"

#include "LogitechHIDPP20IdleSettings.h"


#define LOG_TAG log_tag.c_str()

/*---------------------------------------------------------*\
| Hard cap on per-call non-HID++ drains in the read loop. A |
| high-polling-rate mouse can put 50+ input reports in the  |
| buffer between our reads; this cap prevents pathological  |
| input-flood scenarios from locking up a single read call. |
| 64 is enough headroom for normal congestion at 1 kHz.     |
\*---------------------------------------------------------*/
static const int HIDPP20_READ_DRAIN_BUDGET = 64;

/*---------------------------------------------------------*\
| Per-candidate read timeout (ms) for the Centurion 0x50    |
| device-address probe. USB round-trip is <1ms; 5ms gives   |
| 5x margin. Worst case (no device responds) 256 x 5 =      |
| ~1.3s; typical G522 at addr 0x23 is ~180ms. Matches       |
| Solaar's probe_centurion_device_addr constant.            |
\*---------------------------------------------------------*/
static const int CENTURION_PROBE_PER_ADDR_TIMEOUT_MS = 5;

/*---------------------------------------------------------*\
| Device-name helpers. A placeholder is empty or one of the |
| HIDPP20_NAME_PLACEHOLDER_* strings. A name "looks real"   |
| when it is non-empty, printable ASCII, and a sane length. |
| This rejects 0x0101 firmware-data responses that are      |
| not an actual name.                                       |
\*---------------------------------------------------------*/
static bool HIDPP20NameIsPlaceholder(const std::string& n)
{
    return n.empty()
        || n == HIDPP20_NAME_PLACEHOLDER_STD
        || n == HIDPP20_NAME_PLACEHOLDER_CENTURION;
}

bool LogitechHIDPP20Controller::NameLooksReal(const std::string& n)
{
    if(n.empty() || n.size() > 64)
    {
        return false;
    }

    for(unsigned char c : n)
    {
        if(c < 0x20 || c > 0x7E)
        {
            return false;
        }
    }

    return true;
}

/*---------------------------------------------------------*\
| Observed HID++ 2.0 feature versions. Each row is          |
| a feature ID plus the versions we've empirically          |
| verified working. When feature discovery reports          |
| a version outside this set, we log a one-shot             |
| INFO tripwire so a tester with new hardware               |
| immediately surfaces unknown firmware revs.               |
|                                                           |
| Purely observational, no behavior branches on             |
| version. Solaar has effectively zero version              |
| gating for the RGB features we implement, so we           |
| don't either; the table is a "have we seen this           |
| combination work" ledger, not a compatibility             |
| matrix. Add versions as devices report them.              |
|                                                           |
| A feature_id absent from this table is silent             |
| (no tripwire). Only features we actually                  |
| exercise are worth flagging.                              |
\*---------------------------------------------------------*/
struct HIDPP20FeatureVersionSet
{
    uint16_t    feature_id;
    uint8_t     versions[8];        /* approved versions; first `count` valid */
    uint8_t     count;
};

static constexpr HIDPP20FeatureVersionSet HIDPP20_FEATURE_OBSERVED_VERSIONS[] =
{
    { 0x0620, { 1 }, 1 },
    { 0x1D4B, { 0 }, 1 },
    { 0x4540, { 0, 1 }, 2 },
    { 0x8070, { 0, 3, 5 }, 3 },
    { 0x8071, { 0, 4 }, 2 },
    { 0x8080, { 0 }, 1 },
    { 0x8081, { 0, 2 }, 2 },
};

/*---------------------------------------------------------*\
| Returns true if feature_id is not tracked (silent) or if  |
| version appears in the tracked feature's approved set.    |
\*---------------------------------------------------------*/
static bool FeatureVersionIsObserved(uint16_t feature_id, uint8_t version)
{
    size_t table_len = sizeof(HIDPP20_FEATURE_OBSERVED_VERSIONS)
                     / sizeof(HIDPP20_FEATURE_OBSERVED_VERSIONS[0]);

    for(size_t r = 0; r < table_len; r++)
    {
        const HIDPP20FeatureVersionSet& row = HIDPP20_FEATURE_OBSERVED_VERSIONS[r];

        if(row.feature_id != feature_id)
        {
            continue;
        }

        for(uint8_t i = 0; i < row.count; i++)
        {
            if(row.versions[i] == version)
            {
                return true;
            }
        }

        return false;   /* tracked feature, unknown version */
    }

    return true;        /* feature not tracked, silent */
}

LogitechHIDPP20Controller::LogitechHIDPP20Controller
    (
    hid_device*                 dev,
    const char*                 path,
    uint8_t                     device_index,
    bool                        wireless,
    std::shared_ptr<std::mutex> mutex_ptr,
    uint16_t                    usage_page,
    hid_device*                 perkey_vl_dev
    )
{
    this->dev           = dev;
    this->dev_perkey_vl = perkey_vl_dev;
    this->location      = path;
    this->device_index  = device_index;
    this->wireless      = wireless;
    this->mutex         = mutex_ptr;
    this->long_only     = false;

    this->teardown_pending = false;

    this->initialized                  = false;
    this->sw_control_claimed           = false;
    this->last_fap_error_              = 0;
    this->pipelining_claim_            = false;
    this->discovery_in_progress_.store(false);
    this->sw_control_needs_upgrade_to_5 = false;
    this->prep_applied                 = false;
    this->frame_counter                = 0;
    this->retry_paint_deadline_.store(std::chrono::steady_clock::time_point{});
    this->retry_paint_attempt_.store(0);
    this->wake_full_repaint_pending_.store(false);
    this->init_generation      = 0;
    this->log_tag              = "[LogitechHID++ " + std::string(path) + "]";
    this->reader_thread        = nullptr;
    this->reader_running       = false;
    this->power_thread         = nullptr;
    this->power_thread_running = false;
    this->pending_activity     = -1;
    this->pending_connection   = 0;
    this->device_online        = true;
    this->consecutive_timeouts = 0;
    this->power_state          = HIDPP20_POWER_ACTIVE;
    this->deep_sleep           = false;
    this->consecutive_frame_end_failures = 0;
    this->dim_brightness_pct   = 100;
    this->dim_step             = 0;
    this->idle_timeout_s       = 60;
    this->sleep_timeout_s      = 300;

    caps = {};

    /*-----------------------------------------------------*\
    | Default to standard HID++ transport;                  |
    | DiscoverTransport() may change this during            |
    | Probe() if Centurion is detected.                     |
    \*-----------------------------------------------------*/
    transport.type              = HIDPP20_TRANSPORT_STANDARD;
    transport.usage_page        = usage_page;
    transport.report_id         = LOGITECH_LONG_MESSAGE;
    transport.addressed         = false;
    transport.device_address    = 0x00;
    transport.bridge_feat_idx   = 0;
    transport.sub_device_id     = 0;
    transport.bridge_mtu        = 0;
}

LogitechHIDPP20Controller::~LogitechHIDPP20Controller()
{
    /*-----------------------------------------------------*\
    | Waits, retries and backoff sleeps check this and      |
    | return rather than run their course, so teardown      |
    | takes milliseconds.                                   |
    \*-----------------------------------------------------*/
    teardown_pending.store(true);

    /*-----------------------------------------------------*\
    | Deregister from the node watcher first, under its     |
    | lock: after this returns no nudge can reach this      |
    | controller.                                           |
    \*-----------------------------------------------------*/
    LogitechHIDPP20ReceiverWatcher::UnregisterSubDevice(this);

    if(initialized)
    {
        Shutdown();
    }

    /*-----------------------------------------------------*\
    | Event-watcher controllers run reader/power            |
    | threads without ever initializing;                    |
    | Shutdown() won't have stopped them.                   |
    | Idempotent, so safe to call unconditionally.          |
    \*-----------------------------------------------------*/
    StopSenderThread();
    StopPowerManager();

    if(dev)
    {
        hid_close(dev);
    }

    if(dev_perkey_vl && dev_perkey_vl != dev)
    {
        hid_close(dev_perkey_vl);
    }
}

/*---------------------------------------------------------*\
| Transport-layer I/O                                       |
|                                                           |
| SendMessage/ReadMessage dispatch to the appropriate       |
| transport implementation based on transport.type.         |
| SendAndReceive is a convenience wrapper.                  |
\*---------------------------------------------------------*/

int LogitechHIDPP20Controller::SendMessage
    (
    uint8_t         feat_idx,
    uint8_t         function,
    const uint8_t*  data,
    size_t          len
    )
{
    switch(transport.type)
    {
        case HIDPP20_TRANSPORT_CENTURION:
            return SendCenturion(feat_idx, function, data, len);

        case HIDPP20_TRANSPORT_STANDARD:
        default:
            return SendStandard(feat_idx, function, data, len);
    }
}

int LogitechHIDPP20Controller::ReadMessage
    (
    uint8_t*    feat_idx_out,
    uint8_t*    function_out,
    uint8_t*    data_out,
    size_t      data_max,
    int         timeout_ms
    )
{
    /*-----------------------------------------------------*\
    | When the reader thread is running, it is the sole     |
    | caller of hid_read_timeout. All other reads come from |
    | the queue. Before the reader starts (during           |
    | Probe/Initialize), read directly from HID.            |
    \*-----------------------------------------------------*/
    if(reader_running.load())
    {
        return ReadFromQueue(feat_idx_out, function_out, data_out, data_max, timeout_ms);
    }

    return ReadHIDDirect(feat_idx_out, function_out, data_out, data_max, timeout_ms);
}

int LogitechHIDPP20Controller::ReadHIDDirect
    (
    uint8_t*    feat_idx_out,
    uint8_t*    function_out,
    uint8_t*    data_out,
    size_t      data_max,
    int         timeout_ms
    )
{
    switch(transport.type)
    {
        case HIDPP20_TRANSPORT_CENTURION:
            return ReadCenturionDirect(feat_idx_out, function_out, data_out, data_max, timeout_ms);

        case HIDPP20_TRANSPORT_STANDARD:
        default:
            return ReadStandardDirect(feat_idx_out, function_out, data_out, data_max, timeout_ms);
    }
}

int LogitechHIDPP20Controller::ReadFromQueue
    (
    uint8_t*    feat_idx_out,
    uint8_t*    function_out,
    uint8_t*    data_out,
    size_t      data_max,
    int         timeout_ms
    )
{
    std::unique_lock<std::mutex> lock(response_mutex);

    std::chrono::steady_clock::time_point deadline = std::chrono::steady_clock::now()
                                                    + std::chrono::milliseconds(timeout_ms);

    while(response_queue.empty())
    {
        if(response_cv.wait_until(lock, deadline) == std::cv_status::timeout)
        {
            /*---------------------------------------------*\
            | Offline detection lives at the                |
            | SendAcked layer now: one tick per             |
            | fully-failed call, not per per-attempt        |
            | read window. Streaming policies that          |
            | retry several times don't artificially        |
            | accelerate the offline declaration.           |
            \*---------------------------------------------*/
            return 0;
        }

        if(!reader_running.load())
        {
            return 0;
        }

        /*-------------------------------------------------*\
        | Teardown notifies this cv so blocked readers      |
        | return immediately instead of waiting out their   |
        | read windows on a device being torn down.         |
        \*-------------------------------------------------*/
        if(teardown_pending.load())
        {
            return 0;
        }
    }

    HIDPP20RawMessage msg = response_queue.front();
    response_queue.pop_front();

    if(feat_idx_out)
    {
        *feat_idx_out = msg.feat;
    }

    if(function_out)
    {
        *function_out = msg.func;
    }

    if(data_out && data_max > 0)
    {
        size_t copy_len = (data_max > sizeof(msg.data)) ? sizeof(msg.data) : data_max;
        memcpy(data_out, msg.data, copy_len);
    }

    return msg.result;
}

/*---------------------------------------------------------*\
| Sleep delay_ms in slices, waking early when the link is   |
| about to change or the device went offline. Returns false |
| when interrupted.                                         |
\*---------------------------------------------------------*/
bool LogitechHIDPP20Controller::InterruptibleBackoff(uint16_t delay_ms)
{
    std::chrono::steady_clock::time_point deadline =
        std::chrono::steady_clock::now() + std::chrono::milliseconds(delay_ms);

    while(true)
    {
        if(teardown_pending.load() || !device_online.load())
        {
            return false;
        }

        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

        if(now >= deadline)
        {
            return true;
        }

        int remaining = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                              deadline - now).count();

        std::this_thread::sleep_for(
            std::chrono::milliseconds(remaining < 50 ? remaining : 50));
    }
}

int LogitechHIDPP20Controller::SendAndReceive
    (
    uint8_t         feat_idx,
    uint8_t         function,
    const uint8_t*  send_data,
    size_t          send_len,
    uint8_t*        recv_data,
    size_t          recv_max
    )
{
    /*-----------------------------------------------------*\
    | Thin wrapper around SendAcked with the reliable       |
    | policy. Preserved as a named entry point so           |
    | existing call sites don't need to be touched.         |
    \*-----------------------------------------------------*/
    return SendAcked(feat_idx, function,
                     send_data, send_len,
                     recv_data, recv_max,
                     HIDPP20_POLICY_RELIABLE);
}

int LogitechHIDPP20Controller::SendAcked
    (
    uint8_t                 feat_idx,
    uint8_t                 function,
    const uint8_t*          send_data,
    size_t                  send_len,
    uint8_t*                recv_data,
    size_t                  recv_max,
    const HIDPP20RetryPolicy& policy,
    uint8_t*                hidpp20_error_out
    )
{
    /*-----------------------------------------------------*\
    | Send-and-ack with policy-driven retry: 7-attempt      |
    | exponential backoff for reliable one-shot             |
    | commands, tight 2-attempt for streaming frames.       |
    | Per attempt: sleep backoff_ms[i]; bail if             |
    | offline; send (wire errors retry); read within        |
    | read_window_ms:                                       |
    |  matching response                  -> success        |
    |  our error: BUSY and retry_on_busy  -> resend         |
    |             any other code          -> fail (-1)      |
    |  foreign error / non-matching frame -> discard        |
    |  read timeout                       -> resend         |
    |                                                       |
    | The flush below empties the shared response queue,    |
    | so this call must be the only transaction on the      |
    | wire while it runs. See TransactionMutex().           |
    \*-----------------------------------------------------*/
    std::lock_guard<std::recursive_mutex> transaction_guard(transaction_mutex);

    if(hidpp20_error_out)
    {
        *hidpp20_error_out = 0;
    }

    if(policy.flush_before)
    {
        FlushResponseQueue();
    }

    int     last_result = 0;
    uint8_t last_error  = 0;

    for(uint8_t attempt = 0; attempt < policy.attempts; attempt++)
    {
        /*-------------------------------------------------*\
        | Backoff before each attempt (0 on first).         |
        | Sliced so teardown interrupts the wait, the       |
        | checks below then end the call.                   |
        \*-------------------------------------------------*/
        uint16_t delay_ms = policy.backoff_ms[attempt];

        if(delay_ms > 0)
        {
            InterruptibleBackoff(delay_ms);
        }

        /*-------------------------------------------------*\
        | Bail early if device went offline mid-retry       |
        \*-------------------------------------------------*/
        if(!device_online.load())
        {
            return 0;
        }

        /*-------------------------------------------------*\
        | Bail if teardown started: reliable-policy         |
        | retries against a device being torn down delay    |
        | shutdown by seconds.                              |
        \*-------------------------------------------------*/
        if(teardown_pending.load())
        {
            LOG_TRACE("%s SendAcked[%s] abandoned, link change pending",
                      LOG_TAG, policy.name);
            return 0;
        }

        int send_result = SendMessage(feat_idx, function, send_data, send_len);

        if(send_result < 0)
        {
            LOG_DEBUG("%s SendAcked[%s] wire send failed (attempt %d, result=%d) "
                      "feat=0x%02X func=0x%02X",
                      LOG_TAG, policy.name, attempt, send_result, feat_idx, function);
            last_result = -2;
            continue;
        }

        /*-------------------------------------------------*\
        | Read loop bounded by per-attempt window.          |
        | Drain non-matching HID++ frames within            |
        | this window; they are stale responses or          |
        | unrelated events from prior commands. Only        |
        | retry the send if the window expires with         |
        | no match (lost on wire) or we got BUSY.           |
        \*-------------------------------------------------*/
        std::chrono::steady_clock::time_point window_deadline = std::chrono::steady_clock::now()
                                                               + std::chrono::milliseconds(policy.read_window_ms);
        bool need_resend = false;

        while(!need_resend)
        {
            if(teardown_pending.load())
            {
                LOG_TRACE("%s SendAcked[%s] abandoned, link change pending",
                          LOG_TAG, policy.name);
                return 0;
            }

            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

            if(now >= window_deadline)
            {
                LOG_TRACE("%s SendAcked[%s] window expired (attempt %d)",
                          LOG_TAG, policy.name, attempt);
                last_result = 0;
                break;
            }

            int remaining = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                                  window_deadline - now).count();

            if(remaining <= 0)
            {
                last_result = 0;
                break;
            }

            uint8_t resp_feat = 0;
            uint8_t resp_func = 0;
            uint8_t resp_data[60] = {};

            int rd = ReadMessage(&resp_feat, &resp_func,
                                 resp_data, sizeof(resp_data),
                                 remaining);

            if(rd < 0)
            {
                /* Wire error, propagate, don't retry */
                return -2;
            }

            if(rd == 0)
            {
                /* Window drained with nothing matching, retry the send */
                last_result = 0;
                break;
            }

            /*---------------------------------------------*\
            | HID++ error frame                             |
            | feat=0xFF, func=err_feat, data[0]=err_func,   |
            | data[1]=err_code                              |
            \*---------------------------------------------*/
            if(resp_feat == 0xFF)
            {
                uint8_t err_feat = resp_func;
                uint8_t err_func = resp_data[0];
                uint8_t err_code = resp_data[1];

                /*-----------------------------------------*\
                | Match: either a direct error for our      |
                | request, or a Centurion bridge error      |
                | attributed to the bridge feature index    |
                | when we're routing through it. The bridge |
                | swallows the sub-device feat in the error |
                | response, so all bridge-routed failures   |
                | look like errors from the bridge.         |
                \*-----------------------------------------*/
                bool is_our_error =
                    (err_feat == feat_idx &&
                     (err_func & 0xF0) == (function & 0xF0) &&
                     (err_func & 0x0F) == HIDPP20_SW_ID)
                 || (transport.bridge_feat_idx != 0 &&
                     err_feat == transport.bridge_feat_idx);

                if(is_our_error)
                {
                    if(err_code == 0x08 && policy.retry_on_busy)
                    {
                        /* BUSY: retry the send after backoff */
                        LOG_TRACE("%s SendAcked[%s] BUSY (attempt %d) feat=0x%02X func=0x%02X",
                                  LOG_TAG, policy.name, attempt, feat_idx, function);
                        last_error  = 0x08;
                        last_result = 0;
                        need_resend = true;
                        continue;
                    }

                    /* Non-BUSY HID++ error: hard fail */
                    LOG_DEBUG("%s SendAcked[%s] LogitechHID++ error 0x%02X "
                              "feat=0x%02X func=0x%02X",
                              LOG_TAG, policy.name, err_code, feat_idx, function);

                    /*-------------------------------------*\
                    | Kept for the cache self-heal:         |
                    | 0x06/0x07 is a stale restored index.  |
                    \*-------------------------------------*/
                    last_fap_error_ = err_code;

                    if(hidpp20_error_out)
                    {
                        *hidpp20_error_out = err_code;
                    }

                    return -1;
                }

                /* Error for a different request, stale, discard and keep reading */
                continue;
            }

            /*---------------------------------------------*\
            | Match the software id too; it is the          |
            | only thing tying a HID++ reply to its         |
            | requester. Without it, replies to any         |
            | other program on this device (Solaar          |
            | cycles 0x02..0x0F) are accepted as            |
            | ours: same feature, same function.            |
            \*---------------------------------------------*/
            if(resp_feat == feat_idx &&
               (resp_func & 0xF0) == (function & 0xF0) &&
               (resp_func & 0x0F) == HIDPP20_SW_ID)
            {
                if(recv_data && recv_max > 0)
                {
                    size_t copy = (recv_max > sizeof(resp_data))
                                ? sizeof(resp_data) : recv_max;
                    memcpy(recv_data, resp_data, copy);
                }

                if(attempt > 0)
                {
                    LOG_DEBUG("%s SendAcked[%s] succeeded on attempt %d "
                              "feat=0x%02X func=0x%02X",
                              LOG_TAG, policy.name, attempt, feat_idx, function);
                }

                consecutive_timeouts.store(0);
                return rd;
            }

            /* Non-matching, non-error: stale unrelated frame, keep reading */
        }
    }

    LOG_DEBUG("%s SendAcked[%s] exhausted %d attempts feat=0x%02X func=0x%02X "
              "(last_error=0x%02X)",
              LOG_TAG, policy.name, (int)policy.attempts,
              feat_idx, function, last_error);

    if(hidpp20_error_out)
    {
        *hidpp20_error_out = last_error;
    }

    /*-----------------------------------------------------*\
    | Offline detection: tick once per fully-failed call    |
    | (all retry attempts exhausted with no response). At a |
    | threshold of 10 we declare the device gone. Reset to  |
    | 0 happens on any successful call (above), single      |
    | delayed responses don't push us toward offline.       |
    \*-----------------------------------------------------*/
    if(last_result == 0)
    {
        int timeouts = ++consecutive_timeouts;

        if(timeouts >= 10 && device_online.load())
        {
            LOG_DEBUG("%s Device appears offline (%d consecutive failed calls)",
                      LOG_TAG, timeouts);
            device_online.store(false);
        }
    }

    return last_result;
}

int LogitechHIDPP20Controller::SendAckedIntoFAP
    (
    uint8_t                 feat_idx,
    uint8_t                 function,
    const uint8_t*          send_data,
    size_t                  send_len,
    blankFAPmessage&        response,
    const HIDPP20RetryPolicy& policy
    )
{
    /*-----------------------------------------------------*\
    | Compatibility shim for callers that inherited         |
    | the SendLong+ReadResponse interface and               |
    | inspect response.data[] downstream. Calls             |
    | SendAcked into a local buffer, then                   |
    | reconstructs a blankFAPmessage on success.            |
    \*-----------------------------------------------------*/
    response.init();

    /*-----------------------------------------------------*\
    | Pipelined claim: fire the write, record it, don't     |
    | block. VerifyClaimPipeline reconciles and re-sends    |
    | any miss; claim commands ignore their response.       |
    \*-----------------------------------------------------*/
    if(pipelining_claim_)
    {
        int wr = SendMessage(feat_idx, function, send_data, send_len);

        if(wr > 0 && claim_pipeline_.size() < 16)
        {
            HIDPP20PendingClaimCmd cmd;
            cmd.feat = feat_idx;
            cmd.func = function;
            cmd.len  = (send_len > sizeof(cmd.data)) ? sizeof(cmd.data) : send_len;
            if(send_data != nullptr && cmd.len > 0)
            {
                memcpy(cmd.data, send_data, cmd.len);
            }
            claim_pipeline_.push_back(cmd);
        }

        return wr;
    }

    uint8_t recv[60] = {};
    int     result   = SendAcked(feat_idx, function,
                                 send_data, send_len,
                                 recv, sizeof(recv),
                                 policy);

    if(result > 0)
    {
        response.report_id       = LOGITECH_LONG_MESSAGE;
        response.device_index    = device_index;
        response.feature_index   = feat_idx;
        response.feature_command = function;
        memcpy(response.data, recv, sizeof(response.data));
    }

    return result;
}

/*---------------------------------------------------------*\
| Standard HID++ transport (0xFF00 / 0xFF43)                |
| Report IDs 0x10 (7 bytes) / 0x11 (20 bytes)               |
\*---------------------------------------------------------*/

/*---------------------------------------------------------*\
| Outgoing frame as hex, for trace-level wire comparison.   |
\*---------------------------------------------------------*/
static std::string hex_frame(const uint8_t* buf, size_t len)
{
    std::string out;
    char        byte[4];

    for(size_t i = 0; i < len; i++)
    {
        snprintf(byte, sizeof(byte), "%02X ", buf[i]);
        out += byte;
    }

    return out;
}

int LogitechHIDPP20Controller::SendStandard
    (
    uint8_t         feat_idx,
    uint8_t         function,
    const uint8_t*  data,
    size_t          len
    )
{
    /*-----------------------------------------------------*\
    | Auto-select short (0x10, 7 bytes) vs long             |
    | (0x11, 20 bytes) based on data length.                |
    | Upper layers just provide data; transport             |
    | picks the smallest frame that fits.                   |
    |                                                       |
    | Windows exception: HIDClass splits the HID++ short    |
    | and long message Top-Level Collections into separate  |
    | virtual HID devices (page 0xFF00 usage 1 vs usage 2). |
    | We open the long- message TLC, which rejects 7-byte   |
    | writes. Force long format on Windows so every         |
    | outgoing frame matches the collection we opened,      |
    | Linux hidraw and macOS IOHIDManager expose both TLCs  |
    | through one handle and keep the size-based heuristic. |
    \*-----------------------------------------------------*/
    uint8_t buf[LOGITECH_LONG_MESSAGE_LEN];
    size_t  msg_len;

#if defined(_WIN32)
    const bool prefer_short = false;
#else
    const bool prefer_short = (len <= 3) && !long_only.load();
#endif

    if(prefer_short)
    {
        memset(buf, 0, LOGITECH_SHORT_MESSAGE_LEN);
        buf[0] = LOGITECH_SHORT_MESSAGE;
        buf[1] = device_index;
        buf[2] = feat_idx;
        buf[3] = function | HIDPP20_SW_ID;

        if(data && len > 0)
        {
            memcpy(buf + 4, data, len);
        }

        msg_len = LOGITECH_SHORT_MESSAGE_LEN;
    }
    else
    {
        memset(buf, 0, LOGITECH_LONG_MESSAGE_LEN);
        buf[0] = LOGITECH_LONG_MESSAGE;
        buf[1] = device_index;
        buf[2] = feat_idx;
        buf[3] = function | HIDPP20_SW_ID;

        if(data && len > 0)
        {
            size_t copy_len = (len > 16) ? 16 : len;
            memcpy(buf + 4, data, copy_len);
        }

        msg_len = LOGITECH_LONG_MESSAGE_LEN;
    }

    int result;

    if(mutex)
    {
        std::lock_guard<std::mutex> lock(*mutex);
        result = hid_write(dev, buf, msg_len);
    }
    else
    {
        result = hid_write(dev, buf, msg_len);
    }

    if(LogManager::get()->GetLogLevel() >= LL_TRACE)
    {
        LOG_TRACE("%s TX %s(result=%d)", LOG_TAG, hex_frame(buf, msg_len).c_str(), result);
    }

    /*-----------------------------------------------------*\
    | A collection with no short report rejects the 0x10    |
    | write (G560, G933). Resend as long and stay long.     |
    \*-----------------------------------------------------*/
    if(result < 0 && msg_len == LOGITECH_SHORT_MESSAGE_LEN)
    {
        memset(buf, 0, LOGITECH_LONG_MESSAGE_LEN);
        buf[0] = LOGITECH_LONG_MESSAGE;
        buf[1] = device_index;
        buf[2] = feat_idx;
        buf[3] = function | HIDPP20_SW_ID;

        if(data && len > 0)
        {
            memcpy(buf + 4, data, len);
        }

        if(mutex)
        {
            std::lock_guard<std::mutex> lock(*mutex);
            result = hid_write(dev, buf, LOGITECH_LONG_MESSAGE_LEN);
        }
        else
        {
            result = hid_write(dev, buf, LOGITECH_LONG_MESSAGE_LEN);
        }

        if(LogManager::get()->GetLogLevel() >= LL_TRACE)
        {
            LOG_TRACE("%s TX %s(result=%d)", LOG_TAG, hex_frame(buf, LOGITECH_LONG_MESSAGE_LEN).c_str(), result);
        }

        if(result >= 0)
        {
            LOG_DEBUG("%s Short report rejected, using long frames", LOG_TAG);
            long_only.store(true);
        }
    }

    return result;
}

/*---------------------------------------------------------*\
| Feature 0x8080 very-long (report 0x12) frame: fn3         |
| SetKeyColors rides a 64-byte report that Windows HIDClass |
| splits onto a second Top-Level-Collection (usage 0x0604); |
| that handle is dev_perkey_vl; everything else stays on    |
| dev. Layout matches the long report, just wider.          |
| Fire-and-forget: the ACK lands on dev.                    |
\*---------------------------------------------------------*/
int LogitechHIDPP20Controller::SendVeryLongFrame
    (
    uint8_t         feat_idx,
    uint8_t         function,
    const uint8_t*  data,
    size_t          len
    )
{
    const size_t max_payload = LOGITECH_VERY_LONG_MESSAGE_LEN - 4;

    if(dev_perkey_vl == nullptr)
    {
        return -1;
    }

    uint8_t buf[LOGITECH_VERY_LONG_MESSAGE_LEN];
    memset(buf, 0, sizeof(buf));

    buf[0] = LOGITECH_VERY_LONG_MESSAGE;
    buf[1] = device_index;
    buf[2] = feat_idx;
    buf[3] = function | HIDPP20_SW_ID;

    if(data && len > 0)
    {
        size_t copy_len = (len > max_payload) ? max_payload : len;
        memcpy(buf + 4, data, copy_len);
    }

    int result;

    if(mutex)
    {
        std::lock_guard<std::mutex> lock(*mutex);
        result = hid_write(dev_perkey_vl, buf, sizeof(buf));
    }
    else
    {
        result = hid_write(dev_perkey_vl, buf, sizeof(buf));
    }

    if(result < 0)
    {
        LOG_DEBUG("%s 0x8080 very-long frame write failed (result=%d) "
                  "feat=0x%02X func=0x%02X", LOG_TAG, result, feat_idx, function);
    }

    return result;
}

int LogitechHIDPP20Controller::ReadStandardDirect
    (
    uint8_t*    feat_idx_out,
    uint8_t*    function_out,
    uint8_t*    data_out,
    size_t      data_max,
    int         timeout_ms
    )
{
    /*-----------------------------------------------------*\
    | No mutex needed for reads, when the reader thread     |
    | is running, it is the sole caller. Before the         |
    | reader starts, all access is single-threaded.         |
    |                                                       |
    | Loop within the timeout window draining non-          |
    | HID++ reports (mouse motion, keystrokes, media        |
    | keys, DJ events) until we either find a HID++         |
    | short/long frame or actually time out. A high-        |
    | polling-rate device can put 50+ input reports         |
    | in the hidraw buffer between our calls; without       |
    | the drain loop the synchronous probe path can         |
    | never get past them to find its response.             |
    \*-----------------------------------------------------*/
    std::chrono::steady_clock::time_point deadline = std::chrono::steady_clock::now()
                                                    + std::chrono::milliseconds(timeout_ms);
    int  drained  = 0;

    while(true)
    {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

        if(now >= deadline)
        {
            return 0;
        }

        int remaining_ms = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                                deadline - now).count();

        if(remaining_ms <= 0)
        {
            return 0;
        }

        blankFAPmessage response;
        response.init();

        int result = hid_read_timeout(dev, response.buffer, response.size(), remaining_ms);

        if(result < 0)
        {
            /* Real wire error (e.g. device removed). */
            return result;
        }

        if(result == 0)
        {
            /* hidapi timeout, window expired with nothing pending. */
            return 0;
        }

        /*-------------------------------------------------*\
        | The hidraw also carries HID input reports.        |
        | Drop anything that is not a HID++ short (0x10),   |
        | long (0x11) or very-long (0x12) frame, or         |
        | keystrokes/motion get parsed as HID++ events.     |
        | Very-long matters: 0x8080 fn2 GetKeyColors        |
        | answers a long request with a 64-byte 0x12        |
        | response (14 entries do not fit a long frame).    |
        |                                                   |
        | Drop frames for a different device index:         |
        | receiver slots share one node, so sibling         |
        | replies and the receiver's own notifications      |
        | (index 0xFF) land here too, and SendAcked         |
        | matches only feature+function, so a foreign       |
        | frame would be accepted as our reply.             |
        \*-------------------------------------------------*/
        if(response.buffer[0] == LOGITECH_SHORT_MESSAGE ||
           response.buffer[0] == LOGITECH_LONG_MESSAGE  ||
           response.buffer[0] == LOGITECH_VERY_LONG_MESSAGE)
        {
            if(response.device_index != device_index)
            {
                LOG_TRACE("%s ReadStandardDirect: dropping frame for index 0x%02X (ours 0x%02X)",
                          LOG_TAG, response.device_index, device_index);

                if(++drained > HIDPP20_READ_DRAIN_BUDGET)
                {
                    LOG_DEBUG("%s ReadStandardDirect: drain budget (%d) exceeded",
                              LOG_TAG, HIDPP20_READ_DRAIN_BUDGET);
                    return 0;
                }
                continue;
            }
        }

        if(response.buffer[0] != LOGITECH_SHORT_MESSAGE &&
           response.buffer[0] != LOGITECH_LONG_MESSAGE  &&
           response.buffer[0] != LOGITECH_VERY_LONG_MESSAGE)
        {
            if(++drained > HIDPP20_READ_DRAIN_BUDGET)
            {
                LOG_DEBUG("%s ReadStandardDirect: drain budget (%d) exceeded",
                          LOG_TAG, HIDPP20_READ_DRAIN_BUDGET);
                return 0;
            }
            continue;
        }

        if(feat_idx_out)
        {
            *feat_idx_out = response.feature_index;
        }

        if(function_out)
        {
            *function_out = response.feature_command;
        }

        if(data_out && data_max > 0)
        {
            size_t copy_len = (data_max > sizeof(response.data)) ? sizeof(response.data) : data_max;
            memcpy(data_out, response.data, copy_len);
        }

        return result;
    }
}

/*---------------------------------------------------------*\
| Centurion transport (0xFFA0)                              |
|                                                           |
| Wire format per protocol doc:                             |
|   0x51 (direct): [reportId] [cplLen] [flags] [featIdx]    |
|                  [func|swid] [params...]                  |
|   0x50 (addressed): [reportId] [devAddr] [cplLen] [flags] |
|                     [featIdx] [func|swid] [params...]     |
|                                                           |
| For sub-device access, the parent CentPPBridge wraps      |
| sub-device messages:                                      |
|   params = [devId<<4|lenHi, lenLo, subCPL, subFeatIdx,    |
|             subFunc|swid, subParams...]                   |
|                                                           |
| Selects direct (0x50/0x51) or bridge-wrapped framing      |
| based on transport; routes sub-devices via CentPPBridge.  |
\*---------------------------------------------------------*/

int LogitechHIDPP20Controller::SendCenturion
    (
    uint8_t         feat_idx,
    uint8_t         function,
    const uint8_t*  data,
    size_t          len
    )
{
    uint8_t buf[64];
    memset(buf, 0, sizeof(buf));

    if(transport.bridge_feat_idx != 0)
    {
        /*-------------------------------------------------*\
        | Sub-device message routed through                 |
        | CentPPBridge Parent message: feat=bridge,         |
        | func=sendFragment(0x10) Payload:                  |
        | [devId<<4|lenHi, lenLo, subCPL=0x00,              |
        | subFeatIdx, subFunc|swid, subParams...]           |
        \*-------------------------------------------------*/
        uint16_t sub_msg_len = 3 + (uint16_t)len;  // subCPL + featIdx + func + data

        if(transport.addressed)
        {
            buf[0] = transport.report_id;
            buf[1] = transport.device_address;
            buf[2] = 5 + sub_msg_len;              // cplLen
            buf[3] = 0x00;                          // flags (single fragment)
            buf[4] = transport.bridge_feat_idx;
            buf[5] = 0x10 | HIDPP20_SW_ID;            // sendFragment (func 1)
            buf[6] = (transport.sub_device_id << 4) | ((sub_msg_len >> 8) & 0x0F);
            buf[7] = sub_msg_len & 0xFF;
            buf[8] = 0x00;                          // sub-CPL (single fragment)
            buf[9] = feat_idx;
            buf[10] = function | HIDPP20_SW_ID;

            if(data && len > 0)
            {
                memcpy(buf + 11, data, len);
            }
        }
        else
        {
            buf[0] = transport.report_id;
            buf[1] = 5 + sub_msg_len;              // cplLen: flags(1) + feat(1) + func(1) + hdr(2) + sub
            buf[2] = 0x00;                          // flags
            buf[3] = transport.bridge_feat_idx;
            buf[4] = 0x10 | HIDPP20_SW_ID;            // sendFragment (func 1)
            buf[5] = (transport.sub_device_id << 4) | ((sub_msg_len >> 8) & 0x0F);
            buf[6] = sub_msg_len & 0xFF;
            buf[7] = 0x00;                          // sub-CPL
            buf[8] = feat_idx;
            buf[9] = function | HIDPP20_SW_ID;

            if(data && len > 0)
            {
                memcpy(buf + 10, data, len);
            }
        }
    }
    else
    {
        /*-------------------------------------------------*\
        | Direct parent device message (no bridge)          |
        \*-------------------------------------------------*/
        if(transport.addressed)
        {
            buf[0] = transport.report_id;
            buf[1] = transport.device_address;
            buf[2] = 3 + (uint8_t)len;             // cplLen
            buf[3] = 0x00;                          // flags
            buf[4] = feat_idx;
            buf[5] = function | HIDPP20_SW_ID;

            if(data && len > 0)
            {
                memcpy(buf + 6, data, len);
            }
        }
        else
        {
            buf[0] = transport.report_id;
            buf[1] = 3 + (uint8_t)len;             // cplLen: flags(1) + feat(1) + func(1) + data
            buf[2] = 0x00;                          // flags
            buf[3] = feat_idx;
            buf[4] = function | HIDPP20_SW_ID;

            if(data && len > 0)
            {
                memcpy(buf + 5, data, len);
            }
        }
    }

    int result;

    if(mutex)
    {
        std::lock_guard<std::mutex> lock(*mutex);
        result = hid_write(dev, buf, 64);
    }
    else
    {
        result = hid_write(dev, buf, 64);
    }

    return result;
}

int LogitechHIDPP20Controller::ReadCenturionDirect
    (
    uint8_t*    feat_idx_out,
    uint8_t*    function_out,
    uint8_t*    data_out,
    size_t      data_max,
    int         timeout_ms
    )
{
    uint8_t buf[64];
    memset(buf, 0, sizeof(buf));

    /*-----------------------------------------------------*\
    | Track an overall deadline so the bridge ACK +         |
    | MessageEvent two-read sequence stays within           |
    | timeout_ms total, without this each read could eat    |
    | the full budget independently. Drain non-Centurion    |
    | report IDs within the remaining window rather than    |
    | bailing on the first non-matching frame.              |
    \*-----------------------------------------------------*/
    std::chrono::steady_clock::time_point deadline = std::chrono::steady_clock::now()
                                                    + std::chrono::milliseconds(timeout_ms);
    int  drained  = 0;
    int  result   = 0;

    while(true)
    {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

        if(now >= deadline)
        {
            return 0;
        }

        int remaining_ms = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                                deadline - now).count();

        if(remaining_ms <= 0)
        {
            return 0;
        }

        result = hid_read_timeout(dev, buf, sizeof(buf), remaining_ms);

        if(result < 0)
        {
            return result;
        }

        if(result == 0)
        {
            return 0;
        }

        if(buf[0] == transport.report_id)
        {
            break;
        }

        if(++drained > HIDPP20_READ_DRAIN_BUDGET)
        {
            LOG_DEBUG("%s ReadCenturionDirect: drain budget (%d) exceeded",
                      LOG_TAG, HIDPP20_READ_DRAIN_BUDGET);
            return 0;
        }
    }

    /*-----------------------------------------------------*\
    | Parse based on transport variant                      |
    \*-----------------------------------------------------*/
    int hdr_offset = transport.addressed ? 1 : 0;  // skip device address byte

    uint8_t cpl_len   = buf[1 + hdr_offset];
    // uint8_t cpl_flags = buf[2 + hdr_offset];     // for fragmentation support
    uint8_t resp_feat = buf[3 + hdr_offset];
    uint8_t resp_func = buf[4 + hdr_offset];

    if(transport.bridge_feat_idx != 0 && resp_feat == transport.bridge_feat_idx)
    {
        /*-------------------------------------------------*\
        | CentPPBridge: distinguish events from responses   |
        |                                                   |
        | Bridge events (e.g. ConnectionStateChangedEvent)  |
        | have func high nibble = 0x00 (event index 0) and  |
        | swid = 0. These are NOT wrapped sub-device        |
        | responses; they are bridge-level notifications.   |
        | Return as-is so the reader thread detects them.   |
        |                                                   |
        | Command responses use a two-response pattern:     |
        | 1. ACK: bridge echoes feat+func with our swid     |
        | 2. MessageEvent: func=1x, swid=0, wrapped         |
        |    sub-device response                            |
        \*-------------------------------------------------*/
        if((resp_func & 0xF0) == 0x00 && (resp_func & 0x0F) != HIDPP20_SW_ID)
        {
            /*---------------------------------------------*\
            | Bridge event, return feat/func/data as-is     |
            \*---------------------------------------------*/
            if(feat_idx_out)   *feat_idx_out = resp_feat;
            if(function_out)   *function_out = resp_func;

            if(data_out && data_max > 0)
            {
                size_t avail = (size_t)(cpl_len > 2 ? cpl_len - 2 : 0);
                size_t copy = (avail < data_max) ? avail : data_max;
                memcpy(data_out, buf + 5 + hdr_offset, copy);
            }

            return result;
        }

        if((resp_func & 0x0F) == HIDPP20_SW_ID)
        {
            /*---------------------------------------------*\
            | This is the ACK, discard and read the         |
            | MessageEvent Use the *remaining* window       |
            | from the overall deadline so the two-         |
            | read sequence stays bounded, and drain        |
            | non-Centurion frames within that window.      |
            \*---------------------------------------------*/
            while(true)
            {
                std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

                if(now >= deadline)
                {
                    return 0;
                }

                int remaining_ms = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                                        deadline - now).count();

                if(remaining_ms <= 0)
                {
                    return 0;
                }

                memset(buf, 0, sizeof(buf));
                result = hid_read_timeout(dev, buf, sizeof(buf), remaining_ms);

                if(result < 0)
                {
                    return result;
                }

                if(result == 0)
                {
                    return 0;
                }

                if(buf[0] == transport.report_id)
                {
                    break;
                }

                if(++drained > HIDPP20_READ_DRAIN_BUDGET)
                {
                    LOG_DEBUG("%s ReadCenturionDirect: drain budget (%d) exceeded on bridge MessageEvent",
                              LOG_TAG, HIDPP20_READ_DRAIN_BUDGET);
                    return 0;
                }
            }

            resp_feat = buf[3 + hdr_offset];
            resp_func = buf[4 + hdr_offset];

            if(resp_feat != transport.bridge_feat_idx)
            {
                /*-----------------------------------------*\
                | Not a bridge response, return as-is       |
                \*-----------------------------------------*/
                if(feat_idx_out)   *feat_idx_out = resp_feat;
                if(function_out)   *function_out = resp_func;

                if(data_out && data_max > 0)
                {
                    size_t avail = (size_t)(buf[1 + hdr_offset] > 2 ? buf[1 + hdr_offset] - 2 : 0);
                    size_t copy = (avail < data_max) ? avail : data_max;
                    memcpy(data_out, buf + 5 + hdr_offset, copy);
                }

                return result;
            }
        }

        /*-------------------------------------------------*\
        | MessageEvent: unwrap sub-device response.         |
        | Bridge params: [devId<<4|lenHi, lenLo, subCPL,    |
        | subFeatIdx, subFunc|swid, subData...]             |
        \*-------------------------------------------------*/
        int sub_offset = 5 + hdr_offset + 3;        // past bridge header
        resp_feat = buf[sub_offset];
        resp_func = buf[sub_offset + 1];

        if(feat_idx_out)   *feat_idx_out = resp_feat;
        if(function_out)   *function_out = resp_func;

        if(data_out && data_max > 0)
        {
            size_t avail = (size_t)(result - sub_offset - 2);
            size_t copy = (avail < data_max) ? avail : data_max;
            memcpy(data_out, buf + sub_offset + 2, copy);
        }
    }
    else
    {
        /*-------------------------------------------------*\
        | Direct response                                   |
        \*-------------------------------------------------*/
        if(feat_idx_out)   *feat_idx_out = resp_feat;
        if(function_out)   *function_out = resp_func;

        if(data_out && data_max > 0)
        {
            size_t avail = (size_t)(cpl_len > 2 ? cpl_len - 2 : 0);
            size_t copy = (avail < data_max) ? avail : data_max;
            memcpy(data_out, buf + 5 + hdr_offset, copy);
        }
    }

    (void)cpl_len;

    return result;
}

/*---------------------------------------------------------*\
| Feature Discovery                                         |
\*---------------------------------------------------------*/

uint8_t LogitechHIDPP20Controller::GetFeatureIndex(uint16_t feature_page,
                                                 const HIDPP20RetryPolicy& policy)
{
    /*-----------------------------------------------------*\
    | Check cache first, both Centurion bulk and            |
    | HID++ on-demand lookups store results here.           |
    \*-----------------------------------------------------*/
    std::map<uint16_t, uint8_t>::const_iterator it = caps.feature_map.find(feature_page);

    if(it != caps.feature_map.end())
    {
        return it->second;
    }

    /*-----------------------------------------------------*\
    | Centurion bulk enumeration is complete, so a          |
    | feature not in the map does not exist. No wire        |
    | query is needed.                                      |
    \*-----------------------------------------------------*/
    if(caps.feature_map_complete)
    {
        return 0;
    }

    /*-----------------------------------------------------*\
    | Standard HID++: on-demand IRoot query, result cached. |
    \*-----------------------------------------------------*/
    uint8_t send_data[2];
    send_data[0] = (feature_page >> 8) & 0xFF;
    send_data[1] = feature_page & 0xFF;

    uint8_t recv_data[16] = {};
    int result = SendAcked(LOGITECH_HIDPP_PAGE_ROOT_IDX, FN_8071_GET_INFO,
                           send_data, 2, recv_data, sizeof(recv_data),
                           policy);

    if(result > 0)
    {
        uint8_t index   = recv_data[0];
        uint8_t version = recv_data[2];

        if(index != 0)
        {
            caps.feature_map[feature_page]      = index;
            caps.feature_versions[feature_page] = version;
        }

        if(index != 0)
        {
            LOG_DEBUG("%s Feature 0x%04X V%u -> index 0x%02X",
                      LOG_TAG, feature_page, version, index);

            if(!FeatureVersionIsObserved(feature_page, version))
            {
                LOG_INFO("%s Feature 0x%04X V%u not previously observed, "
                         "tripwire for version-gated behavior",
                         LOG_TAG, feature_page, version);
            }
        }
        else
        {
            LOG_DEBUG("%s Feature 0x%04X not present", LOG_TAG, feature_page);
        }

        return index;
    }

    /*-----------------------------------------------------*\
    | Cache misses too so failed lookups are not re-queried |
    \*-----------------------------------------------------*/
    caps.feature_map[feature_page] = 0;

    LOG_DEBUG("%s Feature 0x%04X not found", LOG_TAG, feature_page);
    return 0;
}

/*---------------------------------------------------------*\
| Return the protocol version byte for a feature, or 0 if   |
| the feature isn't present in this device's feature set.   |
| Populated alongside feature_map during EnumerateFeatures  |
| (Centurion bulk) or GetFeatureIndex (standard HID++       |
| on-demand IRoot.GetFeature).                              |
\*---------------------------------------------------------*/
uint8_t LogitechHIDPP20Controller::GetFeatureVersion(uint16_t feature_page) const
{
    std::map<uint16_t, uint8_t>::const_iterator it = caps.feature_versions.find(feature_page);

    if(it != caps.feature_versions.end())
    {
        return it->second;
    }

    return 0;
}

/*---------------------------------------------------------*\
| Feature/version summary in `solaar show` style, scoped    |
| to the features this controller uses, so firmware that    |
| moved or re-versioned one can be identified from the      |
| log alone. GetFeatureIndex also logs a tripwire for       |
| versions not seen before. Most entries are already        |
| cached; this adds at most a few IRoot queries.            |
\*---------------------------------------------------------*/
void LogitechHIDPP20Controller::LogFeatureSummary()
{
    static const struct
    {
        uint16_t    page;
        const char* name;
    } relevant_features[] =
    {
        { HIDPP20_FEAT_FEATURE_SET,           "FEATURE_SET"           },
        { HIDPP20_FEAT_FIRMWARE_INFO,         "FIRMWARE_INFO/BRIDGE"  },
        { HIDPP20_FEAT_DISABLE_KEYS_BY_USAGE, "DISABLE_KEYS_BY_USAGE" },
        { HIDPP20_FEAT_COLOR_LED_EFFECTS,     "COLOR_LED_EFFECTS"     },
        { HIDPP20_FEAT_RGB_EFFECTS,           "RGB_EFFECTS"           },
        { HIDPP20_FEAT_PER_KEY_LIGHTING_V1,   "PER_KEY_LIGHTING_V1"   },
        { HIDPP20_FEAT_PER_KEY_LIGHTING_V2,   "PER_KEY_LIGHTING_V2"   },
        { HIDPP20_FEAT_KEYBOARD_LAYOUT,       "KEYBOARD_LAYOUT"       },
        { HIDPP20_FEAT_ONBOARD_PROFILES,      "ONBOARD_PROFILES"      },
        { HIDPP20_FEAT_PROFILE_MANAGEMENT,    "PROFILE_MANAGEMENT"    },
        { HIDPP20_FEAT_CENTURION_RGB,         "CENTURION_RGB_0x0600"  },
        { HIDPP20_FEAT_HEADSET_RGB_HOSTMODE,  "HEADSET_RGB_HOSTMODE"  },
        { HIDPP20_FEAT_WIRELESS_STATUS,       "WIRELESS_STATUS"       },
    };

    LOG_DEBUG("%s ===== HID++ 2.0 feature summary (features OpenRGB uses) =====", LOG_TAG);

    for(size_t i = 0; i < sizeof(relevant_features) / sizeof(relevant_features[0]); i++)
    {
        uint16_t page = relevant_features[i].page;
        uint8_t  idx  = GetFeatureIndex(page);

        if(idx != 0)
        {
            LOG_DEBUG("%s   feature 0x%04X %-22s present  idx=0x%02X V%u",
                      LOG_TAG, page, relevant_features[i].name, idx, GetFeatureVersion(page));
        }
        else
        {
            LOG_DEBUG("%s   feature 0x%04X %-22s absent",
                      LOG_TAG, page, relevant_features[i].name);
        }
    }

    LOG_DEBUG("%s ============================================================", LOG_TAG);
}

/*---------------------------------------------------------*\
| Read a Centurion sub-device's name via DeviceName         |
| 0x0101 (bridge-routed). Handles both response             |
| shapes Solaar does: inline [len, name...] in the          |
| fn0 reply, or chunked [len] then fn1 fragments.           |
| Returns false if 0x0101 is absent or yields               |
| nothing. Mirrors Solaar get_name_centurion.               |
\*---------------------------------------------------------*/
bool LogitechHIDPP20Controller::ReadCenturionDeviceName0101(std::string& out)
{
    out.clear();

    uint8_t idx = GetFeatureIndex(HIDPP20_FEAT_CENTURION_DEVICE_NAME);

    if(idx == 0)
    {
        return false;
    }

    /*-----------------------------------------------------*\
    | fn0 getName -> recv[0] = name length (+ inline bytes  |
    | on devices that return the whole name in one reply).  |
    \*-----------------------------------------------------*/
    uint8_t recv[64] = {};
    int rd = SendAcked(idx, 0x00, nullptr, 0, recv, sizeof(recv));

    if(rd <= 0)
    {
        return false;
    }

    unsigned int name_length = recv[0];

    LOG_DEBUG("%s 0x0101 getName: len=%u rd=%d raw=[%02X %02X %02X %02X %02X %02X %02X %02X]",
              LOG_TAG, name_length, rd,
              recv[0], recv[1], recv[2], recv[3], recv[4], recv[5], recv[6], recv[7]);

    if(name_length == 0 || name_length > 64)
    {
        return false;
    }

    /*-----------------------------------------------------*\
    | Use the fn0 reply inline when it already carries      |
    | the whole name (length covered and the byte after     |
    | it printable; rejects a chunked fn0 that returned     |
    | only length + zeros); else fetch in chunks via fn1.   |
    \*-----------------------------------------------------*/
    if((unsigned int)rd >= 1u + name_length
       && name_length <= sizeof(recv) - 1
       && recv[1] >= 0x20 && recv[1] <= 0x7E)
    {
        out.assign((char*)recv + 1, name_length);
    }
    else
    {
        for(unsigned int offset = 0; offset < name_length; offset += 16)
        {
            uint8_t send_data[1] = { (uint8_t)offset };
            uint8_t chunk[20]    = {};
            int cr = SendAcked(idx, 0x10, send_data, 1, chunk, sizeof(chunk));

            if(cr <= 0)
            {
                break;
            }

            unsigned int chunk_len = name_length - offset;
            if(chunk_len > 16)
            {
                chunk_len = 16;
            }

            out.append((char*)chunk, chunk_len);
        }
    }

    while(!out.empty() && out.back() == '\0')
    {
        out.pop_back();
    }

    return !out.empty();
}

void LogitechHIDPP20Controller::DiscoverDeviceName()
{
    /*-----------------------------------------------------*\
    | Centurion sub-devices use 0x0101 (DeviceName).        |
    | Standard HID++ uses 0x0005 (DeviceNameType).          |
    \*-----------------------------------------------------*/
    if(transport.type == HIDPP20_TRANSPORT_CENTURION)
    {
        /*-------------------------------------------------*\
        | Try the HID++ name (0x0101) first so a            |
        | spec-compliant sub-device self-names;             |
        | fall back to the OS/USB name for hardware         |
        | that doesn't return a usable name there.          |
        \*-------------------------------------------------*/
        std::string hidpp_name;

        if(ReadCenturionDeviceName0101(hidpp_name) && LogitechHIDPP20Controller::NameLooksReal(hidpp_name))
        {
            caps.device_name = hidpp_name;
        }
        else
        {
            std::string friendly = GetCenturionSubDeviceName(location);
            caps.device_name = friendly.empty() ? HIDPP20_NAME_PLACEHOLDER_CENTURION
                                                : friendly;
        }

        LOG_VERBOSE("%s Device name (Centurion): %s", LOG_TAG, caps.device_name.c_str());
        return;
    }

    uint8_t feat_idx = GetFeatureIndex(HIDPP20_FEAT_DEVICE_NAME_TYPE);

    if(feat_idx == 0)
    {
        /*-------------------------------------------------*\
        | No name feature, or the lookup for it failed.     |
        | Either way the receiver has already told us what  |
        | this device is called, use that before falling    |
        | back to a name that tells the user nothing.       |
        \*-------------------------------------------------*/
        caps.device_name = LogitechHIDPP20Controller::NameLooksReal(pairing_name) ? pairing_name
                                                              : HIDPP20_NAME_PLACEHOLDER_STD;

        LOG_WARNING("%s DeviceNameType (0x0005) not available, using '%s'",
                    LOG_TAG, caps.device_name.c_str());
        return;
    }

    /*-----------------------------------------------------*\
    | Rescan churn (notification bursts from the previous   |
    | session's teardown) can garble or time out the first  |
    | reads, so validate the length and retry the whole     |
    | discovery. Never leave the name blank, a failed read  |
    | falls back to the placeholder.                        |
    \*-----------------------------------------------------*/
    for(int attempt = 0; attempt < 3; attempt++)
    {
        uint8_t recv[16] = {};
        int result = SendAcked(feat_idx, LOTITECH_CMD_DEVICE_NAME_TYPE_GET_COUNT,
                               nullptr, 0, recv, sizeof(recv));

        if(result <= 0)
        {
            continue;
        }

        unsigned int name_length = recv[0];

        if(name_length == 0 || name_length > 64)
        {
            LOG_DEBUG("%s GetCount returned implausible name length %u (attempt %d)",
                      LOG_TAG, name_length, attempt);
            continue;
        }

        std::string name;
        bool        complete = true;

        for(unsigned int offset = 0; offset < name_length; offset += 16)
        {
            uint8_t send_data[1] = { (uint8_t)offset };
            result = SendAcked(feat_idx, LOGITECH_CMD_DEVICE_NAME_TYPE_GET_DEVICE_NAME,
                               send_data, 1, recv, sizeof(recv));

            if(result <= 0)
            {
                complete = false;
                break;
            }

            unsigned int chunk_len = name_length - offset;
            if(chunk_len > 16)
            {
                chunk_len = 16;
            }

            name.append((char*)recv, chunk_len);
        }

        while(!name.empty() && name.back() == '\0')
        {
            name.pop_back();
        }

        /*-------------------------------------------------*\
        | A garbled read can return the right               |
        | length but junk bytes. Require printable          |
        | ASCII, otherwise the device registers             |
        | under an unprintable name that reads as           |
        | blank and never matches a saved profile.          |
        \*-------------------------------------------------*/
        if(!complete || !LogitechHIDPP20Controller::NameLooksReal(name))
        {
            LOG_DEBUG("%s Device name read attempt %d unusable (len=%zu), retrying",
                      LOG_TAG, attempt, name.size());
            continue;
        }

        caps.device_name = name;
        LOG_VERBOSE("%s Device name: %s", LOG_TAG, caps.device_name.c_str());
        return;
    }

    /*-----------------------------------------------------*\
    | Use the receiver-stored pairing codename, not the     |
    | generic placeholder, when the FAP name read fails.    |
    \*-----------------------------------------------------*/
    caps.device_name = LogitechHIDPP20Controller::NameLooksReal(pairing_name) ? pairing_name
                                                          : HIDPP20_NAME_PLACEHOLDER_STD;
    LOG_WARNING("%s Device name discovery failed after retries, using '%s'",
                LOG_TAG, caps.device_name.c_str());
}

void LogitechHIDPP20Controller::DiscoverDeviceType()
{
    /*-----------------------------------------------------*\
    | Centurion sub-devices don't have 0x0005               |
    | (DeviceNameType). Default to unknown, don't           |
    | assume device type from transport, as                 |
    | Centurion may be used for future devices.             |
    \*-----------------------------------------------------*/
    if(transport.type == HIDPP20_TRANSPORT_CENTURION)
    {
        caps.device_type = 0;
        return;
    }

    uint8_t feat_idx = GetFeatureIndex(HIDPP20_FEAT_DEVICE_NAME_TYPE);

    if(feat_idx == 0)
    {
        caps.device_type = LOGITECH_DEVICE_TYPE_MOUSE;
        return;
    }

    /*-----------------------------------------------------*\
    | Retry like the name read: a timed-out type read under |
    | rescan churn would register a keyboard with a mouse   |
    | layout.                                               |
    \*-----------------------------------------------------*/
    for(int attempt = 0; attempt < 3; attempt++)
    {
        uint8_t recv[16] = {};
        int result = SendAcked(feat_idx, LOGITECH_CMD_DEVICE_NAME_TYPE_GET_TYPE,
                               nullptr, 0, recv, sizeof(recv));

        if(result > 0)
        {
            caps.device_type = recv[0];
            LOG_VERBOSE("%s Device type: %d", LOG_TAG, caps.device_type);
            return;
        }
    }

    caps.device_type = LOGITECH_DEVICE_TYPE_MOUSE;
    LOG_WARNING("%s Device type discovery failed after retries, assuming mouse",
                LOG_TAG);
}

void LogitechHIDPP20Controller::DiscoverTransport()
{
    /*-----------------------------------------------------*\
    | Detect transport type from usage page.                |
    | 0xFF00/0xFF43: Standard HID++ (0x10/0x11 reports)     |
    | 0xFFA0+:       Centurion (64-byte CPL framing)        |
    \*-----------------------------------------------------*/
    if(transport.usage_page == 0xFF00 || transport.usage_page == 0xFF43)
    {
        transport.type = HIDPP20_TRANSPORT_STANDARD;
        return;
    }

    /*-----------------------------------------------------*\
    | Centurion transport, determine variant by probing.    |
    | 0x51 = direct (PRO X 2), 0x50 = addressed (G522). The |
    | report descriptor would tell us which report IDs      |
    | exist, but hid_get_report_descriptor is hidapi        |
    | 0.14.0+ only, so we probe instead: try 0x51 direct    |
    | first, then fall back to the robust 0x50 device-      |
    | address sweep.                                        |
    \*-----------------------------------------------------*/
    transport.type = HIDPP20_TRANSPORT_CENTURION;

    /*-----------------------------------------------------*\
    | Probe 0x51 (direct). If the device answers a 0x51     |
    | frame it speaks the direct variant, no device         |
    | address needed.                                       |
    \*-----------------------------------------------------*/
    transport.report_id = 0x51;
    transport.addressed = false;

    uint8_t probe_buf[64] = {};
    probe_buf[0] = 0x51;
    probe_buf[1] = 3;
    probe_buf[2] = 0x00;
    probe_buf[3] = 0x00;
    probe_buf[4] = 0x00 | HIDPP20_SW_ID;

    int wr = hid_write(dev, probe_buf, 64);

    if(wr > 0)
    {
        uint8_t resp_buf[64] = {};
        int rd = hid_read_timeout(dev, resp_buf, sizeof(resp_buf), 500);

        if(rd > 0 && resp_buf[0] == 0x51)
        {
            LOG_DEBUG("%s Centurion 0x51 (direct) from probe", LOG_TAG);
            return;
        }
    }

    /*-----------------------------------------------------*\
    | No 0x51 reply: assume 0x50 (addressed) and find the   |
    | device address.                                       |
    \*-----------------------------------------------------*/
    transport.report_id = 0x50;
    transport.addressed = true;
    transport.device_address = 0x00;

    /*-----------------------------------------------------*\
    | Device-address sweep. 0x50 frames carry a device      |
    | address byte; the device silently drops frames        |
    | addressed to the wrong ID, so we brute-force probe    |
    | every candidate with an IRoot fn1 GetProtocolVersion  |
    | ping. The first address to respond is taken, and the  |
    | real address is in resp_buf[1] of the reply. Mirrors  |
    | Solaar's probe_centurion_device_addr; see             |
    | CENTURION_PROBE_PER_ADDR_TIMEOUT_MS above for timing. |
    |                                                       |
    | Wire format per candidate:                            |
    |   [0x50, addr, 0x06, 0x00, 0x00, 0x10, 0x00, 0x00,    |
    |    0x00, zero-pad to 64]                              |
    | where 0x06 = cpl_length (flags+payload), 0x10 = fn1   |
    | GetProtocolVersion with sw_id=0.                      |
    \*-----------------------------------------------------*/
    bool     addr_found       = false;
    unsigned probe_count      = 0;
    unsigned write_errors     = 0;

    for(unsigned addr = 0; addr < 256; addr++)
    {
        uint8_t sweep_buf[64] = {};
        sweep_buf[0] = 0x50;
        sweep_buf[1] = (uint8_t)addr;
        sweep_buf[2] = 0x06;
        sweep_buf[3] = 0x00;
        sweep_buf[4] = 0x00;
        sweep_buf[5] = 0x10;

        int swr = hid_write(dev, sweep_buf, 64);
        probe_count++;

        if(swr <= 0)
        {
            write_errors++;
            if(write_errors > 3)
            {
                LOG_DEBUG("%s Centurion 0x50 probe: too many write failures, aborting", LOG_TAG);
                break;
            }
            continue;
        }

        uint8_t resp_buf[64] = {};
        int     rd = hid_read_timeout(dev, resp_buf, sizeof(resp_buf),
                                      CENTURION_PROBE_PER_ADDR_TIMEOUT_MS);

        if(rd >= 2 && resp_buf[0] == 0x50)
        {
            transport.device_address = resp_buf[1];
            addr_found = true;
            break;
        }
    }

    if(addr_found)
    {
        LOG_INFO("%s Centurion 0x50 device_addr=0x%02X (after %u candidates)",
                 LOG_TAG, transport.device_address, probe_count);
    }
    else
    {
        LOG_DEBUG("%s Centurion 0x50 probe: no response from any of 256 candidates",
                  LOG_TAG);
    }
}

void LogitechHIDPP20Controller::EnumerateFeatures(uint8_t feature_set_idx)
{
    caps.feature_map.clear();
    caps.feature_map_complete = false;

    /*-----------------------------------------------------*\
    | Root (0x0000) is always at index 0                    |
    \*-----------------------------------------------------*/
    caps.feature_map[0x0000] = 0;

    if(transport.type == HIDPP20_TRANSPORT_CENTURION)
    {
        /*-------------------------------------------------*\
        | Centurion sub-device: CenturionFeatureSet fn1     |
        | returns ALL features in one bulk response.        |
        | [count, (feat_hi, feat_lo, type, version) x N]    |
        \*-------------------------------------------------*/
        uint8_t send_data[1] = { 0x00 };
        uint8_t recv_data[60] = {};

        int result = SendAcked(feature_set_idx, 0x10,
                               send_data, 1, recv_data, sizeof(recv_data));

        if(result > 0)
        {
            uint8_t count = recv_data[0];

            LOG_DEBUG("%s CenturionFeatureSet: %d features", LOG_TAG, count);

            for(uint8_t i = 0; i < count && (1 + i * 4 + 3) < (int)sizeof(recv_data); i++)
            {
                int offset = 1 + i * 4;
                uint16_t feat_id      = ((uint16_t)recv_data[offset] << 8) | recv_data[offset + 1];
                uint8_t  feat_type    = recv_data[offset + 2];
                uint8_t  feat_version = recv_data[offset + 3];
                uint8_t  feat_idx     = i;  // 0-based: bulk includes root at 0

                caps.feature_map[feat_id]      = feat_idx;
                caps.feature_versions[feat_id] = feat_version;

                LOG_DEBUG("%s   [%2d] Feature 0x%04X V%u type=0x%02X",
                          LOG_TAG, feat_idx, feat_id, feat_version, feat_type);

                if(!FeatureVersionIsObserved(feat_id, feat_version))
                {
                    LOG_INFO("%s Feature 0x%04X V%u not previously observed, "
                             "tripwire for version-gated behavior",
                             LOG_TAG, feat_id, feat_version);
                }
            }

            caps.feature_map_complete = true;
        }
    }
    else
    {
        /*-------------------------------------------------*\
        | Standard HID++: no bulk query available.          |
        | Features are looked up on-demand via              |
        | GetFeatureIndex (IRoot) and cached in             |
        | the feature map. Nothing to do here.              |
        \*-------------------------------------------------*/
        return;
    }
}

void LogitechHIDPP20Controller::DiscoverFirmwareInfo()
{
    /*-----------------------------------------------------*\
    | Centurion sub-devices use 0x0100 (DeviceInfo)         |
    | for firmware version and serial. Standard             |
    | HID++ uses 0x0003 (FirmwareInfo).                     |
    \*-----------------------------------------------------*/
    if(transport.type == HIDPP20_TRANSPORT_CENTURION)
    {
        uint8_t dev_info_idx = GetFeatureIndex(HIDPP20_FEAT_CENTURION_DEVICE_INFO);

        if(dev_info_idx == 0)
        {
            return;
        }

        /*-------------------------------------------------*\
        | fn1 getFirmwareVersion(entityIndex=0):            |
        | main firmware Response: [fwType,                  |
        | additional, version_hi, version_lo]               |
        \*-------------------------------------------------*/
        {
            uint8_t send_data[1] = { 0x00 };
            uint8_t recv_data[16] = {};

            int result = SendAcked(dev_info_idx, 0x10,
                                   send_data, 1, recv_data, sizeof(recv_data));

            if(result > 0)
            {
                uint16_t version = ((uint16_t)recv_data[2] << 8) | recv_data[3];

                char ver_str[32];
                snprintf(ver_str, sizeof(ver_str), "%d.%d",
                         (version >> 8) & 0xFF, version & 0xFF);

                caps.firmware_version = ver_str;

                LOG_DEBUG("%s Firmware (Centurion): %s", LOG_TAG, caps.firmware_version.c_str());
            }
        }

        /*-------------------------------------------------*\
        | fn2 getSerialNumber on 0x0100 (DeviceInfo)        |
        | Response: [stringLen, serial...]                  |
        \*-------------------------------------------------*/
        {
            uint8_t recv_data[16] = {};

            int result = SendAcked(dev_info_idx, 0x20,
                                   nullptr, 0, recv_data, sizeof(recv_data));

            if(result > 0)
            {
                uint8_t slen = recv_data[0];
                if(slen > 15) slen = 15;

                char serial[16] = {};
                memcpy(serial, &recv_data[1], slen);

                /*-----------------------------------------*\
                | A dongle with no stored serial reports a  |
                | length over zeroed bytes, stop at the     |
                | first non-printable so those stay empty.  |
                \*-----------------------------------------*/
                for(uint8_t i = 0; i < slen; i++)
                {
                    if(serial[i] < 0x21 || serial[i] > 0x7E)
                    {
                        serial[i] = '\0';
                        break;
                    }
                }

                caps.serial_number = serial;

                LOG_DEBUG("%s Serial (Centurion): %s", LOG_TAG, caps.serial_number.c_str());
            }
        }

        return;
    }

    uint8_t fw_idx = GetFeatureIndex(HIDPP20_FEAT_FIRMWARE_INFO);

    if(fw_idx == 0)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | fn0 GetEntityCount: entity count, unitId,             |
    | transport PIDs Response: [count,                      |
    | unitId(4), transport(2), PID1(2), PID2(2)]            |
    \*-----------------------------------------------------*/
    uint8_t entity_count = 1;

    {
        uint8_t recv_data[16] = {};
        int result = SendAcked(fw_idx, 0x00,
                               nullptr, 0, recv_data, sizeof(recv_data));

        if(result > 0)
        {
            entity_count = recv_data[0];

            /*---------------------------------------------*\
            | Extract unitId, the stable hardware identity  |
            | across all paths (USB, wireless, dongle).     |
            \*---------------------------------------------*/
            char uid[16];
            snprintf(uid, sizeof(uid), "%02X%02X%02X%02X",
                     recv_data[1], recv_data[2], recv_data[3], recv_data[4]);
            caps.unit_id = uid;

            caps.pid_wireless = ((uint16_t)recv_data[7] << 8) | recv_data[8];
            caps.pid_wired    = ((uint16_t)recv_data[9] << 8) | recv_data[10];
            caps.pid_third    = ((uint16_t)recv_data[11] << 8) | recv_data[12];

            /*---------------------------------------------*\
            | Use unitId as serial if none is reported      |
            \*---------------------------------------------*/
            if(caps.serial_number.empty() && caps.unit_id != "00000000")
            {
                caps.serial_number = caps.unit_id;
            }

            LOG_DEBUG("%s unitId=%s PID1=0x%04X PID2=0x%04X PID3=0x%04X",
                      LOG_TAG, caps.unit_id.c_str(), caps.pid_wireless, caps.pid_wired, caps.pid_third);

            /*---------------------------------------------*\
            | Resolve per-model quirks. The modelId         |
            | slots shift with the device's transports,     |
            | so match an entry against any slot.           |
            \*---------------------------------------------*/
            caps.quirks = 0;

            uint16_t device_pids[3] = { caps.pid_wireless, caps.pid_wired, caps.pid_third };

            size_t quirk_table_len = sizeof(HIDPP20_DEVICE_QUIRK_TABLE)
                                   / sizeof(HIDPP20_DEVICE_QUIRK_TABLE[0]);

            for(size_t q = 0; q < quirk_table_len; q++)
            {
                const HIDPP20DeviceQuirkEntry& entry = HIDPP20_DEVICE_QUIRK_TABLE[q];

                for(size_t p = 0; p < 3; p++)
                {
                    if(device_pids[p] != 0 &&
                       (entry.pid_wireless == device_pids[p] || entry.pid_wired == device_pids[p]))
                    {
                        caps.quirks |= entry.quirks;
                        break;
                    }
                }
            }

            if(caps.quirks & HIDPP20_QUIRK_KEEP_ONBOARD_MODE)
            {
                LogitechHIDPP20IdleSettings::instance()->load();
                if(LogitechHIDPP20IdleSettings::instance()->forceHostMode())
                {
                    caps.quirks &= ~HIDPP20_QUIRK_KEEP_ONBOARD_MODE;
                    LOG_INFO("%s force_host_mode set: KEEP_ONBOARD_MODE quirk disabled",
                             LOG_TAG);
                }
            }

            if(caps.quirks != 0)
            {
                LOG_DEBUG("%s Device quirks: 0x%08X", LOG_TAG, caps.quirks);
            }
        }
    }

    /*-----------------------------------------------------*\
    | fn1 GetFwInfo: iterate entities to find main FW       |
    | (type 0) fwType lower nibble: 0=main,                 |
    | 1=bootloader, 2=HW rev Response: fwType(1),           |
    | prefix(3), bcdVersion(2), bcdBuild(2)                 |
    \*-----------------------------------------------------*/
    for(uint8_t entity = 0; entity < entity_count && entity < 8; entity++)
    {
        uint8_t send_data[1] = { entity };
        uint8_t recv_data[16] = {};

        int result = SendAcked(fw_idx, 0x10,
                               send_data, 1, recv_data, sizeof(recv_data));

        if(result <= 0)
        {
            continue;
        }

        uint8_t fw_type   = recv_data[0] & 0x0F;
        char prefix[4]    = { (char)recv_data[1], (char)recv_data[2], (char)recv_data[3], '\0' };
        uint8_t ver_major = recv_data[4];
        uint8_t ver_minor = recv_data[5];
        uint16_t build    = ((uint16_t)recv_data[6] << 8) | recv_data[7];

        char ver_str[64];
        snprintf(ver_str, sizeof(ver_str), "%s %d.%d.%05u",
                 prefix, ver_major, ver_minor, build);

        LOG_DEBUG("%s Firmware entity %d: type=%d %s", LOG_TAG, entity, fw_type, ver_str);

        if(fw_type == 0)
        {
            caps.firmware_version = ver_str;
        }
    }

    if(caps.firmware_version.empty())
    {
        LOG_DEBUG("%s No main firmware entity found", LOG_TAG);
    }

    /*-----------------------------------------------------*\
    | fn2 GetDeviceSerialNumber: ASCII, up to 16 bytes      |
    \*-----------------------------------------------------*/
    {
        uint8_t recv_data[16] = {};

        int result = SendAcked(fw_idx, 0x20,
                               nullptr, 0, recv_data, sizeof(recv_data));

        if(result > 0)
        {
            char serial[17] = {};
            memcpy(serial, recv_data, 16);
            serial[16] = '\0';

            /*---------------------------------------------*\
            | Trim trailing nulls/spaces                    |
            \*---------------------------------------------*/
            for(int i = 15; i >= 0; i--)
            {
                if(serial[i] == '\0' || serial[i] == ' ')
                {
                    serial[i] = '\0';
                }
                else
                {
                    break;
                }
            }

            if(serial[0] != '\0')
            {
                caps.serial_number = serial;
            }

            LOG_DEBUG("%s Serial: %s", LOG_TAG, caps.serial_number.c_str());
        }
    }
}

void LogitechHIDPP20Controller::DiscoverRGBEffects()
{
    /*-----------------------------------------------------*\
    | Try 0x8071, then 0x0600 (Centurion), then 0x8070      |
    \*-----------------------------------------------------*/
    caps.idx_rgb_effects = GetFeatureIndex(HIDPP20_FEAT_RGB_EFFECTS);
    caps.rgb_feature_page = HIDPP20_FEAT_RGB_EFFECTS;

    if(caps.idx_rgb_effects == 0)
    {
        caps.idx_rgb_effects = GetFeatureIndex(HIDPP20_FEAT_CENTURION_RGB);
        caps.rgb_feature_page = HIDPP20_FEAT_CENTURION_RGB;
    }

    if(caps.idx_rgb_effects == 0)
    {
        caps.idx_rgb_effects = GetFeatureIndex(HIDPP20_FEAT_COLOR_LED_EFFECTS);
        caps.rgb_feature_page = HIDPP20_FEAT_COLOR_LED_EFFECTS;
    }

    if(caps.idx_rgb_effects == 0)
    {
        caps.has_zone_effects = false;
        return;
    }

    /*-----------------------------------------------------*\
    | Resolve function IDs based on which feature           |
    | was found. 0x8071 and 0x0600 share the same           |
    | function layout. 0x8070 has different                 |
    | function numbers and SW control format.               |
    \*-----------------------------------------------------*/
    if(caps.rgb_feature_page == HIDPP20_FEAT_COLOR_LED_EFFECTS)
    {
        caps.fn_set_effect      = 0x30;
        caps.fn_sw_control      = 0x80;
        caps.fn_pwr_config      = 0;
        caps.fn_pwr_mode        = 0;
        caps.has_power_mgmt     = false;
        caps.sw_control_simple  = true;
    }
    else
    {
        caps.fn_set_effect      = 0x10;
        caps.fn_sw_control      = 0x50;
        caps.fn_pwr_config      = 0x70;
        caps.fn_pwr_mode        = 0x80;
        caps.has_power_mgmt     = true;
        caps.sw_control_simple  = false;
    }

    /*-----------------------------------------------------*\
    | GetInfo: discover cluster count                       |
    | 0x8071: data = [0xFF, 0xFF, 0x00]                     |
    | 0x8070: data = [] (empty)                             |
    \*-----------------------------------------------------*/
    uint8_t data[3] = { 0xFF, 0xFF, 0x00 };
    size_t data_len = (caps.rgb_feature_page == HIDPP20_FEAT_RGB_EFFECTS) ? 3 : 0;

    blankFAPmessage response;
    int result = SendAckedIntoFAP(caps.idx_rgb_effects, FN_8071_GET_INFO,
                                  data, data_len, response);

    if(result <= 0)
    {
        caps.has_zone_effects = false;
        return;
    }

    unsigned int cluster_count;

    if(caps.rgb_feature_page == HIDPP20_FEAT_RGB_EFFECTS)
    {
        cluster_count = response.data[2];

        /*-----------------------------------------------------*\
        | 0x8071 GetInfo response layout:                       |
        |   byte 2     numRgbZones                              |
        |   bytes 3-4  extendedCapabilities (BE16)              |
        |   bytes 5-6  effectBlockCount     (BE16)              |
        |   byte 7     supportedClusterIndex                    |
        | Logging the extra fields makes it easy to spot a      |
        | device whose enumerated effect list looks too short   |
        | relative to what it claims it can do.                 |
        \*-----------------------------------------------------*/
        uint16_t ext_caps     = ((uint16_t)response.data[3] << 8) | response.data[4];
        uint16_t effect_blocks = ((uint16_t)response.data[5] << 8) | response.data[6];
        uint8_t  supported_idx = response.data[7];

        LOG_INFO("%s RGBEffects 0x8071 V%u GetInfo: zones=%u extCaps=0x%04X effectBlocks=%u supportedClusterIdx=%u",
                 LOG_TAG, GetFeatureVersion(caps.rgb_feature_page),
                 cluster_count, ext_caps, effect_blocks, supported_idx);
    }
    else
    {
        cluster_count = response.data[0];
        LOG_INFO("%s RGB feature page=0x%04X V%u cluster_count=%u",
                 LOG_TAG, caps.rgb_feature_page,
                 GetFeatureVersion(caps.rgb_feature_page), cluster_count);
    }

    /*-----------------------------------------------------*\
    | GetRgbClusterInfo for each cluster                    |
    \*-----------------------------------------------------*/
    for(unsigned int i = 0; i < cluster_count; i++)
    {
        HIDPP20ZoneCluster cluster;
        cluster.index = i;

        if(caps.rgb_feature_page == HIDPP20_FEAT_RGB_EFFECTS)
        {
            uint8_t query[2] = { (uint8_t)i, 0xFF };
            result = SendAckedIntoFAP(caps.idx_rgb_effects, FN_8071_GET_INFO,
                                      query, 2, response);
        }
        else
        {
            uint8_t query[2] = { (uint8_t)i, 0x00 };
            result = SendAckedIntoFAP(caps.idx_rgb_effects, LOGITECH_CMD_RGB_EFFECTS_GET_INFO,
                                      query, 2, response);
        }

        if(result <= 0)
        {
            continue;
        }

        if(caps.rgb_feature_page == HIDPP20_FEAT_RGB_EFFECTS)
        {
            cluster.location     = (response.data[2] << 8) | response.data[3];
            cluster.effect_count = response.data[4];
        }
        else
        {
            cluster.location     = (response.data[1] << 8) | response.data[2];
            cluster.effect_count = response.data[3];
        }

        LOG_INFO("%s Cluster %d: location=0x%04X effects=%d",
                 LOG_TAG, i, cluster.location, cluster.effect_count);

        /*-------------------------------------------------*\
        | GetEffectInfo for each effect in this cluster     |
        \*-------------------------------------------------*/
        for(unsigned int j = 0; j < cluster.effect_count; j++)
        {
            HIDPP20Effect effect;
            effect.index = j;

            uint8_t eff_query[4] = { (uint8_t)i, (uint8_t)j, 0x00, 0x00 };
            uint8_t eff_fn = (caps.rgb_feature_page == HIDPP20_FEAT_RGB_EFFECTS)
                           ? FN_8071_GET_INFO : LOGITECH_FP8070_EFFECT_INFO;
            result = SendAckedIntoFAP(caps.idx_rgb_effects, eff_fn,
                                      eff_query, 4, response);

            if(result <= 0)
            {
                continue;
            }

            /*---------------------------------------------*\
            | 0x8070 GetZoneEffectInfo and 0x8071/0x0600    |
            | GetEffectInfo share one response shape:       |
            | [0..1] echo (zone/cluster << 8 | effect       |
            | index) [2..3] effect type id [4..5]           |
            | capability bitmask [6..7] default period      |
            | (ms) G810-confirmed. The prior 0x8070         |
            | layout read the echo at [0..1] as the id;     |
            | it only matched on zone 0.                    |
            \*---------------------------------------------*/
            effect.effect_id      = (response.data[2] << 8) | response.data[3];
            effect.capabilities   = (response.data[4] << 8) | response.data[5];
            effect.default_period = (response.data[6] << 8) | response.data[7];

            LOG_INFO("%s   Effect %d: id=0x%04X caps=0x%04X default_period=%dms "
                     "[raw %02X %02X %02X %02X %02X %02X %02X %02X]",
                     LOG_TAG, j, effect.effect_id, effect.capabilities, effect.default_period,
                     response.data[0], response.data[1], response.data[2], response.data[3],
                     response.data[4], response.data[5], response.data[6], response.data[7]);

            cluster.effects.push_back(effect);
        }

        caps.zone_clusters.push_back(cluster);
    }

    /*-----------------------------------------------------*\
    | 0x8070, trace level: read back each zone's            |
    | live effect before anything is claimed, the           |
    | onboard default is still running, so this             |
    | dumps real firmware effect settings (param            |
    | layouts for effects the docs don't cover).            |
    \*-----------------------------------------------------*/
    if(caps.rgb_feature_page == HIDPP20_FEAT_COLOR_LED_EFFECTS
       && LogManager::get()->GetLogLevel() >= LL_TRACE)
    {
        for(const HIDPP20ZoneCluster& zc : caps.zone_clusters)
        {
            uint8_t zone = zc.index;

            uint8_t fns[2] = { LOGITECH_FP8070_GET_EFFECT, LOGITECH_FP8070_GET_COLOUR };

            for(int f = 0; f < 2; f++)
            {
                result = SendAckedIntoFAP(caps.idx_rgb_effects, fns[f], &zone, 1, response);

                LOG_TRACE("%s zone %u fn 0x%02X readback (result=%d): "
                          "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
                          LOG_TAG, zone, fns[f], result,
                          response.data[0],  response.data[1],  response.data[2],  response.data[3],
                          response.data[4],  response.data[5],  response.data[6],  response.data[7],
                          response.data[8],  response.data[9],  response.data[10], response.data[11],
                          response.data[12], response.data[13], response.data[14], response.data[15]);
            }
        }
    }

    caps.has_zone_effects = !caps.zone_clusters.empty();

    /*-----------------------------------------------------*\
    | Probe for device-firmware effect cards. Only defined  |
    | on the 0x8071 RGBEffects path: 0x8070 and 0x0600      |
    | don't expose GetEffectSpecificInfo in the same form.  |
    \*-----------------------------------------------------*/
    DiscoverEffectCards();
}

void LogitechHIDPP20Controller::DiscoverEffectCards()
{
    /*---------------------------------------------------------*\
    | Probes the device for the presence of firmware-resident   |
    | effect cards via 0x8071 fn0 GetEffectSpecificInfo. On     |
    | devices that have them (observed on G502 X PLUS), every   |
    | valid card returns a device-wide template byte pair at a  |
    | fixed position in page 1 of the response, the vendor app  |
    | reads those bytes and echoes them into the per-key prep   |
    | call's `SetEffectByIndex` params[6..7]. Our               |
    | implementation does the same.                             |
    |                                                           |
    | Request format for GetEffectSpecificInfo (0x8071 fn0):    |
    |   [0xFF, effectIdHi, 0x01, effectIdLo, pageIndex]         |
    |                                                           |
    | Response layout in blankFAPmessage::data[] terms (i.e.    |
    | starting AFTER the 4-byte HID++ header                    |
    | report_id/dev_idx/feat_idx/func_byte):                    |
    |                                                           |
    |   data[0..4]  5-byte prefix                               |
    |     [0] 0xFF echo of subfn marker                         |
    |     [1] echo of effectIdHi                                |
    |     [2] 0x01 echo of static constant                      |
    |     [3] 0x00 static zero (NOT an echo of effectIdLo)      |
    |     [4] 0x00 static zero (NOT an echo of pageIndex)       |
    |   data[5..15] 11-byte page payload                        |
    |     [5..6]   header (0x00 0x00)                           |
    |     [7..8]   firmware card ID (BE16; differs per card)    |
    |     [9]      pad                                          |
    |     [10..11] device-wide template bytes (our target)      |
    |     [12..15] trailing zeros                               |
    |                                                           |
    | Devices without effect cards return InvalidArgument for   |
    | any effectIdLo; we detect that as a non-positive result   |
    | and leave caps.has_effect_cards = false so the per-key    |
    | prep falls back to the Static-pass-through path.          |
    \*---------------------------------------------------------*/
    caps.has_effect_cards        = false;
    caps.effect_card_template[0] = 0;
    caps.effect_card_template[1] = 0;

    if(caps.idx_rgb_effects == 0 ||
       caps.rgb_feature_page != HIDPP20_FEAT_RGB_EFFECTS ||
       !device_online.load())
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Query card at effectIdLo=0, page 1. Any               |
    | valid card works; the template bytes are              |
    | device-wide and identical across every card           |
    | on the device, so using card 0 is simplest.           |
    \*-----------------------------------------------------*/
    uint8_t         query[5] = { 0xFF, 0x00, 0x01, 0x00, 0x01 };
    blankFAPmessage response;
    int             result = SendAckedIntoFAP(
                                  caps.idx_rgb_effects,
                                  FN_8071_GET_INFO,
                                  query, sizeof(query),
                                  response,
                                  HIDPP20_POLICY_PROBE);

    if(result <= 0)
    {
        LOG_DEBUG("%s DiscoverEffectCards: no effect cards on this device "
                  "(result=%d)", LOG_TAG, result);
        return;
    }

    caps.has_effect_cards        = true;
    caps.effect_card_template[0] = response.data[10];
    caps.effect_card_template[1] = response.data[11];

    LOG_INFO("%s Effect cards present: template bytes = 0x%02X 0x%02X "
             "(card firmware_id=0x%02X%02X, full data[0..15] = "
             "%02X %02X %02X %02X %02X %02X %02X %02X "
             "%02X %02X %02X %02X %02X %02X %02X %02X)",
             LOG_TAG,
             caps.effect_card_template[0], caps.effect_card_template[1],
             response.data[7], response.data[8],
             response.data[0], response.data[1], response.data[2],
             response.data[3], response.data[4], response.data[5],
             response.data[6], response.data[7], response.data[8],
             response.data[9], response.data[10], response.data[11],
             response.data[12], response.data[13], response.data[14],
             response.data[15]);
}

/*---------------------------------------------------------*\
| Feature 0x0620 Headset RGB Hostmode (Centurion G522 /     |
| PRO X 2). Separate feature from 0x8071/0x0600/0x8070;     |
| no effect cards, no SetSWControl, no power management.    |
|                                                           |
| Zone enumeration is best-effort from fn1 GetRGBZoneInfo.  |
| Falls back to {0x00, 0x01} (two earcups) if decode fails. |
\*---------------------------------------------------------*/
void LogitechHIDPP20Controller::DiscoverHeadsetRGBHostmode()
{
    caps.idx_headset_rgb_hostmode = GetFeatureIndex(HIDPP20_FEAT_HEADSET_RGB_HOSTMODE);

    if(caps.idx_headset_rgb_hostmode == 0)
    {
        return;
    }

    LOG_INFO("%s 0x0620 V%u Headset RGB Hostmode present at feature index %u",
             LOG_TAG,
             GetFeatureVersion(HIDPP20_FEAT_HEADSET_RGB_HOSTMODE),
             caps.idx_headset_rgb_hostmode);

    /*-----------------------------------------------------*\
    | Claim host mode (fn8) before reading zone info,       |
    | under firmware control GetRGBZoneInfo returns         |
    | count=0 (matches Solaar's discover_zones). Read       |
    | the prior mode (fn7) first and only claim if it       |
    | wasn't already host-controlled, so the restore        |
    | below puts it back exactly as we found it.            |
    \*-----------------------------------------------------*/
    blankFAPmessage hm_prior_resp;
    int prior_result = SendAckedIntoFAP(caps.idx_headset_rgb_hostmode,
                                        FN_0620_GET_HOST_MODE_STATE,
                                        nullptr, 0, hm_prior_resp);
    int  prior_mode  = (prior_result > 0) ? hm_prior_resp.data[0] : -1;
    bool claimed     = false;

    if(prior_mode != 0x01)
    {
        uint8_t         hostmode_on = 0x01;
        blankFAPmessage hm_claim_resp;
        if(SendAckedIntoFAP(caps.idx_headset_rgb_hostmode,
                            FN_0620_SET_HOST_MODE_STATE,
                            &hostmode_on, 1, hm_claim_resp) > 0)
        {
            claimed = true;
        }
    }

    /*-----------------------------------------------------*\
    | fn1 GetRGBZoneInfo: empty request, returns a zone     |
    | list. Packing not pinned, log the raw response.       |
    \*-----------------------------------------------------*/
    blankFAPmessage response;
    int result = SendAckedIntoFAP(caps.idx_headset_rgb_hostmode,
                                  FN_0620_GET_RGB_ZONE_INFO,
                                  nullptr, 0, response);

    caps.headset_rgb_hostmode_zone_ids.clear();

    if(result > 0)
    {
        LOG_INFO("%s 0x0620 fn1 GetRGBZoneInfo raw: "
                 "%02X %02X %02X %02X %02X %02X %02X %02X "
                 "%02X %02X %02X %02X %02X %02X %02X %02X",
                 LOG_TAG,
                 response.data[0], response.data[1], response.data[2],
                 response.data[3], response.data[4], response.data[5],
                 response.data[6], response.data[7], response.data[8],
                 response.data[9], response.data[10], response.data[11],
                 response.data[12], response.data[13], response.data[14],
                 response.data[15]);

        /*-------------------------------------------------*\
        | First-pass decode: byte 0 = zone count, bytes     |
        | 1..N = zone IDs. Bounds-check against the 16-byte |
        | data window. Refine once we see real G522 output. |
        \*-------------------------------------------------*/
        uint8_t zone_count = response.data[0];

        if(zone_count > 0 && zone_count <= 15)
        {
            for(uint8_t i = 0; i < zone_count; i++)
            {
                caps.headset_rgb_hostmode_zone_ids.push_back(response.data[1 + i]);
            }
        }
    }
    else
    {
        LOG_DEBUG("%s 0x0620 fn1 GetRGBZoneInfo failed (result=%d)",
                  LOG_TAG, result);
    }

    /*-----------------------------------------------------*\
    | Restore the prior host mode if we changed             |
    | it (Solaar's discover_zones finally-                  |
    | block). The apply path re-claims host mode            |
    | on first paint via ClaimSWControlIfNeeded.            |
    \*-----------------------------------------------------*/
    if(claimed && prior_mode >= 0)
    {
        uint8_t         restore_mode = (uint8_t)prior_mode;
        blankFAPmessage hm_restore_resp;
        SendAckedIntoFAP(caps.idx_headset_rgb_hostmode,
                         FN_0620_SET_HOST_MODE_STATE, &restore_mode, 1,
                         hm_restore_resp);
    }

    if(caps.headset_rgb_hostmode_zone_ids.empty())
    {
        LOG_INFO("%s 0x0620 zone decode produced 0 zones, falling back to "
                 "{0x00, 0x01} (two-earcup layout)", LOG_TAG);
        caps.headset_rgb_hostmode_zone_ids.push_back(0x00);
        caps.headset_rgb_hostmode_zone_ids.push_back(0x01);
    }

    /*-----------------------------------------------------*\
    | Synthesize a single zone cluster so the               |
    | existing RGBController zone UI lights up with         |
    | no special-casing. The 0x0620 path is static-         |
    | color-only; no effect cards, no per-key. One          |
    | cluster, one LED per discovered zone.                 |
    \*-----------------------------------------------------*/
    HIDPP20ZoneCluster cluster;
    cluster.index        = 0;
    cluster.location     = 0;
    cluster.effect_count = 0;
    caps.zone_clusters.clear();
    caps.zone_clusters.push_back(cluster);

    caps.is_headset_rgb_hostmode  = true;
    caps.has_zone_effects         = true;
    caps.rgb_feature_page         = HIDPP20_FEAT_HEADSET_RGB_HOSTMODE;

    /*-----------------------------------------------------*\
    | Pin device type to HEADSET. 0x0620 presence is a      |
    | headset signal and Centurion sub-devices              |
    | otherwise show type=0. DiscoverDeviceType ran         |
    | earlier in the probe sequence, so pin it here         |
    | where we have the evidence.                           |
    \*-----------------------------------------------------*/
    caps.device_type = LOGITECH_DEVICE_TYPE_HEADSET;

    LOG_INFO("%s 0x0620 ready: %zu zone(s), transient (FrameEnd 0x01) mode",
             LOG_TAG, caps.headset_rgb_hostmode_zone_ids.size());
}

void LogitechHIDPP20Controller::DiscoverPerKeyZones()
{
    /*-----------------------------------------------------*\
    | 0x8081 (V2) only: 0x8080 (V1) is a separate           |
    | feature (DiscoverPerKey8080). Probing it with         |
    | 0x8081 functions misreads its GetInfo as a            |
    | zone bitmap, sets has_perkey, and fires the           |
    | 0x8081 FrameEnd path at a device without it.          |
    \*-----------------------------------------------------*/
    caps.idx_perkey_v2 = GetFeatureIndex(HIDPP20_FEAT_PER_KEY_LIGHTING_V2);

    uint8_t perkey_idx = caps.idx_perkey_v2;

    if(perkey_idx == 0)
    {
        caps.has_perkey = false;
        return;
    }

    /*-----------------------------------------------------*\
    | Paginated GetInfo enumeration.                        |
    |                                                       |
    | typeOfInfo is a page index, not a redundant probe.    |
    | Per the 0x8081 spec the device's zone space is up to  |
    | 336 IDs organized as three pages of 112 bits each:    |
    |                                                       |
    | zone_id = (page * 112) + (byte * 8) + bit             |
    |                                                       |
    | An earlier version of this code only queried page 0   |
    | on the assumption that all pages echoed the same      |
    | data. That was wrong, G515 TKL happened to            |
    | concentrate its zones in page 0 so the bug was        |
    | invisible, but devices with G-keys, lightbars,        |
    | media, or logo LEDs report those zones in pages 1     |
    | and 2 and were being silently dropped.                |
    \*-----------------------------------------------------*/
    caps.perkey_zone_ids.clear();

    size_t page_counts[3] = { 0, 0, 0 };

    for(uint8_t page = 0; page < 3; page++)
    {
        /*-------------------------------------------------*\
        | Request body: uint16 BE typeOfInfo + 1 pad        |
        | byte. Short report carries the 3 bytes at         |
        | buf[4..6], so { 0x00, page, 0x00 } places         |
        | page in the low byte of the BE field.             |
        \*-------------------------------------------------*/
        uint8_t query[3] = { 0x00, page, 0x00 };
        blankFAPmessage response;
        int result = SendAckedIntoFAP(perkey_idx, FN_8081_GET_INFO,
                                      query, 3, response);

        if(result <= 0)
        {
            continue;
        }

        /*-------------------------------------------------*\
        | Parse 14-byte bitmap (bytes 2..15 of the          |
        | response). LSB-first bit order within             |
        | each byte. Skip zone 0 on page 0 (matches         |
        | prior behavior; zone 0 is not used).              |
        \*-------------------------------------------------*/
        const uint8_t* bitmap = response.data + 2;
        int start_bit = (page == 0) ? 1 : 0;

        for(int bit_in_page = start_bit; bit_in_page < 112; bit_in_page++)
        {
            int byte_idx = bit_in_page / 8;
            int bit_idx  = bit_in_page % 8;

            if(bitmap[byte_idx] & (1 << bit_idx))
            {
                uint16_t zone_id = (uint16_t)(page * 112 + bit_in_page);

                /*-----------------------------------------*\
                | Wire protocol 0x8081 Set* functions       |
                | take a uint8_t zone ID. Zones >255        |
                | from the bitmap formula can't             |
                | actually be addressed, drop them so       |
                | we don't expose phantom LEDs.             |
                \*-----------------------------------------*/
                if(zone_id > 255)
                {
                    LOG_WARNING("%s Per-key GetInfo page %u reported "
                                "unreachable zone %u (wire protocol "
                                "caps zones at 255); ignoring",
                                LOG_TAG, page, zone_id);
                    continue;
                }

                caps.perkey_zone_ids.push_back(zone_id);
                page_counts[page]++;
            }
        }
    }

    caps.has_perkey = !caps.perkey_zone_ids.empty();

    /*-----------------------------------------------------*\
    | Detect numpad presence from zone bitmask. Numpad      |
    | zones are 80-96 in Solaar's KEYCODES numbering.       |
    \*-----------------------------------------------------*/
    caps.has_numpad = false;

    for(uint16_t zid : caps.perkey_zone_ids)
    {
        if(zid >= 80 && zid <= 96)
        {
            caps.has_numpad = true;
            break;
        }
    }

    LOG_VERBOSE("%s Per-key zones discovered: %zu total "
                "(page0=%zu, page1=%zu, page2=%zu, numpad=%s)",
                LOG_TAG, caps.perkey_zone_ids.size(),
                page_counts[0], page_counts[1], page_counts[2],
                caps.has_numpad ? "yes" : "no");
}

/*---------------------------------------------------------*\
| fn2 GetKeyColors readback for one keyType, paginated in   |
| 14-entry frames. expected_key_count > 0 (fn1 keyCount)    |
| reads exactly ceil(count/14) pages, matching the official |
| app's walk. 0 is probe mode for the fn1-failure fallback: |
| page until a frame yields no live keyId, bounded by fn0   |
| maxKeyCount with a floor because maxKeyCount can be       |
| misreported as 0 (seen on the G810). Skips the 4-byte     |
| header, drops keyId==0 padding, appends live keyIds to    |
| out_ids, returns the count.                               |
\*---------------------------------------------------------*/
size_t LogitechHIDPP20Controller::ReadPerKey8080Colors
    (
    uint8_t                 feat_idx,
    uint16_t                key_type,
    uint16_t                expected_key_count,
    std::vector<uint8_t>&     out_ids,
    bool                    log_colors
    )
{
    size_t start_index = 0;
    size_t guard_pages;

    if(expected_key_count > 0)
    {
        guard_pages = ((size_t)expected_key_count + HIDPP20_8080_KEYS_PER_FRAME - 1)
                    / HIDPP20_8080_KEYS_PER_FRAME;
    }
    else
    {
        /*-------------------------------------------------*\
        | Probe mode: the found==0 break is the real        |
        | terminator, guard_pages only bounds the loop.     |
        \*-------------------------------------------------*/
        guard_pages = (caps.perkey_8080_max_key_count / HIDPP20_8080_KEYS_PER_FRAME) + 4;

        if(guard_pages < 16)
        {
            guard_pages = 16;
        }
    }

    for(size_t page = 0; page < guard_pages; page++)
    {
        uint8_t query[5] =
        {
            (uint8_t)(key_type    >> 8), (uint8_t)(key_type    & 0xFF),
            (uint8_t)(start_index >> 8), (uint8_t)(start_index & 0xFF),
            0x00    /* persistenceOptions: default volatile */
        };

        uint8_t recv[60] = {};
        int     result   = SendAcked(feat_idx, FN_8080_GET_KEY_COLORS, query, 5,
                                     recv, sizeof(recv), HIDPP20_POLICY_PROBE);

        if(result <= 8)
        {
            break;          /* headers only / error / empty */
        }

        /*-------------------------------------------------*\
        | result is the raw report length. Subtract the     |
        | 4-byte HID++ header, cap to the copied payload,   |
        | then skip fn2's own 4-byte response header. A     |
        | 64-byte 0x12 response carries exactly 14          |
        | entries; the old result-based math read one       |
        | entry past the buffer.                            |
        \*-------------------------------------------------*/
        size_t payload = (size_t)result - 4;

        if(payload > sizeof(recv))
        {
            payload = sizeof(recv);
        }

        size_t entries = (payload - 4) / 4;
        size_t found   = 0;

        for(size_t e = 0; e < entries; e++)
        {
            const uint8_t* entry  = recv + 4 + e * 4;
            uint8_t        key_id = entry[0];

            if(key_id == 0)
            {
                continue;   /* padding / empty slot */
            }

            out_ids.push_back(key_id);
            found++;

            if(log_colors)
            {
                LOG_DEBUG("%s     keyType 0x%04X keyId 0x%02X = RGB(%02X,%02X,%02X)",
                          LOG_TAG, key_type, key_id, entry[1], entry[2], entry[3]);
            }
        }

        if(found == 0)
        {
            break;          /* exhausted this keyType */
        }

        start_index += HIDPP20_8080_KEYS_PER_FRAME;
    }

    return out_ids.size();
}

/*---------------------------------------------------------*\
| Feature 0x8080 (Per Key Lighting) discovery. Separate     |
| from 0x8081: keys are (keyType u16 BE, keyId u8 = USB     |
| HID usage on the keyboard keyType); SetKeyColors rides    |
| the 0x12 very-long report. Walks the feature the way the  |
| official app does: fn0 typeFlags is a bitmask of single-  |
| bit keyTypes, fn1 GetKeyTypeInfo returns that keyType's   |
| keyCount (u16 BE at byte 0; NOT a keyType echo), and fn2  |
| GetKeyColors pages the keyIds 14 per frame only where     |
| keyCount >= 1. keyCount == 0 is a valid empty success.    |
| One deviation: on an fn1 failure we probe fn2 directly,   |
| because unlike the official app we have no out-of-band    |
| key list to fall back on. Read-only: fn0/fn1/fn2, all     |
| on dev.                                                   |
\*---------------------------------------------------------*/
void LogitechHIDPP20Controller::DiscoverPerKey8080()
{
    caps.idx_perkey_8080           = 0;
    caps.has_perkey_8080           = false;
    caps.perkey_8080_type_flags    = 0;
    caps.perkey_8080_type_count    = 0;
    caps.perkey_8080_max_key_count = 0;
    caps.perkey_8080_types.clear();

    /*-----------------------------------------------------*\
    | Discovery via IRoot, any version, no hardcoded index  |
    | fallback. If the device doesn't advertise 0x8080 the  |
    | path simply doesn't engage.                           |
    \*-----------------------------------------------------*/
    uint8_t idx = GetFeatureIndex(HIDPP20_FEAT_PER_KEY_LIGHTING_V1);

    if(idx == 0)
    {
        LOG_DEBUG("%s 0x8080 Per Key Lighting not advertised by this device", LOG_TAG);
        return;
    }

    caps.idx_perkey_8080 = idx;

    /*-----------------------------------------------------*\
    | fn0 GetInfo: typeFlags(u16 BE)@0, reserved@2,         |
    | keyTypeCount(u16 BE)@3, maxKeyCount(u16 BE)@5.        |
    \*-----------------------------------------------------*/
    uint8_t info[20] = {};
    int r0 = SendAcked(idx, FN_8080_GET_INFO, nullptr, 0,
                       info, sizeof(info), HIDPP20_POLICY_RELIABLE);

    if(r0 <= 0)
    {
        LOG_DEBUG("%s 0x8080 idx=0x%02X fn0 GetInfo failed (r=%d), "
                  "feature advertised but unreadable", LOG_TAG, idx, r0);
        return;
    }

    uint16_t raw_type_flags = (uint16_t)((info[0] << 8) | info[1]);
    uint16_t key_type_count = (uint16_t)((info[3] << 8) | info[4]);
    uint16_t max_key_count  = (uint16_t)((info[5] << 8) | info[6]);
    uint16_t type_flags     = raw_type_flags;

    /*-----------------------------------------------------*\
    | G910 over-reports typeFlags bit 0x2; mask the logged  |
    | / oracle value only. The walk still visits the raw    |
    | bit and lets fn1/fn2 decide, so a real media keyType  |
    | is never dropped by the mask.                         |
    \*-----------------------------------------------------*/
    if(caps.quirks & HIDPP20_QUIRK_8080_OVERREPORTS_MEDIA)
    {
        type_flags &= ~0x0002;
    }

    caps.perkey_8080_type_flags    = type_flags;
    caps.perkey_8080_type_count    = key_type_count;
    caps.perkey_8080_max_key_count = max_key_count;

    LOG_DEBUG("%s ===== 0x8080 Per Key Lighting structure dump =====", LOG_TAG);
    LOG_DEBUG("%s   idx=0x%02X  typeFlags=0x%04X  keyTypeCount=%u  maxKeyCount=%u",
              LOG_TAG, idx, type_flags, key_type_count, max_key_count);

    /*-----------------------------------------------------*\
    | Walk the set bits of typeFlags across the known       |
    | single-bit keyTypes (0x01 keyboard, 0x02 consumer/    |
    | media, 0x04 G-keys, 0x08 buttons, 0x10 logo, 0x40     |
    | indicators). Set bits outside the known six are       |
    | logged, never probed: the official app has no legend  |
    | for them either.                                      |
    \*-----------------------------------------------------*/
    static const uint16_t known_key_types[] =
    {
        0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0040
    };

    uint16_t known_mask = 0;

    for(uint16_t kt : known_key_types)
    {
        known_mask |= kt;
    }

    if(raw_type_flags & ~known_mask)
    {
        LOG_DEBUG("%s   typeFlags sets unknown bit(s) 0x%04X; not probed",
                  LOG_TAG, (uint16_t)(raw_type_flags & ~known_mask));
    }

    for(uint16_t key_type : known_key_types)
    {
        if(!(raw_type_flags & key_type))
        {
            continue;
        }

        /*-------------------------------------------------*\
        | fn1 GetKeyTypeInfo: keyCount(u16 BE)@0, param     |
        | bytes @2. keyCount == 0 is a valid empty success  |
        | (an advertised keyType with no keys), skip it     |
        | without treating it as an error.                  |
        \*-------------------------------------------------*/
        uint8_t q1[2]     = { (uint8_t)(key_type >> 8), (uint8_t)(key_type & 0xFF) };
        uint8_t resp1[16] = {};
        int  r1     = SendAcked(idx, FN_8080_GET_KEY_TYPE_INFO, q1, 2,
                                resp1, sizeof(resp1), HIDPP20_POLICY_PROBE);

        bool     fn1_ok    = (r1 >= 2);
        uint16_t key_count = fn1_ok ? (uint16_t)((resp1[0] << 8) | resp1[1]) : 0;

        if(fn1_ok && key_count == 0)
        {
            LOG_DEBUG("%s   keyType 0x%04X: fn1 keyCount=0, skipped",
                      LOG_TAG, key_type);
            continue;
        }

        std::vector<uint8_t> key_ids;
        size_t               live;

        if(fn1_ok)
        {
            live = ReadPerKey8080Colors(idx, key_type, key_count, key_ids, true);
        }
        else
        {
            /*---------------------------------------------*\
            | Deviation from the official app: it has an    |
            | out-of-band key list and can drop a keyType   |
            | whose fn1 fails, we cannot, so probe fn2      |
            | directly, bounded by fn0 maxKeyCount.         |
            \*---------------------------------------------*/
            LOG_DEBUG("%s   keyType 0x%04X: fn1 failed (r=%d), probing fn2 directly",
                      LOG_TAG, key_type, r1);
            live = ReadPerKey8080Colors(idx, key_type, 0, key_ids, true);
        }

        if(live == 0)
        {
            LOG_DEBUG("%s   keyType 0x%04X: advertised but no live keyIds%s",
                      LOG_TAG, key_type,
                      fn1_ok ? " (fn2 disagrees with fn1 keyCount)" : "");
            continue;
        }

        HIDPP20PerKey8080Type entry;
        entry.key_type = key_type;
        entry.param[0] = fn1_ok ? resp1[2] : 0;
        entry.param[1] = fn1_ok ? resp1[3] : 0;
        entry.param[2] = fn1_ok ? resp1[4] : 0;
        entry.key_ids  = key_ids;
        caps.perkey_8080_types.push_back(entry);

        LOG_DEBUG("%s   CONFIRMED keyType 0x%04X with %zu keyId(s)%s",
                  LOG_TAG, key_type, key_ids.size(),
                  fn1_ok ? "" : " (fn1 unavailable; fn2-probed)");
    }

    caps.has_perkey_8080 = !caps.perkey_8080_types.empty();

    /*-----------------------------------------------------*\
    | Numpad presence: full-size if any standard numpad HID |
    | usage (0x53 NumLock .. 0x63 Keypad .) was enumerated; |
    | drives FULL vs TKL matrix sizing in SetupZones8080.   |
    \*-----------------------------------------------------*/
    for(size_t t = 0; t < caps.perkey_8080_types.size() && !caps.has_numpad; t++)
    {
        for(uint8_t key_id : caps.perkey_8080_types[t].key_ids)
        {
            if(key_id >= 0x53 && key_id <= 0x63)
            {
                caps.has_numpad = true;
                break;
            }
        }
    }

    /*-----------------------------------------------------*\
    | Diagnostic only: count advertised vs confirmed        |
    | keyTypes. The per-keyType lines above localise any    |
    | gap (empty keyCount, fn1 failure, fn2 disagreement).  |
    | Compared against the quirk-masked oracle value so     |
    | known over-reporters don't warn every scan.           |
    \*-----------------------------------------------------*/
    unsigned int flag_count = 0;

    for(int bit = 0; bit < 16; bit++)
    {
        if(type_flags & (1u << bit))
        {
            flag_count++;
        }
    }

    if(caps.perkey_8080_types.size() != flag_count)
    {
        LOG_DEBUG("%s 0x8080 confirmed %zu keyType(s); typeFlags=0x%04X "
                  "advertises %u; see the per-keyType lines above for "
                  "which advertised keyType(s) yielded no keys.",
                  LOG_TAG, caps.perkey_8080_types.size(), type_flags, flag_count);
    }

    LOG_DEBUG("%s 0x8080 enumeration: %zu keyType(s) confirmed, has_perkey_8080=%s%s",
              LOG_TAG, caps.perkey_8080_types.size(),
              caps.has_perkey_8080 ? "yes" : "no",
              (dev_perkey_vl == nullptr)
                ? " (NOTE: no 0x12 very-long handle, read-only, cannot paint)" : "");
}

void LogitechHIDPP20Controller::DiscoverKeyboardLayout()
{
    uint8_t idx = GetFeatureIndex(HIDPP20_FEAT_KEYBOARD_LAYOUT);

    if(idx == 0)
    {
        caps.keyboard_layout_code = 0;
        return;
    }

    uint8_t recv_data[16] = {};
    int result = SendAcked(idx, 0x00, nullptr, 0, recv_data, sizeof(recv_data));

    if(result > 0)
    {
        caps.keyboard_layout_code = recv_data[0];
        LOG_DEBUG("%s Keyboard layout code: %d", LOG_TAG, caps.keyboard_layout_code);
    }
    else
    {
        caps.keyboard_layout_code = 0;
    }
}

/*---------------------------------------------------------*\
| Probe / Initialize / Shutdown                             |
\*---------------------------------------------------------*/

bool LogitechHIDPP20Controller::Probe()
{
    LOG_DEBUG("%s Probing device at %s (index=0x%02X)",
              LOG_TAG, location.c_str(), device_index);

    /*-----------------------------------------------------*\
    | Detect transport type from usage page                 |
    | before anything else. Centurion devices               |
    | need different framing for all commands.              |
    \*-----------------------------------------------------*/
    DiscoverTransport();

    /*-----------------------------------------------------*\
    | Flush any queued HID reports before probing. The      |
    | device may have unsolicited notifications (battery,   |
    | button events, etc.) sitting in the read buffer.      |
    \*-----------------------------------------------------*/
    {
        uint8_t flush_buf[64];
        int flushed = 0;

        while(flushed < 20)
        {
            int r = hid_read_timeout(dev, flush_buf, sizeof(flush_buf), 0);

            if(r <= 0)
            {
                break;
            }

            flushed++;
        }

        if(flushed > 0)
        {
            LOG_DEBUG("%s Flushed %d queued reports", LOG_TAG, flushed);
        }
    }

    /*-----------------------------------------------------*\
    | Test IRoot by looking up a known feature.             |
    |                                                       |
    | Standard HID++: look up FeatureSet (0x0001), must     |
    | exist. Centurion dongle: look up CentPPBridge         |
    | (0x0003), the dongle doesn't have FeatureSet, but     |
    | must have the bridge to reach the sub-device.         |
    |                                                       |
    | Retry up to 3 times, wireless devices behind          |
    | a shared receiver can return stale responses.         |
    \*-----------------------------------------------------*/
    uint8_t test_idx = 0;

    if(transport.type == HIDPP20_TRANSPORT_CENTURION)
    {
        /*-------------------------------------------------*\
        | Centurion: try CentPPBridge (0x0003) first        |
        | for dongle. If not found, try FeatureSet          |
        | (0x0001) for wired/direct connection where        |
        | the device IS the endpoint.                       |
        |                                                   |
        | This is the "is anyone there?" check, use the     |
        | fast- fail probe policy so non-Centurion or       |
        | unreachable devices bail in ~500ms instead of     |
        | ~6s. Once we have a positive response,            |
        | subsequent discovery uses reliable.               |
        \*-------------------------------------------------*/
        test_idx = GetFeatureIndex(HIDPP20_FEAT_CENTPPBRIDGE, HIDPP20_POLICY_PROBE);

        if(test_idx != 0)
        {
            transport.bridge_feat_idx = test_idx;
            transport.sub_device_id   = 0;

            LOG_DEBUG("%s CentPPBridge at index %d: routing to sub-device",
                      LOG_TAG, test_idx);

            /*---------------------------------------------*\
            | Pre-check sub-device availability via         |
            | getConnectionInfo (CentPPBridge fn0).         |
            | The vendor app does this and refuses          |
            | to call sendFragment when MTU=0.              |
            |                                               |
            | Response format (from protocol doc            |
            | line 910-917): Byte 0: high nibble =          |
            | connection type/state low nibble +            |
            | Byte 1 = sub-device data length /             |
            | MTU Bytes 2+: sub-device descriptors          |
            |                                               |
            | If MTU == 0, no sub-device is connected.      |
            | Calling sendFragment in that state triggers   |
            | an undocumented error code 0x0B and wastes    |
            | the full retry budget. Skip enumeration and   |
            | let the dongle-watcher path take over until   |
            | ConnectionStateChangedEvent fires.            |
            |                                               |
            | Bridge is confirmed responsive at this point; |
            | use reliable policy for the MTU check.        |
            \*---------------------------------------------*/
            uint8_t mtu_recv[16] = {};
            int mtu_result = SendAcked(test_idx, 0x00,
                                       nullptr, 0,
                                       mtu_recv, sizeof(mtu_recv));

            if(mtu_result > 0)
            {
                transport.bridge_mtu =
                    ((uint16_t)(mtu_recv[0] & 0x0F) << 8) | mtu_recv[1];

                LOG_DEBUG("%s CentPPBridge MTU=%u (%s)",
                          LOG_TAG, transport.bridge_mtu,
                          transport.bridge_mtu > 0 ? "sub-device present"
                                                   : "no sub-device");

                if(transport.bridge_mtu == 0)
                {
                    /*-------------------------------------*\
                    | No sub-device, skip enumeration.      |
                    | Mark the feature map complete so      |
                    | on-demand lookups don't hit the       |
                    | wire. The dongle will be              |
                    | registered as a watcher and the       |
                    | sub-device will be probed when        |
                    | ConnectionStateChangedEvent fires.    |
                    \*-------------------------------------*/
                    caps.feature_map_complete = true;
                    DiscoverDeviceName();
                    return true;
                }
            }
            else
            {
                LOG_DEBUG("%s CentPPBridge getConnectionInfo failed (result=%d)",
                          LOG_TAG, mtu_result);
            }
        }
        else
        {
            LOG_DEBUG("%s No CentPPBridge: Centurion direct connection", LOG_TAG);
            test_idx = GetFeatureIndex(HIDPP20_FEAT_FEATURE_SET, HIDPP20_POLICY_PROBE);
        }
    }
    else
    {
        /*-------------------------------------------------*\
        | Standard HID++: probe FeatureSet (0x0001):        |
        | fast-fail. The probe policy already includes      |
        | its own retry; the outer loop is preserved for    |
        | buffer-flushing behavior between attempts.        |
        \*-------------------------------------------------*/
        for(int attempt = 0; attempt < 3 && test_idx == 0; attempt++)
        {
            if(attempt > 0)
            {
                uint8_t retry_buf[64];

                while(hid_read_timeout(dev, retry_buf, sizeof(retry_buf), 10) > 0)
                {
                }

                LOG_DEBUG("%s IRoot retry %d at %s", LOG_TAG, attempt + 1, location.c_str());
            }

            test_idx = GetFeatureIndex(HIDPP20_FEAT_FEATURE_SET, HIDPP20_POLICY_PROBE);
        }
    }

    if(test_idx == 0)
    {
        LOG_DEBUG("%s IRoot probe failed at %s, device does not respond",
                  LOG_TAG, location.c_str());
        return false;
    }

    /*-----------------------------------------------------*\
    | If retries were needed, flush delayed responses from  |
    | failed attempts before continuing with discovery.     |
    \*-----------------------------------------------------*/
    {
        uint8_t post_buf[64];

        while(hid_read_timeout(dev, post_buf, sizeof(post_buf), 10) > 0)
        {
        }
    }

    /*-----------------------------------------------------*\
    | Enumerate all features in bulk. For standard          |
    | HID++, uses FeatureSet GetCount + GetFeatureId        |
    | loop. For Centurion sub-devices, uses bulk            |
    | GetFeatureId (single response). After this,           |
    | GetFeatureIndex uses the map, no wire traffic.        |
    \*-----------------------------------------------------*/
    {
        /*-------------------------------------------------*\
        | For Centurion bridged, FeatureSet is at index 1   |
        | on the sub-device. For standard HID++, test_idx   |
        | is the FeatureSet index from the IRoot probe.     |
        \*-------------------------------------------------*/
        uint8_t fs_idx = (transport.type == HIDPP20_TRANSPORT_CENTURION &&
                          transport.bridge_feat_idx != 0)
                       ? 1    // CenturionFeatureSet always at index 1 on sub-device
                       : test_idx;

        EnumerateFeatures(fs_idx);

        /*-------------------------------------------------*\
        | If bridged and bulk enumeration failed, the       |
        | sub-device isn't reachable (e.g., headset off     |
        | or on USB cable). Mark map as complete so         |
        | lookups don't hit the wire. The device will be    |
        | discovered with no features; it can be re-        |
        | probed when the sub-device comes online.          |
        \*-------------------------------------------------*/
        if(transport.bridge_feat_idx != 0 && !caps.feature_map_complete)
        {
            LOG_DEBUG("%s Sub-device not reachable through bridge, dongle only",
                      LOG_TAG);
            caps.feature_map_complete = true;
        }
    }

    /*-----------------------------------------------------*\
    | Discover device identity. On Centurion with           |
    | bridge, this now queries the sub-device               |
    | (headset) through the bridge, not the dongle.         |
    \*-----------------------------------------------------*/
    DiscoverDeviceName();
    log_tag = "[LogitechHID++ " + caps.device_name + "]";
    DiscoverDeviceType();
    DiscoverFirmwareInfo();

    /*-----------------------------------------------------*\
    | Discover profile management features                  |
    \*-----------------------------------------------------*/
    caps.idx_profile_management     = GetFeatureIndex(HIDPP20_FEAT_PROFILE_MANAGEMENT);
    caps.idx_onboard_profiles       = GetFeatureIndex(HIDPP20_FEAT_ONBOARD_PROFILES);
    caps.idx_disable_keys_by_usage  = GetFeatureIndex(HIDPP20_FEAT_DISABLE_KEYS_BY_USAGE);

    /*-----------------------------------------------------*\
    | Discover RGB capabilities                             |
    \*-----------------------------------------------------*/
    DiscoverRGBEffects();
    if(caps.idx_rgb_effects == 0)
    {
        DiscoverHeadsetRGBHostmode();
    }
    DiscoverPerKeyZones();
    DiscoverPerKey8080();
    DiscoverKeyboardLayout();

    /*-----------------------------------------------------*\
    | Probe WirelessStatus (0x1D4B) for reconnect           |
    | detection. Lightspeed devices behind kernel-          |
    | managed receivers send WirelessStatus events          |
    | when they reconnect after power cycle. Cache          |
    | the feature index so the reader thread can            |
    | detect these events without sending commands.         |
    \*-----------------------------------------------------*/
    caps.idx_wireless_status = GetFeatureIndex(HIDPP20_FEAT_WIRELESS_STATUS);

    /*-----------------------------------------------------*\
    | Diagnostic: log the OpenRGB-relevant                  |
    | feature/version map (solaar-show-style)               |
    | for firmware-mismatch identification.                 |
    \*-----------------------------------------------------*/
    LogFeatureSummary();

    if(!caps.has_zone_effects && !caps.has_perkey && !caps.has_perkey_8080)
    {
        LOG_DEBUG("%s %s: no RGB features found", LOG_TAG, caps.device_name.c_str());

        /*-------------------------------------------------*\
        | Centurion dongles with a bridge stay alive        |
        | to watch for sub-device connection events,        |
        | even without RGB.                                 |
        \*-------------------------------------------------*/
        if(transport.bridge_feat_idx != 0)
        {
            return true;
        }

        return false;
    }

    LOG_VERBOSE("%s %s: zones=%zu perkey=%zu",
                LOG_TAG, caps.device_name.c_str(),
                caps.zone_clusters.size(), caps.perkey_zone_ids.size());

    return true;
}

std::string LogitechHIDPP20Controller::ProbeIdentity()
{
    LOG_DEBUG("%s Identifying device at %s (index=0x%02X)", LOG_TAG, location.c_str(), device_index);

    DiscoverTransport();

    FlushResponseQueue();

    /*-----------------------------------------------------*\
    | Nothing else is worth asking until IRoot answers.     |
    \*-----------------------------------------------------*/
    if(GetFeatureIndex(HIDPP20_FEAT_FEATURE_SET, HIDPP20_POLICY_PROBE) == 0)
    {
        return "";
    }

    DiscoverFirmwareInfo();

    /*-----------------------------------------------------*\
    | Centurion nodes have no FirmwareInfo unitId;          |
    | they identify by their DeviceInfo serial. A           |
    | node that answers but stores no serial (PRO           |
    | X 2 dongle) books under its path, so it               |
    | cannot be recognized over a second link.              |
    \*-----------------------------------------------------*/
    if(caps.unit_id.empty())
    {
        if(!caps.serial_number.empty())
        {
            return caps.serial_number;
        }

        if(transport.type == HIDPP20_TRANSPORT_CENTURION)
        {
            LOG_DEBUG("%s No stored serial; booking by node path", LOG_TAG);
            return location + "#node";
        }
    }

    return caps.unit_id;
}

std::string LogitechHIDPP20Controller::ProbeName()
{
    DiscoverDeviceName();

    /*-----------------------------------------------------*\
    | DiscoverDeviceName falls back to a placeholder        |
    | when the device has nothing to say. Placeholders      |
    | are for the device list, where they are replaced      |
    | the moment a real name turns up. They are not         |
    | identities: a caller writing one into settings        |
    | would leave behind an entry naming nothing.           |
    \*-----------------------------------------------------*/
    if(caps.device_name == HIDPP20_NAME_PLACEHOLDER_STD
    || caps.device_name == HIDPP20_NAME_PLACEHOLDER_CENTURION
    || !LogitechHIDPP20Controller::NameLooksReal(caps.device_name))
    {
        return "";
    }

    return caps.device_name;
}

void LogitechHIDPP20Controller::Initialize()
{
    /*-----------------------------------------------------*\
    | No device state changes here, let firmware effects    |
    | keep running until DeviceUpdateMode claims control    |
    | with real colors ready via ClaimSWControlIfNeeded().  |
    \*-----------------------------------------------------*/
    init_generation++;
    initialized = true;
}

void LogitechHIDPP20Controller::Shutdown()
{
    if(!initialized)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Sender first: no frame may be mid-flight              |
    | while SW control is released below.                   |
    \*-----------------------------------------------------*/
    StopSenderThread();
    StopPowerManager();

    /*-----------------------------------------------------*\
    | Release SW control                                    |
    \*-----------------------------------------------------*/
    if(caps.idx_rgb_effects != 0)
    {
        SetSWControl(0, 0);
    }

    /*-----------------------------------------------------*\
    | Restore firmware mode (skip if we never left onboard) |
    \*-----------------------------------------------------*/
    if(!(caps.quirks & HIDPP20_QUIRK_KEEP_ONBOARD_MODE))
    {
        if(caps.idx_profile_management != 0)
        {
            uint8_t data[1] = { 0x03 };
            blankFAPmessage response;
            SendAckedIntoFAP(caps.idx_profile_management, FN_8101_GET_SET_MODE,
                             data, 1, response);
        }
        else if(caps.idx_onboard_profiles != 0)
        {
            uint8_t data[1] = { 0x01 };
            blankFAPmessage response;
            SendAckedIntoFAP(caps.idx_onboard_profiles, FN_8100_SET_ONBOARD_MODE,
                             data, 1, response);
        }
    }

    /*-----------------------------------------------------*\
    | Release 0x0620 Headset RGB hostmode claim.            |
    | Best-effort; pairs with the SetHostModeState(1)       |
    | claim in ClaimSWControlIfNeeded.                      |
    \*-----------------------------------------------------*/
    if(caps.idx_headset_rgb_hostmode != 0)
    {
        uint8_t         off          = 0x00;
        blankFAPmessage release_response;
        SendAckedIntoFAP(caps.idx_headset_rgb_hostmode,
                         FN_0620_SET_HOST_MODE_STATE,
                         &off, 1, release_response);
    }

    initialized = false;
}

/*---------------------------------------------------------*\
| Accessors                                                 |
\*---------------------------------------------------------*/

const HIDPP20DeviceCapabilities& LogitechHIDPP20Controller::GetCapabilities() const
{
    return caps;
}

std::string LogitechHIDPP20Controller::GetDeviceLocation()
{
    return "HID: " + location;
}

std::string LogitechHIDPP20Controller::GetSerialString()
{
    return caps.serial_number;
}

uint32_t LogitechHIDPP20Controller::GetInitGeneration() const
{
    return init_generation;
}

/*---------------------------------------------------------*\
| SW Control and Power                                      |
\*---------------------------------------------------------*/

int LogitechHIDPP20Controller::SetSWControl(uint8_t mode, uint8_t flags)
{
    if(caps.idx_rgb_effects == 0)
    {
        return 0;
    }

    blankFAPmessage response;
    int             result;

    if(caps.sw_control_simple)
    {
        /*-------------------------------------------------*\
        | 0x8070: simple [enabled, persist]. the            |
        | official app/legacy send [1,1] to claim and       |
        | [0,0] to release, persist mirrors enabled.        |
        \*-------------------------------------------------*/
        uint8_t en = (uint8_t)(mode > 0 ? 0x01 : 0x00);
        uint8_t data[2] = { en, en };
        result = SendAckedIntoFAP(caps.idx_rgb_effects, caps.fn_sw_control,
                                  data, 2, response);
    }
    else
    {
        /*-------------------------------------------------*\
        | 0x8071/0x0600: [0x01(set), mode, flags]           |
        \*-------------------------------------------------*/
        uint8_t data[3] = { 0x01, mode, flags };
        result = SendAckedIntoFAP(caps.idx_rgb_effects, caps.fn_sw_control,
                                  data, 3, response);
    }

    LOG_DEBUG("%s SetSWControl mode=%d flags=0x%02X result=%d",
              LOG_TAG, mode, flags, result);

    return result;
}

void LogitechHIDPP20Controller::SetRGBPowerMode(uint8_t mode)
{
    if(caps.idx_rgb_effects == 0 || !caps.has_power_mgmt)
    {
        return;
    }

    uint8_t data[2] = { 0x01, mode };
    blankFAPmessage response;
    SendAckedIntoFAP(caps.idx_rgb_effects, caps.fn_pwr_mode,
                     data, 2, response);

    LOG_DEBUG("%s SetRGBPowerMode mode=%d", LOG_TAG, mode);
}

void LogitechHIDPP20Controller::SetHostMode()
{
    if(caps.idx_profile_management != 0)
    {
        uint8_t data[1] = { 0x05 };
        blankFAPmessage response;
        SendAckedIntoFAP(caps.idx_profile_management, FN_8101_GET_SET_MODE,
                         data, 1, response);

        LOG_DEBUG("%s ProfileManagement set to host mode", LOG_TAG);
    }
    else if(caps.idx_onboard_profiles != 0)
    {
        uint8_t         set_data[1] = { 0x02 };
        blankFAPmessage set_response;
        SendAckedIntoFAP(caps.idx_onboard_profiles, FN_8100_SET_ONBOARD_MODE,
                         set_data, 1, set_response);

        LOG_DEBUG("%s OnboardProfiles set to host mode", LOG_TAG);
    }
}

bool LogitechHIDPP20Controller::ClaimSWControlIfNeeded()
{
    if(sw_control_claimed)
    {
        return true;
    }

    /*-----------------------------------------------------*\
    | 0x0620 headset (G522/G321/G315): the claim is         |
    | a single SetHostModeState(1); none of the             |
    | 0x8070/0x8071/0x0600 paths apply. Sticky; wake        |
    | resets the flag so first paint re-claims.             |
    | Released by SetHostModeState(0) in Shutdown.          |
    \*-----------------------------------------------------*/
    if(caps.idx_headset_rgb_hostmode != 0)
    {
        if(!device_online.load())
        {
            return false;
        }

        uint8_t         on   = 0x01;
        blankFAPmessage resp;
        int result = SendAckedIntoFAP(caps.idx_headset_rgb_hostmode,
                                      FN_0620_SET_HOST_MODE_STATE, &on, 1, resp);

        if(result <= 0)
        {
            LOG_DEBUG("%s 0x0620 SetHostModeState(1) claim failed (result=%d)",
                      LOG_TAG, result);
            return false;
        }

        sw_control_claimed = true;
        LOG_DEBUG("%s 0x0620 host mode claimed", LOG_TAG);
        return true;
    }

    /*-----------------------------------------------------*\
    | 0x8070 per-key keyboards (those exposing 0x8080: G810 |
    | family) write per-key directly with NO claim, like    |
    | the legacy controllers. FIXME: a partial SetSWControl |
    | claim breaks per-key on the G810; the full sequence   |
    | is host-mode -> SetSWControl(1,1) -> SetZoneEffect    |
    | off per zone -> per-key. Test on real hardware which  |
    | parts each model supports before claiming here.       |
    \*-----------------------------------------------------*/
    if(caps.rgb_feature_page == HIDPP20_FEAT_COLOR_LED_EFFECTS
       && caps.idx_perkey_8080 != 0)
    {
        return true;
    }

    if(caps.idx_rgb_effects == 0 || !device_online.load())
    {
        return false;
    }

    /*-----------------------------------------------------*\
    | 0x8070 ZONE-ONLY devices (no 0x8080: G560/G933,       |
    | 0x8070 mice): the claim is just SetSWControl(1,1),    |
    | the official app and Solaar both do exactly this,     |
    | with no power/host-mode tail. No per-key layer to     |
    | conflict, so the zone layer is not suppressed.        |
    \*-----------------------------------------------------*/
    if(caps.rgb_feature_page == HIDPP20_FEAT_COLOR_LED_EFFECTS)
    {
        int sw_result = SetSWControl(1, 1);

        if(sw_result <= 0)
        {
            LOG_DEBUG("%s 0x8070 SetSWControl(1,1) claim failed (result=%d)",
                      LOG_TAG, sw_result);
            return false;
        }

        sw_control_claimed = true;
        return true;
    }

    /*-----------------------------------------------------*\
    | Two-phase claim to avoid the visible                  |
    | onboard->host flash.                                  |
    |                                                       |
    | SW Control flags bits (from a wire capture, the       |
    | doc's overview mislabels them): bit 0 = effect        |
    | control, bit 1 = power management, bit 2 = NV         |
    | config. Setting the effect bit suspends the firmware  |
    | effect engine, and anything not painted since         |
    | renders as a firmware default (the 0xFFFFFF per-key   |
    | buffer, or the saved effect card on bad input).       |
    |                                                       |
    | So claim with flags=6: the firmware engine keeps      |
    | rendering until we replace its output. We cannot      |
    | stay there, the idle/wake machine needs flags=5/3     |
    | as its active/idle signals, so after the first        |
    | complete per-key frame, when per-key masks zone       |
    | output anyway, upgrade 6->5 invisibly.                |
    |                                                       |
    | No power-timer writes at claim: our                   |
    | StartSleep trigger fires SetRgbPowerMode(3)           |
    | explicitly when a fade is wanted.                     |
    |                                                       |
    | Fire the whole claim back-to-back, no per-command ACK |
    | wait; VerifyClaimPipeline reconciles it.              |
    \*-----------------------------------------------------*/
    claim_pipeline_.clear();
    pipelining_claim_ = true;

    int claim_result = SetSWControl(3, 6);

    if(claim_result <= 0)
    {
        pipelining_claim_ = false;
        claim_pipeline_.clear();
        LOG_DEBUG("%s SW control claim failed (SetSWControl(3,6) result=%d)",
                  LOG_TAG, claim_result);
        return false;
    }

    /*-----------------------------------------------------*\
    | Keyboard-family handshake on feature 0x4522           |
    | (DisableKeysByUsage). G815 / G915 / G Pro send        |
    | this fn3 + fn1 empty-payload pair between             |
    | SetSWControl and the first mode write.                |
    | Feature-gated inside, no-op on G502 / G515.           |
    \*-----------------------------------------------------*/
    DoDisableKeysByUsageHandshake();

    SetRGBPowerMode(1);
    WritePowerConfig(idle_timeout_s, sleep_timeout_s);

    /*-----------------------------------------------------*\
    | KEEP_ONBOARD_MODE (G915 TKL): host mode breaks the    |
    | F-row (onboard-profile-mapped G-keys); per-key        |
    | works onboard.                                        |
    \*-----------------------------------------------------*/
    if(caps.quirks & HIDPP20_QUIRK_KEEP_ONBOARD_MODE)
    {
        LOG_INFO("%s KEEP_ONBOARD_MODE quirk: skipping onboard->host switch",
                 LOG_TAG);
    }
    else
    {
        SetHostMode();
    }

    /*-----------------------------------------------------*\
    | Reconcile the batch's ACKs; blocking-retry any miss.  |
    \*-----------------------------------------------------*/
    pipelining_claim_ = false;
    VerifyClaimPipeline();

    written_idle_s  = idle_timeout_s;
    written_sleep_s = sleep_timeout_s;

    sw_control_claimed            = true;
    sw_control_needs_upgrade_to_5 = true;
    prep_applied                  = false;

    LOG_DEBUG("%s Claimed SW control at flags=6 "
              "(effect engine still autonomous until first per-key frame)",
              LOG_TAG);
    return true;
}

void LogitechHIDPP20Controller::VerifyClaimPipeline()
{
    /*-----------------------------------------------------*\
    | Reconcile the pipelined claim's ACKs. Acks arrive     |
    | interleaved across features, so match each echoed     |
    | (feat, func, sw_id) to any outstanding pending, not   |
    | in send order; re-send an unmatched one blocking.     |
    \*-----------------------------------------------------*/
    std::vector<bool> acked(claim_pipeline_.size(), false);
    size_t            need     = claim_pipeline_.size();
    size_t            got      = 0;
    size_t            max_read = claim_pipeline_.size() + 4;   /* slack for stray frames */

    for(size_t r = 0; r < max_read && got < need; r++)
    {
        uint8_t rfeat     = 0;
        uint8_t rfunc     = 0;
        uint8_t rdata[60] = {};
        int     rd        = ReadMessage(&rfeat, &rfunc, rdata, sizeof(rdata), 60);

        if(rd <= 0)
        {
            break;   /* no more responses within the window */
        }
        if((rfunc & 0x0F) != HIDPP20_SW_ID)
        {
            continue;   /* not one of our acks */
        }

        for(size_t i = 0; i < claim_pipeline_.size(); i++)
        {
            if(!acked[i]
               && rfeat == claim_pipeline_[i].feat
               && (rfunc & 0xF0) == (claim_pipeline_[i].func & 0xF0))
            {
                acked[i] = true;
                got++;
                break;
            }
        }
    }

    for(size_t i = 0; i < claim_pipeline_.size(); i++)
    {
        if(!acked[i])
        {
            LOG_DEBUG("%s claim pipeline: re-sending feat=0x%02X func=0x%02X blocking",
                      LOG_TAG, claim_pipeline_[i].feat, claim_pipeline_[i].func);
            blankFAPmessage response;
            SendAckedIntoFAP(claim_pipeline_[i].feat, claim_pipeline_[i].func,
                             claim_pipeline_[i].data, claim_pipeline_[i].len, response);
        }
    }

    claim_pipeline_.clear();
}

void LogitechHIDPP20Controller::UpgradeSwControlAfterFirstPaint()
{
    /*-----------------------------------------------------*\
    | Called by                                             |
    | RGBController_LogitechHIDPP20::DeviceUpdateLEDs       |
    | immediately after the first successful PerKeyFrameEnd |
    | of a newly-claimed session. At this point the per-key |
    | buffer is populated with real host colors, so the     |
    | per-key layer masks the zone layer, so the 6->5       |
    | transition does not expose the firmware's default     |
    | LED buffer. Upgrading to flags=5 puts the device into |
    | the "active steady state" that OnUserActivity expects |
    | for idle detection events.                            |
    \*-----------------------------------------------------*/
    if(!sw_control_needs_upgrade_to_5)
    {
        return;
    }

    if(caps.idx_rgb_effects == 0 || !device_online.load())
    {
        sw_control_needs_upgrade_to_5 = false;
        return;
    }

    int result = SetSWControl(3, 5);

    if(result > 0)
    {
        sw_control_needs_upgrade_to_5 = false;
        LOG_DEBUG("%s Upgraded SW control to flags=5 "
                  "(per-key layer now masks zone layer)", LOG_TAG);
    }
    else
    {
        LOG_DEBUG("%s SW control upgrade to flags=5 failed (result=%d)",
                  LOG_TAG, result);
        /* Leave the flag set so the next frame will retry. */
    }
}

void LogitechHIDPP20Controller::DoDisableKeysByUsageHandshake()
{
    /*-----------------------------------------------------*\
    | G815 / G915 / G Pro keyboards send this two-call      |
    | handshake on feature 0x4522 (DisableKeysByUsage)      |
    | before any mode change or per-key write. The          |
    | original OpenRGB G815 + G915 controllers both do it   |
    | in their BeginModeSet and InitializeDirect paths.     |
    | Both payloads are empty; bare function calls,         |
    | suggesting they're state reads used as a firmware     |
    | sync point, not actual disable-keys writes (those     |
    | would require a keyset in the payload).               |
    |                                                       |
    | Feature-gated: caps.idx_disable_keys_by_usage         |
    | is only non-zero on devices that enumerate            |
    | 0x4522. G502 and G515 do not enumerate it, so         |
    | this is a no-op on those.                             |
    \*-----------------------------------------------------*/
    if(caps.idx_disable_keys_by_usage == 0 || !device_online.load())
    {
        return;
    }

    blankFAPmessage response;
    SendAckedIntoFAP(caps.idx_disable_keys_by_usage, 0x30,
                     nullptr, 0, response, HIDPP20_POLICY_PROBE);
    SendAckedIntoFAP(caps.idx_disable_keys_by_usage, 0x10,
                     nullptr, 0, response, HIDPP20_POLICY_PROBE);

    LOG_DEBUG("%s 0x4522 DisableKeysByUsage handshake sent (fn3 + fn1)", LOG_TAG);
}

/*---------------------------------------------------------*\
| Observed per-key prep sequence                            |
|                                                           |
| Two SetEffectByIndex calls cloned byte-for-byte from a    |
| wire capture of the vendor app talking to a G502 X PLUS   |
| (wired-ish connection via Lightspeed receiver). The two   |
| frames are:                                               |
|                                                           |
|   First, ~260ms after SetOnboardMode(02):                 |
|     1101091a ff 02 00 00 00 00 00 00 20 64 00 00 01 ...   |
|     RgbEffects.SetEffectByIndex                           |
|       cluster=0xFF (all clusters)                         |
|       effectIdx=0x02 (Breathing on G502's enumerated set) |
|       params=[00 00 00 00 00 00 20 64 00 00] (10 bytes)   |
|         positions [6]=0x20, [7]=0x64 are non-zero. The    |
|         Breathing effect parameter layout documented in   |
|         the protocol reference has period/brightness in   |
|         those slots, but the exact meaning of these two   |
|         values in this context is NOT understood. The     |
|         vendor app sends them verbatim on every claim; we |
|         mirror.                                           |
|       persist=0x01                                        |
|                                                           |
|   Second, ~95ms after the first:                          |
|     1101091a ff 04 00 00 00 00 00 00 00 00 00 00 01 ...   |
|     RgbEffects.SetEffectByIndex                           |
|       cluster=0xFF                                        |
|       effectIdx=0x04: OUT OF RANGE on G502 X PLUS (the    |
|         device only enumerates effects 0..3 via           |
|         GetEffectInfo). Likely a "custom / direct mode"   |
|         slot the firmware accepts but does not advertise  |
|         through the normal enumeration.                   |
|       params=[00 x 10]                                    |
|       persist=0x01                                        |
|                                                           |
| The function we call is caps.fn_set_effect (0x10 on       |
| 0x8071, 0x30 on 0x8070, same as SetZoneEffect uses).      |
|                                                           |
| The values are not derived from the effect param layout   |
| tables: they are observed-working bytes, and that is the  |
| contract. DeviceUpdateLEDs gates which devices run this.  |
\*---------------------------------------------------------*/

void LogitechHIDPP20Controller::DoObservedPerKeyPrep()
{
    if(caps.idx_rgb_effects == 0 || !device_online.load())
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Prep1: SetEffectByIndex(cluster=0xFF,                 |
    | effectIdx=2, params) with the device-wide             |
    | template bytes at params[6..7].                       |
    |                                                       |
    | The template bytes are discovered at feature-         |
    | discovery time via GetEffectSpecificInfo on any       |
    | firmware effect card; the vendor app does the same    |
    | read-then-echo pattern, and on a G502 X PLUS the      |
    | read value is 0x20 0x64 across every card. We         |
    | don't know what those bytes mean semantically,        |
    | just that the device expects to see them echoed       |
    | back verbatim in this position when priming the       |
    | firmware effect engine for per-key takeover.          |
    \*-----------------------------------------------------*/
    uint8_t prep1[16] =
    {
        0xFF, 0x02,                                 /* cluster, effectIdx */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        /* params[0..5] */
        caps.effect_card_template[0],              /* params[6], device */
        caps.effect_card_template[1],              /* params[7], device */
        0x00, 0x00,                                 /* params[8..9] */
        0x01,                                       /* persist */
        0x00, 0x00, 0x00                           /* padding */
    };
    blankFAPmessage prep1_resp;
    SendAckedIntoFAP(caps.idx_rgb_effects, caps.fn_set_effect,
                     prep1, 16, prep1_resp);

    /*-----------------------------------------------------*\
    | Prep2: SetEffectByIndex at the first out-of-range     |
    | slot above the last enumerated effect, all params     |
    | zero (G502: effectIdx=4, matching the capture).       |
    | Best understanding: an unadvertised custom/direct-    |
    | mode slot, without writing it the per-key pipeline    |
    | does not engage and the firmware renders the saved    |
    | effect card during the claim->paint window.           |
    \*-----------------------------------------------------*/
    uint8_t num_effects = 0;

    if(!caps.zone_clusters.empty())
    {
        size_t count = caps.zone_clusters[0].effects.size();
        num_effects  = (count > 0xFFu) ? 0xFFu : (uint8_t)count;
    }

    uint8_t prep2[16] =
    {
        0xFF, num_effects,                          /* cluster, first OOR slot */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        /* params[0..5] */
        0x00, 0x00, 0x00, 0x00,                    /* params[6..9] */
        0x01,                                       /* persist */
        0x00, 0x00, 0x00                           /* padding */
    };
    blankFAPmessage prep2_resp;
    SendAckedIntoFAP(caps.idx_rgb_effects, caps.fn_set_effect,
                     prep2, 16, prep2_resp);

    LOG_DEBUG("%s DoObservedPerKeyPrep: prep1 template=0x%02X%02X "
              "prep2 idx=%u (OOR slot above %u enumerated effects)",
              LOG_TAG,
              caps.effect_card_template[0], caps.effect_card_template[1],
              num_effects, num_effects);
}

void LogitechHIDPP20Controller::DoKeyboardFamilyPerKeyPrep()
{
    /*-----------------------------------------------------*\
    | G815 / G915 / G Pro per-key takeover prep, cloned     |
    | from the InitializeDirect sequence in their legacy    |
    | OpenRGB controllers. Three steps after the            |
    | claim-time 0x4522 handshake (which fires from         |
    | ClaimSWControlIfNeeded):                              |
    |                                                       |
    | 1. For each enumerated cluster, SetEffectByIndex      |
    |    with effectIdx=0 (Off) and persist=1. This         |
    |    deactivates the firmware effect engine per         |
    |    cluster, unlike the G515 static-black fallback     |
    |    which leaves the engine running with a black       |
    |    static color.                                      |
    |                                                       |
    | 2. Send a primer SetIndividualRgbZones write          |
    |    covering one zone (the first enumerated) at        |
    |    black. G915 uses Escape specifically; we use       |
    |    the first enumerated zone for portability.         |
    |                                                       |
    | 3. FrameEnd, so the primer write commits and the      |
    |    per-key layer becomes the visible output.          |
    |                                                       |
    | Gate (caller's responsibility): feature 0x4522        |
    | present AND per-key V2 present. G502 / G515 fail      |
    | the 0x4522 side; older keyboards without 0x8081       |
    | fail the per-key side.                                |
    \*-----------------------------------------------------*/
    if(caps.idx_rgb_effects == 0 || caps.idx_perkey_v2 == 0 || !device_online.load())
    {
        return;
    }

    for(size_t i = 0; i < caps.zone_clusters.size(); i++)
    {
        uint8_t cluster_off[16] =
        {
            caps.zone_clusters[i].index, 0x00,          /* cluster, effectIdx=0 (Off) */
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x01,                                       /* persist */
            0x00, 0x00, 0x00
        };
        blankFAPmessage cluster_resp;
        SendAckedIntoFAP(caps.idx_rgb_effects, caps.fn_set_effect,
                         cluster_off, 16, cluster_resp);
    }

    if(caps.perkey_zone_ids.empty())
    {
        LOG_DEBUG("%s DoKeyboardFamilyPerKeyPrep: no per-key zones enumerated, "
                  "skipping primer key", LOG_TAG);
        return;
    }

    uint8_t primer_zone = (uint8_t)(caps.perkey_zone_ids[0] & 0xFF);
    uint8_t primer[4]   = { primer_zone, 0x00, 0x00, 0x00 };

    std::vector<uint8_t> primer_zones;
    primer_zones.push_back(primer_zone);
    SendPerKeyData(caps.idx_perkey_v2, FN_8081_SET_INDIVIDUAL,
                   primer, 4, primer_zones);

    PerKeyFrameEnd();

    LOG_DEBUG("%s DoKeyboardFamilyPerKeyPrep: %zu clusters -> Off, "
              "primer zone=0x%02X, FrameEnd committed",
              LOG_TAG, caps.zone_clusters.size(), primer_zone);
}

/*---------------------------------------------------------*\
| Retry-paint scheduling                                    |
|                                                           |
| Called by RGBController_LogitechHIDPP20::DeviceUpdateLEDs |
| when a full pass completes with `acked_zones.size() !=    |
| attempted_zones.size()` (partial commit). The retry       |
| re-runs a whole DeviceUpdateLEDs cycle from the power     |
| thread so the uncommitted zones (marked                   |
| HIDPP20_UNCOMMITTED in sent_colors) get another shot.     |
|                                                           |
| Streaming animation frames also call ScheduleRetryPaint   |
| on partial commit, but the next animation frame almost    |
| always CancelRetryPaint()s before the deadline fires,     |
| so the retry is a free no-op in the streaming path.       |
| The retry only actually fires when no follow-up frame     |
| arrives, which matches our two problem cases:             |
|   1. First frame after a reconnect-transient claim        |
|      (Direct mode, no animation timer).                   |
|   2. Last frame of an animation that then stops.          |
\*---------------------------------------------------------*/

bool LogitechHIDPP20Controller::ScheduleRetryPaint()
{
    size_t max_attempts =
        sizeof(HIDPP20_REPAINT_RETRY_BACKOFF_MS) / sizeof(uint16_t);

    uint8_t attempt = retry_paint_attempt_.load();

    if(attempt >= max_attempts)
    {
        /*-------------------------------------------------*\
        | Retry budget exhausted. Give up for               |
        | this sequence; the next fresh failure             |
        | (after a full_commit clears the attempt           |
        | counter) will start from attempt 0.               |
        \*-------------------------------------------------*/
        retry_paint_deadline_.store(std::chrono::steady_clock::time_point{});
        LOG_DEBUG("%s retry paint budget exhausted (%zu attempts)",
                  LOG_TAG, max_attempts);
        return false;
    }

    uint16_t delay_ms = HIDPP20_REPAINT_RETRY_BACKOFF_MS[attempt];
    std::chrono::steady_clock::time_point deadline = std::chrono::steady_clock::now()
                                                    + std::chrono::milliseconds(delay_ms);

    retry_paint_deadline_.store(deadline);

    LOG_DEBUG("%s retry paint scheduled attempt=%u delay=%ums",
              LOG_TAG, attempt, delay_ms);

    return true;
}

void LogitechHIDPP20Controller::CancelRetryPaint()
{
    retry_paint_deadline_.store(std::chrono::steady_clock::time_point{});
    retry_paint_attempt_.store(0);
}

void LogitechHIDPP20Controller::TickRetryPaintIfPending()
{
    /*-----------------------------------------------------*\
    | Called from the power thread's main loop              |
    | each tick. Checks the retry deadline and              |
    | fires the repaint callback when it expires.           |
    | The callback runs DeviceUpdateLEDs on the             |
    | power thread's context, not recursively               |
    | from inside another DeviceUpdateLEDs call.            |
    \*-----------------------------------------------------*/
    std::chrono::steady_clock::time_point deadline = retry_paint_deadline_.load();

    if(deadline == std::chrono::steady_clock::time_point{})
    {
        return;
    }

    if(std::chrono::steady_clock::now() < deadline)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Clear the deadline before firing so a concurrent      |
    | ScheduleRetryPaint (from a different thread) doesn't  |
    | double-fire on the same tick. Advance the attempt     |
    | counter so the next ScheduleRetryPaint (if this       |
    | retry also fails) picks the next backoff slot.        |
    \*-----------------------------------------------------*/
    retry_paint_deadline_.store(std::chrono::steady_clock::time_point{});
    retry_paint_attempt_.fetch_add(1);

    LOG_DEBUG("%s retry paint firing", LOG_TAG);

    if(request_repaint_fn)
    {
        request_repaint_fn();
    }
}

/*---------------------------------------------------------*\
| Per-key lighting (0x8081)                                 |
\*---------------------------------------------------------*/

void LogitechHIDPP20Controller::SetPerKeyColors
    (
    const std::vector<std::pair<uint16_t, RGBColor>>& zone_colors
    )
{
    if(!device_online.load()) return;

    uint8_t perkey_idx = (caps.idx_perkey_v2 != 0) ? caps.idx_perkey_v2 : caps.idx_perkey_v1;

    if(perkey_idx == 0)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Batch into SetIndividualRgbZones (fn1): 4             |
    | entries/packet Each entry = [zone_id, R,              |
    | G, B]. Track the zones in each batch so               |
    | PerKeyFrameEnd can report which committed.            |
    \*-----------------------------------------------------*/
    uint8_t              data[16];
    std::vector<uint8_t> batch_zones;
    int                  count = 0;

    for(size_t i = 0; i < zone_colors.size(); i++)
    {
        int offset = count * 4;
        data[offset + 0] = (uint8_t)zone_colors[i].first;
        data[offset + 1] = RGBGetRValue(zone_colors[i].second);
        data[offset + 2] = RGBGetGValue(zone_colors[i].second);
        data[offset + 3] = RGBGetBValue(zone_colors[i].second);
        batch_zones.push_back((uint8_t)zone_colors[i].first);
        count++;

        if(count == 4 || i == zone_colors.size() - 1)
        {
            SendPerKeyData(perkey_idx, FN_8081_SET_INDIVIDUAL,
                           data, count * 4, batch_zones);
            memset(data, 0, sizeof(data));
            batch_zones.clear();
            count = 0;
        }
    }
}

void LogitechHIDPP20Controller::SetAllPerKeyColor(RGBColor color)
{
    if(!device_online.load()) return;

    uint8_t perkey_idx = (caps.idx_perkey_v2 != 0) ? caps.idx_perkey_v2 : caps.idx_perkey_v1;

    if(perkey_idx == 0)
    {
        return;
    }

    uint8_t r = RGBGetRValue(color);
    uint8_t g = RGBGetGValue(color);
    uint8_t b = RGBGetBValue(color);

    /*-----------------------------------------------------*\
    | Use SetRangeRgbZones (fn5): [start, end, R,           |
    | G, B] x 3 per packet. Sets all zones in a             |
    | contiguous range to one color. Gaps in zone           |
    | IDs are silently ignored by firmware. For             |
    | uniform color this is far more efficient than         |
    | fn6: 1-2 packets vs 8 packets for 94 zones.           |
    \*-----------------------------------------------------*/
    uint8_t min_zone = 255, max_zone = 0;

    for(uint16_t zid : caps.perkey_zone_ids)
    {
        if(zid > 0 && zid <= 255)
        {
            if((uint8_t)zid < min_zone) min_zone = (uint8_t)zid;
            if((uint8_t)zid > max_zone) max_zone = (uint8_t)zid;
        }
    }

    if(min_zone <= max_zone)
    {
        uint8_t              data[5] = { min_zone, max_zone, r, g, b };
        std::vector<uint8_t> batch_zones;

        for(uint16_t zid : caps.perkey_zone_ids)
        {
            if(zid >= min_zone && zid <= max_zone)
            {
                batch_zones.push_back((uint8_t)zid);
            }
        }

        SendPerKeyData(perkey_idx, FN_8081_SET_RANGE, data, 5, batch_zones);
    }
}

void LogitechHIDPP20Controller::SendPerKeyData
    (
    uint8_t                     perkey_idx,
    uint8_t                     function,
    const uint8_t*              data,
    size_t                      len,
    const std::vector<uint8_t>& zone_ids
    )
{
    /*-----------------------------------------------------*\
    | Pipelined within the frame, paced by the device's ACK |
    | stream: at most HIDPP20_PERKEY_WINDOW packets in      |
    | flight unacked, on USB this is what stops the         |
    | firmware dropping responses. A stalled stream aborts  |
    | the frame; unacked zones ride the next delta.         |
    \*-----------------------------------------------------*/
    if(frame_attempted_zones.empty() && frame_packets.empty())
    {
        frame_first_write = std::chrono::steady_clock::now();
    }

    frame_attempted_zones.insert(frame_attempted_zones.end(),
                                 zone_ids.begin(), zone_ids.end());

    if(frame_aborted)
    {
        return;
    }

    while(frame_packets.size() - frame_responses_seen >= perkey_window)
    {
        if(ProcessOnePerKeyResponse(HIDPP20_PERKEY_ACK_WAIT_MS, perkey_idx) <= 0)
        {
            LOG_DEBUG("%s per-key ACK stream stalled (%zu in flight), aborting frame",
                      LOG_TAG, frame_packets.size() - frame_responses_seen);
            frame_aborted = true;
            return;
        }
    }

    int send_result = SendMessage(perkey_idx, function, data, len);

    if(send_result < 0)
    {
        /*-------------------------------------------------*\
        | Never left the host, no ACK will come. Not        |
        | recorded as a packet, so its zones simply         |
        | never ack and ride the next frame's delta.        |
        \*-------------------------------------------------*/
        LOG_DEBUG("%s per-key write wire send failed (result=%d) func=0x%02X",
                  LOG_TAG, send_result, function);
        return;
    }

    PerKeyPacket packet;
    packet.function = function;
    packet.len      = (len > sizeof(packet.payload)) ? sizeof(packet.payload) : len;
    packet.zone_ids = zone_ids;
    packet.acked    = false;
    memset(packet.payload, 0, sizeof(packet.payload));
    memcpy(packet.payload, data, packet.len);

    frame_packets.push_back(std::move(packet));
    fe_staged_zones += zone_ids.size();
}

int LogitechHIDPP20Controller::ProcessOnePerKeyResponse(int timeout_ms, uint8_t perkey_idx)
{
    uint8_t resp_feat = 0;
    uint8_t resp_func = 0;
    uint8_t resp_data[60] = {};

    int rd = ReadMessage(&resp_feat, &resp_func, resp_data, sizeof(resp_data), timeout_ms);

    if(rd <= 0)
    {
        return rd;
    }

    /*-----------------------------------------------------*\
    | HID++ error frame: feat=0xFF, func=err_feat,          |
    | data[0]=err_func, data[1]=err_code.                   |
    \*-----------------------------------------------------*/
    if(resp_feat == 0xFF)
    {
        uint8_t err_feat = resp_func;
        uint8_t err_func = resp_data[0];
        uint8_t err_code = resp_data[1];

        if(err_feat == perkey_idx && (err_func & 0x0F) == HIDPP20_SW_ID &&
           (err_func & 0xF0) != FN_8081_FRAME_END)
        {
            /*---------------------------------------------*\
            | A write was rejected outright. It answered,   |
            | but nothing it carried committed.             |
            \*---------------------------------------------*/
            LOG_DEBUG("%s per-key write error 0x%02X func=0x%02X",
                      LOG_TAG, err_code, err_func);
            frame_responses_seen++;
        }

        return 1;
    }

    if(resp_feat != perkey_idx || (resp_func & 0x0F) != HIDPP20_SW_ID ||
       (resp_func & 0xF0) == FN_8081_FRAME_END)
    {
        return 1;
    }

    frame_responses_seen++;

    if(MatchPerKeyAck(resp_func & 0xF0, resp_data, frame_acked_zones) >= 0)
    {
        frame_exact_acks++;
    }
    else
    {
        /*-------------------------------------------------*\
        | Echo format unknown for this function, count      |
        | it toward the group, and log the payload so a     |
        | real run teaches us what the firmware echoes.     |
        \*-------------------------------------------------*/
        frame_unmatched_acks[resp_func & 0xF0]++;

        char hex[16 * 3 + 1];
        for(int b = 0; b < 16; b++)
        {
            snprintf(&hex[b * 3], 4, "%02X ", resp_data[b]);
        }
        LOG_TRACE("%s per-key ACK echo unmatched func=0x%02X data=[%s]",
                  LOG_TAG, resp_func, hex);
    }

    return 1;
}

void LogitechHIDPP20Controller::ResetPerKeyFrameState()
{
    frame_attempted_zones.clear();
    frame_packets.clear();
    frame_acked_zones.clear();
    frame_unmatched_acks.clear();
    frame_responses_seen = 0;
    frame_exact_acks     = 0;
    frame_aborted        = false;
}

void LogitechHIDPP20Controller::DrainStaleResponses()
{
    std::chrono::steady_clock::time_point deadline =
        std::chrono::steady_clock::now() + std::chrono::milliseconds(150);

    while(std::chrono::steady_clock::now() < deadline)
    {
        uint8_t resp_data[60];

        if(ReadMessage(nullptr, nullptr, resp_data, sizeof(resp_data), 25) <= 0)
        {
            return;
        }
    }
}

/*---------------------------------------------------------*\
| Per-key frame sender thread: the effect engine            |
| submits snapshots at whatever rate it likes;              |
| the sender pushes the newest at the pace the              |
| link sustains and skips the rest. Wire time is            |
| paid here, never on the animating thread.                 |
\*---------------------------------------------------------*/
void LogitechHIDPP20Controller::SetPerKeyFrameSender(std::function<void(std::vector<RGBColor>&)> fn)
{
    perkey_frame_fn = fn;
    sender_running.store(true);
    sender_thread = new std::thread(&LogitechHIDPP20Controller::SenderThreadFunc, this);
}

bool LogitechHIDPP20Controller::HasPerKeyFrameSender() const
{
    return sender_running.load();
}

void LogitechHIDPP20Controller::SubmitPerKeyFrame(const std::vector<RGBColor>& frame)
{
    if(!sender_running.load())
    {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(pending_frame_mutex);

        if(pending_frame_valid)
        {
            pending_frames_skipped++;
        }

        pending_frame       = frame;
        pending_frame_valid = true;
    }

    pending_frame_cv.notify_one();
}

void LogitechHIDPP20Controller::SenderThreadFunc()
{
    while(sender_running.load())
    {
        std::vector<RGBColor> frame;
        uint32_t              skipped = 0;

        {
            std::unique_lock<std::mutex> lock(pending_frame_mutex);

            pending_frame_cv.wait(lock, [this]
            {
                return pending_frame_valid || !sender_running.load();
            });

            if(!sender_running.load())
            {
                return;
            }

            frame                  = std::move(pending_frame);
            pending_frame_valid    = false;
            skipped                = pending_frames_skipped;
            pending_frames_skipped = 0;
        }

        if(skipped > 0)
        {
            LOG_TRACE("%s sender skipped %u stale frame(s)", LOG_TAG, skipped);
        }

        /*-------------------------------------------------*\
        | Teardown started. The frame cannot land and       |
        | would hold the transaction mutex that stopping    |
        | the threads needs, so keep it and let the loop    |
        | exit.                                             |
        \*-------------------------------------------------*/
        if(teardown_pending.load())
        {
            {
                std::lock_guard<std::mutex> lock(pending_frame_mutex);

                if(!pending_frame_valid)
                {
                    pending_frame       = std::move(frame);
                    pending_frame_valid = true;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        if(perkey_frame_fn)
        {
            perkey_frame_fn(frame);
        }
    }
}

void LogitechHIDPP20Controller::StopSenderThread()
{
    sender_running.store(false);
    pending_frame_cv.notify_all();

    if(sender_thread && sender_thread->joinable())
    {
        sender_thread->join();
    }

    delete sender_thread;
    sender_thread = nullptr;
}

/*---------------------------------------------------------*\
| Attribute one write ACK to one outstanding packet by      |
| payload echo (decoded from captures, G515/G502):          |
|  fn1 SetIndividualRgbZones:  resp[i] = zone of entry i,   |
|      packed contiguously; 0xFF = that zone rejected       |
|  fn5 SetRangeRgbZones:       resp[k] = start zone of      |
|      range entry k                                        |
|  fn6 SetRgbZonesSingleValue: resp = [R, G, B, zone]       |
| Anything else falls back to per-function count            |
| matching in the caller. Returns the matched               |
| packet index or -1, appending zones the echo              |
| proves to acked_zones_out.                                |
\*---------------------------------------------------------*/
int LogitechHIDPP20Controller::MatchPerKeyAck
    (
    uint8_t               resp_func_hi,
    const uint8_t*        resp_data,
    std::vector<uint8_t>& acked_zones_out
    )
{
    for(size_t p = 0; p < frame_packets.size(); p++)
    {
        PerKeyPacket& packet = frame_packets[p];

        if(packet.acked || packet.function != resp_func_hi)
        {
            continue;
        }

        if(packet.function == FN_8081_SET_INDIVIDUAL)
        {
            size_t entries = packet.len / 4;
            bool   matches = true;
            size_t echoed  = 0;

            for(size_t j = 0; j < entries; j++)
            {
                uint8_t req_zone  = packet.payload[j * 4];
                uint8_t resp_zone = resp_data[j];

                if(resp_zone == req_zone)
                {
                    echoed++;
                }
                else if(resp_zone != 0xFF)
                {
                    matches = false;
                    break;
                }
            }

            if(!matches || echoed == 0)
            {
                continue;
            }

            for(size_t j = 0; j < entries; j++)
            {
                if(resp_data[j] == packet.payload[j * 4])
                {
                    acked_zones_out.push_back(packet.payload[j * 4]);
                }
                else
                {
                    LOG_DEBUG("%s per-key zone 0x%02X rejected by firmware",
                              LOG_TAG, packet.payload[j * 4]);
                }
            }

            packet.acked = true;
            return (int)p;
        }

        if(packet.function == FN_8081_SET_RANGE)
        {
            size_t entries = packet.len / 5;
            bool   matches = (entries > 0);

            for(size_t k = 0; k < entries; k++)
            {
                if(resp_data[k] != packet.payload[k * 5])
                {
                    matches = false;
                    break;
                }
            }

            if(!matches)
            {
                continue;
            }

            acked_zones_out.insert(acked_zones_out.end(),
                                   packet.zone_ids.begin(), packet.zone_ids.end());
            packet.acked = true;
            return (int)p;
        }

        if(packet.function == FN_8081_SET_SINGLE_VALUE)
        {
            if(memcmp(resp_data, packet.payload, 3) != 0)
            {
                continue;
            }

            bool zone_in_packet = false;

            for(uint8_t z : packet.zone_ids)
            {
                if(z == resp_data[3])
                {
                    zone_in_packet = true;
                    break;
                }
            }

            if(!zone_in_packet)
            {
                continue;
            }

            acked_zones_out.insert(acked_zones_out.end(),
                                   packet.zone_ids.begin(), packet.zone_ids.end());
            packet.acked = true;
            return (int)p;
        }

        /*-------------------------------------------------*\
        | Unknown function, echo the request verbatim.      |
        \*-------------------------------------------------*/
        if(memcmp(resp_data, packet.payload, packet.len) == 0)
        {
            acked_zones_out.insert(acked_zones_out.end(),
                                   packet.zone_ids.begin(), packet.zone_ids.end());
            packet.acked = true;
            return (int)p;
        }
    }

    return -1;
}

PerKeyFrameResult LogitechHIDPP20Controller::PerKeyFrameEnd()
{
    PerKeyFrameResult result;
    result.frame_end_acked = false;
    result.attempted_zones = std::move(frame_attempted_zones);

    uint8_t perkey_idx = (caps.idx_perkey_v2 != 0) ? caps.idx_perkey_v2 : caps.idx_perkey_v1;

    if(!device_online.load() || perkey_idx == 0)
    {
        result.acked_zones = std::move(frame_acked_zones);
        ResetPerKeyFrameState();
        return result;
    }

    std::chrono::steady_clock::time_point send_done = std::chrono::steady_clock::now();

    if(!frame_aborted)
    {
        /*-------------------------------------------------*\
        | Phase A: collect the remaining write ACKs, a      |
        | short tail, the window kept the outstanding       |
        | count small. The per-read timeout is a quiet      |
        | guard: a dropped ACK is never coming.             |
        \*-------------------------------------------------*/
        std::chrono::steady_clock::time_point ack_deadline =
            send_done + std::chrono::milliseconds(250);

        while(frame_responses_seen < frame_packets.size())
        {
            int remaining = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                                  ack_deadline - std::chrono::steady_clock::now()).count();

            if(remaining <= 0)
            {
                break;
            }

            if(remaining > (int)HIDPP20_PERKEY_ACK_TAIL_MS)
            {
                remaining = (int)HIDPP20_PERKEY_ACK_TAIL_MS;
            }

            if(ProcessOnePerKeyResponse(remaining, perkey_idx) <= 0)
            {
                break;
            }
        }
    }

    std::chrono::steady_clock::time_point acks_done = std::chrono::steady_clock::now();

    int      busy_polls   = 0;
    uint16_t predicted_ms = 0;
    bool     fe_probe     = false;

    /*-----------------------------------------------------*\
    | Phase B: commit. LONG message (0x11), 16              |
    | zero bytes; short-format hits intermittent            |
    | BUSY. Skipped when the frame aborted (landed          |
    | writes latch with the next FrameEnd).                 |
    |                                                       |
    | BUSY here is frame-rate backpressure, not             |
    | failure: the not-ready window scales with keys        |
    | written. Sleep the learned prediction first (the      |
    | link stays free for input), then poll the             |
    | remainder. The wait is the frame cadence and          |
    | paces the sender, never the effect engine.            |
    \*-----------------------------------------------------*/
    size_t commit_zones = fe_staged_zones;

    if(!frame_aborted)
    {
        float predicted = fe_busy_us_per_zone * (float)commit_zones / 1000.0f;

        if(predicted > 100.0f)
        {
            predicted = 100.0f;
        }

        predicted_ms = (uint16_t)predicted;

        /*-------------------------------------------------*\
        | Probe: every so often, sleep deliberately         |
        | short to test whether the device got faster. A    |
        | miss costs a few ms (ready is within the          |
        | shaved step, the NEAR poll picks it up); a        |
        | clean accept adopts the shorter window.           |
        \*-------------------------------------------------*/
        if(predicted_ms > HIDPP20_FE_PROBE_STEP_MS &&
           ++fe_frames_since_probe >= HIDPP20_FE_PROBE_INTERVAL)
        {
            fe_frames_since_probe = 0;
            predicted_ms -= HIDPP20_FE_PROBE_STEP_MS;
            fe_probe = true;
        }

        if(predicted_ms > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(predicted_ms));
        }

        uint8_t data[16] = {};
        int send_result = SendMessage(perkey_idx, FN_8081_FRAME_END, data, sizeof(data));

        if(send_result < 0)
        {
            LOG_DEBUG("%s FrameEnd wire send failed (result=%d)", LOG_TAG, send_result);
        }
        else
        {
            std::chrono::steady_clock::time_point fe_deadline =
                std::chrono::steady_clock::now() + std::chrono::milliseconds(250);

            while(!result.frame_end_acked)
            {
                int remaining = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                                      fe_deadline - std::chrono::steady_clock::now()).count();

                if(remaining <= 0)
                {
                    LOG_DEBUG("%s FrameEnd timed out waiting for ACK", LOG_TAG);
                    break;
                }

                uint8_t resp_feat = 0;
                uint8_t resp_func = 0;
                uint8_t resp_data[60] = {};

                int rd = ReadMessage(&resp_feat, &resp_func,
                                     resp_data, sizeof(resp_data), remaining);

                if(rd < 0)
                {
                    LOG_DEBUG("%s FrameEnd read error (result=%d)", LOG_TAG, rd);
                    break;
                }

                if(rd == 0)
                {
                    LOG_DEBUG("%s FrameEnd timed out waiting for ACK", LOG_TAG);
                    break;
                }

                if(resp_feat == 0xFF)
                {
                    uint8_t err_feat = resp_func;
                    uint8_t err_func = resp_data[0];
                    uint8_t err_code = resp_data[1];

                    if(err_feat != perkey_idx || (err_func & 0x0F) != HIDPP20_SW_ID)
                    {
                        continue;
                    }

                    if((err_func & 0xF0) != FN_8081_FRAME_END)
                    {
                        LOG_DEBUG("%s per-key write error 0x%02X func=0x%02X",
                                  LOG_TAG, err_code, err_func);
                        frame_responses_seen++;
                        continue;
                    }

                    if(err_code == 0x08)
                    {
                        busy_polls++;
                        std::this_thread::sleep_for(std::chrono::milliseconds(
                            (predicted_ms > 0) ? HIDPP20_FRAME_END_BUSY_POLL_NEAR_MS
                                               : HIDPP20_FRAME_END_BUSY_POLL_MS));
                        SendMessage(perkey_idx, FN_8081_FRAME_END, data, sizeof(data));
                        continue;
                    }

                    LOG_DEBUG("%s FrameEnd error 0x%02X (polls=%d)",
                              LOG_TAG, err_code, busy_polls);
                    break;
                }

                if(resp_feat != perkey_idx || (resp_func & 0x0F) != HIDPP20_SW_ID)
                {
                    continue;
                }

                if((resp_func & 0xF0) == FN_8081_FRAME_END)
                {
                    result.frame_end_acked = true;
                    break;
                }

                /*-----------------------------------------*\
                | A straggler write ACK: attribute it.      |
                \*-----------------------------------------*/
                frame_responses_seen++;

                if(MatchPerKeyAck(resp_func & 0xF0, resp_data, frame_acked_zones) >= 0)
                {
                    frame_exact_acks++;
                }
                else
                {
                    frame_unmatched_acks[(uint8_t)(resp_func & 0xF0)]++;
                }
            }
        }
    }

    /*-----------------------------------------------------*\
    | Learn the busy window from what happened, last        |
    | write ACK to accepted FrameEnd.                       |
    |  probe frame, clean accept -> adopt shorter window    |
    |  probe frame, BUSY         -> deliberate miss         |
    |  normal frame, BUSY        -> ran short, pull         |
    |                               toward measurement      |
    \*-----------------------------------------------------*/
    if(result.frame_end_acked)
    {
        /*-------------------------------------------------*\
        | Committed; the staging is latched and             |
        | the next commit starts from empty.                |
        \*-------------------------------------------------*/
        fe_staged_zones = 0;
    }

    if(result.frame_end_acked && commit_zones > 0)
    {
        float zones = (float)commit_zones;

        if(fe_probe)
        {
            if(busy_polls == 0)
            {
                fe_busy_us_per_zone = 0.7f * fe_busy_us_per_zone
                                    + 0.3f * ((float)predicted_ms * 1000.0f / zones);
            }
        }
        else if(busy_polls > 0)
        {
            int ready_ms = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                                 std::chrono::steady_clock::now() - acks_done).count();

            fe_busy_us_per_zone = 0.7f * fe_busy_us_per_zone
                                + 0.3f * ((float)ready_ms * 1000.0f / zones);

            if(fe_busy_us_per_zone > 2000.0f)
            {
                fe_busy_us_per_zone = 2000.0f;
            }
        }
    }

    /*-----------------------------------------------------*\
    | Settle the fallback groups: for each                  |
    | function whose ACKs failed the echo compare,          |
    | commit the group's unacked packets only if            |
    | the response count covers all of them.                |
    \*-----------------------------------------------------*/
    for(std::pair<const uint8_t, size_t>& group : frame_unmatched_acks)
    {
        size_t unacked = 0;

        for(const PerKeyPacket& packet : frame_packets)
        {
            if(!packet.acked && packet.function == group.first)
            {
                unacked++;
            }
        }

        if(group.second == unacked)
        {
            for(PerKeyPacket& packet : frame_packets)
            {
                if(!packet.acked && packet.function == group.first)
                {
                    packet.acked = true;
                    frame_acked_zones.insert(frame_acked_zones.end(),
                                             packet.zone_ids.begin(),
                                             packet.zone_ids.end());
                }
            }
        }
    }

    result.acked_zones = std::move(frame_acked_zones);

    /*-----------------------------------------------------*\
    | Adapt the write window. ACK loss is backpressure      |
    | too: a sent packet that never answered means          |
    | this link outran the device's response                |
    | generation, halve the window. A run of loss-free      |
    | frames earns it back one step at a time.              |
    \*-----------------------------------------------------*/
    if(!frame_packets.empty())
    {
        size_t lost = 0;

        for(const PerKeyPacket& packet : frame_packets)
        {
            if(!packet.acked)
            {
                lost++;
            }
        }

        if(lost > 0)
        {
            perkey_clean_frames = 0;

            if(perkey_window > HIDPP20_PERKEY_WINDOW_MIN)
            {
                perkey_window /= 2;

                if(perkey_window < HIDPP20_PERKEY_WINDOW_MIN)
                {
                    perkey_window = HIDPP20_PERKEY_WINDOW_MIN;
                }

                LOG_DEBUG("%s %zu write ACK(s) lost, write window now %zu",
                          LOG_TAG, lost, perkey_window);
            }
        }
        else if(perkey_window < HIDPP20_PERKEY_WINDOW_MAX &&
                ++perkey_clean_frames >= HIDPP20_PERKEY_WINDOW_GROW_AFTER)
        {
            perkey_clean_frames = 0;
            perkey_window++;
        }
    }

    bool complete = result.frame_end_acked &&
                    (result.acked_zones.size() == result.attempted_zones.size());

    if(!complete)
    {
        /*-------------------------------------------------*\
        | Late ACKs from this frame may still be in         |
        | flight. Consume them so the next frame's          |
        | matching starts clean.                            |
        \*-------------------------------------------------*/
        DrainStaleResponses();
    }

    if(!result.attempted_zones.empty())
    {
        std::chrono::steady_clock::time_point frame_done = std::chrono::steady_clock::now();

        int send_ms = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                            send_done - frame_first_write).count();
        int ack_ms  = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                            acks_done - send_done).count();
        int fe_ms   = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                            frame_done - acks_done).count();

        if(complete)
        {
            LOG_TRACE("%s frame committed: %zu zones, %zu packets (%zu echo-matched) "
                      "send=%dms ack=%dms fe=%dms pred=%ums busy=%d win=%zu",
                      LOG_TAG, result.attempted_zones.size(), frame_packets.size(),
                      frame_exact_acks, send_ms, ack_ms, fe_ms, predicted_ms, busy_polls,
                      perkey_window);
        }
        else
        {
            LOG_DEBUG("%s frame %s: %zu/%zu zones acked (%zu packets, %zu echo-matched), "
                      "frame_end=%d, send=%dms ack=%dms fe=%dms pred=%ums busy=%d win=%zu",
                      LOG_TAG, frame_aborted ? "aborted" : "partial",
                      result.acked_zones.size(), result.attempted_zones.size(),
                      frame_packets.size(), frame_exact_acks,
                      (int)result.frame_end_acked, send_ms, ack_ms, fe_ms, predicted_ms,
                      busy_polls, perkey_window);
        }
    }

    ResetPerKeyFrameState();

    /*-----------------------------------------------------*\
    | Deep-sleep detection: FrameEnd failures while         |
    | SLEEPING may mean the firmware fade finished.         |
    | Count consecutive failures (an aborted frame          |
    | counts, a stalled stream and a sleeping device        |
    | look alike); at the threshold, suppress sends         |
    | until Wake(). Any ACK resets the count.               |
    \*-----------------------------------------------------*/
    if(result.frame_end_acked)
    {
        consecutive_frame_end_failures.store(0);
    }
    else if(power_state == HIDPP20_POWER_SLEEPING)
    {
        int failures = consecutive_frame_end_failures.fetch_add(1) + 1;

        if(failures >= HIDPP20_DEEP_SLEEP_FAILURE_THRESHOLD && !deep_sleep.load())
        {
            deep_sleep.store(true);
            LOG_DEBUG("%s Device entered deep sleep (%d consecutive FrameEnd failures)",
                      LOG_TAG, failures);
        }
    }

    return result;
}

/*---------------------------------------------------------*\
| Feature 0x8080 set + commit: fn3 SetKeyColors on the      |
| 0x12 very-long report via dev_perkey_vl, payload          |
| [keyType u16 BE, count u16 BE, (keyId,R,G,B) x            |
| count], <=14 tuples per packet (larger groups             |
| split). Colors appear only after fn5 FlushLEDS.           |
\*---------------------------------------------------------*/
void LogitechHIDPP20Controller::SetPerKey8080
    (
    const std::vector<std::pair<uint16_t, std::vector<std::pair<uint8_t, RGBColor>>>>& by_type
    )
{
    if(caps.idx_perkey_8080 == 0 || dev_perkey_vl == nullptr)
    {
        return;
    }

    bool kt40_id12_only = (caps.quirks & HIDPP20_QUIRK_8080_KEYTYPE40_ID12_ONLY) != 0;

    for(size_t t = 0; t < by_type.size(); t++)
    {
        uint16_t key_type = by_type[t].first;
        const std::vector<std::pair<uint8_t, RGBColor>>& src_keys = by_type[t].second;

        /*-------------------------------------------------*\
        | G410 keyType 0x40 accepts only keyId 1            |
        | and 2; drop any others before encoding.           |
        \*-------------------------------------------------*/
        std::vector<std::pair<uint8_t, RGBColor>>          filtered;
        const std::vector<std::pair<uint8_t, RGBColor>>*     keys = &src_keys;

        if(kt40_id12_only && key_type == 0x40)
        {
            for(size_t k = 0; k < src_keys.size(); k++)
            {
                if(src_keys[k].first == 1 || src_keys[k].first == 2)
                {
                    filtered.push_back(src_keys[k]);
                }
            }
            keys = &filtered;
        }

        for(size_t off = 0; off < keys->size(); off += HIDPP20_8080_KEYS_PER_FRAME)
        {
            size_t count = keys->size() - off;
            if(count > HIDPP20_8080_KEYS_PER_FRAME)
            {
                count = HIDPP20_8080_KEYS_PER_FRAME;
            }

            /*---------------------------------------------*\
            | Payload: keyType(u16 BE) + count(u16 BE) +    |
            | count x (keyId, R, G, B). SendVeryLongFrame   |
            | prepends the 4-byte HID++ header.             |
            \*---------------------------------------------*/
            uint8_t payload[LOGITECH_VERY_LONG_MESSAGE_LEN - 4];
            memset(payload, 0, sizeof(payload));

            payload[0] = (uint8_t)(key_type >> 8);
            payload[1] = (uint8_t)(key_type & 0xFF);
            payload[2] = (uint8_t)(count    >> 8);
            payload[3] = (uint8_t)(count    & 0xFF);

            size_t pos = 4;
            for(size_t k = 0; k < count; k++)
            {
                RGBColor color = (*keys)[off + k].second;
                payload[pos++] = (*keys)[off + k].first;     /* keyId (USB HID usage) */
                payload[pos++] = RGBGetRValue(color);
                payload[pos++] = RGBGetGValue(color);
                payload[pos++] = RGBGetBValue(color);
            }

            SendVeryLongFrame(caps.idx_perkey_8080, FN_8080_SET_KEY_COLORS,
                              payload, pos);
        }
    }
}

/*---------------------------------------------------------*\
| fn5 FlushLEDS: commit. Empty body (persist = default),    |
| sent on the 0x11 long report via dev.                     |
\*---------------------------------------------------------*/
void LogitechHIDPP20Controller::PerKeyCommit8080()
{
    if(caps.idx_perkey_8080 == 0)
    {
        return;
    }

    uint8_t recv[20] = {};
    int     result   = SendAcked(caps.idx_perkey_8080, FN_8080_FLUSH_LEDS, nullptr, 0,
                                 recv, sizeof(recv), HIDPP20_POLICY_RELIABLE);

    if(result <= 0)
    {
        LOG_DEBUG("%s 0x8080 FlushLEDS commit not acked (result=%d), "
                  "colors may not have latched", LOG_TAG, result);
    }
}

/*---------------------------------------------------------*\
| Zone effects (0x8071 / 0x8070)                            |
\*---------------------------------------------------------*/

void LogitechHIDPP20Controller::SetZoneEffect
    (
    uint8_t         cluster_idx,
    uint8_t         effect_idx,
    uint16_t        effect_id,
    unsigned char   r,
    unsigned char   g,
    unsigned char   b,
    uint16_t        period,
    unsigned char   brightness,
    unsigned char   direction,
    bool            persist
    )
{
    if(caps.idx_rgb_effects == 0 || !device_online.load())
    {
        return;
    }

    /*-----------------------------------------------------*\
    | SetEffectByIndex (fn1 on 0x8071, fn3 on 0x8070)       |
    | 0x8071/0x0600: [cluster, effect_idx, 10-byte params,  |
    |                 persist at [12]]                      |
    | 0x8070:        [zone,    effect_idx, 10-byte params,  |
    |                 persist at [12] (Bit 2-3 Power,       |
    |                 Bit 1-0 Persistence)]                 |
    \*-----------------------------------------------------*/
    uint8_t data[16];
    memset(data, 0, sizeof(data));

    data[0] = cluster_idx;
    data[1] = effect_idx;

    /*-----------------------------------------------------*\
    | Build 10-byte params (data[2..11]) per effect type    |
    | Layouts from protocol docs and observed wire captures |
    \*-----------------------------------------------------*/
    switch(effect_id)
    {
        case 0x0001:    // Static
            data[2] = r;
            data[3] = g;
            data[4] = b;
            /*---------------------------------------------*\
            | "Fixed color" marker, only set when there's   |
            | an actual color. All-black means "Off /       |
            | pass-through to per-key buffer", which uses   |
            | byte 5 = 0x00 instead. Shared across pages:   |
            | 0x8070's func-3 Static tail is R, G, B,       |
            | 0x02 at byte 5, same position as 0x8071.      |
            \*---------------------------------------------*/
            if(r != 0 || g != 0 || b != 0)
            {
                data[5] = 0x02;
            }
            break;

        case 0x000A:    // Breathing
            /*-----------------------------------------------------*\
            | Effect param layout (10 bytes, indices into data[]):  |
            |   data[2..4]  = R, G, B                               |
            |   data[5..6]  = periodHi, periodLo (BE16 milliseconds)|
            |   data[7]     = 0                                     |
            |   data[8]     = brightness 0..100                     |
            \*-----------------------------------------------------*/
            data[2] = r;
            data[3] = g;
            data[4] = b;
            data[5] = (period >> 8) & 0xFF;
            data[6] = period & 0xFF;
            data[8] = brightness;
            break;

        case 0x0003:    // Color Cycle / Spectrum
            /*------------------------------------------------------*\
            | Effect param layout (10 bytes, indices into data[]):   |
            |   data[7..8]  = periodHi, periodLo (BE16 milliseconds) |
            |   data[9]     = brightness 0..100                      |
            \*------------------------------------------------------*/
            data[7] = (period >> 8) & 0xFF;
            data[8] = period & 0xFF;
            data[9] = brightness;
            break;

        case 0x0007:    // Audio visualizer
            /*------------------------------------------------------*\
            | Effect param layout (10 bytes, indices into data[]):   |
            |   data[2]     = control: 0 = fixed color, 1 = cycle    |
            |                 colors (factory default); >=2 stops    |
            |                 the audio pulse                        |
            |   data[3..5]  = R, G, B (fixed-color pulse)            |
            |   data[7..8]  = periodHi, periodLo (BE16 milliseconds) |
            \*------------------------------------------------------*/
            if(r != 0 || g != 0 || b != 0)
            {
                data[2] = 0x00;
                data[3] = r;
                data[4] = g;
                data[5] = b;
            }
            else
            {
                data[2] = 0x01;
            }

            data[7] = (period >> 8) & 0xFF;
            data[8] = period & 0xFF;
            break;

        case 0x0004:    // Color Wave
            /*---------------------------------------------*\
            | Plain (non-saturation) wave: the period is    |
            | split NON-contiguously and there is no        |
            | saturation byte:                              |
            |  data[8]  period low                          |
            |  data[9]  direction (1..8)                    |
            |  data[10] brightness 1..100                   |
            |  data[11] period high                         |
            | Distinct from the 0x0016 saturation-wave      |
            | layout below (saturation@3, period BE@8..9,   |
            | direction@11); mixing them gets               |
            | InvalidArgument.                              |
            \*---------------------------------------------*/
            data[8]  = period & 0xFF;
            data[9]  = direction;
            data[10] = brightness ? brightness : 1;
            data[11] = (period >> 8) & 0xFF;
            break;

        case 0x000B:    // Ripple
            data[2] = r;
            data[3] = g;
            data[4] = b;
            data[6] = (period >> 8) & 0xFF;
            data[7] = period & 0xFF;
            break;

        case 0x000E:    // Decomposition: no color; period BE@8-9, intensity@10
            data[8]  = (period >> 8) & 0xFF;
            data[9]  = period & 0xFF;
            data[10] = brightness;
            break;

        case 0x000F:    // Signature1: no color; period BE@7-8, intensity@9
        case 0x0010:    // Signature2: same layout
            data[7] = (period >> 8) & 0xFF;
            data[8] = period & 0xFF;
            data[9] = brightness;
            break;

        case 0x0015:    // Cycle (saturation variant)
            /*------------------------------------------------------*\
            | Saturation-bearing variant of 0x0003. Param block      |
            | (10 bytes, indices into data[]):                       |
            |   data[3]     = saturation 0..255 (hardcoded full)     |
            |   data[8..9]  = periodHi, periodLo (BE16 milliseconds) |
            |   data[10]    = intensity 0..100                       |
            | Layout from Solaar LEDEffects 0x15 (saturation@1,      |
            | period@6, intensity@8 in the param block).             |
            \*------------------------------------------------------*/
            data[3]  = 0xFF;
            data[8]  = (period >> 8) & 0xFF;
            data[9]  = period & 0xFF;
            data[10] = brightness;
            break;

        case 0x0016:    // Wave (saturation variant)
            /*------------------------------------------------------*\
            | Saturation-bearing variant of 0x0004. Param block:     |
            |   data[3]     = saturation 0..255 (hardcoded full)     |
            |   data[8..9]  = periodHi, periodLo (BE16 milliseconds) |
            |   data[10]    = intensity 0..100                       |
            |   data[11]    = direction (Logitech wire value)        |
            | Layout from Solaar LEDEffects 0x16 (saturation@1,      |
            | period@6, intensity@8, direction@9). The caller maps   |
            | OpenRGB's 6 direction slots to the wire values.        |
            \*------------------------------------------------------*/
            data[3]  = 0xFF;
            data[8]  = (period >> 8) & 0xFF;
            data[9]  = period & 0xFF;
            data[10] = brightness;
            data[11] = direction;
            break;

        case 0x0017:    // Ripple (saturation variant)
            /*------------------------------------------------------*\
            | Saturation-bearing variant of 0x000B. Param block:     |
            |   data[2..4]  = R, G, B                                |
            |   data[5]     = saturation 0..255 (hardcoded full)     |
            |   data[8..9]  = periodHi, periodLo (BE16 milliseconds) |
            | Layout from Solaar LEDEffects 0x17 (color@0,           |
            | saturation@3, period@6). No intensity param.           |
            \*------------------------------------------------------*/
            data[2] = r;
            data[3] = g;
            data[4] = b;
            data[5] = 0xFF;
            data[8] = (period >> 8) & 0xFF;
            data[9] = period & 0xFF;
            break;

        default:        // Unknown: best-effort
            data[2] = r;
            data[3] = g;
            data[4] = b;
            data[5] = (period >> 8) & 0xFF;
            data[6] = period & 0xFF;
            break;
    }

    /*-----------------------------------------------------*\
    | Inline persist byte at [12]: Persistence bits 1-0,    |
    | Power bits 2-3. Only a Save sets it, so live painting |
    | stays volatile.                                       |
    \*-----------------------------------------------------*/
    data[12] = persist ? 0x01 : 0x00;

    LOG_DEBUG("%s SetEffect cluster=%u idx=%u id=0x%04X "
              "data=[%02X %02X %02X %02X %02X %02X %02X %02X "
              "%02X %02X %02X %02X %02X %02X %02X %02X]",
              LOG_TAG, cluster_idx, effect_idx, effect_id,
              data[0], data[1], data[2], data[3],
              data[4], data[5], data[6], data[7],
              data[8], data[9], data[10], data[11],
              data[12], data[13], data[14], data[15]);

    blankFAPmessage response;
    SendAckedIntoFAP(caps.idx_rgb_effects, caps.fn_set_effect,
                     data, 16, response);
}

/*---------------------------------------------------------*\
| Feature 0x0620 Headset RGB Hostmode: static color write.  |
|                                                           |
| Claim is sticky from SetHostMode(); this function         |
| only writes colors + FrameEnd. Picks fn5                  |
| SetRgbZonesSingleValue when all zones share a             |
| color, else fn2 SetIndividualRgbZones. FrameEnd           |
| byte 0 is always 0x01 (transient), 0x02 was               |
| tested and does not work on G522 firmware.                |
\*---------------------------------------------------------*/
void LogitechHIDPP20Controller::SetHeadsetRGBHostmodeColors
    (
    const std::vector<RGBColor>& zone_colors
    )
{
    if(caps.idx_headset_rgb_hostmode == 0 || !device_online.load())
    {
        return;
    }

    const std::vector<uint8_t>& zones = caps.headset_rgb_hostmode_zone_ids;
    if(zones.empty())
    {
        return;
    }

    if(zone_colors.empty())
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Group zones by color, one fn5 per distinct color,     |
    | then one FrameEnd: Solaar's write_zone_map byte-for-  |
    | byte. Host mode is already claimed (sticky); the      |
    | frame is data + FrameEnd only. Zone i takes           |
    | zone_colors[i], the last color filling any shortfall. |
    \*-----------------------------------------------------*/
    std::map<RGBColor, std::vector<uint8_t>> groups;
    for(size_t i = 0; i < zones.size(); i++)
    {
        RGBColor c = (i < zone_colors.size()) ? zone_colors[i] : zone_colors.back();
        groups[c].push_back(zones[i]);
    }

    blankFAPmessage response;

    for(const std::pair<const RGBColor, std::vector<uint8_t>>& group : groups)
    {
        RGBColor                    color = group.first;
        const std::vector<uint8_t>& ids   = group.second;

        /*-------------------------------------------------*\
        | fn5 SetRgbZonesSingleValue:                       |
        | [R, G, B, count, zones...]                        |
        \*-------------------------------------------------*/
        uint8_t payload[16];
        payload[0] = RGBGetRValue(color);
        payload[1] = RGBGetGValue(color);
        payload[2] = RGBGetBValue(color);

        size_t n = ids.size();
        if(n > sizeof(payload) - 4) n = sizeof(payload) - 4;
        payload[3] = (uint8_t)n;
        for(size_t i = 0; i < n; i++)
        {
            payload[4 + i] = ids[i];
        }

        SendAckedIntoFAP(caps.idx_headset_rgb_hostmode,
                         FN_0620_SET_RGB_ZONES_SINGLE_VALUE,
                         payload, 4 + n, response);
    }

    /*-----------------------------------------------------*\
    | fn6 FrameEnd: byte 0 = 0x01 (transient                |
    | commit). Never 0x00 (silently discarded) and          |
    | never 0x02 (tested broken on G522 firmware).          |
    \*-----------------------------------------------------*/
    uint8_t frame_end[4] = { 0x01, 0x00, 0x00, 0x00 };
    SendAckedIntoFAP(caps.idx_headset_rgb_hostmode, FN_0620_FRAME_END,
                     frame_end, sizeof(frame_end), response);

    LOG_TRACE("%s 0x0620 wrote %zu zone(s) in %zu color group(s), FrameEnd[0x01]",
              LOG_TAG, zones.size(), groups.size());
}

/*---------------------------------------------------------*\
| Power management (idle/dim/sleep/wake)                    |
|                                                           |
| Matches Solaar's RGBPowerManager state machine:           |
|   ACTIVE -> DIMMING -> IDLE -> SLEEPING                   |
|                                                           |
| Uses firmware onUserActivity events from 0x8071 for       |
| idle/active detection. SW control flags cycle:            |
|   7 (init) -> 5 (active, monitor idle) ->                 |
|   3 (idle, monitor active) -> 5 (wake)                    |
\*---------------------------------------------------------*/

void LogitechHIDPP20Controller::SetRepaintCallback(std::function<void()> repaint)
{
    request_repaint_fn = repaint;
}

void LogitechHIDPP20Controller::SetReapplyActiveModeCallback(std::function<bool()> cb)
{
    reapply_active_mode_fn = cb;
}

void LogitechHIDPP20Controller::SetPairingName(const std::string& name)
{
    if(!LogitechHIDPP20Controller::NameLooksReal(name))
    {
        return;
    }

    pairing_name = name;

    /*-----------------------------------------------------*\
    | Label the controller while the device is asleep.      |
    | The real 0x0005 name read replaces this once the      |
    | device answers.                                       |
    \*-----------------------------------------------------*/
    if(caps.device_name.empty())
    {
        caps.device_name = name;
        log_tag          = "[LogitechHID++ " + name + "]";
    }
}

HIDPP20PowerState LogitechHIDPP20Controller::GetPowerState() const
{
    return power_state;
}

int LogitechHIDPP20Controller::GetDimBrightness() const
{
    return dim_brightness_pct.load();
}

bool LogitechHIDPP20Controller::HasBridge() const
{
    return transport.bridge_feat_idx != 0;
}

bool LogitechHIDPP20Controller::IsOnline() const
{
    return device_online.load();
}

bool LogitechHIDPP20Controller::IsDeepSleep() const
{
    return deep_sleep.load();
}

void LogitechHIDPP20Controller::ReprobeSubDevice()
{
    /*-----------------------------------------------------*\
    | Called by the power thread when a sub-device          |
    | connects through the Centurion bridge. The reader     |
    | thread is running, so all commands go through         |
    | SendAndRead -> ReadFromQueue.                         |
    |                                                       |
    | We clear the sub-device feature cache and             |
    | re-discover everything. The bridge_feat_idx and       |
    | dongle name are kept.                                 |
    \*-----------------------------------------------------*/
    LOG_DEBUG("%s Re-probing sub-device through bridge", LOG_TAG);

    /*-----------------------------------------------------*\
    | Let the sub-device settle after connection before     |
    | sending commands through the bridge.                  |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    FlushResponseQueue();

    /*-----------------------------------------------------*\
    | Clear sub-device feature map but keep bridge index.   |
    | This forces fresh lookups through the bridge.         |
    \*-----------------------------------------------------*/
    caps.feature_map.clear();
    caps.feature_map_complete = false;
    caps.has_zone_effects     = false;
    caps.has_perkey            = false;
    caps.has_effect_cards      = false;
    caps.effect_card_template[0] = 0;
    caps.effect_card_template[1] = 0;
    caps.zone_clusters.clear();
    caps.perkey_zone_ids.clear();
    caps.idx_rgb_effects           = 0;
    caps.idx_perkey_v2              = 0;
    caps.idx_perkey_v1              = 0;
    caps.idx_profile_management     = 0;
    caps.idx_onboard_profiles       = 0;
    caps.idx_disable_keys_by_usage  = 0;
    caps.fn_set_effect              = 0;
    caps.fn_sw_control          = 0;
    caps.fn_pwr_config          = 0;
    caps.fn_pwr_mode            = 0;
    caps.has_power_mgmt         = false;
    caps.sw_control_simple      = false;
    caps.nv_sleep_ramp_known    = false;
    caps.nv_sleep_ramp_enabled  = false;
    caps.nv_sleep_ramp_seconds  = 0;

    /*-----------------------------------------------------*\
    | Re-populate feature map. CenturionFeatureSet          |
    | is always at index 1 on the sub-device.               |
    \*-----------------------------------------------------*/
    EnumerateFeatures(1);

    if(!caps.feature_map_complete)
    {
        LOG_DEBUG("%s Sub-device not reachable after connect event", LOG_TAG);
        caps.feature_map_complete = true;
        return;
    }

    /*-----------------------------------------------------*\
    | The sub-device's real name only becomes               |
    | readable now that it's reachable, so always re-       |
    | discover, but never let a failed re-read              |
    | downgrade a good name to a placeholder.               |
    \*-----------------------------------------------------*/
    std::string old_name = caps.device_name;
    DiscoverDeviceName();

    if(HIDPP20NameIsPlaceholder(caps.device_name) && !HIDPP20NameIsPlaceholder(old_name))
    {
        caps.device_name = old_name;
    }

    if(caps.device_name != old_name)
    {
        log_tag = "[LogitechHID++ " + caps.device_name + "]";
    }

    DiscoverDeviceType();
    DiscoverFirmwareInfo();

    /*-----------------------------------------------------*\
    | Discover RGB features                                 |
    \*-----------------------------------------------------*/
    caps.idx_profile_management     = GetFeatureIndex(HIDPP20_FEAT_PROFILE_MANAGEMENT);
    caps.idx_onboard_profiles       = GetFeatureIndex(HIDPP20_FEAT_ONBOARD_PROFILES);
    caps.idx_disable_keys_by_usage  = GetFeatureIndex(HIDPP20_FEAT_DISABLE_KEYS_BY_USAGE);

    DiscoverRGBEffects();
    if(caps.idx_rgb_effects == 0)
    {
        DiscoverHeadsetRGBHostmode();
    }
    DiscoverPerKeyZones();
    DiscoverPerKey8080();
    DiscoverKeyboardLayout();

    if(!caps.has_zone_effects && !caps.has_perkey && !caps.has_perkey_8080)
    {
        LOG_DEBUG("%s Sub-device has no RGB features", LOG_TAG);
        return;
    }

    LOG_INFO("%s Sub-device probed: zones=%zu perkey=%zu",
             LOG_TAG, caps.zone_clusters.size(), caps.perkey_zone_ids.size());

    /*-----------------------------------------------------*\
    | Bring the sub-device back up on the existing          |
    | controller. A dongle with no sub-device at            |
    | detection never got a controller, so                  |
    | DetectionManager re-probes until one answers.         |
    \*-----------------------------------------------------*/
    Initialize();
}

/*---------------------------------------------------------*\
| Take software control and retry until it sticks: a        |
| device that just came back is booting its onboard         |
| profile, and a claim in that window is dropped silently;  |
| we would paint at a device that is not listening. Retry   |
| on a fast backoff until ReapplyActiveMode reports the     |
| claim ACKed (~50ms in practice).                          |
\*---------------------------------------------------------*/
bool LogitechHIDPP20Controller::ReclaimSWControl(const char* reason, bool force)
{
    /*-----------------------------------------------------*\
    | force (reconnect): the device just rebooted its       |
    | onboard profile, so any claim flag is stale, tear     |
    | it down so every attempt runs the whole sequence      |
    | instead of believing it already holds control.        |
    |                                                       |
    | !force: a set flag means the effect engine's own      |
    | repaint re-claimed here first. Leave it: redoing the  |
    | flags 6->5 claim re-exposes the firmware's onboard    |
    | colors for ~50ms, a visible flash. The reapply below  |
    | still runs DeviceUpdateMode (per-key prep lands       |
    | either way); the ClaimSWControlIfNeeded inside it     |
    | no-ops while claimed.                                 |
    \*-----------------------------------------------------*/
    if(force || !sw_control_claimed)
    {
        sw_control_claimed            = false;
        sw_control_needs_upgrade_to_5 = false;
        prep_applied                  = false;
        retry_paint_deadline_.store(std::chrono::steady_clock::time_point{});
        retry_paint_attempt_.store(0);
    }

    size_t attempt_count = sizeof(HIDPP20_RECLAIM_BACKOFF_MS) / sizeof(uint16_t);

    for(size_t i = 0; i < attempt_count; i++)
    {
        if(HIDPP20_RECLAIM_BACKOFF_MS[i] > 0)
        {
            InterruptibleBackoff(HIDPP20_RECLAIM_BACKOFF_MS[i]);
        }

        if(!device_online.load())
        {
            return false;
        }

        if(teardown_pending.load())
        {
            LOG_DEBUG("%s SW control claim abandoned, link change pending", LOG_TAG);
            return false;
        }

        if(reapply_active_mode_fn && reapply_active_mode_fn())
        {
            LOG_INFO("%s SW control claimed after %s (attempt %zu/%zu)",
                     LOG_TAG, reason, i + 1, attempt_count);
            return true;
        }
    }

    LOG_WARNING("%s SW control claim failed after %s (%zu attempts); the device is still "
                "showing its onboard profile", LOG_TAG, reason, attempt_count);

    return false;
}

void LogitechHIDPP20Controller::ReconnectDevice()
{
    /*-----------------------------------------------------*\
    | Called by power thread when a WirelessStatus          |
    | reconnect event arrives. Race the firmware            |
    | boot animation: hammer the SW-control claim           |
    | + per-key push on a fast-backoff schedule             |
    | until the claim ACKs (matches the vendor              |
    | app, which lands control in ~50ms).                   |
    |                                                       |
    | Both firmware events (reconnect=1/config_needed=1     |
    | then config_needed=0) drive the same retry loop. One  |
    | push per event is not enough: it either races the     |
    | boot animation or lands after it, and an unretried    |
    | claim lets the 10s firmware watchdog drop the device  |
    | back to onboard mode.                                 |
    \*-----------------------------------------------------*/
    LOG_DEBUG("%s Reconnecting device", LOG_TAG);

    FlushResponseQueue();

    bool first_event = !device_online.load();

    if(first_event)
    {
        device_online.store(true);
        consecutive_timeouts.store(0);
        frame_counter = 0;

        {
            std::lock_guard<std::mutex> lock(power_mutex);
            dim_brightness_pct.store(100);
            power_state = HIDPP20_POWER_ACTIVE;
        }
    }

    last_fap_error_ = 0;
    ReclaimSWControl("reconnect", true);
    /*-----------------------------------------------------*\
    | Catch a stale restored map whose reclaim raced the    |
    | radio and didn't surface the mismatch.                |
    \*-----------------------------------------------------*/
    HealStaleMapIfNeeded("reconnect (post-heal)");

    if(first_event && caps.has_power_mgmt)
    {
        ReadFirmwareTimers();
        ReadNvSleepRampConfig();
    }
}

bool LogitechHIDPP20Controller::DiscoveryInProgress() const
{
    return discovery_in_progress_.load();
}

void LogitechHIDPP20Controller::RediscoverFeatures()
{
    /*-----------------------------------------------------*\
    | Clear the cached feature map and its derivations,     |
    | then re-run discovery on the current handle. Each     |
    | link assigns different indices to the same            |
    | features (G515: RGBEffects at 0x09 wireless,          |
    | elsewhere on USB); a stale map turns every cached     |
    | lookup into error 0x07. Caller owns state outside     |
    | the map (claim flag, counters, threads).              |
    |                                                       |
    | Fence the effect engine out during discovery:         |
    | has_perkey and the zone/effect data flap false, so a  |
    | paint/claim landing here would reach flags=5 with an  |
    | empty per-key layer. DeviceUpdateLEDs gates on this.  |
    \*-----------------------------------------------------*/
    discovery_in_progress_.store(true);

    caps.feature_map.clear();
    caps.feature_map_complete = false;
    caps.has_zone_effects      = false;
    caps.has_perkey             = false;
    caps.has_effect_cards       = false;
    caps.effect_card_template[0] = 0;
    caps.effect_card_template[1] = 0;
    caps.zone_clusters.clear();
    caps.perkey_zone_ids.clear();
    caps.idx_rgb_effects            = 0;
    caps.idx_perkey_v2               = 0;
    caps.idx_perkey_v1               = 0;
    caps.idx_wireless_status         = 0;
    caps.idx_profile_management      = 0;
    caps.idx_onboard_profiles        = 0;
    caps.idx_disable_keys_by_usage   = 0;
    caps.fn_set_effect               = 0;
    caps.fn_sw_control           = 0;
    caps.fn_pwr_config           = 0;
    caps.fn_pwr_mode             = 0;
    caps.has_power_mgmt          = false;
    caps.sw_control_simple       = false;
    caps.nv_sleep_ramp_known     = false;
    caps.nv_sleep_ramp_enabled   = false;
    caps.nv_sleep_ramp_seconds   = 0;

    /*-----------------------------------------------------*\
    | idx_unified_battery lives outside caps                |
    | (discovered lazily by QueryExternalPower on first     |
    | use) so it isn't cleared by the caps reset above.     |
    | Clear it here too so the next QueryExternalPower      |
    | call re-probes on the new path, the old path's        |
    | feature index may not exist, or may map to a          |
    | different feature entirely, on the new map.           |
    \*-----------------------------------------------------*/
    idx_unified_battery = 0;
    last_power_raw      = 0xFFFF;

    /*-----------------------------------------------------*\
    | Force ApplyPowerSavingProfile's dedup to              |
    | re-emit its "Idle management: ..." line on            |
    | the next call so a path transition always             |
    | produces a full state confirmation in the             |
    | log, symmetric with the QueryExternalPower            |
    | re-log. Inverting ps_last_logged_external             |
    | guarantees the boolean comparison trips               |
    | regardless of the current power state.                |
    \*-----------------------------------------------------*/
    ps_last_logged_pct      = -1;
    ps_last_logged_idle     = -1;
    ps_last_logged_sleep    = -1;
    ps_last_logged_external = !ps_on_external_power;

    /*-----------------------------------------------------*\
    | Standard HID++ features are looked up on-demand, no   |
    | bulk enumeration needed. Just re-discover everything. |
    \*-----------------------------------------------------*/
    DiscoverDeviceName();
    log_tag = "[LogitechHID++ " + caps.device_name + "]";
    DiscoverDeviceType();
    DiscoverFirmwareInfo();

    caps.idx_profile_management     = GetFeatureIndex(HIDPP20_FEAT_PROFILE_MANAGEMENT);
    caps.idx_onboard_profiles       = GetFeatureIndex(HIDPP20_FEAT_ONBOARD_PROFILES);
    caps.idx_wireless_status        = GetFeatureIndex(HIDPP20_FEAT_WIRELESS_STATUS);
    caps.idx_disable_keys_by_usage  = GetFeatureIndex(HIDPP20_FEAT_DISABLE_KEYS_BY_USAGE);

    DiscoverRGBEffects();
    if(caps.idx_rgb_effects == 0)
    {
        DiscoverHeadsetRGBHostmode();
    }
    DiscoverPerKeyZones();
    DiscoverPerKey8080();
    DiscoverKeyboardLayout();

    /*-----------------------------------------------------*\
    | Discovery done and the map is whole, so let the       |
    | effect engine paint again and cache this link         |
    | for a switch-back.                                    |
    \*-----------------------------------------------------*/
    discovery_in_progress_.store(false);
    CacheCurrentLinkIndexMap();
}

/*---------------------------------------------------------*\
| Per-link feature-index cache: indices are assigned per    |
| transport, so a switch-back restores instead of           |
| rediscovering.                                            |
\*---------------------------------------------------------*/
std::string LogitechHIDPP20Controller::CurrentLinkKey() const
{
    /*-----------------------------------------------------*\
    | Key the link: rx#slot over the dongle, usb#idx        |
    | direct. hidraw paths are reused by the kernel so      |
    | aren't used. A slot collision across dongles is       |
    | caught by the reclaim self-heal.                      |
    \*-----------------------------------------------------*/
    return std::string(wireless ? "rx#" : "usb#") + std::to_string((int)device_index);
}

HIDPP20LinkIndexMap LogitechHIDPP20Controller::SnapshotLinkIndexMap() const
{
    /*-----------------------------------------------------*\
    | Snapshot all of caps, not just indices: the effect-   |
    | card template, zone clusters and per-key data are     |
    | cleared and rediscovered per link too, so a partial-  |
    | index restore would run prep with an empty template.  |
    \*-----------------------------------------------------*/
    HIDPP20LinkIndexMap m;
    m.caps                = caps;
    m.idx_unified_battery = idx_unified_battery;
    m.valid               = true;
    return m;
}

void LogitechHIDPP20Controller::RestoreLinkIndexMap(const HIDPP20LinkIndexMap& m)
{
    caps                = m.caps;
    idx_unified_battery = m.idx_unified_battery;
}

void LogitechHIDPP20Controller::CacheCurrentLinkIndexMap()
{
    /*-----------------------------------------------------*\
    | Skip an empty map and Centurion bridge sub-devices    |
    | (their indices route through the bridge).             |
    \*-----------------------------------------------------*/
    if(caps.idx_rgb_effects == 0 || transport.bridge_feat_idx != 0)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Don't cache a partial discovery over a good           |
    | entry: a flaky link can find RGB effects but          |
    | read back an empty effect-card template.              |
    \*-----------------------------------------------------*/
    if(caps.has_effect_cards
       && caps.effect_card_template[0] == 0
       && caps.effect_card_template[1] == 0)
    {
        return;
    }

    link_index_cache_[CurrentLinkKey()] = SnapshotLinkIndexMap();
}

void LogitechHIDPP20Controller::HealStaleMapIfNeeded(const char* reclaim_reason)
{
    /*-----------------------------------------------------*\
    | A reclaim hitting 0x06/0x07 means the loaded map is   |
    | wrong (re-paired slot). Drop the entry, rediscover,   |
    | reclaim once. No re-check, so it can't loop. A        |
    | no-response failure leaves last_fap_error_ 0, so this |
    | only fires on a real index mismatch.                  |
    \*-----------------------------------------------------*/
    if(last_fap_error_ != 0x06 && last_fap_error_ != 0x07)
    {
        return;
    }

    LOG_WARNING("%s Feature map for %s rejected (err 0x%02X), rediscovering",
                LOG_TAG, CurrentLinkKey().c_str(), last_fap_error_);

    link_index_cache_.erase(CurrentLinkKey());
    RediscoverFeatures();
    last_fap_error_ = 0;
    ReclaimSWControl(reclaim_reason, false);
}

void LogitechHIDPP20Controller::StartEventWatcher()
{
    /*-----------------------------------------------------*\
    | Reader and power threads without power management,    |
    | for devices whose only events are WirelessStatus,     |
    | and for receiver-slot devices with no events of       |
    | their own, so the node watcher's connection nudge     |
    | lands on a power thread.                              |
    \*-----------------------------------------------------*/
    if(reader_running)
    {
        return;
    }

    pending_connection = 0;
    reader_running = true;
    reader_thread = new std::thread(&LogitechHIDPP20Controller::ReaderThreadFunc, this);

    power_thread_running = true;
    power_thread = new std::thread(&LogitechHIDPP20Controller::PowerThreadFunc, this);

    LOG_DEBUG("%s Event watcher started", LOG_TAG);
}

void LogitechHIDPP20Controller::StartPowerManager()
{
    if(caps.idx_rgb_effects == 0 || !caps.has_power_mgmt)
    {
        return;
    }

    if(reader_running)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Cache the boot link's index map (initial              |
    | discovery ran via Initialize, not                     |
    | RediscoverFeatures) so the first switch away and      |
    | back restores instead of rediscovering.               |
    \*-----------------------------------------------------*/
    CacheCurrentLinkIndexMap();

    ReadFirmwareTimers();
    ReadNvSleepRampConfig();
    ReadActiveProfileSector();

    LogitechHIDPP20IdleSettings::instance()->load();
    QueryExternalPower();
    ApplyPowerSavingProfile();

    /*-----------------------------------------------------*\
    | Seed the periodic idle-settings poll clock            |
    | so the first tick of the power thread's               |
    | 500ms re-read happens one interval from now,          |
    | not immediately (we just applied above).              |
    \*-----------------------------------------------------*/
    last_idle_poll = std::chrono::steady_clock::now();

    /*-----------------------------------------------------*\
    | Don't claim SW control here. The device runs its      |
    | firmware effect (or saved hardware profile) until     |
    | DeviceUpdateLEDs is called for the first time, at     |
    | which point claim + push happen atomically.           |
    |                                                       |
    | Reader and power threads still start so we can detect |
    | migration events (USB plug-in) and process activity   |
    | events once SW control is eventually claimed.         |
    \*-----------------------------------------------------*/
    power_state = HIDPP20_POWER_ACTIVE;
    pending_activity = -1;

    reader_running = true;
    reader_thread = new std::thread(&LogitechHIDPP20Controller::ReaderThreadFunc, this);

    power_thread_running = true;
    power_thread = new std::thread(&LogitechHIDPP20Controller::PowerThreadFunc, this);

    LOG_DEBUG("%s Power manager started (idle=%us sleep=%us)",
              LOG_TAG, idle_timeout_s, sleep_timeout_s);
}

void LogitechHIDPP20Controller::StopPowerManager()
{
    if(!reader_running && !power_thread_running)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Stop the power thread first (it may be waiting        |
    | on the queue).                                        |
    \*-----------------------------------------------------*/
    power_thread_running = false;
    response_cv.notify_all();

    if(power_thread && power_thread->joinable())
    {
        power_thread->join();
    }

    delete power_thread;
    power_thread = nullptr;

    /*-----------------------------------------------------*\
    | Then stop reader thread                               |
    \*-----------------------------------------------------*/
    reader_running = false;

    if(reader_thread && reader_thread->joinable())
    {
        reader_thread->join();
    }

    delete reader_thread;
    reader_thread = nullptr;

    /*-----------------------------------------------------*\
    | Wake if we were dimmed/sleeping so Shutdown() can     |
    | cleanly release SW control.                           |
    \*-----------------------------------------------------*/
    if(power_state != HIDPP20_POWER_ACTIVE)
    {
        Wake();
    }

    LOG_DEBUG("%s Power manager stopped", LOG_TAG);
}

void LogitechHIDPP20Controller::ReaderThreadFunc()
{
    /*-----------------------------------------------------*\
    | Sole HID reader. NEVER sends commands;                |
    | that would deadlock (we'd wait on our own             |
    | queue for the response). Events are                   |
    | flagged via atomic for the power thread.              |
    \*-----------------------------------------------------*/
    while(reader_running.load())
    {
        uint8_t feat = 0, func = 0;
        uint8_t data[60] = {};
        int result = ReadHIDDirect(&feat, &func, data, sizeof(data), 50);

        if(result < 0)
        {
            /*---------------------------------------------*\
            | HID read error: device handle is invalid      |
            | (device physically removed). Mark             |
            | offline and sleep to avoid spinning.          |
            \*---------------------------------------------*/
            if(device_online.load())
            {
                LOG_DEBUG("%s HID read error: device removed", LOG_TAG);
                device_online.store(false);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        if(result > 0)
        {
            /*---------------------------------------------*\
            | Check for firmware events first. Events are   |
            | flagged for the power thread and NOT added to |
            | the response queue; they aren't command       |
            | responses and would pollute the queue.        |
            \*---------------------------------------------*/
            if(caps.idx_rgb_effects != 0 &&
               feat == caps.idx_rgb_effects &&
               (func & 0xF0) == 0x10 &&
               (func & 0x0F) != HIDPP20_SW_ID)
            {
                pending_activity.store((int)data[0]);
                continue;
            }

            /*---------------------------------------------*\
            | Feature 0x1D4B event 0: WirelessStatus        |
            | Device reconnected after power cycle. Use     |
            | cached map lookup only, reader thread         |
            | must never send commands (deadlock risk).     |
            \*---------------------------------------------*/
            {
                std::map<uint16_t, uint8_t>::const_iterator it = caps.feature_map.find(0x1D4B);
                uint8_t ws_idx = (it != caps.feature_map.end()) ? it->second : 0;

                if(ws_idx != 0 && feat == ws_idx &&
                   (func & 0xF0) == 0x00 &&
                   (func & 0x0F) != HIDPP20_SW_ID)
                {
                    uint8_t reconnect    = data[0];
                    uint8_t config_needed = data[1];

                    LOG_DEBUG("%s WirelessStatus event: reconnect=%d config_needed=%d",
                              LOG_TAG, reconnect, config_needed);

                    /*-------------------------------------*\
                    | Forward both events to the power      |
                    | thread. Each call into                |
                    | ReconnectDevice runs the fast-        |
                    | backoff reclaim loop, so the          |
                    | second event acts as a belt-and-      |
                    | suspenders re-claim once the          |
                    | firmware boot fully settles.          |
                    \*-------------------------------------*/
                    pending_connection.store(1);

                    continue;
                }
            }

            /*---------------------------------------------*\
            | Only queue responses to OUR commands. Our     |
            | commands use HIDPP20_SW_ID (0x0A) in the      |
            | low nibble. Firmware-generated messages       |
            | (battery, sync, etc.) use SW_ID 0, drop       |
            | those silently. Error responses               |
            | (feat=0xFF) are always queued.                |
            \*---------------------------------------------*/
            if(feat != 0xFF && (func & 0x0F) != HIDPP20_SW_ID)
            {
                continue;
            }
            {
                std::lock_guard<std::mutex> lock(response_mutex);
                HIDPP20RawMessage msg;
                msg.feat   = feat;
                msg.func   = func;
                msg.result = result;
                memcpy(msg.data, data, sizeof(msg.data));
                response_queue.push_back(msg);
            }
            response_cv.notify_all();
        }
    }
}

void LogitechHIDPP20Controller::PowerThreadFunc()
{
    /*-----------------------------------------------------*\
    | Handles power state machine and sends commands. Reads |
    | responses from the queue (filled by reader thread).   |
    \*-----------------------------------------------------*/
    while(power_thread_running.load())
    {
        /*-------------------------------------------------*\
        | 1. Check for pending firmware events              |
        \*-------------------------------------------------*/
        int activity = pending_activity.exchange(-1);

        if(activity >= 0)
        {
            std::lock_guard<std::mutex> lock(power_mutex);
            OnUserActivity((uint8_t)activity);
        }

        /*-------------------------------------------------*\
        | 1b. Check for connection state changes            |
        \*-------------------------------------------------*/
        int connection = pending_connection.exchange(0);

        if(connection > 0)
        {
            if(HasBridge())
            {
                ReprobeSubDevice();
            }
            else
            {
                ReconnectDevice();
            }
        }
        else if(connection < 0)
        {
            LOG_DEBUG("%s Device disconnected", LOG_TAG);
            device_online.store(false);
        }

        /*-------------------------------------------------*\
        | 2. Power management timing                        |
        \*-------------------------------------------------*/
        {
            std::lock_guard<std::mutex> lock(power_mutex);

            switch(power_state)
            {
                case HIDPP20_POWER_DIMMING:
                {
                    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
                    if(now >= next_dim_time)
                    {
                        DimRampStep();
                        next_dim_time = now + std::chrono::milliseconds(DIM_INTERVAL_MS);
                    }
                    break;
                }

                case HIDPP20_POWER_IDLE:
                    /*-------------------------------------*\
                    | Poll dim brightness target, if the    |
                    | user is dragging the slider,          |
                    | ps_dim_target_pct updates in-         |
                    | memory and we pick it up here on      |
                    | the next 50ms tick without any        |
                    | callback/repaint chain.               |
                    |                                       |
                    | Gated on ps_dim_enabled: a profile    |
                    | (or the default unconfigured          |
                    | fallback) can enter IDLE state via    |
                    | the skip-dim path in OnUserActivity,  |
                    | and we must not dim in that case:     |
                    | only sleep when the deadline hits.    |
                    \*-------------------------------------*/
                    if(ps_dim_enabled &&
                       dim_brightness_pct.load() != ps_dim_target_pct)
                    {
                        dim_brightness_pct.store(ps_dim_target_pct);

                        if(request_repaint_fn)
                        {
                            request_repaint_fn();
                        }
                    }

                    if(sleep_timeout_s > 0 && ps_sleep_enabled &&
                       std::chrono::steady_clock::now() >= sleep_deadline)
                    {
                        StartSleep();
                    }
                    break;

                default:
                    break;
            }
        }

        /*-------------------------------------------------*\
        | Fast poll of idle settings + external-power       |
        | flag. QueryExternalPower is a single HID++        |
        | 0x1004 GetStatus call, cheap on wire and lets     |
        | ApplyPowerSavingProfile pick between the          |
        | on_battery and plugged_in profiles within half a  |
        | second of a power-source transition. The idle-    |
        | settings reload itself is purely in-memory.       |
        \*-------------------------------------------------*/
        if(caps.has_power_mgmt)
        {
            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            if(now - last_idle_poll >= std::chrono::milliseconds(500))
            {
                last_idle_poll = now;
                QueryExternalPower();
                ApplyPowerSavingProfile();
            }
        }

        /*-------------------------------------------------*\
        | Fire any pending retry-paint whose                |
        | deadline has come due. The callback runs          |
        | DeviceUpdateLEDs on this thread's context,        |
        | not recursively inside another call.              |
        \*-------------------------------------------------*/
        TickRetryPaintIfPending();

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

bool LogitechHIDPP20Controller::IsCurrentlyWireless() const
{
    return wireless;
}

bool LogitechHIDPP20Controller::QueryExternalPower()
{
    /*-----------------------------------------------------*\
    | Query HID++ 2.0 feature 0x1004                        |
    | (UnifiedBattery) fn1 GetStatus and determine          |
    | whether the device is drawing external power.         |
    |                                                       |
    | Response layout: byte 2: Charging Status 0 =          |
    | Discharging 1 = Charging (wired) 2 = Charging         |
    | (slow) 3 = Complete 4 = Error 5 = Wireless            |
    | Charging byte 3: External Power Status 0 = no         |
    | external power non-zero = external power present      |
    |                                                       |
    | We consider the device externally powered if          |
    | EITHER byte is non-zero: some devices leave byte 3    |
    | at 0 whenever they are actively charging and rely     |
    | on byte 2 alone to signal the wired state. The        |
    | pre-refactor QueryOnBattery used the same OR          |
    | semantic (expressed from the on-battery side) and     |
    | was known to work across the Logitech lineup.         |
    |                                                       |
    | Updates ps_on_external_power and returns              |
    | the new value. On failure returns the                 |
    | cached value without touching it.                     |
    \*-----------------------------------------------------*/
    if(caps.idx_rgb_effects == 0 || !caps.has_power_mgmt)
    {
        return ps_on_external_power;
    }

    if(idx_unified_battery == 0)
    {
        idx_unified_battery = GetFeatureIndex(HIDPP20_FEAT_UNIFIED_BATTERY,
                                              HIDPP20_POLICY_PROBE);

        if(idx_unified_battery == 0)
        {
            /*---------------------------------------------*\
            | Device doesn't expose UnifiedBattery.         |
            | Wired-only devices (no battery) report the    |
            | feature absent; we treat them as              |
            | permanently externally powered.               |
            \*---------------------------------------------*/
            ps_on_external_power = true;
            return ps_on_external_power;
        }
    }

    uint8_t send_data[1] = {0};
    uint8_t recv_data[16] = {};

    int result = SendAcked(idx_unified_battery, 0x10,
                           send_data, 0, recv_data, sizeof(recv_data),
                           HIDPP20_POLICY_PROBE);

    if(result <= 0)
    {
        LOG_TRACE("%s QueryExternalPower: GetStatus failed (result=%d): using cached",
                  LOG_TAG, result);
        return ps_on_external_power;
    }

    uint8_t charge_status  = recv_data[2];
    uint8_t external_power = recv_data[3];

    ps_on_external_power = (charge_status != 0) || (external_power != 0);

    uint16_t raw = ((uint16_t)charge_status << 8) | external_power;
    if(raw != last_power_raw)
    {
        last_power_raw = raw;
        LOG_TRACE("%s QueryExternalPower: charge_status=%u external_power=%u -> %s",
                  LOG_TAG, charge_status, external_power,
                  ps_on_external_power ? "external" : "battery");
    }

    return ps_on_external_power;
}

void LogitechHIDPP20Controller::ApplyPowerSavingProfile()
{
    /*-----------------------------------------------------*\
    | Re-read the JSON every invocation. This is a cheap    |
    | in-memory SettingsManager hash lookup + a handful     |
    | of field copies, safe to do on every 500ms power-     |
    | thread tick. Any write from the plugin (or a manual   |
    | JSON edit) therefore applies within one poll          |
    | interval without any cross-boundary signalling.       |
    \*-----------------------------------------------------*/
    LogitechHIDPP20IdleSettings* settings = LogitechHIDPP20IdleSettings::instance();
    settings->load();

    bool prev_dim   = ps_dim_enabled;
    bool prev_sleep = ps_sleep_enabled;

    /*-----------------------------------------------------*\
    | Start from the firmware-timer baseline. Both the      |
    | configured and unconfigured paths return to these if  |
    | they don't explicitly override, so a profile that     |
    | sets idle_timeout_s does not leave a stale value      |
    | behind after the user resets to an empty config.      |
    \*-----------------------------------------------------*/
    idle_timeout_s  = fw_idle_timeout_s;
    sleep_timeout_s = fw_sleep_timeout_s;

    if(!settings->isConfigured())
    {
        /*-------------------------------------------------*\
        | Unconfigured: no plugin in use. We still hold SW  |
        | control so firmware will NOT dim or sleep         |
        | autonomously; it only emits idle events and       |
        | expects the host to act. Run a basic default      |
        | profile ourselves: no dim on idle (OpenRGB users  |
        | generally expect lights to stay on), but still go |
        | to sleep at the firmware-configured timeout.      |
        \*-------------------------------------------------*/
        ps_dim_enabled    = false;
        ps_dim_target_pct = DIM_TARGET_PCT;
        ps_sleep_enabled  = true;

        /*-------------------------------------------------*\
        | Restore firmware defaults if we previously wrote  |
        | custom values from a plugin profile.              |
        \*-------------------------------------------------*/
        if(written_idle_s != fw_idle_timeout_s || written_sleep_s != fw_sleep_timeout_s)
        {
            WritePowerConfig(fw_idle_timeout_s, fw_sleep_timeout_s);
            written_idle_s  = fw_idle_timeout_s;
            written_sleep_s = fw_sleep_timeout_s;
        }

        if(prev_dim   != ps_dim_enabled      || prev_sleep != ps_sleep_enabled ||
           ps_last_logged_pct      != ps_dim_target_pct    ||
           ps_last_logged_idle     != (int)idle_timeout_s  ||
           ps_last_logged_sleep    != (int)sleep_timeout_s ||
           ps_last_logged_external != ps_on_external_power)
        {
            ps_last_logged_pct      = ps_dim_target_pct;
            ps_last_logged_idle     = idle_timeout_s;
            ps_last_logged_sleep    = sleep_timeout_s;
            ps_last_logged_external = ps_on_external_power;
            LOG_DEBUG("%s Idle management: defaults (dim=off, firmware sleep=%us)",
                      LOG_TAG, sleep_timeout_s);
        }
        return;
    }

    /*-----------------------------------------------------*\
    | Configured: pick the active profile based on          |
    | whether the device is currently externally            |
    | powered. ps_on_external_ power is refreshed           |
    | by QueryExternalPower() on the same 500 ms            |
    | power-thread poll that calls us.                      |
    \*-----------------------------------------------------*/
    const LogitechHIDPP20IdleProfile& profile = ps_on_external_power
        ? settings->pluggedIn()
        : settings->onBattery();

    ps_dim_enabled    = profile.dim_when_idle;
    ps_dim_target_pct = profile.dim_when_idle ? profile.dim_brightness : DIM_TARGET_PCT;
    ps_sleep_enabled  = profile.allow_sleep;

    if(profile.dim_when_idle)
    {
        idle_timeout_s = (uint16_t)profile.idle_timeout_s;
    }
    /* else: idle_timeout_s stays at fw_idle_timeout_s from above */

    if(profile.allow_sleep)
    {
        sleep_timeout_s = (uint16_t)profile.sleep_timeout_s;
    }
    else
    {
        /*-------------------------------------------------*\
        | Signal "don't sleep" to the state machine.        |
        | The IDLE branch of PowerThreadFunc gates          |
        | on sleep_timeout_s>0.                             |
        \*-------------------------------------------------*/
        sleep_timeout_s = 0;
    }

    /*-----------------------------------------------------*\
    | Write our timer values to the device RAM so           |
    | the firmware's idle detection aligns with             |
    | our host-side state machine. Only writes              |
    | when values actually change to avoid                  |
    | spamming the bus on every 500ms poll tick.            |
    \*-----------------------------------------------------*/
    if(idle_timeout_s != written_idle_s || sleep_timeout_s != written_sleep_s)
    {
        WritePowerConfig(idle_timeout_s, sleep_timeout_s);
        written_idle_s  = idle_timeout_s;
        written_sleep_s = sleep_timeout_s;
    }

    if(prev_dim   != ps_dim_enabled      || prev_sleep != ps_sleep_enabled ||
       ps_last_logged_pct      != ps_dim_target_pct    ||
       ps_last_logged_idle     != (int)idle_timeout_s  ||
       ps_last_logged_sleep    != (int)sleep_timeout_s ||
       ps_last_logged_external != ps_on_external_power)
    {
        ps_last_logged_pct      = ps_dim_target_pct;
        ps_last_logged_idle     = idle_timeout_s;
        ps_last_logged_sleep    = sleep_timeout_s;
        ps_last_logged_external = ps_on_external_power;
        LOG_DEBUG("%s Idle management: power=%s dim=%s(%d%%) idle=%us sleep=%s(%us)",
                  LOG_TAG,
                  ps_on_external_power ? "external" : "battery",
                  ps_dim_enabled   ? "on" : "off", ps_dim_target_pct,
                  idle_timeout_s,
                  ps_sleep_enabled ? "on" : "off", sleep_timeout_s);
    }
}

void LogitechHIDPP20Controller::FlushResponseQueue()
{
    {
        std::lock_guard<std::mutex> lock(response_mutex);
        response_queue.clear();
    }

    /*-----------------------------------------------------*\
    | Before the reader thread exists, stale frames sit in  |
    | the hidraw fd buffer. A HID++ reply carries nothing   |
    | tying it to its request, every IRoot answer has the   |
    | same 0x00/0x00 header, so a late reply from a timed-  |
    | out call is accepted as the next call's answer and    |
    | every lookup after it is off by one. Drain first.     |
    \*-----------------------------------------------------*/
    if(reader_running.load() || dev == nullptr)
    {
        return;
    }

    uint8_t scratch[64];

    for(int drained = 0; drained < 64; drained++)
    {
        if(hid_read_timeout(dev, scratch, sizeof(scratch), 0) <= 0)
        {
            break;
        }
    }
}

void LogitechHIDPP20Controller::DispatchEvent
    (
    uint8_t         feat,
    uint8_t         func,
    const uint8_t*  data
    )
{
    if(caps.idx_rgb_effects == 0 || data == nullptr)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | onUserActivity = event 1 on RGB Effects (0x8071)      |
    | Event function byte: (1 << 4) | fw_swid Our           |
    | commands use HIDPP20_SW_ID (0x0A); firmware           |
    | events use a different sw_id (typically 0).           |
    \*-----------------------------------------------------*/
    if(feat == caps.idx_rgb_effects &&
       (func & 0xF0) == 0x10 &&
       (func & 0x0F) != HIDPP20_SW_ID)
    {
        OnUserActivity(data[0]);
    }
}

void LogitechHIDPP20Controller::OnUserActivity(uint8_t activity_type)
{
    /*-----------------------------------------------------*\
    | power_mutex must already be held by the caller.       |
    \*-----------------------------------------------------*/
    if(activity_type == 0)
    {
        /*-------------------------------------------------*\
        | IDLE event: firmware detected inactivity.         |
        | Only act if we're currently ACTIVE. Firmware      |
        | sends a burst of ~8 events; ignore repeats.       |
        \*-------------------------------------------------*/
        if(power_state != HIDPP20_POWER_ACTIVE)
        {
            return;
        }

        if(!ps_dim_enabled && !ps_sleep_enabled)
        {
            return;
        }

        LOG_DEBUG("%s onUserActivity: IDLE: starting dim", LOG_TAG);

        /*-------------------------------------------------*\
        | Flush stale per-key ACKs before sending commands  |
        \*-------------------------------------------------*/
        FlushResponseQueue();

        /*-------------------------------------------------*\
        | flags=3 (EFFECT|POWER): keep effect control       |
        | and monitor for user activity. TODO: Solaar       |
        | uses 0x02 idle / 0x04 active and still gets       |
        | onUserActivity, our EFFECT bit may be             |
        | unneeded. Test on hardware before changing.       |
        \*-------------------------------------------------*/
        SetSWControl(3, 3);

        if(!ps_dim_enabled)
        {
            power_state = HIDPP20_POWER_IDLE;

            uint16_t sleep_delay = (sleep_timeout_s > idle_timeout_s)
                                 ? (sleep_timeout_s - idle_timeout_s) : 0;
            sleep_deadline = std::chrono::steady_clock::now()
                           + std::chrono::seconds(sleep_delay);

            LOG_DEBUG("%s Dim disabled, skipping to IDLE (sleep in %us)",
                      LOG_TAG, sleep_delay);
        }
        else
        {
            StartDimRamp();
        }
    }
    else
    {
        /*-------------------------------------------------*\
        | ACTIVE event: user resumed typing.                |
        | Only act if we're NOT already active.             |
        \*-------------------------------------------------*/
        if(power_state == HIDPP20_POWER_ACTIVE)
        {
            return;
        }

        LOG_DEBUG("%s onUserActivity: ACTIVE: waking", LOG_TAG);
        Wake();
    }
}

void LogitechHIDPP20Controller::StartDimRamp()
{
    /*-----------------------------------------------------*\
    | Start the brightness ramp from 100% to                |
    | DIM_TARGET_PCT. The actual dimming happens in         |
    | DeviceUpdateLEDs; it reads dim_brightness_pct         |
    | and scales the color buffer output. This is           |
    | our own host-side animation, independent of           |
    | the firmware's sleep-ramp timer.                      |
    \*-----------------------------------------------------*/
    dim_step      = 0;
    next_dim_time = std::chrono::steady_clock::now();
    power_state   = HIDPP20_POWER_DIMMING;

    LOG_DEBUG("%s Dim ramp started (100%% -> %d%%)", LOG_TAG, ps_dim_target_pct);
}

void LogitechHIDPP20Controller::DimRampStep()
{
    /*-----------------------------------------------------*\
    | power_mutex must already be held by the caller.       |
    | Adjusts brightness and requests a repaint so          |
    | DeviceUpdateLEDs pushes the dimmed colors.            |
    \*-----------------------------------------------------*/
    if(power_state != HIDPP20_POWER_DIMMING)
    {
        return;
    }

    dim_step++;

    int target     = ps_dim_target_pct;
    int brightness = 100 - ((100 - target) * dim_step / DIM_STEPS);

    if(brightness < target)
    {
        brightness = target;
    }

    dim_brightness_pct.store(brightness);

    /*-----------------------------------------------------*\
    | Request repaint so DeviceUpdateLEDs applies           |
    | the new brightness. For animations this is            |
    | redundant (the animation loop already calls           |
    | it), but for static colors this is the only           |
    | way to push the dimmed output.                        |
    |                                                       |
    | Do NOT bump init_generation here; that would clear    |
    | sent_colors and make the next DeviceUpdateLEDs treat  |
    | the frame as a first-push, firing the                 |
    | SetZoneEffect(0xFF, static black, persist=true) prep  |
    | call. On mice that flash as a brief black-out per dim |
    | step. Delta tracking already handles the changed      |
    | brightness correctly: snapshot is the scaled output,  |
    | sent_colors holds the previously scaled frame, and    |
    | the diff catches every pixel that moved.              |
    \*-----------------------------------------------------*/
    if(request_repaint_fn)
    {
        request_repaint_fn();
    }

    /*-----------------------------------------------------*\
    | Check if dim ramp is complete                         |
    \*-----------------------------------------------------*/
    if(dim_step >= DIM_STEPS)
    {
        power_state = HIDPP20_POWER_IDLE;

        /*-------------------------------------------------*\
        | Pull the sleep deadline forward by the            |
        | firmware's off-ramp duration so the               |
        | firmware fade *ends* at the user-configured       |
        | sleep_timeout_s. Without this we'd be late        |
        | by nv_sleep_ramp_seconds (30s on G515).           |
        \*-------------------------------------------------*/
        uint16_t effective_sleep = sleep_timeout_s;

        if(caps.nv_sleep_ramp_known && caps.nv_sleep_ramp_enabled
           && caps.nv_sleep_ramp_seconds < sleep_timeout_s)
        {
            effective_sleep -= caps.nv_sleep_ramp_seconds;
        }

        uint16_t sleep_delay = (effective_sleep > idle_timeout_s)
                             ? (effective_sleep - idle_timeout_s) : 0;

        sleep_deadline = std::chrono::steady_clock::now()
                       + std::chrono::seconds(sleep_delay);

        LOG_DEBUG("%s Dim complete, IDLE (sleep in %us, effective_sleep=%us)",
                  LOG_TAG, sleep_delay, effective_sleep);
    }
}

void LogitechHIDPP20Controller::StartSleep()
{
    /*-----------------------------------------------------*\
    | SetRgbPowerMode(3) = firmware-managed fade to         |
    | off. The firmware handles the fade internally.        |
    |                                                       |
    | Set power_state BEFORE sending the command so that    |
    | DeviceUpdateLEDs sees SLEEPING and stops pushing      |
    | frames before the sleep command hits the wire.        |
    | Suppression is the safe default: a write arriving     |
    | after SetRgbPowerMode (3) can otherwise wake the      |
    | device and cancel the sleep. Devices carrying         |
    | FADE_ACCEPTS_WRITES opt out of suppression, their     |
    | firmware tolerates writes during the fade.            |
    \*-----------------------------------------------------*/
    LOG_DEBUG("%s Entering sleep (SetRgbPowerMode 3)", LOG_TAG);

    power_state = HIDPP20_POWER_SLEEPING;

    uint8_t data[3] = { 0x01, 0x03, 0x00 };
    blankFAPmessage response;
    int result = SendAckedIntoFAP(caps.idx_rgb_effects, caps.fn_pwr_mode,
                                  data, 3, response);

    if(result <= 0)
    {
        LOG_DEBUG("%s SetRgbPowerMode(3) failed after retries (result=%d), "
                  "reverting to IDLE", LOG_TAG, result);
        power_state = HIDPP20_POWER_IDLE;
    }
}

void LogitechHIDPP20Controller::Wake()
{
    /*-----------------------------------------------------*\
    | Called from OnUserActivity(1) when the firmware       |
    | reports user input. Works for DIMMING, IDLE and       |
    | SLEEPING uniformly; the only wrinkle is that          |
    | SLEEPING means we previously sent                     |
    | SetRgbPowerMode(3) to start the fade, so we must      |
    | cancel it with SetRgbPowerMode(1) first.              |
    |                                                       |
    | Per the 0x8071 lifecycle a proper wake is power       |
    | mode 1 (if we were sleeping), then                    |
    | SetSWControl(3,5) to re-claim rendering from the      |
    | firmware's idle-monitor mode, then re-push the        |
    | current lighting state at full brightness.            |
    |                                                       |
    | The re-push uses request_repaint_fn                   |
    | (DeviceUpdateLEDs) NOT reapply_active_mode_fn,        |
    | which re-runs the full claim and per-key prep.        |
    | Wake is NOT a reconnect: the device handle,           |
    | feature map, SW control claim and per-key prep        |
    | are all still intact. Re-running the claim would      |
    | briefly reset the zone effect layer and flash the     |
    | firmware default colors for ~50ms before per-key      |
    | takes back over.                                      |
    |                                                       |
    | power_mutex must already be held by the caller.       |
    \*-----------------------------------------------------*/
    HIDPP20PowerState prev = power_state;

    FlushResponseQueue();

    if(prev == HIDPP20_POWER_SLEEPING)
    {
        /*-------------------------------------------------*\
        | Cancel the firmware's fade-to-off. SW             |
        | control is still ours; this is not a              |
        | reconnect. The device stays on the same           |
        | hidraw handle, same feature map, same claim.      |
        \*-------------------------------------------------*/
        SetRGBPowerMode(1);
    }

    SetSWControl(3, 5);
    dim_brightness_pct.store(100);
    deep_sleep.store(false);
    consecutive_frame_end_failures.store(0);
    power_state = HIDPP20_POWER_ACTIVE;

    LOG_DEBUG("%s Woke from state %d", LOG_TAG, prev);

    /*-----------------------------------------------------*\
    | Re-push the current lighting state at full            |
    | brightness.                                           |
    |                                                       |
    | We use request_repaint_fn (lightweight: just calls    |
    | DeviceUpdateLEDs) NOT reapply_active_mode_fn          |
    | (heavyweight: reruns ClaimSWControlIfNeeded ->        |
    | SetOnboardMode -> per-key prep sequence ->            |
    | DeviceUpdateMode). On wake-from-dim/idle the device   |
    | is still in host mode, SW control is still claimed,   |
    | and the per-key prep has already been established so  |
    | all we need is a fresh paint at restored brightness.  |
    |                                                       |
    | The brightness was restored to 100% above             |
    | (dim_brightness_pct.store(100)), so DeviceUpdateLEDs  |
    | will apply the full-brightness multiplier to the      |
    | snapshot. Since sent_colors was recorded at the       |
    | previous (dimmed) brightness, the delta detects a     |
    | change on every zone and pushes a full frame          |
    | naturally, no sent_colors.clear() needed.             |
    |                                                       |
    | ReapplyActiveMode (the heavyweight path) is           |
    | reserved for reconnects where the device was          |
    | fully re-enumerated and needs the complete            |
    | claim + prep + mode re-establishment.                 |
    \*-----------------------------------------------------*/
    wake_full_repaint_pending_.store(true);

    if(request_repaint_fn)
    {
        request_repaint_fn();
    }
}

bool LogitechHIDPP20Controller::ConsumeWakeFullRepaint()
{
    return wake_full_repaint_pending_.exchange(false);
}

void LogitechHIDPP20Controller::ReadFirmwareTimers()
{
    if(caps.idx_rgb_effects == 0 || !caps.has_power_mgmt)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | GetRgbPowerModeConfig (fn7, sub-function 0x00 = get)  |
    | Response: [echo], idle_hi/lo, sleep_hi/lo             |
    \*-----------------------------------------------------*/
    uint8_t send_data[1] = { 0x00 };
    uint8_t recv_data[16] = {};

    int result = SendAndReceive(caps.idx_rgb_effects, caps.fn_pwr_config,
                                send_data, 1, recv_data, sizeof(recv_data));

    if(result > 0)
    {
        uint16_t idle  = ((uint16_t)recv_data[3] << 8) | recv_data[4];
        uint16_t sleep = ((uint16_t)recv_data[5] << 8) | recv_data[6];

        if(idle > 0)
        {
            idle_timeout_s    = idle;
            fw_idle_timeout_s = idle;
        }

        if(sleep > 0)
        {
            sleep_timeout_s    = sleep;
            fw_sleep_timeout_s = sleep;
        }

        written_idle_s  = idle;
        written_sleep_s = sleep;

        LOG_TRACE("%s Firmware timers: idle=%us sleep=%us", LOG_TAG, idle_timeout_s, sleep_timeout_s);
    }
    else
    {
        written_idle_s  = idle_timeout_s;
        written_sleep_s = sleep_timeout_s;

        LOG_DEBUG("%s Failed to read firmware timers, using defaults (idle=%us sleep=%us)",
                  LOG_TAG, idle_timeout_s, sleep_timeout_s);
    }
}

void LogitechHIDPP20Controller::ReadNvSleepRampConfig()
{
    /*----------------------------------------------------------*\
    | RGBEffects fn3 NV_CONFIG (0x30) read of capability 0x0020  |
    | (Off Ramp / Sleep Transition).                             |
    |                                                            |
    | Wire format (matches observed wire capture):               |
    |   request:  short msg, data = [0x00, cap_hi, cap_lo]       |
    |             where 0x00 = sub-function GET                  |
    |   response: long msg, data = [echo (3 bytes), enabled,     |
    |             ramp_seconds, ...]                             |
    |                                                            |
    | G515 default observed from vendor app: enabled=0x01,       |
    | seconds=0x1E (= 30 seconds dim ramp before sleep).         |
    \*----------------------------------------------------------*/
    if(caps.idx_rgb_effects == 0 || !caps.has_power_mgmt)
    {
        return;
    }

    uint8_t send_data[3] = { 0x00, 0x00, 0x20 };
    uint8_t recv_data[16] = {};

    int result = SendAndReceive(caps.idx_rgb_effects, FN_8071_NV_CONFIG,
                                send_data, sizeof(send_data),
                                recv_data, sizeof(recv_data));

    if(result <= 0)
    {
        LOG_DEBUG("%s NvConfig 0x0020 read failed (result=%d)", LOG_TAG, result);
        return;
    }

    if(recv_data[0] != 0x00 || recv_data[1] != 0x00 || recv_data[2] != 0x20)
    {
        LOG_DEBUG("%s NvConfig 0x0020 read: unexpected echo %02X %02X %02X",
                  LOG_TAG, recv_data[0], recv_data[1], recv_data[2]);
        return;
    }

    caps.nv_sleep_ramp_enabled = (recv_data[3] != 0);
    caps.nv_sleep_ramp_seconds = recv_data[4];
    caps.nv_sleep_ramp_known   = true;

    LOG_DEBUG("%s NvConfig 0x0020 (sleep ramp): enabled=%d ramp=%us "
              "raw=[%02X %02X %02X %02X %02X %02X %02X %02X]",
              LOG_TAG,
              (int)caps.nv_sleep_ramp_enabled,
              (unsigned)caps.nv_sleep_ramp_seconds,
              recv_data[3], recv_data[4], recv_data[5], recv_data[6],
              recv_data[7], recv_data[8], recv_data[9], recv_data[10]);
}

void LogitechHIDPP20Controller::WritePowerConfig(uint16_t idle_s, uint16_t sleep_s)
{
    /*-----------------------------------------------------*\
    | SetRgbPowerModeConfig (fn7, sub-function 0x01 = set)  |
    | Wire format (long message, 16 bytes payload, matches  |
    | the GET response layout at the same offsets): [0x01,  |
    | 0x00, 0x00, idle_hi, idle_lo, sleep_hi, sleep_lo,     |
    | 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00] |
    |                                                       |
    | These are the firmware's *runtime* power timers,      |
    | the values reset on power cycle but persist           |
    | across SW control release/reclaim, so we need to      |
    | write them ourselves on every claim to be safe.       |
    \*-----------------------------------------------------*/
    if(caps.idx_rgb_effects == 0 || !caps.has_power_mgmt)
    {
        return;
    }

    uint8_t data[16] = {};
    data[0] = 0x01;     // sub-function: SET
    data[3] = (uint8_t)((idle_s  >> 8) & 0xFF);
    data[4] = (uint8_t)( idle_s        & 0xFF);
    data[5] = (uint8_t)((sleep_s >> 8) & 0xFF);
    data[6] = (uint8_t)( sleep_s       & 0xFF);

    blankFAPmessage response;
    SendAckedIntoFAP(caps.idx_rgb_effects, caps.fn_pwr_config,
                     data, sizeof(data), response);

    LOG_DEBUG("%s WritePowerConfig: idle=%us sleep=%us", LOG_TAG, idle_s, sleep_s);
}

void LogitechHIDPP20Controller::ReadActiveProfileSector()
{
    /*----------------------------------------------------------*\
    | Diagnostic-only read of the active profile sector via      |
    | ProfileManagement (0x8101) load + paged readBuffer.        |
    |                                                            |
    | This sector is the canonical storage for persisted device  |
    | state on G-series devices: idle/sleep timers, baseline     |
    | RGB effect, FKC enable, and more. The HID++ feature        |
    | endpoints (0x8071, 0x8081, 0x1B05, ...) are mostly status  |
    | hooks; the configuration database lives here. We don't     |
    | act on the contents, just log them so we can see what      |
    | the device thinks its persisted state is.                  |
    |                                                            |
    | Wire format mirrors observed wire capture (load followed   |
    | by 7x readBuffer):                                         |
    |   load:        long msg, [partition=0x01, sector=0x01,     |
    |                           size_hi=0x00, size_lo=0x63,      |
    |                           padding to 16 bytes]             |
    |   readBuffer:  short msg, [offset_hi, offset_lo, 0]        |
    |                returns long msg with 16 bytes of data      |
    |                                                            |
    | Sector size 0x63 = 99 bytes is what the vendor app         |
    | requested for the G515 active profile. Other devices may   |
    | differ; it is hardcoded because this path is diagnostic.   |
    \*----------------------------------------------------------*/
    if(caps.idx_profile_management == 0)
    {
        return;
    }

    constexpr uint16_t SECTOR_SIZE = 0x63;  // 99 bytes
    constexpr uint16_t PAGE_SIZE   = 16;

    /*-----------------------------------------------------*\
    | Step 1: load the sector into the device's read buffer |
    \*-----------------------------------------------------*/
    uint8_t load_data[16] = {};
    load_data[0] = 0x01;                                    // partition: NVS/flash
    load_data[1] = 0x01;                                    // sector: active profile
    load_data[2] = (uint8_t)((SECTOR_SIZE >> 8) & 0xFF);    // size hi
    load_data[3] = (uint8_t)( SECTOR_SIZE       & 0xFF);    // size lo

    blankFAPmessage load_resp;
    int load_result = SendAckedIntoFAP(caps.idx_profile_management, FN_8101_LOAD,
                                       load_data, sizeof(load_data), load_resp);

    if(load_result <= 0)
    {
        LOG_DEBUG("%s ProfileSector load failed (result=%d)", LOG_TAG, load_result);
        return;
    }

    /*-----------------------------------------------------*\
    | Step 2: page the sector out 16 bytes at a time        |
    \*-----------------------------------------------------*/
    uint8_t sector_buf[SECTOR_SIZE] = {};

    for(uint16_t offset = 0; offset < SECTOR_SIZE; offset += PAGE_SIZE)
    {
        uint8_t read_req[3] = {
            (uint8_t)((offset >> 8) & 0xFF),
            (uint8_t)( offset       & 0xFF),
            0x00
        };
        uint8_t page_resp[20] = {};

        int result = SendAndReceive(caps.idx_profile_management, FN_8101_READBUFFER,
                                    read_req, sizeof(read_req),
                                    page_resp, sizeof(page_resp));

        if(result <= 0)
        {
            LOG_DEBUG("%s ProfileSector readBuffer offset=0x%04X failed (result=%d)",
                      LOG_TAG, (unsigned)offset, result);
            return;
        }

        size_t copy_len = (offset + PAGE_SIZE > SECTOR_SIZE)
                        ? (size_t)(SECTOR_SIZE - offset)
                        : PAGE_SIZE;
        memcpy(sector_buf + offset, page_resp, copy_len);
    }

    /*-----------------------------------------------------*\
    | Step 3: log as a hexdump, one row per 16 bytes        |
    \*-----------------------------------------------------*/
    LOG_DEBUG("%s ProfileSector partition=NVS sector=1 size=%u bytes:",
              LOG_TAG, (unsigned)SECTOR_SIZE);

    for(uint16_t row = 0; row < SECTOR_SIZE; row += PAGE_SIZE)
    {
        size_t row_len = (row + PAGE_SIZE > SECTOR_SIZE)
                       ? (size_t)(SECTOR_SIZE - row)
                       : PAGE_SIZE;

        char hex[64] = {};
        char* p = hex;
        for(size_t i = 0; i < row_len; i++)
        {
            snprintf(p, 4, "%02X ", sector_buf[row + i]);
            p += 3;
        }

        LOG_DEBUG("%s   %04X: %s", LOG_TAG, (unsigned)row, hex);
    }
}

