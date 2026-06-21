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
#include "LogitechHIDPP20Controller.h"
#include "RGBController_LogitechHIDPP20.h"
#include "LogManager.h"

#include "LogitechHIDPP20IdleSettings.h"


#define LOG_TAG log_tag.c_str()

/*----------------------------------------------------------*\
| Hard cap on per-call non-HID++ drains in the read loop.    |
| A high-polling-rate mouse can put 50+ input reports in the |
| buffer between our reads; this cap prevents pathological   |
| input-flood scenarios from locking up a single read call.  |
| 64 is enough headroom for normal congestion at 1 kHz.      |
\*----------------------------------------------------------*/
static const int HIDPP20_READ_DRAIN_BUDGET = 64;

/*----------------------------------------------------------*\
| Per-candidate read timeout (ms) for the Centurion 0x50     |
| device-address probe. USB round-trip is <1ms; 5ms gives    |
| 5x margin. Worst case (no device responds) 256 × 5 =       |
| ~1.3s; typical G522 at addr 0x23 is ~180ms. Matches        |
| Solaar's probe_centurion_device_addr constant.             |
\*----------------------------------------------------------*/
static const int CENTURION_PROBE_PER_ADDR_TIMEOUT_MS = 5;

/*----------------------------------------------------------*\
| Observed HID++ 2.0 feature versions. Each row is a feature |
| ID plus the versions we've empirically verified working.   |
| When feature discovery reports a version outside this set, |
| we log a one-shot INFO tripwire so a tester with new       |
| hardware immediately surfaces unknown firmware revs.       |
|                                                            |
| Purely observational — no behavior branches on version.    |
| Solaar has effectively zero version gating for the RGB     |
| features we implement, so we don't either; the table is a  |
| "have we seen this combination work" ledger, not a         |
| compatibility matrix. Add versions as devices report them. |
|                                                            |
| A feature_id absent from this table is silent (no          |
| tripwire). Only features we actually exercise are worth    |
| flagging.                                                  |
\*----------------------------------------------------------*/
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
    { 0x4540, { 1 }, 1 },
    { 0x8071, { 4 }, 1 },
    { 0x8081, { 0, 2 }, 2 },
};

/*----------------------------------------------------------*\
| Returns true if feature_id is not tracked (silent) or if   |
| version appears in the tracked feature's approved set.     |
\*----------------------------------------------------------*/
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

    return true;        /* feature not tracked — silent */
}

LogitechHIDPP20Controller::LogitechHIDPP20Controller
    (
    hid_device*                 dev,
    const char*                 path,
    uint8_t                     device_index,
    bool                        wireless,
    std::shared_ptr<std::mutex> mutex_ptr,
    uint16_t                    usage_page
    )
{
    this->dev           = dev;
    this->location      = path;
    this->device_index  = device_index;
    this->wireless      = wireless;
    this->mutex         = mutex_ptr;
    this->initialized                  = false;
    this->sw_control_claimed           = false;
    this->sw_control_needs_upgrade_to_5 = false;
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
    this->pending_path_check   = 0;
    this->device_online        = true;
    this->consecutive_timeouts = 0;
    this->watcher_mode         = false;
    this->power_state          = HIDPP20_POWER_ACTIVE;
    this->deep_sleep           = false;
    this->consecutive_frame_end_failures = 0;
    this->dim_brightness_pct   = 100;
    this->dim_step             = 0;
    this->idle_timeout_s       = 60;
    this->sleep_timeout_s      = 300;

    caps = {};

    /*---------------------------------------------------------*\
    | Default to standard HID++ transport; DiscoverTransport()  |
    | may change this during Probe() if Centurion is detected.  |
    \*---------------------------------------------------------*/
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
    if(initialized)
    {
        Shutdown();
    }

    if(dev)
    {
        hid_close(dev);
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
    /*---------------------------------------------------------*\
    | When the reader thread is running, it is the sole caller  |
    | of hid_read_timeout. All other reads come from the queue. |
    | Before the reader starts (during Probe/Initialize), read  |
    | directly from HID.                                        |
    \*---------------------------------------------------------*/
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
            /*-------------------------------------------------*\
            | Offline detection lives at the SendAcked layer    |
            | now: one tick per fully-failed call, not per      |
            | per-attempt read window. Streaming policies that  |
            | retry several times don't artificially accelerate |
            | the offline declaration.                          |
            \*-------------------------------------------------*/
            return 0;
        }

        if(!reader_running.load())
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
    /*---------------------------------------------------------*\
    | Thin wrapper around SendAcked with the reliable policy.   |
    | Preserved as a named entry point so existing call sites   |
    | don't need to be touched.                                 |
    \*---------------------------------------------------------*/
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
    /*----------------------------------------------------------*\
    | Universal send-and-ack with policy-driven retry. Mirrors   |
    | the firmware's own event burst pattern: 7-attempt          |
    | exponential backoff for reliable one-shot commands, tight  |
    | 2-attempt for streaming animation frames.                  |
    |                                                            |
    | Loop semantics per attempt:                                |
    |   1. Sleep backoff_ms[i] (0 on first attempt)              |
    |   2. Bail if device went offline                           |
    |   3. SendMessage; on wire error, mark and retry            |
    |   4. Read loop bounded by read_window_ms:                  |
    |      - matching response  -> success                       |
    |      - HID++ error 0xFF for our request:                   |
    |          BUSY (0x08) + retry_on_busy -> retry the send     |
    |          other code                  -> hard fail (-1)     |
    |      - HID++ error for different request -> discard        |
    |      - non-matching, non-error frame     -> discard        |
    |      - read timeout (0)                  -> retry the send |
    \*----------------------------------------------------------*/
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
        /*-----------------------------------------------------*\
        | Backoff before each attempt (0 on first)              |
        \*-----------------------------------------------------*/
        uint16_t delay_ms = policy.backoff_ms[attempt];

        if(delay_ms > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        }

        /*-----------------------------------------------------*\
        | Bail early if device went offline mid-retry           |
        \*-----------------------------------------------------*/
        if(!device_online.load())
        {
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

        /*-----------------------------------------------------*\
        | Read loop bounded by per-attempt window. Drain        |
        | non-matching HID++ frames within this window — they   |
        | are stale responses or unrelated events from prior    |
        | commands. Only retry the send if the window expires   |
        | with no match (lost on wire) or we got BUSY.          |
        \*-----------------------------------------------------*/
        std::chrono::steady_clock::time_point window_deadline = std::chrono::steady_clock::now()
                                                               + std::chrono::milliseconds(policy.read_window_ms);
        bool need_resend = false;

        while(!need_resend)
        {
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
                /* Wire error — propagate, don't retry */
                return -2;
            }

            if(rd == 0)
            {
                /* Window drained with nothing matching — retry the send */
                last_result = 0;
                break;
            }

            /*-------------------------------------------------*\
            | HID++ error frame                                 |
            | feat=0xFF, func=err_feat, data[0]=err_func,       |
            | data[1]=err_code                                  |
            \*-------------------------------------------------*/
            if(resp_feat == 0xFF)
            {
                uint8_t err_feat = resp_func;
                uint8_t err_func = resp_data[0];
                uint8_t err_code = resp_data[1];

                /*-----------------------------------------------*\
                | Match: either a direct error for our request,   |
                | or a Centurion bridge error attributed to the   |
                | bridge feature index when we're routing through |
                | it. The bridge swallows the sub-device feat in  |
                | the error response, so all bridge-routed        |
                | failures look like errors from the bridge.      |
                \*-----------------------------------------------*/
                bool is_our_error =
                    (err_feat == feat_idx &&
                     (err_func & 0xF0) == (function & 0xF0))
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

                    if(hidpp20_error_out)
                    {
                        *hidpp20_error_out = err_code;
                    }

                    return -1;
                }

                /* Error for a different request — stale, discard and keep reading */
                continue;
            }

            /*-------------------------------------------------*\
            | Match our expected response                       |
            \*-------------------------------------------------*/
            if(resp_feat == feat_idx &&
               (resp_func & 0xF0) == (function & 0xF0))
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

    /*----------------------------------------------------------*\
    | Offline detection: tick once per fully-failed call (all    |
    | retry attempts exhausted with no response). At a threshold |
    | of 10 we declare the device gone. Reset to 0 happens on    |
    | any successful call (above) — single delayed responses     |
    | don't push us toward offline.                              |
    \*----------------------------------------------------------*/
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
    /*---------------------------------------------------------*\
    | Compatibility shim for callers that inherited the         |
    | SendLong+ReadResponse interface and inspect               |
    | response.data[] downstream. Calls SendAcked into a local  |
    | buffer, then reconstructs a blankFAPmessage on success.   |
    \*---------------------------------------------------------*/
    response.init();

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

int LogitechHIDPP20Controller::SendStandard
    (
    uint8_t         feat_idx,
    uint8_t         function,
    const uint8_t*  data,
    size_t          len
    )
{
    /*-----------------------------------------------------------*\
    | Auto-select short (0x10, 7 bytes) vs long (0x11, 20 bytes)  |
    | based on data length. Upper layers just provide data;       |
    | transport picks the smallest frame that fits.               |
    |                                                             |
    | Windows exception: HIDClass splits the HID++ short and long |
    | message Top-Level Collections into separate virtual HID     |
    | devices (page 0xFF00 usage 1 vs usage 2). We open the long- |
    | message TLC, which rejects 7-byte writes. Force long format |
    | on Windows so every outgoing frame matches the collection   |
    | we opened — Linux hidraw and macOS IOHIDManager expose both |
    | TLCs through one handle and keep the size-based heuristic.  |
    \*-----------------------------------------------------------*/
    uint8_t buf[LOGITECH_LONG_MESSAGE_LEN];
    size_t  msg_len;

#if defined(_WIN32)
    const bool prefer_short = false;
#else
    const bool prefer_short = (len <= 3);
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
    /*---------------------------------------------------------*\
    | No mutex needed for reads — when the reader thread is     |
    | running, it is the sole caller. Before the reader starts, |
    | all access is single-threaded.                            |
    |                                                           |
    | Loop within the timeout window draining non-HID++ reports |
    | (mouse motion, keystrokes, media keys, DJ events) until   |
    | we either find a HID++ short/long frame or actually time  |
    | out. A high-polling-rate device can put 50+ input reports |
    | in the hidraw buffer between our calls; without the drain |
    | loop the synchronous probe path can never get past them   |
    | to find its response.                                     |
    \*---------------------------------------------------------*/
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
            /* hidapi timeout — window expired with nothing pending. */
            return 0;
        }

        /*-----------------------------------------------------*\
        | Validate report ID. The hidraw can carry HID input    |
        | reports (keyboard, mouse, media keys) in addition to  |
        | HID++. Drop anything that isn't a HID++ short (0x10)  |
        | or long (0x11) message and keep draining within the   |
        | remaining window — otherwise the parser would read    |
        | buf[2]/buf[3] as feat/func and misinterpret           |
        | keystrokes/motion as HID++ events.                    |
        \*-----------------------------------------------------*/
        if(response.buffer[0] != LOGITECH_SHORT_MESSAGE &&
           response.buffer[0] != LOGITECH_LONG_MESSAGE)
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
| TODO: Full implementation — currently stubbed.            |
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
        /*-----------------------------------------------------*\
        | Sub-device message routed through CentPPBridge        |
        | Parent message: feat=bridge, func=sendFragment(0x10)  |
        | Payload: [devId<<4|lenHi, lenLo, subCPL=0x00,         |
        |           subFeatIdx, subFunc|swid, subParams...]     |
        \*-----------------------------------------------------*/
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
        /*-----------------------------------------------------*\
        | Direct parent device message (no bridge)              |
        \*-----------------------------------------------------*/
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

    /*----------------------------------------------------------*\
    | Track an overall deadline so the bridge ACK + MessageEvent |
    | two-read sequence stays within timeout_ms total — without  |
    | this each read could eat the full budget independently.    |
    | Drain non-Centurion report IDs within the remaining window |
    | rather than bailing on the first non-matching frame.       |
    \*----------------------------------------------------------*/
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

    /*---------------------------------------------------------*\
    | Parse based on transport variant                          |
    \*---------------------------------------------------------*/
    int hdr_offset = transport.addressed ? 1 : 0;  // skip device address byte

    uint8_t cpl_len   = buf[1 + hdr_offset];
    // uint8_t cpl_flags = buf[2 + hdr_offset];     // for fragmentation support
    uint8_t resp_feat = buf[3 + hdr_offset];
    uint8_t resp_func = buf[4 + hdr_offset];

    if(transport.bridge_feat_idx != 0 && resp_feat == transport.bridge_feat_idx)
    {
        /*--------------------------------------------------------*\
        | CentPPBridge — distinguish events from command responses |
        |                                                          |
        | Bridge events (e.g. ConnectionStateChangedEvent) have    |
        | func high nibble = 0x00 (event index 0) and swid = 0.    |
        | These are NOT wrapped sub-device responses — they are    |
        | bridge-level notifications. Return as-is so the reader   |
        | thread can detect them.                                  |
        |                                                          |
        | Command responses follow a two-response pattern:         |
        | 1. ACK: bridge echoes feat+func with our swid            |
        | 2. MessageEvent: func=1x, swid=0, wrapped sub-device     |
        \*--------------------------------------------------------*/
        if((resp_func & 0xF0) == 0x00 && (resp_func & 0x0F) != HIDPP20_SW_ID)
        {
            /*-------------------------------------------------*\
            | Bridge event — return feat/func/data as-is        |
            \*-------------------------------------------------*/
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
            /*---------------------------------------------------*\
            | This is the ACK — discard and read the MessageEvent |
            | Use the *remaining* window from the overall         |
            | deadline so the two-read sequence stays bounded,    |
            | and drain non-Centurion frames within that window.  |
            \*---------------------------------------------------*/
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
                /*---------------------------------------------*\
                | Not a bridge response — return as-is          |
                \*---------------------------------------------*/
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

        /*-----------------------------------------------------*\
        | MessageEvent — unwrap sub-device response.            |
        | Bridge params: [devId<<4|lenHi, lenLo, subCPL,        |
        |                 subFeatIdx, subFunc|swid, subData...] |
        \*-----------------------------------------------------*/
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
        /*-----------------------------------------------------*\
        | Direct response                                       |
        \*-----------------------------------------------------*/
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
    /*-----------------------------------------------------------*\
    | Check cache first — both Centurion bulk and HID++ on-demand |
    | lookups store results here.                                 |
    \*-----------------------------------------------------------*/
    std::map<uint16_t, uint8_t>::const_iterator it = caps.feature_map.find(feature_page);

    if(it != caps.feature_map.end())
    {
        return it->second;
    }

    /*---------------------------------------------------------*\
    | Centurion bulk enumeration is complete — if a feature     |
    | isn't in the map, it doesn't exist. No wire query needed. |
    \*---------------------------------------------------------*/
    if(caps.feature_map_complete)
    {
        return 0;
    }

    /*---------------------------------------------------------*\
    | Standard HID++: on-demand IRoot query, cache the result.  |
    \*---------------------------------------------------------*/
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
                LOG_INFO("%s Feature 0x%04X V%u not previously observed — "
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

    /*---------------------------------------------------------*\
    | Cache the miss too so we don't re-query failed lookups    |
    \*---------------------------------------------------------*/
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

void LogitechHIDPP20Controller::DiscoverDeviceName()
{
    /*---------------------------------------------------------*\
    | Centurion sub-devices use 0x0101 (DeviceName).            |
    | Standard HID++ uses 0x0005 (DeviceNameType).              |
    \*---------------------------------------------------------*/
    if(transport.type == HIDPP20_TRANSPORT_CENTURION)
    {
        /*------------------------------------------------------*\
        | Centurion sub-device: 0x0101 getName returns firmware  |
        | data, not a readable name. Fall back to a platform-    |
        | specific lookup that reads the friendly name from the  |
        | OS's HID enumeration (sysfs HID_NAME on Linux,         |
        | hid_device_info::product_string on Windows).           |
        \*------------------------------------------------------*/
        std::string friendly = GetCenturionSubDeviceName(location);

        if(!friendly.empty())
        {
            caps.device_name = friendly;
        }
        else
        {
            caps.device_name = "Logitech Centurion Device";
        }

        LOG_VERBOSE("%s Device name (Centurion): %s", LOG_TAG, caps.device_name.c_str());
        return;
    }

    uint8_t feat_idx = GetFeatureIndex(HIDPP20_FEAT_DEVICE_NAME_TYPE);

    if(feat_idx == 0)
    {
        caps.device_name = "Logitech HID++ Device";
        return;
    }

    uint8_t recv[16] = {};
    int result = SendAcked(feat_idx, LOTITECH_CMD_DEVICE_NAME_TYPE_GET_COUNT,
                           nullptr, 0, recv, sizeof(recv));

    if(result <= 0)
    {
        caps.device_name = "Logitech HID++ Device";
        return;
    }

    unsigned int name_length = recv[0];
    caps.device_name.clear();

    for(unsigned int offset = 0; offset < name_length; offset += 16)
    {
        uint8_t send_data[1] = { (uint8_t)offset };
        result = SendAcked(feat_idx, LOGITECH_CMD_DEVICE_NAME_TYPE_GET_DEVICE_NAME,
                           send_data, 1, recv, sizeof(recv));

        if(result <= 0)
        {
            break;
        }

        unsigned int chunk_len = name_length - offset;
        if(chunk_len > 16)
        {
            chunk_len = 16;
        }

        caps.device_name.append((char*)recv, chunk_len);
    }

    LOG_VERBOSE("%s Device name: %s", LOG_TAG, caps.device_name.c_str());
}

void LogitechHIDPP20Controller::DiscoverDeviceType()
{
    /*----------------------------------------------------------*\
    | Centurion sub-devices don't have 0x0005 (DeviceNameType).  |
    | Default to unknown — don't assume device type from         |
    | transport, as Centurion may be used for future devices.    |
    \*----------------------------------------------------------*/
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

    uint8_t recv[16] = {};
    int result = SendAcked(feat_idx, LOGITECH_CMD_DEVICE_NAME_TYPE_GET_TYPE,
                           nullptr, 0, recv, sizeof(recv));

    if(result > 0)
    {
        caps.device_type = recv[0];
        LOG_VERBOSE("%s Device type: %d", LOG_TAG, caps.device_type);
    }
    else
    {
        caps.device_type = LOGITECH_DEVICE_TYPE_MOUSE;
    }
}

void LogitechHIDPP20Controller::DiscoverTransport()
{
    /*---------------------------------------------------------*\
    | Detect transport type from usage page.                    |
    | 0xFF00/0xFF43: Standard HID++ (0x10/0x11 reports)         |
    | 0xFFA0+:       Centurion (64-byte CPL framing)            |
    \*---------------------------------------------------------*/
    if(transport.usage_page == 0xFF00 || transport.usage_page == 0xFF43)
    {
        transport.type = HIDPP20_TRANSPORT_STANDARD;
        return;
    }

    /*---------------------------------------------------------*\
    | Centurion transport — determine variant from the report   |
    | descriptor. 0x51 = direct (PRO X 2), 0x50 = addressed     |
    | (G522). Parse the descriptor to find which report IDs     |
    | are available with 63-byte payload.                       |
    \*---------------------------------------------------------*/
    transport.type = HIDPP20_TRANSPORT_CENTURION;

    /*---------------------------------------------------------*\
    | Parse report descriptor to find Centurion report IDs      |
    \*---------------------------------------------------------*/
    uint8_t desc[4096];
    int desc_len = hid_get_report_descriptor(dev, desc, sizeof(desc));

    bool has_0x51 = false;
    bool has_0x50 = false;

    if(desc_len > 0)
    {
        for(int i = 0; i < desc_len; i++)
        {
            if(desc[i] == 0x85 && i + 1 < desc_len)
            {
                if(desc[i + 1] == 0x51) has_0x51 = true;
                if(desc[i + 1] == 0x50) has_0x50 = true;
            }
        }
    }

    if(has_0x51)
    {
        transport.report_id = 0x51;
        transport.addressed = false;
        LOG_DEBUG("%s Centurion 0x51 (direct) from descriptor", LOG_TAG);
    }
    else if(has_0x50)
    {
        transport.report_id = 0x50;
        transport.addressed = true;
        transport.device_address = 0x00;

        LOG_DEBUG("%s Centurion 0x50 (addressed) from descriptor", LOG_TAG);

        /*-----------------------------------------------------*\
        | Device-address sweep. 0x50 frames carry a device      |
        | address byte; the device silently drops frames        |
        | addressed to the wrong ID, so we brute-force probe    |
        | every candidate with an IRoot fn1 GetProtocolVersion  |
        | ping. First response wins — real address lives in     |
        | resp_buf[1] of the reply. Mirrors Solaar's            |
        | probe_centurion_device_addr; see                      |
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
            uint8_t probe_buf[64] = {};
            probe_buf[0] = 0x50;
            probe_buf[1] = (uint8_t)addr;
            probe_buf[2] = 0x06;
            probe_buf[3] = 0x00;
            probe_buf[4] = 0x00;
            probe_buf[5] = 0x10;

            int wr = hid_write(dev, probe_buf, 64);
            probe_count++;

            if(wr <= 0)
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
    else
    {
        /*-----------------------------------------------------*\
        | Descriptor didn't reveal report IDs — fall back to    |
        | probing 0x51 then 0x50 by trial.                      |
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

        transport.report_id = 0x50;
        transport.addressed = true;
        transport.device_address = 0x00;

        memset(probe_buf, 0, sizeof(probe_buf));
        probe_buf[0] = 0x50;
        probe_buf[1] = 0x00;
        probe_buf[2] = 3;
        probe_buf[3] = 0x00;
        probe_buf[4] = 0x00;
        probe_buf[5] = 0x00 | HIDPP20_SW_ID;

        wr = hid_write(dev, probe_buf, 64);

        if(wr > 0)
        {
            uint8_t resp_buf[64] = {};
            int rd = hid_read_timeout(dev, resp_buf, sizeof(resp_buf), 500);

            if(rd > 0 && resp_buf[0] == 0x50)
            {
                transport.device_address = resp_buf[1];
                LOG_DEBUG("%s Centurion 0x50 (addressed) from probe, device_addr=0x%02X",
                          LOG_TAG, transport.device_address);
                return;
            }
        }

        LOG_DEBUG("%s Centurion transport: no response (usage_page=0x%04X)",
                  LOG_TAG, transport.usage_page);
    }
}

void LogitechHIDPP20Controller::EnumerateFeatures(uint8_t feature_set_idx)
{
    caps.feature_map.clear();
    caps.feature_map_complete = false;

    /*---------------------------------------------------------*\
    | Root (0x0000) is always at index 0                        |
    \*---------------------------------------------------------*/
    caps.feature_map[0x0000] = 0;

    if(transport.type == HIDPP20_TRANSPORT_CENTURION)
    {
        /*-----------------------------------------------------*\
        | Centurion sub-device: CenturionFeatureSet fn1 returns |
        | ALL features in a single bulk response.               |
        | [count, (feat_hi, feat_lo, type, version) × count]    |
        \*-----------------------------------------------------*/
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
                    LOG_INFO("%s Feature 0x%04X V%u not previously observed — "
                             "tripwire for version-gated behavior",
                             LOG_TAG, feat_id, feat_version);
                }
            }

            caps.feature_map_complete = true;
        }
    }
    else
    {
        /*-----------------------------------------------------*\
        | Standard HID++: no bulk query available. Features are |
        | looked up on-demand via GetFeatureIndex (IRoot) and   |
        | cached in the feature map. Nothing to do here.        |
        \*-----------------------------------------------------*/
        return;
    }
}

void LogitechHIDPP20Controller::DiscoverFirmwareInfo()
{
    /*--------------------------------------------------------------*\
    | Centurion sub-devices use 0x0100 (DeviceInfo) for firmware     |
    | version and serial. Standard HID++ uses 0x0003 (FirmwareInfo). |
    \*--------------------------------------------------------------*/
    if(transport.type == HIDPP20_TRANSPORT_CENTURION)
    {
        uint8_t dev_info_idx = GetFeatureIndex(HIDPP20_FEAT_CENTURION_DEVICE_INFO);

        if(dev_info_idx == 0)
        {
            return;
        }

        /*------------------------------------------------------*\
        | fn1 getFirmwareVersion(entityIndex=0) — main firmware  |
        | Response: [fwType, additional, version_hi, version_lo] |
        \*------------------------------------------------------*/
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

        /*-----------------------------------------------------*\
        | fn2 getSerialNumber on 0x0100 (DeviceInfo)            |
        | Response: [stringLen, serial...]                      |
        \*-----------------------------------------------------*/
        {
            uint8_t recv_data[16] = {};

            int result = SendAcked(dev_info_idx, 0x20,
                                   nullptr, 0, recv_data, sizeof(recv_data));

            if(result > 0)
            {
                uint8_t slen = recv_data[0];
                if(slen > 15) slen = 15;

                caps.serial_number = std::string((char*)&recv_data[1], slen);

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

    /*------------------------------------------------------------*\
    | fn0 GetEntityCount — entity count, unitId, transport PIDs    |
    | Response: [count, unitId(4), transport(2), PID1(2), PID2(2)] |
    \*------------------------------------------------------------*/
    uint8_t entity_count = 1;

    {
        uint8_t recv_data[16] = {};
        int result = SendAcked(fw_idx, 0x00,
                               nullptr, 0, recv_data, sizeof(recv_data));

        if(result > 0)
        {
            entity_count = recv_data[0];

            /*-------------------------------------------------*\
            | Extract unitId — stable hardware identity across  |
            | all connection paths (USB, wireless, dongle).     |
            \*-------------------------------------------------*/
            char uid[16];
            snprintf(uid, sizeof(uid), "%02X%02X%02X%02X",
                     recv_data[1], recv_data[2], recv_data[3], recv_data[4]);
            caps.unit_id = uid;

            caps.pid_wireless = ((uint16_t)recv_data[7] << 8) | recv_data[8];
            caps.pid_wired    = ((uint16_t)recv_data[9] << 8) | recv_data[10];

            /*-------------------------------------------------*\
            | Use unitId as serial if device doesn't report one |
            \*-------------------------------------------------*/
            if(caps.serial_number.empty() && caps.unit_id != "00000000")
            {
                caps.serial_number = caps.unit_id;
            }

            LOG_DEBUG("%s unitId=%s PID1=0x%04X PID2=0x%04X",
                      LOG_TAG, caps.unit_id.c_str(), caps.pid_wireless, caps.pid_wired);

            /*-------------------------------------------------*\
            | Resolve per-model quirks from the PID pair.       |
            \*-------------------------------------------------*/
            caps.quirks = 0;

            size_t quirk_table_len = sizeof(HIDPP20_DEVICE_QUIRK_TABLE)
                                   / sizeof(HIDPP20_DEVICE_QUIRK_TABLE[0]);

            for(size_t q = 0; q < quirk_table_len; q++)
            {
                const HIDPP20DeviceQuirkEntry& entry = HIDPP20_DEVICE_QUIRK_TABLE[q];

                if((entry.pid_wireless != 0 && entry.pid_wireless == caps.pid_wireless) ||
                   (entry.pid_wired    != 0 && entry.pid_wired    == caps.pid_wired))
                {
                    caps.quirks |= entry.quirks;
                }
            }

            if(caps.quirks != 0)
            {
                LOG_DEBUG("%s Device quirks: 0x%08X", LOG_TAG, caps.quirks);
            }
        }
    }

    /*----------------------------------------------------------*\
    | fn1 GetFwInfo — iterate entities to find main FW (type 0)  |
    | fwType lower nibble: 0=main, 1=bootloader, 2=HW rev        |
    | Response: fwType(1), prefix(3), bcdVersion(2), bcdBuild(2) |
    \*----------------------------------------------------------*/
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

    /*---------------------------------------------------------*\
    | fn2 GetDeviceSerialNumber — ASCII serial up to 16 bytes   |
    \*---------------------------------------------------------*/
    {
        uint8_t recv_data[16] = {};

        int result = SendAcked(fw_idx, 0x20,
                               nullptr, 0, recv_data, sizeof(recv_data));

        if(result > 0)
        {
            char serial[17] = {};
            memcpy(serial, recv_data, 16);
            serial[16] = '\0';

            /*-------------------------------------------------*\
            | Trim trailing nulls/spaces                        |
            \*-------------------------------------------------*/
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
    /*---------------------------------------------------------*\
    | Try 0x8071 first, then 0x0600 (Centurion), then 0x8070    |
    \*---------------------------------------------------------*/
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

    /*------------------------------------------------------------*\
    | Resolve function IDs based on which feature was found.       |
    | 0x8071 and 0x0600 share the same function layout.            |
    | 0x8070 has different function numbers and SW control format. |
    \*------------------------------------------------------------*/
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

    /*---------------------------------------------------------*\
    | GetInfo: discover cluster count                           |
    | 0x8071: data = [0xFF, 0xFF, 0x00]                         |
    | 0x8070: data = [] (empty)                                 |
    \*---------------------------------------------------------*/
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

    /*---------------------------------------------------------*\
    | GetRgbClusterInfo for each cluster                        |
    \*---------------------------------------------------------*/
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

        /*------------------------------------------------------*\
        | GetEffectInfo for each effect in this cluster          |
        \*------------------------------------------------------*/
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

            effect.effect_id     = (response.data[2] << 8) | response.data[3];
            effect.capabilities  = (response.data[4] << 8) | response.data[5];
            effect.default_period = (response.data[6] << 8) | response.data[7];

            LOG_INFO("%s   Effect %d: id=0x%04X caps=0x%04X default_period=%dms",
                     LOG_TAG, j, effect.effect_id, effect.capabilities, effect.default_period);

            cluster.effects.push_back(effect);
        }

        caps.zone_clusters.push_back(cluster);
    }

    caps.has_zone_effects = !caps.zone_clusters.empty();

    /*---------------------------------------------------------*\
    | Probe for device-firmware effect cards. Only defined on   |
    | the 0x8071 RGBEffects path — 0x8070 and 0x0600 don't      |
    | expose GetEffectSpecificInfo in the same form.            |
    \*---------------------------------------------------------*/
    DiscoverEffectCards();
}

void LogitechHIDPP20Controller::DiscoverEffectCards()
{
    /*---------------------------------------------------------*\
    | Probes the device for the presence of firmware-resident   |
    | effect cards via 0x8071 fn0 GetEffectSpecificInfo. On     |
    | devices that have them (observed on G502 X PLUS), every   |
    | valid card returns a device-wide template byte pair at a  |
    | fixed position in page 1 of the response — the vendor app |
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

    /*----------------------------------------------------------*\
    | Query card at effectIdLo=0, page 1. Any valid card works — |
    | the template bytes are device-wide and identical across    |
    | every card on the device — so using card 0 is simplest.    |
    \*----------------------------------------------------------*/
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
| PRO X 2). Separate feature from 0x8071/0x0600/0x8070 —    |
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

    /*---------------------------------------------------------*\
    | fn1 GetRGBZoneInfo — empty request, returns a zone list.  |
    | Exact packing is not fully pinned down by the protocol    |
    | doc; log the raw response so a tester's log is enough to  |
    | refine the decoder.                                       |
    \*---------------------------------------------------------*/
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

        /*------------------------------------------------------*\
        | First-pass decode: byte 0 = zone count, bytes 1..N =   |
        | zone IDs. Bounds-check against the 16-byte data        |
        | window. Refine once we see real G522 output.           |
        \*------------------------------------------------------*/
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

    if(caps.headset_rgb_hostmode_zone_ids.empty())
    {
        LOG_INFO("%s 0x0620 zone decode produced 0 zones — falling back to "
                 "{0x00, 0x01} (two-earcup layout)", LOG_TAG);
        caps.headset_rgb_hostmode_zone_ids.push_back(0x00);
        caps.headset_rgb_hostmode_zone_ids.push_back(0x01);
    }

    /*---------------------------------------------------------*\
    | Synthesize a single zone cluster so the existing          |
    | RGBController zone UI lights up with no special-casing.   |
    | The 0x0620 path is static-color-only; no effect cards, no |
    | per-key. One cluster, one LED per discovered zone.        |
    \*---------------------------------------------------------*/
    HIDPP20ZoneCluster cluster;
    cluster.index        = 0;
    cluster.location     = 0;
    cluster.effect_count = 0;
    caps.zone_clusters.clear();
    caps.zone_clusters.push_back(cluster);

    caps.is_headset_rgb_hostmode  = true;
    caps.has_zone_effects         = true;
    caps.rgb_feature_page         = HIDPP20_FEAT_HEADSET_RGB_HOSTMODE;

    /*---------------------------------------------------------*\
    | Pin device type to HEADSET. 0x0620 presence is a headset  |
    | signal and Centurion sub-devices otherwise show type=0.   |
    | DiscoverDeviceType ran earlier in the probe sequence, so  |
    | pin it here where we have the evidence.                   |
    \*---------------------------------------------------------*/
    caps.device_type = LOGITECH_DEVICE_TYPE_HEADSET;

    LOG_INFO("%s 0x0620 ready: %zu zone(s), transient (FrameEnd 0x01) mode",
             LOG_TAG, caps.headset_rgb_hostmode_zone_ids.size());
}

void LogitechHIDPP20Controller::DiscoverPerKeyZones()
{
    /*---------------------------------------------------------*\
    | Try 0x8081 first, fall back to 0x8080                     |
    \*---------------------------------------------------------*/
    caps.idx_perkey_v2 = GetFeatureIndex(HIDPP20_FEAT_PER_KEY_LIGHTING_V2);

    if(caps.idx_perkey_v2 == 0)
    {
        caps.idx_perkey_v1 = GetFeatureIndex(HIDPP20_FEAT_PER_KEY_LIGHTING_V1);
    }

    uint8_t perkey_idx = (caps.idx_perkey_v2 != 0) ? caps.idx_perkey_v2 : caps.idx_perkey_v1;

    if(perkey_idx == 0)
    {
        caps.has_perkey = false;
        return;
    }

    /*----------------------------------------------------------*\
    | Paginated GetInfo enumeration.                             |
    |                                                            |
    | typeOfInfo is a page index, not a redundant probe. Per     |
    | the 0x8081 spec the device's zone space is up to 336 IDs   |
    | organized as three pages of 112 bits each:                 |
    |                                                            |
    |     zone_id = (page * 112) + (byte * 8) + bit              |
    |                                                            |
    | An earlier version of this code only queried page 0 on     |
    | the assumption that all pages echoed the same data. That   |
    | was wrong — G515 TKL happened to concentrate its zones     |
    | in page 0 so the bug was invisible, but devices with       |
    | G-keys, lightbars, media, or logo LEDs report those        |
    | zones in pages 1 and 2 and were being silently dropped.    |
    \*----------------------------------------------------------*/
    caps.perkey_zone_ids.clear();

    size_t page_counts[3] = { 0, 0, 0 };

    for(uint8_t page = 0; page < 3; page++)
    {
        /*------------------------------------------------------*\
        | Request body: uint16 BE typeOfInfo + 1 pad byte.       |
        | Short report carries the 3 bytes at buf[4..6], so      |
        | { 0x00, page, 0x00 } places page in the low byte of    |
        | the BE field.                                          |
        \*------------------------------------------------------*/
        uint8_t query[3] = { 0x00, page, 0x00 };
        blankFAPmessage response;
        int result = SendAckedIntoFAP(perkey_idx, FN_8081_GET_INFO,
                                      query, 3, response);

        if(result <= 0)
        {
            continue;
        }

        /*------------------------------------------------------*\
        | Parse 14-byte bitmap (bytes 2..15 of the response).    |
        | LSB-first bit order within each byte. Skip zone 0 on   |
        | page 0 (matches prior behavior; zone 0 is not used).   |
        \*------------------------------------------------------*/
        const uint8_t* bitmap = response.data + 2;
        int start_bit = (page == 0) ? 1 : 0;

        for(int bit_in_page = start_bit; bit_in_page < 112; bit_in_page++)
        {
            int byte_idx = bit_in_page / 8;
            int bit_idx  = bit_in_page % 8;

            if(bitmap[byte_idx] & (1 << bit_idx))
            {
                uint16_t zone_id = (uint16_t)(page * 112 + bit_in_page);

                /*----------------------------------------------*\
                | Wire protocol 0x8081 Set* functions take a     |
                | uint8_t zone ID. Zones >255 from the bitmap    |
                | formula can't actually be addressed — drop     |
                | them so we don't expose phantom LEDs.          |
                \*----------------------------------------------*/
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

    /*----------------------------------------------------------*\
    | Detect numpad presence from zone bitmask.                  |
    | Numpad zones are 80-96 in Solaar's KEYCODES numbering.     |
    \*----------------------------------------------------------*/
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

    /*-----------------------------------------------------------*\
    | Detect transport type from usage page before anything else. |
    | Centurion devices need different framing for all commands.  |
    \*-----------------------------------------------------------*/
    DiscoverTransport();

    /*----------------------------------------------------------*\
    | Flush any queued HID reports before probing.               |
    | The device may have unsolicited notifications (battery,    |
    | button events, etc.) sitting in the read buffer.           |
    \*----------------------------------------------------------*/
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

    /*----------------------------------------------------------*\
    | Test IRoot by looking up a known feature.                  |
    |                                                            |
    | Standard HID++: look up FeatureSet (0x0001) — must exist.  |
    | Centurion dongle: look up CentPPBridge (0x0003) — the      |
    |   dongle doesn't have FeatureSet, but must have the        |
    |   bridge to reach the sub-device.                          |
    |                                                            |
    | Retry up to 3 times — wireless devices behind a shared     |
    | receiver can return stale responses.                       |
    \*----------------------------------------------------------*/
    uint8_t test_idx = 0;

    if(transport.type == HIDPP20_TRANSPORT_CENTURION)
    {
        /*------------------------------------------------------*\
        | Centurion: try CentPPBridge (0x0003) first for dongle. |
        | If not found, try FeatureSet (0x0001) for wired/direct |
        | connection where the device IS the endpoint.           |
        |                                                        |
        | This is the "is anyone there?" check — use the fast-   |
        | fail probe policy so non-Centurion or unreachable      |
        | devices bail in ~500ms instead of ~6s. Once we have a  |
        | positive response, subsequent discovery uses reliable. |
        \*------------------------------------------------------*/
        test_idx = GetFeatureIndex(HIDPP20_FEAT_CENTPPBRIDGE, HIDPP20_POLICY_PROBE);

        if(test_idx != 0)
        {
            transport.bridge_feat_idx = test_idx;
            transport.sub_device_id   = 0;

            LOG_DEBUG("%s CentPPBridge at index %d — routing to sub-device",
                      LOG_TAG, test_idx);

            /*---------------------------------------------------*\
            | Pre-check sub-device availability via               |
            | getConnectionInfo (CentPPBridge fn0). The vendor    |
            | app does this and refuses to call sendFragment      |
            | when MTU=0.                                         |
            |                                                     |
            | Response format (from protocol doc line 910-917):   |
            |   Byte 0: high nibble = connection type/state       |
            |           low nibble + Byte 1 = sub-device data     |
            |           length / MTU                              |
            |   Bytes 2+: sub-device descriptors                  |
            |                                                     |
            | If MTU == 0, no sub-device is connected. Calling    |
            | sendFragment in that state triggers an undocumented |
            | error code 0x0B and wastes the full retry budget.   |
            | Skip enumeration and let the dongle-watcher path    |
            | take over until ConnectionStateChangedEvent fires.  |
            |                                                     |
            | Bridge is confirmed responsive at this point —      |
            | use reliable policy for the MTU check.              |
            \*---------------------------------------------------*/
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
                    /*----------------------------------------*\
                    | No sub-device — skip enumeration. Mark   |
                    | the feature map complete so on-demand    |
                    | lookups don't hit the wire. The dongle   |
                    | will be registered as a watcher and the  |
                    | sub-device will be probed when           |
                    | ConnectionStateChangedEvent fires.       |
                    \*----------------------------------------*/
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
            LOG_DEBUG("%s No CentPPBridge — Centurion direct connection", LOG_TAG);
            test_idx = GetFeatureIndex(HIDPP20_FEAT_FEATURE_SET, HIDPP20_POLICY_PROBE);
        }
    }
    else
    {
        /*-------------------------------------------------------*\
        | Standard HID++: probe FeatureSet (0x0001) — fast-fail.  |
        | The probe policy already includes its own retry; the    |
        | outer loop is preserved for buffer-flushing behavior    |
        | between attempts.                                       |
        \*-------------------------------------------------------*/
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
        LOG_DEBUG("%s IRoot probe failed at %s — device does not respond",
                  LOG_TAG, location.c_str());
        return false;
    }

    /*----------------------------------------------------------*\
    | If retries were needed, flush delayed responses from       |
    | failed attempts before continuing with discovery.          |
    \*----------------------------------------------------------*/
    {
        uint8_t post_buf[64];

        while(hid_read_timeout(dev, post_buf, sizeof(post_buf), 10) > 0)
        {
        }
    }

    /*-----------------------------------------------------------*\
    | Enumerate all features in bulk. For standard HID++, uses    |
    | FeatureSet GetCount + GetFeatureId loop. For Centurion      |
    | sub-devices, uses bulk GetFeatureId (single response).      |
    | After this, GetFeatureIndex uses the map — no wire traffic. |
    \*-----------------------------------------------------------*/
    {
        /*------------------------------------------------------*\
        | For Centurion bridged, FeatureSet is at index 1 on the |
        | sub-device. For standard HID++, test_idx is the        |
        | FeatureSet index from the IRoot probe.                 |
        \*------------------------------------------------------*/
        uint8_t fs_idx = (transport.type == HIDPP20_TRANSPORT_CENTURION &&
                          transport.bridge_feat_idx != 0)
                       ? 1    // CenturionFeatureSet always at index 1 on sub-device
                       : test_idx;

        EnumerateFeatures(fs_idx);

        /*------------------------------------------------------*\
        | If bridged and bulk enumeration failed, the sub-device |
        | isn't reachable (e.g., headset off or on USB cable).   |
        | Mark map as complete so lookups don't hit the wire.    |
        | The device will be discovered with no features — it    |
        | can be re-probed when the sub-device comes online.     |
        \*------------------------------------------------------*/
        if(transport.bridge_feat_idx != 0 && !caps.feature_map_complete)
        {
            LOG_DEBUG("%s Sub-device not reachable through bridge — dongle only",
                      LOG_TAG);
            caps.feature_map_complete = true;
        }
    }

    /*----------------------------------------------------------*\
    | Discover device identity.                                  |
    | On Centurion with bridge, this now queries the sub-device  |
    | (headset) through the bridge, not the dongle.              |
    \*----------------------------------------------------------*/
    DiscoverDeviceName();
    log_tag = "[LogitechHID++ " + caps.device_name + "]";
    DiscoverDeviceType();
    DiscoverFirmwareInfo();

    /*---------------------------------------------------------*\
    | Discover profile management features                      |
    \*---------------------------------------------------------*/
    caps.idx_profile_management     = GetFeatureIndex(HIDPP20_FEAT_PROFILE_MANAGEMENT);
    caps.idx_onboard_profiles       = GetFeatureIndex(HIDPP20_FEAT_ONBOARD_PROFILES);
    caps.idx_disable_keys_by_usage  = GetFeatureIndex(HIDPP20_FEAT_DISABLE_KEYS_BY_USAGE);

    /*---------------------------------------------------------*\
    | Discover RGB capabilities                                 |
    \*---------------------------------------------------------*/
    DiscoverRGBEffects();
    if(caps.idx_rgb_effects == 0)
    {
        DiscoverHeadsetRGBHostmode();
    }
    DiscoverPerKeyZones();
    DiscoverKeyboardLayout();

    /*---------------------------------------------------------*\
    | Probe WirelessStatus (0x1D4B) for reconnect detection.    |
    | Lightspeed devices behind kernel-managed receivers send   |
    | WirelessStatus events when they reconnect after power     |
    | cycle. Cache the feature index so the reader thread can   |
    | detect these events without sending commands.             |
    \*---------------------------------------------------------*/
    caps.idx_wireless_status = GetFeatureIndex(HIDPP20_FEAT_WIRELESS_STATUS);

    if(!caps.has_zone_effects && !caps.has_perkey)
    {
        LOG_DEBUG("%s %s: no RGB features found", LOG_TAG, caps.device_name.c_str());

        /*------------------------------------------------------*\
        | Centurion dongles with bridge stay alive to watch for  |
        | sub-device connection events, even without RGB.        |
        \*------------------------------------------------------*/
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

void LogitechHIDPP20Controller::Initialize()
{
    /*-----------------------------------------------------------*\
    | No device state changes here — let firmware effects keep    |
    | running until DeviceUpdateMode claims control with real     |
    | colors ready via ClaimSWControlIfNeeded().                  |
    \*-----------------------------------------------------------*/
    init_generation++;
    initialized = true;
}

void LogitechHIDPP20Controller::Shutdown()
{
    if(!initialized)
    {
        return;
    }

    StopPowerManager();

    /*---------------------------------------------------------*\
    | Release SW control                                        |
    \*---------------------------------------------------------*/
    if(caps.idx_rgb_effects != 0)
    {
        SetSWControl(0, 0);
    }

    /*---------------------------------------------------------*\
    | Restore firmware mode                                     |
    \*---------------------------------------------------------*/
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

    /*---------------------------------------------------------*\
    | Release 0x0620 Headset RGB hostmode claim. Best-effort;   |
    | mirrors the SetHostMode() additive branch.                |
    \*---------------------------------------------------------*/
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
        /*------------------------------------------------------*\
        | 0x8070: simple [enabled(bool), persist(bool)]          |
        \*------------------------------------------------------*/
        uint8_t data[2] = { (uint8_t)(mode > 0 ? 0x01 : 0x00), 0x00 };
        result = SendAckedIntoFAP(caps.idx_rgb_effects, caps.fn_sw_control,
                                  data, 2, response);
    }
    else
    {
        /*------------------------------------------------------*\
        | 0x8071/0x0600: [0x01(set), mode, flags]                |
        \*------------------------------------------------------*/
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
        /*-------------------------------------------------------*\
        | Observed vendor-app wire pattern on G502 X PLUS:        |
        | unconditional SetOnboardMode(host) immediately          |
        | followed by a GetOnboardMode verify read. The vendor    |
        | app never reads first — it writes fn1 with 0x02 then    |
        | re-queries fn2, ignoring the response value (no retry   |
        | logic, no branching on it). The verify appears to be a  |
        | state-settle / sync point rather than a check, but      |
        | since we don't know its firmware-side effect, we mirror |
        | it.                                                     |
        |                                                         |
        | An earlier revision of this function added a pre-Set    |
        | GetOnboardMode guard to skip the write when already     |
        | in host mode. That deviated from the observed wire      |
        | behavior, so it has been removed.                       |
        \*-------------------------------------------------------*/
        uint8_t         set_data[1] = { 0x02 };
        blankFAPmessage set_response;
        SendAckedIntoFAP(caps.idx_onboard_profiles, FN_8100_SET_ONBOARD_MODE,
                         set_data, 1, set_response);

        blankFAPmessage verify_response;
        SendAckedIntoFAP(caps.idx_onboard_profiles, FN_8100_GET_ONBOARD_MODE,
                         nullptr, 0, verify_response);

        LOG_DEBUG("%s OnboardProfiles set to host mode, verify=0x%02X",
                  LOG_TAG, verify_response.data[0]);
    }

    /*-------------------------------------------------------*\
    | 0x0620 Headset RGB hostmode claim. Additive — a headset |
    | exposing 0x0620 typically won't also have 0x8100/0x8101 |
    | but we don't assume mutual exclusion. Sticky claim, not |
    | re-issued per write; wake path re-enters SetHostMode    |
    | after reconnect which reinstates it for free.           |
    \*-------------------------------------------------------*/
    if(caps.idx_headset_rgb_hostmode != 0)
    {
        uint8_t         on           = 0x01;
        blankFAPmessage claim_response;
        SendAckedIntoFAP(caps.idx_headset_rgb_hostmode,
                         FN_0620_SET_HOST_MODE_STATE,
                         &on, 1, claim_response);

        LOG_DEBUG("%s 0x0620 SetHostModeState(1) sent", LOG_TAG);
    }
}

bool LogitechHIDPP20Controller::ClaimSWControlIfNeeded()
{
    if(sw_control_claimed)
    {
        return true;
    }

    if(caps.idx_rgb_effects == 0 || !device_online.load())
    {
        return false;
    }

    /*----------------------------------------------------------*\
    | Two-phase claim to avoid the onboard→host transition       |
    | flash (warm-white ~3000K, ~50ms) visible on G502 X PLUS.   |
    |                                                            |
    | The SW Control `flags` bits are not "Zone/Power/Effect"    |
    | as the overview labels them — derived from a G502 wire     |
    | capture, bit 0 = effect control, bit 1 = power management, |
    | bit 2 = NV config. Setting the effect bit suspends the     |
    | firmware's autonomous effect engine, and anything the host |
    | hasn't explicitly painted since that moment shows as the   |
    | firmware's default LED buffer — on the G502 X PLUS that    |
    | default is warm-white.                                     |
    |                                                            |
    | The observed vendor-app behavior paints the G502 with      |
    | flags=6 (power+NV, NOT effect) the entire time, so the     |
    | firmware effect engine keeps rendering the onboard         |
    | profile's output right up to the moment SetEffectByIndex   |
    | replaces it — no visible gap.                              |
    |                                                            |
    | We can't just stay on flags=6 forever: the idle/wake       |
    | state machine in OnUserActivity uses flags=5/3 as its      |
    | active/idle signals and needs those specific values for    |
    | the firmware to generate the right onUserActivity events.  |
    | So we claim at flags=6, let the first per-key frame paint  |
    | through the transition, then upgrade to flags=5 only       |
    | after the per-key layer is active — at that point per-key  |
    | masks any zone output anyway, so the 6→5 transition is     |
    | invisible.                                                 |
    |                                                            |
    | The previous sequence wrote `(3,7)` then `(3,5)` to mimic  |
    | the vendor app's *keyboard first-contact* behavior. That   |
    | was right for the initial G515 bring-up but wrong for the  |
    | mouse — on the G502 X PLUS the effect bit at claim time    |
    | is the root cause of the startup flash.                    |
    |                                                            |
    | The vendor-app claim sequence does two WritePowerConfig    |
    | calls around SetHostMode, writing (a) its profile's sleep  |
    | value then (b) that value minus the firmware off-ramp.     |
    | We don't write timers at all: we don't have a profile we   |
    | want to impose on the device, and our host-side StartSleep |
    | trigger already fires SetRgbPowerMode(3) explicitly at the |
    | moment we want the fade to begin.                          |
    \*----------------------------------------------------------*/
    int claim_result = SetSWControl(3, 6);

    if(claim_result <= 0)
    {
        LOG_DEBUG("%s SW control claim failed (SetSWControl(3,6) result=%d)",
                  LOG_TAG, claim_result);
        return false;
    }

    /*---------------------------------------------------------*\
    | Keyboard-family handshake on feature 0x4522               |
    | (DisableKeysByUsage). G815 / G915 / G Pro send this fn3 + |
    | fn1 empty-payload pair between SetSWControl and the first |
    | mode write. Feature-gated inside — no-op on G502 / G515.  |
    \*---------------------------------------------------------*/
    DoDisableKeysByUsageHandshake();

    SetRGBPowerMode(1);
    WritePowerConfig(idle_timeout_s, sleep_timeout_s);
    SetHostMode();
    WritePowerConfig(idle_timeout_s, sleep_timeout_s);

    written_idle_s  = idle_timeout_s;
    written_sleep_s = sleep_timeout_s;

    sw_control_claimed            = true;
    sw_control_needs_upgrade_to_5 = true;

    LOG_DEBUG("%s Claimed SW control at flags=6 "
              "(effect engine still autonomous until first per-key frame)",
              LOG_TAG);
    return true;
}

void LogitechHIDPP20Controller::UpgradeSwControlAfterFirstPaint()
{
    /*---------------------------------------------------------*\
    | Called by RGBController_LogitechHIDPP20::DeviceUpdateLEDs |
    | immediately after the first successful PerKeyFrameEnd of  |
    | a newly-claimed session. At this point the per-key buffer |
    | is populated with real host colors, so the per-key layer  |
    | masks the zone layer and the 6→5 transition no longer     |
    | exposes the firmware's default LED buffer. Upgrading to   |
    | flags=5 puts the device into the "active steady state"    |
    | that OnUserActivity expects for idle detection events.    |
    \*---------------------------------------------------------*/
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
    /*----------------------------------------------------------*\
    | G815 / G915 / G Pro keyboards send this two-call           |
    | handshake on feature 0x4522 (DisableKeysByUsage) before    |
    | any mode change or per-key write. The original OpenRGB     |
    | G815 + G915 controllers both do it in their BeginModeSet   |
    | and InitializeDirect paths. Both payloads are empty —      |
    | bare function calls — suggesting they're state reads       |
    | used as a firmware sync point, not actual disable-keys     |
    | writes (those would require a keyset in the payload).      |
    |                                                            |
    | Feature-gated: caps.idx_disable_keys_by_usage is only      |
    | non-zero on devices that enumerate 0x4522. G502 and G515   |
    | do not enumerate it, so this is a no-op on those.          |
    \*----------------------------------------------------------*/
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
|   Frame 2297 (17.348s, ~262ms after SetOnboardMode(02)):  |
|     1101091a ff 02 00 00 00 00 00 00 20 64 00 00 01 …     |
|     RgbEffects.SetEffectByIndex                           |
|       cluster=0xFF (all clusters)                         |
|       effectIdx=0x02 (Breathing on G502's enumerated set) |
|       params=[00 00 00 00 00 00 20 64 00 00] (10 bytes)   |
|         — positions [6]=0x20, [7]=0x64 are non-zero. The  |
|         Breathing effect parameter layout documented in   |
|         the protocol reference has period/brightness in   |
|         those slots, but the exact meaning of these two   |
|         values in this context is NOT understood. The     |
|         vendor app sends them verbatim on every claim; we |
|         mirror.                                           |
|       persist=0x01                                        |
|                                                           |
|   Frame 2321 (17.443s, ~95ms after frame 2297):           |
|     1101091a ff 04 00 00 00 00 00 00 00 00 00 00 01 …     |
|     RgbEffects.SetEffectByIndex                           |
|       cluster=0xFF                                        |
|       effectIdx=0x04 — OUT OF RANGE on G502 X PLUS (the   |
|         device only enumerates effects 0..3 via           |
|         GetEffectInfo). Likely a "custom / direct mode"   |
|         slot the firmware accepts but does not advertise  |
|         through the normal enumeration.                   |
|       params=[00 × 10]                                    |
|       persist=0x01                                        |
|                                                           |
| The function we call is caps.fn_set_effect (0x10 on       |
| 0x8071, 0x30 on 0x8070, same as SetZoneEffect uses).      |
|                                                           |
| We do NOT attempt to derive these values from the effect  |
| param layout tables because we don't understand what      |
| they mean. They're observed-working bytes from the wire   |
| capture and that's the contract. If this prep sequence    |
| later turns out to work on other devices, the gating in   |
| DeviceUpdateLEDs can be loosened.                         |
\*---------------------------------------------------------*/

void LogitechHIDPP20Controller::DoObservedPerKeyPrep()
{
    if(caps.idx_rgb_effects == 0 || !device_online.load())
    {
        return;
    }

    /*----------------------------------------------------------*\
    | Prep1: SetEffectByIndex(cluster=0xFF, effectIdx=2, params) |
    | with the device-wide template bytes at params[6..7].       |
    |                                                            |
    | The template bytes are discovered at feature-discovery     |
    | time via GetEffectSpecificInfo on any firmware effect      |
    | card; the vendor app does the same read-then-echo          |
    | pattern, and on a G502 X PLUS the read value is 0x20 0x64  |
    | across every card. We don't know what those bytes mean     |
    | semantically — just that the device expects to see them    |
    | echoed back verbatim in this position when priming the     |
    | firmware effect engine for per-key takeover.               |
    \*----------------------------------------------------------*/
    uint8_t prep1[16] =
    {
        0xFF, 0x02,                                 /* cluster, effectIdx */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        /* params[0..5] */
        caps.effect_card_template[0],              /* params[6] — device */
        caps.effect_card_template[1],              /* params[7] — device */
        0x00, 0x00,                                 /* params[8..9] */
        0x01,                                       /* persist */
        0x00, 0x00, 0x00                           /* padding */
    };
    blankFAPmessage prep1_resp;
    SendAckedIntoFAP(caps.idx_rgb_effects, caps.fn_set_effect,
                     prep1, 16, prep1_resp);

    /*---------------------------------------------------------*\
    | Prep2: SetEffectByIndex with effectIdx set to the first   |
    | out-of-range slot above the last enumerated effect, all   |
    | params zero.                                              |
    |                                                           |
    | On the G502 X PLUS (4 enumerated effects: 0..3) this      |
    | means effectIdx=4 — matches the value in pcap frame 2321. |
    | On other devices, effectIdx is parameterized by effect    |
    | count so the same "first OOR slot" semantic holds.        |
    |                                                           |
    | The RE thread's working theory is that this is a firmware |
    | "custom/direct mode" slot the effect engine accepts but   |
    | doesn't advertise through GetEffectInfo. Without that     |
    | slot being written, the per-key pipeline doesn't enter    |
    | cleanly and the firmware exposes its default LED state    |
    | during the claim→paint window (the 3000K warm-white flash |
    | we previously observed on cold starts).                   |
    \*---------------------------------------------------------*/
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
    /*---------------------------------------------------------*\
    | G815 / G915 / G Pro per-key takeover prep, cloned from    |
    | the InitializeDirect sequence in their legacy OpenRGB     |
    | controllers. Three steps after the claim-time 0x4522      |
    | handshake (which fires from ClaimSWControlIfNeeded):      |
    |                                                           |
    |   1. For each enumerated cluster, SetEffectByIndex with   |
    |      effectIdx=0 (Off) and persist=1. This deactivates    |
    |      the firmware effect engine per-cluster — different   |
    |      from the G515 static-black fallback, which leaves    |
    |      the effect engine running with a black static color. |
    |                                                           |
    |   2. Send a primer SetIndividualRgbZones write covering   |
    |      one zone (the first enumerated) at black. G915 uses  |
    |      Escape specifically; we use the first enumerated     |
    |      zone for portability.                                |
    |                                                           |
    |   3. FrameEnd, so the primer write commits and the        |
    |      per-key layer becomes the visible output.            |
    |                                                           |
    | Gate (caller's responsibility): feature 0x4522 present    |
    | AND per-key V2 feature present. G502 / G515 fail the      |
    | 0x4522 side; older keyboards without 0x8081 fail the      |
    | per-key side.                                             |
    \*---------------------------------------------------------*/
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
| arrives — which matches our two problem cases:            |
|   1. First frame after a reconnect-transient claim        |
|      (Direct mode, no animation timer).                   |
|   2. Last frame of an animation that then stops.          |
\*---------------------------------------------------------*/

void LogitechHIDPP20Controller::ScheduleRetryPaint()
{
    size_t max_attempts =
        sizeof(HIDPP20_REPAINT_RETRY_BACKOFF_MS) / sizeof(uint16_t);

    uint8_t attempt = retry_paint_attempt_.load();

    if(attempt >= max_attempts)
    {
        /*-----------------------------------------------------*\
        | Retry budget exhausted. Give up for this sequence —   |
        | the next fresh failure (after a full_commit clears    |
        | the attempt counter) will start from attempt 0.       |
        \*-----------------------------------------------------*/
        retry_paint_deadline_.store(std::chrono::steady_clock::time_point{});
        LOG_DEBUG("%s retry paint budget exhausted (%zu attempts)",
                  LOG_TAG, max_attempts);
        return;
    }

    uint16_t delay_ms = HIDPP20_REPAINT_RETRY_BACKOFF_MS[attempt];
    std::chrono::steady_clock::time_point deadline = std::chrono::steady_clock::now()
                                                    + std::chrono::milliseconds(delay_ms);

    retry_paint_deadline_.store(deadline);

    LOG_DEBUG("%s retry paint scheduled attempt=%u delay=%ums",
              LOG_TAG, attempt, delay_ms);
}

void LogitechHIDPP20Controller::CancelRetryPaint()
{
    retry_paint_deadline_.store(std::chrono::steady_clock::time_point{});
    retry_paint_attempt_.store(0);
}

void LogitechHIDPP20Controller::TickRetryPaintIfPending()
{
    /*---------------------------------------------------------*\
    | Called from the power thread's main loop each tick.       |
    | Checks the retry deadline and fires the repaint callback  |
    | when it expires. The callback runs DeviceUpdateLEDs on    |
    | the power thread's context — not recursively from inside  |
    | another DeviceUpdateLEDs call.                            |
    \*---------------------------------------------------------*/
    std::chrono::steady_clock::time_point deadline = retry_paint_deadline_.load();

    if(deadline == std::chrono::steady_clock::time_point{})
    {
        return;
    }

    if(std::chrono::steady_clock::now() < deadline)
    {
        return;
    }

    /*---------------------------------------------------------*\
    | Clear the deadline before firing so a concurrent          |
    | ScheduleRetryPaint (from a different thread) doesn't      |
    | double-fire on the same tick. Advance the attempt counter |
    | so the next ScheduleRetryPaint (if this retry also fails) |
    | picks the next backoff slot.                              |
    \*---------------------------------------------------------*/
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

    /*---------------------------------------------------------*\
    | Batch into SetIndividualRgbZones (fn1): 4 entries/packet  |
    | Each entry = [zone_id, R, G, B]. Track the zones in each  |
    | batch so PerKeyFrameEnd can report which committed.       |
    \*---------------------------------------------------------*/
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

    /*----------------------------------------------------------*\
    | Use SetRangeRgbZones (fn5): [start, end, R, G, B] × 3      |
    | per packet. Sets all zones in a contiguous range to one    |
    | color. Gaps in zone IDs are silently ignored by firmware.  |
    | For uniform color this is far more efficient than fn6:     |
    | 1-2 packets vs 8 packets for 94 zones.                     |
    \*----------------------------------------------------------*/
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
    /*-----------------------------------------------------------*\
    | Truly fire-and-forget. Push the packet onto the wire,       |
    | record the zones it covers in outstanding_writes, and       |
    | return. PerKeyFrameEnd will drain the response queue at     |
    | end-of-frame and FIFO-match each ACK back to the            |
    | corresponding outstanding entry.                            |
    |                                                             |
    | The retry/backoff machinery is intentionally NOT used       |
    | here — when a streaming frame fails, we don't want to       |
    | delay the next frame retrying old data. The carry-over      |
    | of uncommitted zones via sent_colors[i]=HIDPP20_UNCOMMITTED |
    | naturally ensures missed keys land in the next frame.       |
    \*-----------------------------------------------------------*/
    int send_result = SendMessage(perkey_idx, function, data, len);

    if(send_result < 0)
    {
        LOG_DEBUG("%s SendPerKeyData wire send failed (result=%d) func=0x%02X",
                  LOG_TAG, send_result, function);
        /* Still record the outstanding entry — its zones will  */
        /* be reported as unacked, which is correct.             */
    }

    OutstandingPerKeyWrite entry;
    entry.function = function;
    entry.zone_ids = zone_ids;
    outstanding_writes.push_back(std::move(entry));
}

PerKeyFrameResult LogitechHIDPP20Controller::PerKeyFrameEnd()
{
    PerKeyFrameResult result;
    result.frame_end_acked = false;

    /*---------------------------------------------------------*\
    | Build attempted_zones from the outstanding writes list    |
    | up front so the caller can use it for both the success    |
    | and failure paths.                                        |
    \*---------------------------------------------------------*/
    for(size_t w = 0; w < outstanding_writes.size(); w++)
    {
        const std::vector<uint8_t>& zone_ids = outstanding_writes[w].zone_ids;

        for(size_t z = 0; z < zone_ids.size(); z++)
        {
            result.attempted_zones.push_back(zone_ids[z]);
        }
    }

    if(!device_online.load())
    {
        outstanding_writes.clear();
        return result;
    }

    uint8_t perkey_idx = (caps.idx_perkey_v2 != 0) ? caps.idx_perkey_v2 : caps.idx_perkey_v1;

    if(perkey_idx == 0)
    {
        outstanding_writes.clear();
        return result;
    }

    /*---------------------------------------------------------*\
    | Send FrameEnd directly. No retry, no backoff: a streaming |
    | frame failure means the next frame's delta will pick up   |
    | the missed keys, and we don't want to delay that frame.   |
    |                                                           |
    | Format (matches observed wire capture): LONG message      |
    | (0x11), 16 bytes of zeros. Firmware expects long-format   |
    | FrameEnd                                                  |
    | — short-format hits intermittent BUSY.                    |
    \*---------------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(30));

    uint8_t data[16] = {};
    int send_result = SendMessage(perkey_idx, FN_8081_FRAME_END, data, sizeof(data));

    if(send_result < 0)
    {
        LOG_DEBUG("%s FrameEnd wire send failed (result=%d)", LOG_TAG, send_result);
        outstanding_writes.clear();
        return result;
    }

    /*---------------------------------------------------------*\
    | Drain responses in FIFO order until we either see the     |
    | FrameEnd ACK or run out the wait budget. Each per-key     |
    | write response is matched (by feature + function high     |
    | nibble) to the head of outstanding_writes; matched zones  |
    | go into acked_zones. The FrameEnd response itself is the  |
    | terminating event.                                        |
    |                                                           |
    | Wait budget: 300ms. Generous enough to absorb the slow    |
    | batch-ACK behavior we've seen on G515 (~700ms p99) for    |
    | dense per-key frames, but won't actually consume that     |
    | much time on healthy devices — the loop exits the moment  |
    | the FrameEnd response shows up.                           |
    \*---------------------------------------------------------*/
    std::chrono::steady_clock::time_point deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(300);
    size_t outstanding_idx = 0;
    int    busy_retries    = 0;

    while(true)
    {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        if(now >= deadline)
        {
            LOG_DEBUG("%s FrameEnd timed out waiting for ACK (matched %zu/%zu writes)",
                      LOG_TAG, outstanding_idx, outstanding_writes.size());
            break;
        }

        int remaining = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                              deadline - now).count();
        if(remaining <= 0)
        {
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
            LOG_DEBUG("%s FrameEnd read error (result=%d)", LOG_TAG, rd);
            break;
        }

        if(rd == 0)
        {
            /* timeout */
            LOG_DEBUG("%s FrameEnd timed out waiting for ACK (matched %zu/%zu writes)",
                      LOG_TAG, outstanding_idx, outstanding_writes.size());
            break;
        }

        /*-----------------------------------------------------*\
        | HID++ error frame: feat=0xFF, func=err_feat,          |
        | data[0]=err_func, data[1]=err_code.                   |
        |                                                       |
        | The case we care about is BUSY (0x08) for our         |
        | FrameEnd: the firmware is still draining the per-key  |
        | write queue and asks us to re-send. Without this we   |
        | hang on the deadline waiting for an ACK that never    |
        | comes, since BUSY-rejected commands are not queued.   |
        |                                                       |
        | Re-send with a tight budget — 3 retries, 30ms gap.    |
        | If BUSY persists past that, give up for this frame    |
        | and let delta carry-over handle it next frame.        |
        \*-----------------------------------------------------*/
        if(resp_feat == 0xFF)
        {
            uint8_t err_feat = resp_func;
            uint8_t err_func_byte = resp_data[0];
            uint8_t err_code = resp_data[1];

            bool is_our_frame_end =
                (err_feat == perkey_idx) &&
                ((err_func_byte & 0xF0) == FN_8081_FRAME_END);

            if(is_our_frame_end)
            {
                size_t max_busy_retries =
                    sizeof(HIDPP20_FRAME_END_BUSY_BACKOFF_MS) / sizeof(uint16_t);

                if(err_code == 0x08 && (size_t)busy_retries < max_busy_retries)
                {
                    uint16_t delay_ms = HIDPP20_FRAME_END_BUSY_BACKOFF_MS[busy_retries];
                    busy_retries++;
                    LOG_TRACE("%s FrameEnd BUSY, re-sending (retry %d, delay %ums)",
                              LOG_TAG, busy_retries, delay_ms);
                    std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
                    SendMessage(perkey_idx, FN_8081_FRAME_END, data, sizeof(data));
                    continue;
                }

                /* Non-BUSY error or out of retries — frame committed=false */
                LOG_DEBUG("%s FrameEnd error 0x%02X (retries=%d)",
                          LOG_TAG, err_code, busy_retries);
                break;
            }

            /* Error for an unrelated request — discard and keep reading */
            continue;
        }

        /*-----------------------------------------------------*\
        | Discard frames that aren't from our perkey feature.   |
        \*-----------------------------------------------------*/
        if(resp_feat != perkey_idx)
        {
            continue;
        }

        uint8_t resp_func_hi = resp_func & 0xF0;

        /*-----------------------------------------------------*\
        | FrameEnd response — terminator.                       |
        \*-----------------------------------------------------*/
        if(resp_func_hi == FN_8081_FRAME_END)
        {
            result.frame_end_acked = true;
            break;
        }

        /*-----------------------------------------------------*\
        | Per-key write response. Match against the next        |
        | outstanding entry by function high nibble. If the     |
        | head doesn't match (a write was dropped on the wire   |
        | or the firmware is responding out of order), skip     |
        | unmatched heads — those entries' zones will be left   |
        | out of acked_zones and treated as uncommitted.        |
        \*-----------------------------------------------------*/
        while(outstanding_idx < outstanding_writes.size() &&
              outstanding_writes[outstanding_idx].function != resp_func_hi)
        {
            outstanding_idx++;
        }

        if(outstanding_idx >= outstanding_writes.size())
        {
            /* No matching outstanding write — stale or unexpected response */
            continue;
        }

        for(uint8_t z : outstanding_writes[outstanding_idx].zone_ids)
        {
            result.acked_zones.push_back(z);
        }
        outstanding_idx++;
    }

    /*---------------------------------------------------------*\
    | Deep-sleep detection. If FrameEnd failed (no ACK) while   |
    | we're in the SLEEPING state, the device may have finished |
    | its firmware fade and entered deep sleep. Track           |
    | consecutive failures; once we hit the threshold, suppress |
    | further frame sends until Wake() clears the flag.         |
    |                                                           |
    | A successful ACK resets the counter — transient BUSY      |
    | bursts during the fade don't accumulate.                  |
    \*---------------------------------------------------------*/
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

    outstanding_writes.clear();
    return result;
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

    /*---------------------------------------------------------*\
    | SetEffectByIndex (fn1 on 0x8071, fn3 on 0x8070)           |
    | 0x8071/0x0600: [cluster, effect_idx, 10-byte params,      |
    |                 persist at [12]]                          |
    | 0x8070:        [zone,    effect_idx, 10-byte params,      |
    |                 persist at [12] (Bit 2-3 Power,           |
    |                 Bit 1-0 Persistence)]                     |
    \*---------------------------------------------------------*/
    uint8_t data[16];
    memset(data, 0, sizeof(data));

    data[0] = cluster_idx;
    data[1] = effect_idx;

    /*----------------------------------------------------------*\
    | Build 10-byte params (data[2..11]) per effect type         |
    | Layouts from protocol docs and observed wire captures      |
    \*----------------------------------------------------------*/
    switch(effect_id)
    {
        case 0x0001:    // Static
            data[2] = r;
            data[3] = g;
            data[4] = b;
            /*-----------------------------------------------------*\
            | "Fixed color" marker — only set when there's an       |
            | actual color. All-black means "Off / pass-through to  |
            | per-key buffer", which uses byte 5 = 0x00 instead.    |
            \*-----------------------------------------------------*/
            if(r != 0 || g != 0 || b != 0)
            {
                data[5] = 0x02;
            }
            break;

        case 0x000A:    // Breathing
            /*------------------------------------------------------*\
            | Effect param layout (10 bytes, indices into data[]):   |
            |   data[2..4]  = R, G, B                                |
            |   data[5..6]  = periodHi, periodLo (BE16 milliseconds) |
            |   data[7]     = 0                                      |
            |   data[8]     = brightness 0..100                      |
            \*------------------------------------------------------*/
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

        case 0x0004:    // Color Wave
            data[3] = (period > 0) ? (uint8_t)(period / 100) : 50;
            data[8] = 0x01;     // pattern
            data[9] = 0x00;     // waveform
            data[11] = 0x01;    // direction
            break;

        case 0x000B:    // Ripple
            data[2] = r;
            data[3] = g;
            data[4] = b;
            data[6] = (period >> 8) & 0xFF;
            data[7] = period & 0xFF;
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

        default:        // Unknown — best-effort
            data[2] = r;
            data[3] = g;
            data[4] = b;
            data[5] = (period >> 8) & 0xFF;
            data[6] = period & 0xFF;
            break;
    }

    /*------------------------------------------------------*\
    | 16-byte payload for all pages; persist at byte[12].    |
    | 0x8070 and 0x8071/0x0600 share the same byte position  |
    | per LogitechProtocolCommon setMode convention.         |
    \*------------------------------------------------------*/
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
| Feature 0x0620 Headset RGB Hostmode — static color write. |
|                                                           |
| Claim is sticky from SetHostMode(); this function only    |
| writes colors + FrameEnd. Picks fn5 SetRgbZonesSingleValue|
| when all zones share a color, else fn2                    |
| SetIndividualRgbZones. FrameEnd byte 0 is always 0x01     |
| (transient) — 0x02 was tested and does not work on G522   |
| firmware.                                                 |
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

    /*---------------------------------------------------------*\
    | If fewer colors than zones, fill the tail with the last   |
    | provided color. If zero colors, nothing to write.         |
    \*---------------------------------------------------------*/
    if(zone_colors.empty())
    {
        return;
    }

    /*---------------------------------------------------------*\
    | Uniformity check: same color across every zone?           |
    \*---------------------------------------------------------*/
    RGBColor first = zone_colors[0];
    bool all_same  = true;
    for(size_t i = 1; i < zones.size(); i++)
    {
        RGBColor c = (i < zone_colors.size()) ? zone_colors[i] : zone_colors.back();
        if(c != first)
        {
            all_same = false;
            break;
        }
    }

    uint8_t         payload[16];
    size_t          payload_len = 0;
    uint8_t         function    = 0;
    blankFAPmessage response;

    if(all_same)
    {
        /*------------------------------------------------------*\
        | fn5 SetRgbZonesSingleValue: [R, G, B, count, zones...] |
        \*------------------------------------------------------*/
        function    = FN_0620_SET_RGB_ZONES_SINGLE_VALUE;
        payload[0]  = RGBGetRValue(first);
        payload[1]  = RGBGetGValue(first);
        payload[2]  = RGBGetBValue(first);
        payload[3]  = (uint8_t)zones.size();

        size_t n = zones.size();
        if(n > sizeof(payload) - 4) n = sizeof(payload) - 4;
        for(size_t i = 0; i < n; i++)
        {
            payload[4 + i] = zones[i];
        }
        payload_len = 4 + n;
    }
    else
    {
        /*------------------------------------------------------*\
        | fn2 SetIndividualRgbZones: [zone, R, G, B] × N         |
        | Each entry is 4 bytes; 16-byte payload fits 4 entries. |
        \*------------------------------------------------------*/
        function = FN_0620_SET_INDIVIDUAL_RGB_ZONES;
        size_t n = zones.size();
        if(n > sizeof(payload) / 4) n = sizeof(payload) / 4;
        for(size_t i = 0; i < n; i++)
        {
            RGBColor c = (i < zone_colors.size()) ? zone_colors[i] : zone_colors.back();
            payload[i * 4 + 0] = zones[i];
            payload[i * 4 + 1] = RGBGetRValue(c);
            payload[i * 4 + 2] = RGBGetGValue(c);
            payload[i * 4 + 3] = RGBGetBValue(c);
        }
        payload_len = n * 4;
    }

    SendAckedIntoFAP(caps.idx_headset_rgb_hostmode, function,
                     payload, payload_len, response);

    /*---------------------------------------------------------*\
    | fn6 FrameEnd — byte 0 = 0x01 (transient commit). Never    |
    | 0x00 (silently discarded) and never 0x02 (tested broken   |
    | on G522 firmware).                                        |
    \*---------------------------------------------------------*/
    uint8_t frame_end[4] = { 0x01, 0x00, 0x00, 0x00 };
    SendAckedIntoFAP(caps.idx_headset_rgb_hostmode, FN_0620_FRAME_END,
                     frame_end, sizeof(frame_end), response);

    LOG_TRACE("%s 0x0620 wrote %zu zone(s), fn=0x%02X, FrameEnd[0x01]",
              LOG_TAG, zones.size(), function);
}

/*---------------------------------------------------------*\
| Power management (idle/dim/sleep/wake)                    |
|                                                           |
| Matches Solaar's RGBPowerManager state machine:           |
|   ACTIVE → DIMMING → IDLE → SLEEPING                      |
|                                                           |
| Uses firmware onUserActivity events from 0x8071 for       |
| idle/active detection. SW control flags cycle:            |
|   7 (init) → 5 (active, monitor idle) →                   |
|   3 (idle, monitor active) → 5 (wake)                     |
\*---------------------------------------------------------*/

void LogitechHIDPP20Controller::SetRepaintCallback(std::function<void()> repaint)
{
    request_repaint_fn = repaint;
}

void LogitechHIDPP20Controller::SetReapplyActiveModeCallback(std::function<bool()> cb)
{
    reapply_active_mode_fn = cb;
}

void LogitechHIDPP20Controller::SetRegisterCallback(std::function<void(RGBController*)> cb)
{
    register_controller_fn = cb;
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
    /*----------------------------------------------------------*\
    | Called by power thread when a sub-device connects through  |
    | the Centurion bridge. The reader thread is running, so all |
    | commands go through SendAndRead → ReadFromQueue.           |
    |                                                            |
    | We clear the sub-device feature cache and re-discover      |
    | everything. The bridge_feat_idx and dongle name are kept.  |
    \*----------------------------------------------------------*/
    LOG_DEBUG("%s Re-probing sub-device through bridge", LOG_TAG);

    /*----------------------------------------------------------*\
    | Give the sub-device a moment to settle after connection    |
    | before sending commands through the bridge.                |
    \*----------------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    FlushResponseQueue();

    /*----------------------------------------------------------*\
    | Clear sub-device feature map but keep bridge index.        |
    | This forces fresh lookups through the bridge.              |
    \*----------------------------------------------------------*/
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

    /*---------------------------------------------------------*\
    | Re-populate feature map. CenturionFeatureSet is always at |
    | index 1 on the sub-device.                                |
    \*---------------------------------------------------------*/
    EnumerateFeatures(1);

    if(!caps.feature_map_complete)
    {
        LOG_DEBUG("%s Sub-device not reachable after connect event", LOG_TAG);
        caps.feature_map_complete = true;
        return;
    }

    /*---------------------------------------------------------*\
    | Discover sub-device identity if not already known.        |
    \*---------------------------------------------------------*/
    if(caps.device_name.empty() || caps.device_name.find("PRO X 2") == std::string::npos)
    {
        /*------------------------------------------------------*\
        | Dongle may have a sysfs-derived name; get real name    |
        | from the sub-device now that it's reachable.           |
        \*------------------------------------------------------*/
        std::string old_name = caps.device_name;
        DiscoverDeviceName();

        if(caps.device_name != old_name)
        {
            log_tag = "[LogitechHID++ " + caps.device_name + "]";
        }
    }

    DiscoverDeviceType();
    DiscoverFirmwareInfo();

    /*---------------------------------------------------------*\
    | Discover RGB features                                     |
    \*---------------------------------------------------------*/
    caps.idx_profile_management     = GetFeatureIndex(HIDPP20_FEAT_PROFILE_MANAGEMENT);
    caps.idx_onboard_profiles       = GetFeatureIndex(HIDPP20_FEAT_ONBOARD_PROFILES);
    caps.idx_disable_keys_by_usage  = GetFeatureIndex(HIDPP20_FEAT_DISABLE_KEYS_BY_USAGE);

    DiscoverRGBEffects();
    if(caps.idx_rgb_effects == 0)
    {
        DiscoverHeadsetRGBHostmode();
    }
    DiscoverPerKeyZones();
    DiscoverKeyboardLayout();

    if(!caps.has_zone_effects && !caps.has_perkey)
    {
        LOG_DEBUG("%s Sub-device has no RGB features", LOG_TAG);
        return;
    }

    LOG_INFO("%s Sub-device probed: zones=%zu perkey=%zu",
             LOG_TAG, caps.zone_clusters.size(), caps.perkey_zone_ids.size());

    /*---------------------------------------------------------*\
    | Create and register RGBController for the sub-device.     |
    \*---------------------------------------------------------*/
    Initialize();

    if(register_controller_fn)
    {
        RGBController_LogitechHIDPP20* rgb = new RGBController_LogitechHIDPP20(this);
        register_controller_fn(rgb);

        LOG_INFO("%s Registered RGB controller for sub-device", LOG_TAG);
    }
}

void LogitechHIDPP20Controller::ReconnectDevice()
{
    /*----------------------------------------------------------*\
    | Called by power thread when a WirelessStatus reconnect     |
    | event arrives. Race the firmware boot animation: hammer    |
    | the SW-control claim + per-key push on a fast-backoff      |
    | schedule until the claim ACKs (matches the vendor app,     |
    | which lands control in ~50ms).                             |
    |                                                            |
    | Previously this code split work across the two firmware    |
    | events (reconnect=1/config_needed=1 then config_needed=0)  |
    | and only pushed once per event. The first push raced the   |
    | boot, the second only fired after the animation finished,  |
    | and a failed claim left sw_control_claimed=true so the     |
    | 10s firmware watchdog dropped us back to onboard mode.     |
    \*----------------------------------------------------------*/
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

    /*----------------------------------------------------------*\
    | Always invalidate any stale claim so the loop below runs   |
    | the full claim sequence on each attempt until it sticks.   |
    \*----------------------------------------------------------*/
    sw_control_claimed           = false;
    sw_control_needs_upgrade_to_5 = false;
    retry_paint_deadline_.store(std::chrono::steady_clock::time_point{});
    retry_paint_attempt_.store(0);

    bool   claimed       = false;
    size_t attempt_count = sizeof(HIDPP20_RECLAIM_BACKOFF_MS) / sizeof(uint16_t);

    for(size_t i = 0; i < attempt_count; i++)
    {
        if(HIDPP20_RECLAIM_BACKOFF_MS[i] > 0)
        {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(HIDPP20_RECLAIM_BACKOFF_MS[i]));
        }

        if(!device_online.load())
        {
            return;
        }

        if(reapply_active_mode_fn && reapply_active_mode_fn())
        {
            claimed = true;
            LOG_INFO("%s Device reconnected — SW control reclaimed (attempt %zu/%zu)",
                     LOG_TAG, i + 1, attempt_count);
            break;
        }
    }

    if(!claimed)
    {
        LOG_WARNING("%s Device reconnected — SW control reclaim failed after %zu attempts",
                    LOG_TAG, attempt_count);
    }

    if(first_event && caps.has_power_mgmt)
    {
        ReadFirmwareTimers();
        ReadNvSleepRampConfig();
    }
}

void LogitechHIDPP20Controller::RediscoverFeatures()
{
    /*---------------------------------------------------------*\
    | Clear the cached HID++ feature map and all index/         |
    | function-byte derivations, then re-run the standard       |
    | discovery sequence on the current hid_device handle.      |
    |                                                           |
    | Used in two situations:                                   |
    |   - FullReprobe (a previously-unreachable device just     |
    |     became reachable; reader thread is already running)   |
    |   - SwapHIDHandle (USB/wireless path migration; the new   |
    |     path's HID++ feature map may have completely          |
    |     different feature indices than the old one — observed |
    |     on the G515 LS TKL where wireless RGBEffects sits at  |
    |     idx 0x09 but the USB path puts it elsewhere, causing  |
    |     INVALID_FEATURE_INDEX (0x07) errors on every cached   |
    |     idx_rgb_effects/fn_set_effect/etc. lookup).           |
    |                                                           |
    | Caller is responsible for state that lives outside the    |
    | feature map (sw_control_claimed, frame counters, online   |
    | flag, threads).                                           |
    \*---------------------------------------------------------*/
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

    /*---------------------------------------------------------*\
    | idx_unified_battery lives outside caps (discovered lazily |
    | by QueryExternalPower on first use) so it isn't cleared   |
    | by the caps reset above. Clear it here too so the next    |
    | QueryExternalPower call re-probes on the new path — the   |
    | old path's feature index may not exist, or may map to a   |
    | different feature entirely, on the new map.               |
    \*---------------------------------------------------------*/
    idx_unified_battery = 0;
    last_power_raw      = 0xFFFF;

    /*---------------------------------------------------------*\
    | Force ApplyPowerSavingProfile's dedup to re-emit its      |
    | "Idle management: ..." line on the next call so a path    |
    | transition always produces a full state confirmation in   |
    | the log, symmetric with the QueryExternalPower re-log.    |
    | Inverting ps_last_logged_external guarantees the boolean  |
    | comparison trips regardless of the current power state.   |
    \*---------------------------------------------------------*/
    ps_last_logged_pct      = -1;
    ps_last_logged_idle     = -1;
    ps_last_logged_sleep    = -1;
    ps_last_logged_external = !ps_on_external_power;

    /*---------------------------------------------------------*\
    | Standard HID++ features are looked up on-demand — no bulk |
    | enumeration needed. Just re-discover everything.          |
    \*---------------------------------------------------------*/
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
    DiscoverKeyboardLayout();
}

void LogitechHIDPP20Controller::FullReprobe()
{
    /*----------------------------------------------------------*\
    | Called by power thread when a failed-probe device becomes  |
    | reachable. Like ReprobeSubDevice but for non-bridge        |
    | standard HID++ devices. Reader thread is running.          |
    \*----------------------------------------------------------*/
    LOG_DEBUG("%s Full re-probe of previously unreachable device", LOG_TAG);

    FlushResponseQueue();
    RediscoverFeatures();

    if(!caps.has_zone_effects && !caps.has_perkey)
    {
        LOG_DEBUG("%s Device has no RGB features after re-probe", LOG_TAG);
        return;
    }

    LOG_INFO("%s Re-probe complete: zones=%zu perkey=%zu",
             LOG_TAG, caps.zone_clusters.size(), caps.perkey_zone_ids.size());

    device_online.store(true);
    consecutive_timeouts.store(0);
    watcher_mode.store(false);
    Initialize();

    if(register_controller_fn)
    {
        RGBController_LogitechHIDPP20* rgb = new RGBController_LogitechHIDPP20(this);
        register_controller_fn(rgb);
        LOG_INFO("%s Registered RGB controller after re-probe", LOG_TAG);
    }
}

void LogitechHIDPP20Controller::StartProbeWatcher()
{
    /*---------------------------------------------------------*\
    | Start reader + power threads in watcher mode for a device |
    | that failed initial probe. The power thread periodically  |
    | retries IRoot until the device becomes reachable.         |
    \*---------------------------------------------------------*/
    if(reader_running)
    {
        return;
    }

    watcher_mode.store(true);
    device_online.store(false);

    pending_connection = 0;
    reader_running = true;
    reader_thread = new std::thread(&LogitechHIDPP20Controller::ReaderThreadFunc, this);

    power_thread_running = true;
    power_thread = new std::thread(&LogitechHIDPP20Controller::PowerThreadFunc, this);

    LOG_DEBUG("%s Probe watcher started (retrying every 5s)", LOG_TAG);
}

/*---------------------------------------------------------*\
| ScanForDevice and GetCenturionSubDeviceName live in       |
| LogitechHIDPP20Controller_Linux.cpp /                     |
| LogitechHIDPP20Controller_Windows_MacOS.cpp. Both are     |
| the only parts of this controller that touch platform-    |
| specific HID enumeration details (sysfs vs hidapi         |
| serial_number).                                           |
\*---------------------------------------------------------*/

void LogitechHIDPP20Controller::SwapHIDHandle
    (
    hid_device*         new_dev,
    const std::string&  new_path
    )
{
    /*----------------------------------------------------------*\
    | Stop reader/power threads, swap HID handle, restart.       |
    | Called from the power thread — we can't join ourselves,    |
    | so we stop the reader, swap, and flag for restart.         |
    |                                                            |
    | Actually, we're called from the power thread's scan loop.  |
    | The reader thread is running. We need to:                  |
    | 1. Stop the reader thread                                  |
    | 2. Close old handle, set new one                           |
    | 3. Restart reader thread                                   |
    | 4. Reinit device                                           |
    | The power thread keeps running throughout.                 |
    \*----------------------------------------------------------*/

    /*----------------------------------------------------------*\
    | Stop reader thread                                         |
    \*----------------------------------------------------------*/
    reader_running = false;

    if(reader_thread && reader_thread->joinable())
    {
        reader_thread->join();
    }

    delete reader_thread;
    reader_thread = nullptr;

    /*---------------------------------------------------------*\
    | Swap HID handle                                           |
    \*---------------------------------------------------------*/
    hid_close(dev);
    dev       = new_dev;
    location  = new_path;
    log_tag   = "[LogitechHID++ " + caps.device_name + "]";

    /*----------------------------------------------------------*\
    | Re-discover transport (might change between Centurion and  |
    | standard HID++ if device switches connection types).       |
    \*----------------------------------------------------------*/
    DiscoverTransport();

    /*---------------------------------------------------------*\
    | Reset state for new connection                            |
    \*---------------------------------------------------------*/
    FlushResponseQueue();
    device_online.store(true);
    consecutive_timeouts.store(0);
    sw_control_claimed           = false;
    sw_control_needs_upgrade_to_5 = false;
    retry_paint_deadline_.store(std::chrono::steady_clock::time_point{});
    retry_paint_attempt_.store(0);
    frame_counter = 0;

    {
        std::lock_guard<std::mutex> lock(power_mutex);
        dim_brightness_pct.store(100);
        power_state = HIDPP20_POWER_ACTIVE;
    }

    /*----------------------------------------------------------*\
    | Restart reader thread on new handle                        |
    \*----------------------------------------------------------*/
    reader_running = true;
    reader_thread = new std::thread(&LogitechHIDPP20Controller::ReaderThreadFunc, this);

    /*--------------------------------------------------------------*\
    | Re-discover the HID++ feature map on the new path. The         |
    | wireless dongle path and the USB-direct path expose            |
    | DIFFERENT feature index assignments for the same logical       |
    | features — observed on the G515 LS TKL where wireless          |
    | RGBEffects sits at idx 0x09 but the USB path puts it           |
    | elsewhere. Without this rediscovery, the cached idx_*          |
    | values from the old path point at the wrong features on        |
    | the new one and every reclaim call returns HID++ error         |
    | 0x07 INVALID_FEATURE_INDEX.                                    |
    |                                                                |
    | Must run after the reader thread is restarted (the             |
    | discovery uses queue-backed reads) and before the              |
    | reapply_active_mode_fn callback (so the SW control claim sees  |
    | correct indices).                                              |
    \*--------------------------------------------------------------*/
    RediscoverFeatures();

    /*---------------------------------------------------------*\
    | Reinit device with colors                                 |
    \*---------------------------------------------------------*/
    if(reapply_active_mode_fn)
    {
        reapply_active_mode_fn();
    }

    if(caps.has_power_mgmt)
    {
        ReadFirmwareTimers();
        ReadNvSleepRampConfig();
        QueryExternalPower();
        ApplyPowerSavingProfile();
    }

    LOG_INFO("%s Device reconnected on new path — colors restored", LOG_TAG);
}

void LogitechHIDPP20Controller::StartEventWatcher()
{
    /*------------------------------------------------------------*\
    | Start reader thread only (no power thread) to watch for      |
    | connection events on Centurion dongles without sub-devices.  |
    | When ConnectionStateChangedEvent arrives, pending_connection |
    | is set for the power thread — but since there's no power     |
    | thread, we need a minimal processing loop.                   |
    \*------------------------------------------------------------*/
    if(reader_running)
    {
        return;
    }

    pending_connection = 0;
    reader_running = true;
    reader_thread = new std::thread(&LogitechHIDPP20Controller::ReaderThreadFunc, this);

    /*----------------------------------------------------------*\
    | Start power thread to process connection events.           |
    | It won't do dim/sleep (no RGB) but it handles              |
    | pending_connection for sub-device re-probe.                |
    \*----------------------------------------------------------*/
    power_thread_running = true;
    power_thread = new std::thread(&LogitechHIDPP20Controller::PowerThreadFunc, this);

    LOG_DEBUG("%s Event watcher started (watching for sub-device)", LOG_TAG);
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

    ReadFirmwareTimers();
    ReadNvSleepRampConfig();
    ReadActiveProfileSector();

    LogitechHIDPP20IdleSettings::instance()->load();
    QueryExternalPower();
    ApplyPowerSavingProfile();

    /*-----------------------------------------------------------*\
    | Seed the periodic idle-settings poll clock so the first     |
    | tick of the power thread's 500ms re-read happens one        |
    | interval from now, not immediately (we just applied above). |
    \*-----------------------------------------------------------*/
    last_idle_poll = std::chrono::steady_clock::now();

    /*----------------------------------------------------------*\
    | Don't claim SW control here. The device runs its firmware  |
    | effect (or saved hardware profile) until DeviceUpdateLEDs  |
    | is called for the first time, at which point claim + push  |
    | happen atomically.                                         |
    |                                                            |
    | Reader and power threads still start so we can detect      |
    | migration events (USB plug-in) and process activity events |
    | once SW control is eventually claimed.                     |
    \*----------------------------------------------------------*/
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

    /*----------------------------------------------------------*\
    | Stop power thread first (it may be waiting on the queue)   |
    \*----------------------------------------------------------*/
    power_thread_running = false;
    response_cv.notify_all();

    if(power_thread && power_thread->joinable())
    {
        power_thread->join();
    }

    delete power_thread;
    power_thread = nullptr;

    /*----------------------------------------------------------*\
    | Then stop reader thread                                    |
    \*----------------------------------------------------------*/
    reader_running = false;

    if(reader_thread && reader_thread->joinable())
    {
        reader_thread->join();
    }

    delete reader_thread;
    reader_thread = nullptr;

    /*----------------------------------------------------------*\
    | Wake if we were dimmed/sleeping so Shutdown() can          |
    | cleanly release SW control.                                |
    \*----------------------------------------------------------*/
    if(power_state != HIDPP20_POWER_ACTIVE)
    {
        Wake();
    }

    LOG_DEBUG("%s Power manager stopped", LOG_TAG);
}

void LogitechHIDPP20Controller::ReaderThreadFunc()
{
    /*----------------------------------------------------------*\
    | Sole HID reader. NEVER sends commands — that would         |
    | deadlock (we'd wait on our own queue for the response).    |
    | Events are flagged via atomic for the power thread.        |
    \*----------------------------------------------------------*/
    while(reader_running.load())
    {
        uint8_t feat = 0, func = 0;
        uint8_t data[60] = {};
        int result = ReadHIDDirect(&feat, &func, data, sizeof(data), 50);

        if(result < 0)
        {
            /*--------------------------------------------------*\
            | HID read error — device handle is invalid (device  |
            | physically removed). Mark offline and sleep to     |
            | avoid spinning.                                    |
            \*--------------------------------------------------*/
            if(device_online.load())
            {
                LOG_DEBUG("%s HID read error — device removed", LOG_TAG);
                device_online.store(false);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        if(result > 0)
        {
            /*--------------------------------------------------*\
            | Check for firmware events first.                   |
            | Events are flagged for the power thread and NOT    |
            | added to the response queue — they aren't command  |
            | responses and would pollute the queue.             |
            \*--------------------------------------------------*/
            if(caps.idx_rgb_effects != 0 &&
               feat == caps.idx_rgb_effects &&
               (func & 0xF0) == 0x10 &&
               (func & 0x0F) != HIDPP20_SW_ID)
            {
                pending_activity.store((int)data[0]);
                continue;
            }

            /*--------------------------------------------------*\
            | CentPPBridge event 0: ConnectionStateChangedEvent  |
            | Sub-device connected or disconnected from dongle.  |
            \*--------------------------------------------------*/
            if(transport.bridge_feat_idx != 0 &&
               feat == transport.bridge_feat_idx &&
               (func & 0xF0) == 0x00 &&
               (func & 0x0F) != HIDPP20_SW_ID)
            {
                /*--------------------------------------------------*\
                | ConnectionStateChangedEvent payload:               |
                |   data[0] = ? (always 0 in observed packets)       |
                |   data[1] = number of connected sub-devices        |
                |   data[2] = ?                                      |
                | Connect: [00 01 00], Disconnect: [00 00 00]        |
                \*--------------------------------------------------*/
                uint8_t num_devices = data[1];
                LOG_DEBUG("%s Bridge ConnectionStateChanged: %d sub-device(s) (data: %02X %02X %02X)",
                          LOG_TAG, num_devices, data[0], data[1], data[2]);
                /*-------------------------------------------------*\
                | Store +1 for connected, -1 for disconnected.      |
                | Power thread checks sign to decide action.        |
                \*-------------------------------------------------*/
                pending_connection.store(num_devices > 0 ? 1 : -1);
                continue;
            }

            /*---------------------------------------------------*\
            | Feature 0x1D4B event 0: WirelessStatus              |
            | Device reconnected after power cycle.               |
            | Use cached map lookup only — reader thread must     |
            | never send commands (deadlock risk).                |
            \*---------------------------------------------------*/
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

                    /*--------------------------------------------------*\
                    | Forward both events to the power thread. Each      |
                    | call into ReconnectDevice runs the fast-backoff    |
                    | reclaim loop, so the second event acts as a        |
                    | belt-and-suspenders re-claim once the firmware     |
                    | boot fully settles.                                |
                    \*--------------------------------------------------*/
                    pending_connection.store(1);

                    continue;
                }
            }

            /*---------------------------------------------------*\
            | HID++1.0 Device Connection notifications from the   |
            | Lightspeed receiver. sub_id 0x40 = Device           |
            | Disconnection, 0x41 = Device Connection Status.     |
            | Either one is a signal that the paired device's     |
            | preferred path just changed — typically because     |
            | the user plugged in (or unplugged) the USB cable    |
            | on the device itself.                               |
            |                                                     |
            | These arrive on the paired device's hidraw with     |
            | device_index=0x01 (not the receiver's own           |
            | endpoint) because the kernel dj-receiver driver     |
            | routes them to the device's virtual hidraw. We      |
            | see them because our reader is attached to that     |
            | hidraw.                                             |
            |                                                     |
            | Flag a force path-check for the power thread. It    |
            | will run ScanForDevice(true) which bypasses the     |
            | device_online gate so the scan can find a           |
            | different-PID migration candidate even while the    |
            | current path hasn't failed yet. If no such          |
            | candidate exists (same path, false alarm), the      |
            | scan is a no-op.                                    |
            |                                                     |
            | This is how we catch the wireless→USB transition:   |
            | the notification fires BEFORE the firmware fully    |
            | switches its data flow, giving us a window to       |
            | migrate proactively. The reverse direction          |
            | (USB→wireless) is already handled via the USB fd    |
            | becoming invalid on cable unplug.                   |
            \*---------------------------------------------------*/
            if(feat == 0x40 || feat == 0x41)
            {
                /*--------------------------------------------------*\
                | Set 75 retries × 200ms = ~15 second window. The    |
                | keyboard's USB HID++ interface (page 0xFF00) can   |
                | take 10+ seconds to appear after the boot HID      |
                | interface — the first DJ notification fires when   |
                | the boot interface comes up, but hid_enumerate     |
                | won't return the HID++ interface until the kernel  |
                | finishes setting up all three interfaces. Until    |
                | the path-check clears, the device is dark/         |
                | uncontrolled, so we poll at the same fast cadence  |
                | as offline recovery (200ms) to minimize how long   |
                | the user sees default firmware behavior.           |
                |                                                    |
                | A subsequent DJ notification before timeout resets |
                | the counter to 75, extending the retry window.     |
                \*--------------------------------------------------*/
                LOG_DEBUG("%s LogitechHID++1.0 connection notification sub_id=0x%02X "
                          "flags=0x%02X (path change — forcing scan retries)",
                          LOG_TAG, feat, func);
                pending_path_check.store(75);
                continue;
            }

            /*----------------------------------------------------*\
            | Only queue responses to OUR commands.                |
            | Our commands use HIDPP20_SW_ID (0x0A) in the low     |
            | nibble. Firmware-generated messages (battery,        |
            | sync, etc.) use SW_ID 0 — drop those silently.       |
            | Error responses (feat=0xFF) are always queued.       |
            \*----------------------------------------------------*/
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
    /*----------------------------------------------------------*\
    | Handles power state machine and sends commands.            |
    | Reads responses from the queue (filled by reader thread).  |
    \*----------------------------------------------------------*/
    std::chrono::steady_clock::time_point last_probe_time = std::chrono::steady_clock::now();

    while(power_thread_running.load())
    {
        /*------------------------------------------------------*\
        | Watcher mode: device failed initial probe. Retry       |
        | IRoot every 5 seconds until device becomes reachable.  |
        \*------------------------------------------------------*/
        if(watcher_mode.load())
        {
            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

            if(now - last_probe_time >= std::chrono::seconds(5))
            {
                last_probe_time = now;

                uint8_t test_idx = GetFeatureIndex(HIDPP20_FEAT_FEATURE_SET);

                if(test_idx != 0)
                {
                    LOG_INFO("%s Device became reachable — initiating full probe", LOG_TAG);
                    FullReprobe();
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        /*------------------------------------------------------*\
        | 1. Check for pending firmware events                   |
        \*------------------------------------------------------*/
        int activity = pending_activity.exchange(-1);

        if(activity >= 0)
        {
            std::lock_guard<std::mutex> lock(power_mutex);
            OnUserActivity((uint8_t)activity);
        }

        /*------------------------------------------------------*\
        | 1b. Check for connection state changes                 |
        \*------------------------------------------------------*/
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

        /*-------------------------------------------------------*\
        | 1c. Reactive scan for connection migration.             |
        |                                                         |
        | Three modes feed this loop:                             |
        |   a) device_online == true, no path-check pending       |
        |        -> 2s idle interval, scan is a no-op             |
        |   b) device_online == false                             |
        |        -> 200ms fast interval, scan tries to reclaim    |
        |   c) pending_path_check > 0 (DJ notification fired)     |
        |        -> 200ms fast interval, force-scan bypasses      |
        |           the online gate to find a different-PID       |
        |           migration candidate; counter decrements per   |
        |           failed attempt and clears on success          |
        |                                                         |
        | The retry counter exists because the keyboard's USB     |
        | HID++ interface can take 10+ seconds to enumerate       |
        | after the boot HID interface comes up — the first DJ    |
        | notification fires too early to find anything. 75 *     |
        | 200ms = ~15s window, plenty for the slowest observed    |
        | enumeration.                                            |
        \*-------------------------------------------------------*/
        if(!caps.unit_id.empty())
        {
            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            int  path_retries = pending_path_check.load();
            bool online       = device_online.load();

            std::chrono::milliseconds interval = (online && path_retries == 0)
                                                ? std::chrono::milliseconds(2000)
                                                : std::chrono::milliseconds(200);

            if(now - last_probe_time >= interval)
            {
                last_probe_time = now;

                bool force   = (path_retries > 0) || !online;
                bool success = ScanForDevice(force);

                if(path_retries > 0)
                {
                    if(success)
                    {
                        pending_path_check.store(0);
                    }
                    else
                    {
                        pending_path_check.fetch_sub(1);
                    }
                }
            }
        }

        /*------------------------------------------------------*\
        | 2. Power management timing                             |
        \*------------------------------------------------------*/
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
                    /*---------------------------------------------------*\
                    | Poll dim brightness target — if the user is         |
                    | dragging the slider, ps_dim_target_pct updates      |
                    | in-memory and we pick it up here on the next 50ms   |
                    | tick without any callback/repaint chain.            |
                    |                                                     |
                    | Gated on ps_dim_enabled: a profile (or the default  |
                    | unconfigured fallback) can enter IDLE state via     |
                    | the skip-dim path in OnUserActivity, and we must    |
                    | not dim in that case — only sleep when the deadline |
                    | hits.                                               |
                    \*---------------------------------------------------*/
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

        /*------------------------------------------------------*\
        | Fast poll of idle settings + external-power flag.      |
        | QueryExternalPower is a single HID++ 0x1004 GetStatus  |
        | call — cheap on wire and lets ApplyPowerSavingProfile  |
        | pick between the on_battery and plugged_in profiles    |
        | within half a second of a power-source transition.     |
        | The idle-settings reload itself is purely in-memory.   |
        \*------------------------------------------------------*/
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

        /*------------------------------------------------------*\
        | Fire any pending retry-paint whose deadline has come   |
        | due. The callback runs DeviceUpdateLEDs on this        |
        | thread's context, not recursively inside another call. |
        \*------------------------------------------------------*/
        TickRetryPaintIfPending();

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

bool LogitechHIDPP20Controller::IsCurrentlyWireless() const
{
    return wireless;
}

bool LogitechHIDPP20Controller::QueryWirelessStatus()
{
    if(caps.idx_wireless_status == 0)
    {
        LOG_DEBUG("%s QueryWirelessStatus: feature not present", LOG_TAG);
        return false;
    }

    uint8_t send_data[1] = {0};
    uint8_t recv_data[16] = {};
    uint8_t hidpp_err = 0;

    int result = SendAcked(caps.idx_wireless_status, 0,
                           send_data, 0, recv_data, sizeof(recv_data),
                           HIDPP20_POLICY_PROBE, &hidpp_err);

    LOG_DEBUG("%s QueryWirelessStatus: result=%d err=0x%02X "
              "data=[%02X %02X %02X %02X %02X %02X]",
              LOG_TAG, result, hidpp_err,
              recv_data[0], recv_data[1], recv_data[2],
              recv_data[3], recv_data[4], recv_data[5]);

    if(result <= 0)
    {
        return false;
    }

    return true;
}

bool LogitechHIDPP20Controller::QueryExternalPower()
{
    /*----------------------------------------------------------*\
    | Query HID++ 2.0 feature 0x1004 (UnifiedBattery) fn1        |
    | GetStatus and determine whether the device is drawing      |
    | external power.                                            |
    |                                                            |
    | Response layout:                                           |
    |   byte 2: Charging Status                                  |
    |     0 = Discharging                                        |
    |     1 = Charging (wired)                                   |
    |     2 = Charging (slow)                                    |
    |     3 = Complete                                           |
    |     4 = Error                                              |
    |     5 = Wireless Charging                                  |
    |   byte 3: External Power Status                            |
    |     0 = no external power                                  |
    |     non-zero = external power present                      |
    |                                                            |
    | We consider the device externally powered if EITHER byte   |
    | is non-zero: some devices leave byte 3 at 0 whenever they  |
    | are actively charging and rely on byte 2 alone to signal   |
    | the wired state. The pre-refactor QueryOnBattery used the  |
    | same OR semantic (expressed from the on-battery side) and  |
    | was known to work across the Logitech lineup.              |
    |                                                            |
    | Updates ps_on_external_power and returns the new value.    |
    | On failure returns the cached value without touching it.   |
    \*----------------------------------------------------------*/
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
            /*-----------------------------------------------------*\
            | Device doesn't expose UnifiedBattery. Wired-only      |
            | devices (no battery) report the feature absent; we    |
            | treat them as permanently externally powered.         |
            \*-----------------------------------------------------*/
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
        LOG_TRACE("%s QueryExternalPower: GetStatus failed (result=%d) — using cached",
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
    /*----------------------------------------------------------*\
    | Re-read the JSON every invocation. This is a cheap         |
    | in-memory SettingsManager hash lookup + a handful of       |
    | field copies — safe to do on every 500ms power-thread      |
    | tick. Any write from the plugin (or a manual JSON edit)    |
    | therefore applies within one poll interval without any     |
    | cross-boundary signalling.                                 |
    \*----------------------------------------------------------*/
    LogitechHIDPP20IdleSettings* settings = LogitechHIDPP20IdleSettings::instance();
    settings->load();

    bool prev_dim   = ps_dim_enabled;
    bool prev_sleep = ps_sleep_enabled;

    /*----------------------------------------------------------*\
    | Start from the firmware-timer baseline. Both the           |
    | configured and unconfigured paths return to these if       |
    | they don't explicitly override, so a profile that sets     |
    | idle_timeout_s does not leave a stale value behind after   |
    | the user resets to an empty config.                        |
    \*----------------------------------------------------------*/
    idle_timeout_s  = fw_idle_timeout_s;
    sleep_timeout_s = fw_sleep_timeout_s;

    if(!settings->isConfigured())
    {
        /*---------------------------------------------------------*\
        | Unconfigured: no plugin in use. We still hold SW control  |
        | so firmware will NOT dim or sleep autonomously — it only  |
        | emits idle events and expects the host to act. Run a      |
        | basic default profile ourselves: no dim on idle (OpenRGB  |
        | users generally expect lights to stay on), but still go   |
        | to sleep at the firmware-configured timeout.              |
        \*---------------------------------------------------------*/
        ps_dim_enabled    = false;
        ps_dim_target_pct = DIM_TARGET_PCT;
        ps_sleep_enabled  = true;

        /*------------------------------------------------------*\
        | Restore firmware defaults on the device if we          |
        | previously wrote custom values from a plugin profile.  |
        \*------------------------------------------------------*/
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

    /*---------------------------------------------------------*\
    | Configured: pick the active profile based on whether the  |
    | device is currently externally powered. ps_on_external_   |
    | power is refreshed by QueryExternalPower() on the same    |
    | 500 ms power-thread poll that calls us.                   |
    \*---------------------------------------------------------*/
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
        /*------------------------------------------------------*\
        | Signal "don't sleep" to the state machine. The IDLE    |
        | branch of PowerThreadFunc gates on sleep_timeout_s>0.  |
        \*------------------------------------------------------*/
        sleep_timeout_s = 0;
    }

    /*----------------------------------------------------------*\
    | Write our timer values to the device RAM so the firmware's |
    | idle detection aligns with our host-side state machine.    |
    | Only writes when values actually change to avoid spamming  |
    | the bus on every 500ms poll tick.                          |
    \*----------------------------------------------------------*/
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
    std::lock_guard<std::mutex> lock(response_mutex);
    response_queue.clear();
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

    /*------------------------------------------------------------*\
    | onUserActivity = event 1 on RGB Effects (0x8071)             |
    | Event function byte: (1 << 4) | fw_swid                      |
    | Our commands use HIDPP20_SW_ID (0x0A); firmware events use   |
    | a different sw_id (typically 0).                             |
    \*------------------------------------------------------------*/
    if(feat == caps.idx_rgb_effects &&
       (func & 0xF0) == 0x10 &&
       (func & 0x0F) != HIDPP20_SW_ID)
    {
        OnUserActivity(data[0]);
    }
}

void LogitechHIDPP20Controller::OnUserActivity(uint8_t activity_type)
{
    /*----------------------------------------------------------*\
    | power_mutex must already be held by the caller.            |
    \*----------------------------------------------------------*/

    if(activity_type == 0)
    {
        /*------------------------------------------------------*\
        | IDLE event — firmware detected inactivity.             |
        | Only act if we're currently ACTIVE.                    |
        | Firmware sends a burst of ~8 events; ignore repeats.   |
        \*------------------------------------------------------*/
        if(power_state != HIDPP20_POWER_ACTIVE)
        {
            return;
        }

        if(!ps_dim_enabled && !ps_sleep_enabled)
        {
            return;
        }

        LOG_DEBUG("%s onUserActivity: IDLE — starting dim", LOG_TAG);

        /*------------------------------------------------------*\
        | Flush stale per-key ACKs before sending commands       |
        \*------------------------------------------------------*/
        FlushResponseQueue();

        /*------------------------------------------------------*\
        | Switch to flags=3 (ZONE|POWER): release EFFECT to      |
        | firmware, monitor for user activity (keypress).        |
        \*------------------------------------------------------*/
        SetSWControl(3, 3);

        if(!ps_dim_enabled)
        {
            power_state = HIDPP20_POWER_IDLE;

            uint16_t sleep_delay = (sleep_timeout_s > idle_timeout_s)
                                 ? (sleep_timeout_s - idle_timeout_s) : 0;
            sleep_deadline = std::chrono::steady_clock::now()
                           + std::chrono::seconds(sleep_delay);

            LOG_DEBUG("%s Dim disabled — skipping to IDLE (sleep in %us)",
                      LOG_TAG, sleep_delay);
        }
        else
        {
            StartDimRamp();
        }
    }
    else
    {
        /*------------------------------------------------------*\
        | ACTIVE event — user resumed typing.                    |
        | Only act if we're NOT already active.                  |
        \*------------------------------------------------------*/
        if(power_state == HIDPP20_POWER_ACTIVE)
        {
            return;
        }

        LOG_DEBUG("%s onUserActivity: ACTIVE — waking", LOG_TAG);
        Wake();
    }
}

void LogitechHIDPP20Controller::StartDimRamp()
{
    /*----------------------------------------------------------*\
    | Start the brightness ramp from 100% to DIM_TARGET_PCT.     |
    | The actual dimming happens in DeviceUpdateLEDs — it reads  |
    | dim_brightness_pct and scales the color buffer output.     |
    | This is our own host-side animation, independent of the    |
    | firmware's sleep-ramp timer.                               |
    \*----------------------------------------------------------*/
    dim_step      = 0;
    next_dim_time = std::chrono::steady_clock::now();
    power_state   = HIDPP20_POWER_DIMMING;

    LOG_DEBUG("%s Dim ramp started (100%% -> %d%%)", LOG_TAG, ps_dim_target_pct);
}

void LogitechHIDPP20Controller::DimRampStep()
{
    /*----------------------------------------------------------*\
    | power_mutex must already be held by the caller.            |
    | Adjusts brightness and requests a repaint so               |
    | DeviceUpdateLEDs pushes the dimmed colors.                 |
    \*----------------------------------------------------------*/
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

    /*----------------------------------------------------------*\
    | Request repaint so DeviceUpdateLEDs applies the new        |
    | brightness. For animations this is redundant (the          |
    | animation loop already calls it), but for static colors    |
    | this is the only way to push the dimmed output.            |
    |                                                            |
    | Do NOT bump init_generation here — that would clear        |
    | sent_colors and make the next DeviceUpdateLEDs treat the   |
    | frame as a first-push, firing the SetZoneEffect(0xFF,      |
    | static black, persist=true) prep call. On mice that flash  |
    | as a brief black-out per dim step. Delta tracking already  |
    | handles the changed brightness correctly: snapshot is the  |
    | scaled output, sent_colors holds the previously scaled     |
    | frame, and the diff catches every pixel that moved.        |
    \*----------------------------------------------------------*/
    if(request_repaint_fn)
    {
        request_repaint_fn();
    }

    /*----------------------------------------------------------*\
    | Check if dim ramp is complete                              |
    \*----------------------------------------------------------*/
    if(dim_step >= DIM_STEPS)
    {
        power_state = HIDPP20_POWER_IDLE;

        /*------------------------------------------------------*\
        | Pull the sleep deadline forward by the firmware's      |
        | off-ramp duration so the firmware fade *ends* at the   |
        | user-configured sleep_timeout_s. Without this we'd     |
        | be late by nv_sleep_ramp_seconds (30s on G515).        |
        \*------------------------------------------------------*/
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

        LOG_DEBUG("%s Dim complete — IDLE (sleep in %us, effective_sleep=%us)",
                  LOG_TAG, sleep_delay, effective_sleep);
    }
}

void LogitechHIDPP20Controller::StartSleep()
{
    /*----------------------------------------------------------*\
    | SetRgbPowerMode(3) = firmware-managed fade to off.         |
    | The firmware handles the fade internally.                  |
    |                                                            |
    | Set power_state BEFORE sending the command so that         |
    | DeviceUpdateLEDs sees SLEEPING and stops pushing frames    |
    | before the sleep command hits the wire. Suppression is     |
    | the safe default: a write arriving after SetRgbPowerMode   |
    | (3) can otherwise wake the device and cancel the sleep.    |
    | Devices carrying FADE_ACCEPTS_WRITES opt out of            |
    | suppression — their firmware tolerates writes during the   |
    | fade.                                                      |
    \*----------------------------------------------------------*/
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
    /*----------------------------------------------------------*\
    | Called from OnUserActivity(1) when the firmware tells us   |
    | the device has seen user input. Works for DIMMING, IDLE,   |
    | and SLEEPING states uniformly — the only wrinkle is that   |
    | SLEEPING means we previously sent SetRgbPowerMode(3) to    |
    | initiate the fade, so we have to explicitly cancel it      |
    | with SetRgbPowerMode(1) first.                             |
    |                                                            |
    | Per the 0x8071 protocol lifecycle, a proper wake is: power |
    | mode 1 (if we were sleeping), then SetSWControl(3,5) to    |
    | re-claim                                                   |
    | rendering from the firmware's idle-monitor mode, then      |
    | re-push the current lighting state at full brightness.     |
    |                                                            |
    | The re-push uses request_repaint_fn (DeviceUpdateLEDs)     |
    | NOT reapply_active_mode_fn (which re-runs the full claim   |
    | + per-key prep sequence). Wake is NOT a reconnect — the    |
    | device handle, feature map, SW control claim, and per-key  |
    | prep are all still intact. Re-running the claim would      |
    | briefly reset the zone effect layer and flash the firmware |
    | default colors for ~50ms before per-key takes back over.   |
    |                                                            |
    | power_mutex must already be held by the caller.            |
    \*----------------------------------------------------------*/
    HIDPP20PowerState prev = power_state;

    FlushResponseQueue();

    if(prev == HIDPP20_POWER_SLEEPING)
    {
        /*------------------------------------------------------*\
        | Cancel the firmware's fade-to-off. SW control is still |
        | ours; this is not a reconnect. The device stays on the |
        | same hidraw handle, same feature map, same claim.      |
        \*------------------------------------------------------*/
        SetRGBPowerMode(1);
    }

    SetSWControl(3, 5);
    dim_brightness_pct.store(100);
    deep_sleep.store(false);
    consecutive_frame_end_failures.store(0);
    power_state = HIDPP20_POWER_ACTIVE;

    LOG_DEBUG("%s Woke from state %d", LOG_TAG, prev);

    /*----------------------------------------------------------*\
    | Re-push the current lighting state at full brightness.     |
    |                                                            |
    | We use request_repaint_fn (lightweight: just calls         |
    | DeviceUpdateLEDs) NOT reapply_active_mode_fn (heavyweight: |
    | re-runs ClaimSWControlIfNeeded → SetOnboardMode → per-key  |
    | prep sequence → DeviceUpdateMode). On a wake-from-dim/idle |
    | the device is still in host mode, SW control is still      |
    | claimed, and the per-key prep has already been established |
    | — all we need is a fresh paint at restored brightness.     |
    |                                                            |
    | The brightness was restored to 100% above                  |
    | (dim_brightness_pct.store(100)), so DeviceUpdateLEDs will  |
    | apply the full-brightness multiplier to the snapshot.      |
    | Since sent_colors was recorded at the previous (dimmed)    |
    | brightness, the delta detects a change on every zone and   |
    | pushes a full frame naturally — no sent_colors.clear()     |
    | needed.                                                    |
    |                                                            |
    | ReapplyActiveMode (the heavyweight path) is reserved for   |
    | reconnects where the device was fully re-enumerated and    |
    | needs the complete claim + prep + mode re-establishment.   |
    \*----------------------------------------------------------*/
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

    /*----------------------------------------------------------*\
    | GetRgbPowerModeConfig (fn7, sub-function 0x00 = get)       |
    | Response: [echo], idle_hi, idle_lo, sleep_hi, sleep_lo     |
    \*----------------------------------------------------------*/
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
    /*----------------------------------------------------------*\
    | SetRgbPowerModeConfig (fn7, sub-function 0x01 = set)       |
    | Wire format (long message, 16 bytes payload, matches the   |
    | GET response layout at the same offsets):                  |
    |   [0x01, 0x00, 0x00, idle_hi, idle_lo, sleep_hi, sleep_lo, |
    |    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]   |
    |                                                            |
    | These are the firmware's *runtime* power timers — the      |
    | values reset on power cycle but persist across SW control  |
    | release/reclaim, so we need to write them ourselves on     |
    | every claim to be safe.                                    |
    \*----------------------------------------------------------*/
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
    | act on the contents — just log them so we can see what     |
    | the device thinks its persisted state is.                  |
    |                                                            |
    | Wire format mirrors observed wire capture (load followed   |
    | by 7× readBuffer):                                         |
    |   load:        long msg, [partition=0x01, sector=0x01,     |
    |                           size_hi=0x00, size_lo=0x63,      |
    |                           padding to 16 bytes]             |
    |   readBuffer:  short msg, [offset_hi, offset_lo, 0]        |
    |                returns long msg with 16 bytes of data      |
    |                                                            |
    | Sector size 0x63 = 99 bytes is what the vendor app         |
    | requested for the G515 active profile. Other devices may   |
    | differ — we                                                |
    | hardcode it for now since this is diagnostic-only.         |
    \*----------------------------------------------------------*/
    if(caps.idx_profile_management == 0)
    {
        return;
    }

    constexpr uint16_t SECTOR_SIZE = 0x63;  // 99 bytes
    constexpr uint16_t PAGE_SIZE   = 16;

    /*---------------------------------------------------------*\
    | Step 1: load the sector into the device's read buffer     |
    \*---------------------------------------------------------*/
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

    /*---------------------------------------------------------*\
    | Step 2: page the sector out 16 bytes at a time            |
    \*---------------------------------------------------------*/
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

    /*----------------------------------------------------------*\
    | Step 3: log as a hexdump, one row per 16 bytes             |
    \*----------------------------------------------------------*/
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

