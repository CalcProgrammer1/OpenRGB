/*---------------------------------------------------------*\
| LogitechHIDPP20Controller.h                               |
|                                                           |
|   Unified Logitech HID++ 2.0 controller                   |
|                                                           |
|   Uses feature discovery (IRoot 0x0000) to dynamically    |
|   determine device capabilities and adapt to any HID++    |
|   2.0 device with RGB lighting support.                   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <memory>
#include <thread>
#include <atomic>
#include <functional>
#include <chrono>
#include <deque>
#include <condition_variable>
#include <hidapi.h>
#include "RGBController.h"
#include "LogitechProtocolCommon.h"

/*-----------------------------------------------------*\
| HID++ 2.0 Feature Page IDs                            |
\*-----------------------------------------------------*/
#define HIDPP20_FEAT_IROOT                    0x0000
#define HIDPP20_FEAT_FEATURE_SET              0x0001
#define HIDPP20_FEAT_DEVICE_NAME_TYPE         0x0005
#define HIDPP20_FEAT_ONBOARD_PROFILES         0x8100
#define HIDPP20_FEAT_PROFILE_MANAGEMENT       0x8101
#define HIDPP20_FEAT_FIRMWARE_INFO            0x0003
#define HIDPP20_FEAT_CENTPPBRIDGE             0x0003  /* same ID, different meaning on Centurion */
#define HIDPP20_FEAT_COLOR_LED_EFFECTS        0x8070
#define HIDPP20_FEAT_RGB_EFFECTS              0x8071
#define HIDPP20_FEAT_PER_KEY_LIGHTING_V1      0x8080
#define HIDPP20_FEAT_PER_KEY_LIGHTING_V2      0x8081
#define HIDPP20_FEAT_KEYBOARD_LAYOUT          0x4540
#define HIDPP20_FEAT_DISABLE_KEYS_BY_USAGE    0x4522
#define HIDPP20_FEAT_CENTURION_RGB            0x0600
#define HIDPP20_FEAT_HEADSET_RGB_HOSTMODE     0x0620
#define HIDPP20_FEAT_CENTURION_DEVICE_INFO   0x0100
#define HIDPP20_FEAT_CENTURION_DEVICE_NAME   0x0101
#define HIDPP20_FEAT_UNIFIED_BATTERY          0x1004
#define HIDPP20_FEAT_WIRELESS_STATUS          0x1D4B

/*-----------------------------------------------------*\
| HID++ 2.0 Function IDs (byte 3 high nibble)           |
| Function ID is shifted left 4 bits, low nibble = swID |
\*-----------------------------------------------------*/
/*-----------------------------------------------------*\
| HID++ Software ID — identifies our responses.         |
| Must avoid: 0x00 (firmware), 0x01 (vendor app),       |
| 0x02-0x0F (Solaar cycles these).                      |
| There are only 16 values (4-bit field), and all are   |
| claimed. We pick a fixed value and will coordinate    |
| with Solaar to exclude it from its cycle.             |
\*-----------------------------------------------------*/
#define HIDPP20_SW_ID                         0x07

/*-----------------------------------------------------*\
| Feature 0x8071 functions                              |
\*-----------------------------------------------------*/
#define FN_8071_GET_INFO                    0x00
#define FN_8071_SET_EFFECT                  0x10
#define FN_8071_SET_PATTERN                 0x20
#define FN_8071_NV_CONFIG                   0x30
#define FN_8071_BIN_INFO                    0x40
#define FN_8071_SW_CONTROL                  0x50
#define FN_8071_SYNC                        0x60
#define FN_8071_PWR_CONFIG                  0x70
#define FN_8071_PWR_MODE                    0x80

/*-----------------------------------------------------*\
| Feature 0x8081 functions                              |
\*-----------------------------------------------------*/
#define FN_8081_GET_INFO                    0x00
#define FN_8081_SET_INDIVIDUAL              0x10
#define FN_8081_SET_CONSECUTIVE             0x20
#define FN_8081_SET_DELTA_5BIT              0x30
#define FN_8081_SET_DELTA_4BIT              0x40
#define FN_8081_SET_RANGE                   0x50
#define FN_8081_SET_SINGLE_VALUE            0x60
#define FN_8081_FRAME_END                   0x70

/*-----------------------------------------------------*\
| Feature 0x0620 functions (headset RGB hostmode)       |
\*-----------------------------------------------------*/
#define FN_0620_GET_INFO                    0x00
#define FN_0620_GET_RGB_ZONE_INFO           0x10
#define FN_0620_SET_INDIVIDUAL_RGB_ZONES    0x20
#define FN_0620_SET_CONSECUTIVE_RGB_ZONES   0x30
#define FN_0620_SET_RANGE_RGB_ZONES         0x40
#define FN_0620_SET_RGB_ZONES_SINGLE_VALUE  0x50
#define FN_0620_FRAME_END                   0x60
#define FN_0620_GET_HOST_MODE_STATE         0x70
#define FN_0620_SET_HOST_MODE_STATE         0x80

/*-----------------------------------------------------*\
| Feature 0x8100 functions                              |
\*-----------------------------------------------------*/
#define FN_8100_SET_ONBOARD_MODE            0x10
#define FN_8100_GET_ONBOARD_MODE            0x20

/*-----------------------------------------------------*\
| Feature 0x8101 functions                              |
\*-----------------------------------------------------*/
#define FN_8101_GET_SET_MODE                0x60
#define FN_8101_LOAD                        0x80    // load(partition, sector, size)
#define FN_8101_READBUFFER                  0xC0    // readBuffer(offset)

/*-----------------------------------------------------*\
| Zone cluster effect entry                             |
\*-----------------------------------------------------*/
struct HIDPP20Effect
{
    uint8_t     index;
    uint16_t    effect_id;
    uint16_t    capabilities;
    uint16_t    default_period;
};

/*-----------------------------------------------------*\
| Zone cluster info from 0x8071 GetRgbClusterInfo       |
\*-----------------------------------------------------*/
struct HIDPP20ZoneCluster
{
    uint8_t                     index;
    uint16_t                    location;
    uint8_t                     effect_count;
    std::vector<HIDPP20Effect>    effects;
};

/*-----------------------------------------------------*\
| Per-model device quirks — behavioral differences that |
| can't be detected via feature probing.                |
\*-----------------------------------------------------*/
enum HIDPP20DeviceQuirks : uint32_t
{
    HIDPP20_QUIRK_NONE                = 0,
    HIDPP20_QUIRK_FADE_ACCEPTS_WRITES = (1 << 0),  // firmware accepts host frames during sleep fade without waking
};

struct HIDPP20DeviceQuirkEntry
{
    uint16_t    pid_wireless;
    uint16_t    pid_wired;
    uint32_t    quirks;
};

/*---------------------------------------------------------*\
| Default: suppress frames while SLEEPING. Safe everywhere  |
| — it cannot wake a device that treats writes as activity. |
| Devices listed here opt out of suppression because their  |
| firmware accepts writes during the fade without           |
| cancelling sleep.                                         |
\*---------------------------------------------------------*/
static constexpr HIDPP20DeviceQuirkEntry HIDPP20_DEVICE_QUIRK_TABLE[] =
{
    { 0x40B4, 0xC355, HIDPP20_QUIRK_FADE_ACCEPTS_WRITES },  // G515 LS TKL
};

/*-----------------------------------------------------*\
| Device capabilities discovered via feature probing    |
\*-----------------------------------------------------*/
struct HIDPP20DeviceCapabilities
{
    std::string     device_name;
    uint8_t         device_type;
    std::string     firmware_version;
    std::string     serial_number;
    std::string     unit_id;            // stable hardware ID (from FirmwareInfo fn0)
    uint16_t        pid_wireless;       // wireless virtual PID (from FirmwareInfo fn0)
    uint16_t        pid_wired;          // wired USB PID (from FirmwareInfo fn0)
    uint32_t        quirks;             // resolved from HIDPP20_DEVICE_QUIRK_TABLE after PID discovery

    /*--------------------------------------------------*\
    | Complete feature map (feature_id → runtime index)  |
    | Built once by EnumerateFeatures, used by all       |
    | subsequent GetFeatureIndex lookups (no wire).      |
    \*--------------------------------------------------*/
    std::map<uint16_t, uint8_t> feature_map;
    std::map<uint16_t, uint8_t> feature_versions;  /* feature_id -> version byte */
    bool        feature_map_complete;   // true after bulk enumeration

    /*-------------------------------------------------*\
    | Feature indices (0 = not supported)               |
    \*-------------------------------------------------*/
    uint8_t         idx_onboard_profiles;
    uint8_t         idx_profile_management;
    uint8_t         idx_rgb_effects;
    uint8_t         idx_headset_rgb_hostmode;   /* 0x0620 — Centurion headset RGB */
    uint8_t         idx_perkey_v2;
    uint8_t         idx_perkey_v1;
    uint8_t         idx_wireless_status;
    uint8_t         idx_disable_keys_by_usage;  /* 0x4522 — keyboard-family handshake */
    uint16_t        rgb_feature_page;

    /*--------------------------------------------------*\
    | Resolved function IDs for the RGB effects feature  |
    | Varies between 0x8070, 0x8071, 0x0600              |
    \*--------------------------------------------------*/
    uint8_t         fn_set_effect;
    uint8_t         fn_sw_control;
    uint8_t         fn_pwr_config;
    uint8_t         fn_pwr_mode;
    bool            has_power_mgmt;
    bool            sw_control_simple;

    /*--------------------------------------------------*\
    | Persistent NV settings read from RGBEffects fn3    |
    | (FN_8071_NV_CONFIG). Capability 0x0020 is the      |
    | sleep ramp / off-ramp transition (enabled +        |
    | ramp_seconds).                                     |
    \*--------------------------------------------------*/
    bool            nv_sleep_ramp_known;
    bool            nv_sleep_ramp_enabled;
    uint8_t         nv_sleep_ramp_seconds;

    /*---------------------------------------------------*\
    | Device-firmware effect cards (0x8071 fn0            |
    | GetEffectSpecificInfo). Populated by                |
    | DiscoverEffectCards at feature-discovery time.      |
    |                                                     |
    | has_effect_cards — probe returned a valid response  |
    |   for firmware card 0 page 1 (no InvalidArgument).  |
    | effect_card_template[0..1] — device-wide constant   |
    |   bytes read from that response at data[10..11].    |
    |   Echoed back into prep1 of DoObservedPerKeyPrep    |
    |   at SetEffectByIndex params[6..7].                 |
    |                                                     |
    | Gate for the observed per-key prep is now           |
    | has_effect_cards — replaces the earlier             |
    | "effects.size() < 5" heuristic, which was a proxy   |
    | that correlated with "has cards" on the devices we  |
    | happened to know but had no principled meaning.     |
    \*---------------------------------------------------*/
    bool            has_effect_cards;
    uint8_t         effect_card_template[2];

    /*-------------------------------------------------*\
    | Discovered zone and LED data                      |
    \*-------------------------------------------------*/
    std::vector<HIDPP20ZoneCluster>   zone_clusters;
    std::vector<uint16_t>           perkey_zone_ids;
    std::vector<uint8_t>            headset_rgb_hostmode_zone_ids;  /* 0x0620 fn1 result */
    bool                            has_perkey;
    bool                            has_zone_effects;
    bool                            is_headset_rgb_hostmode;        /* 0x0620 path selected */
    bool                            has_numpad;
    uint8_t                         keyboard_layout_code;
};

/*------------------------------------------------------*\
| Transport type — determines wire framing               |
\*------------------------------------------------------*/
enum HIDPP20TransportType
{
    HIDPP20_TRANSPORT_STANDARD,       // 0xFF00/0xFF43: report IDs 0x10/0x11, 7/20 bytes
    HIDPP20_TRANSPORT_CENTURION       // 0xFFA0: report ID 0x51 or 0x50, 64 bytes,
                                    // with CPL framing and CentPPBridge sub-device routing
};

/*------------------------------------------------------*\
| Transport layer — abstracts wire format differences    |
|                                                        |
| Standard HID++ and Centurion both carry the same       |
| feature/function/data payload, but with different      |
| report framing. This struct holds transport state      |
| so SendMessage/ReadMessage can adapt.                  |
\*------------------------------------------------------*/
struct HIDPP20Transport
{
    HIDPP20TransportType  type;
    uint16_t            usage_page;         // 0xFF00, 0xFF43, or 0xFFA0
    uint8_t             report_id;          // 0x10/0x11 for standard, 0x51/0x50 for Centurion
    bool                addressed;          // Centurion 0x50 has device address byte
    uint8_t             device_address;     // Centurion 0x50: device address (e.g., 0x23)
    uint8_t             bridge_feat_idx;    // CentPPBridge feature index on parent (0 if N/A)
    uint8_t             sub_device_id;      // CentPPBridge sub-device ID (typically 0)
    uint16_t            bridge_mtu;         // CentPPBridge MTU from getConnectionInfo:
                                            //   0 = no sub-device, sendFragment will fail
                                            //   >0 = sub-device present, payload size in bytes
};

/*-----------------------------------------------------*\
| Power management state machine                        |
| Matches Solaar's RGBPowerManager states               |
\*-----------------------------------------------------*/
enum HIDPP20PowerState
{
    HIDPP20_POWER_ACTIVE   = 0,
    HIDPP20_POWER_DIMMING  = 1,
    HIDPP20_POWER_IDLE     = 2,
    HIDPP20_POWER_SLEEPING = 3,
};

/*-----------------------------------------------------*\
| Parsed HID++ message for the response queue           |
\*-----------------------------------------------------*/
struct HIDPP20RawMessage
{
    uint8_t feat;
    uint8_t func;
    uint8_t data[60];
    int     result;
};

/*------------------------------------------------------*\
| Per-key write tracking. SendPerKeyData is fire-and-    |
| forget at the wire layer; we track which zones each    |
| outstanding write covers so PerKeyFrameEnd can match   |
| ACKs back by FIFO order and report which zones the     |
| firmware actually committed.                           |
\*------------------------------------------------------*/
struct OutstandingPerKeyWrite
{
    uint8_t              function;     // FN_8081_* (high nibble carries the type)
    std::vector<uint8_t> zone_ids;     // zones covered by this packet
};

/*-----------------------------------------------------*\
| Result of a per-key frame commit. The caller uses     |
| these to update its delta-tracking state:             |
|   - frame_end_acked: did the FrameEnd packet ACK?     |
|   - acked_zones:    zones whose write packet ACKed    |
|   - attempted_zones: every zone written this frame    |
\*-----------------------------------------------------*/
struct PerKeyFrameResult
{
    bool                 frame_end_acked;
    std::vector<uint8_t> acked_zones;
    std::vector<uint8_t> attempted_zones;
};

/*-------------------------------------------------------*\
| Retry policy for SendAcked.                             |
|                                                         |
| Controls the send/read/retry loop for a single HID++    |
| request. Three canned policies cover all use cases:     |
|                                                         |
|   Reliable: probe/discovery/set/get  (~6s worst case)   |
|   FrameEnd: per-key commit gate      (~230ms worst)     |
|   Streaming: per-key write inside the animation loop    |
|              (~80ms worst)                              |
|                                                         |
| backoff_ms[i] is the delay applied BEFORE the i-th      |
| attempt. backoff_ms[0] is normally 0 (no delay before   |
| the first send). The schedule mirrors the firmware's    |
| own event burst pattern (63→125→250→500→1000→2000ms,    |
| "catch at least one of N").                             |
\*-------------------------------------------------------*/
struct HIDPP20RetryPolicy
{
    const uint16_t* backoff_ms;     // schedule[i] = delay before attempt i
    uint8_t         attempts;       // length of backoff_ms (>=1)
    uint16_t        read_window_ms; // per-attempt read budget
    bool            flush_before;   // flush response queue at call start
    bool            retry_on_busy;  // BUSY (0x08) -> retry the send
    const char*     name;           // for logging ("reliable", etc.)
};

/*------------------------------------------------------*\
| Canned backoff schedules.                              |
\*------------------------------------------------------*/
static constexpr uint16_t HIDPP20_BACKOFF_RELIABLE[] =
    { 0, 63, 125, 250, 500, 1000, 2000 };
static constexpr uint16_t HIDPP20_BACKOFF_PROBE[] =
    { 0, 100 };

/*------------------------------------------------------*\
| SW-control reclaim backoff. Used by ReconnectDevice    |
| to retry the claim+push sequence after a wireless      |
| reconnect, racing the firmware boot animation. The     |
| vendor app typically lands control in ~50ms; this      |
| schedule fits                                          |
| 6 attempts inside ~620ms so the animation never gets   |
| a chance to become visible.                            |
\*------------------------------------------------------*/
static constexpr uint16_t HIDPP20_RECLAIM_BACKOFF_MS[] =
    { 0, 20, 40, 80, 160, 320 };

/*------------------------------------------------------*\
| FrameEnd BUSY retry backoff. Used by PerKeyFrameEnd    |
| when the firmware returns HID++ error 0x08 (BUSY)      |
| because it's still draining the per-key write queue.   |
| First retry is fast (~2 USB round trips) for the       |
| common case where BUSY was transient; subsequent       |
| retries give actual drain headroom. Total worst case   |
| ~180ms, fits inside the PerKeyFrameEnd 300ms deadline  |
| with margin for the eventual ACK to land.              |
\*------------------------------------------------------*/
static constexpr uint16_t HIDPP20_FRAME_END_BUSY_BACKOFF_MS[] =
    { 30, 60, 90 };

/*-----------------------------------------------------*\
| Deep-sleep detection threshold. After StartSleep()    |
| commands the firmware fade, the device eventually     |
| enters deep sleep and returns BUSY to every FrameEnd. |
| Once this many consecutive FrameEnd attempts exhaust  |
| all BUSY retries while power_state == SLEEPING, we    |
| suppress further frame sends until Wake() fires.      |
\*-----------------------------------------------------*/
static constexpr int HIDPP20_DEEP_SLEEP_FAILURE_THRESHOLD = 5;

/*------------------------------------------------------*\
| Per-key frame retry backoff. Used when a full          |
| DeviceUpdateLEDs pass completes with some zones        |
| unacked (partial commit). The retry re-runs a whole    |
| frame from the power thread, so the backoff is         |
| between full frames, not individual packets.           |
|                                                        |
| First value aligned to the power thread's 50ms poll    |
| cadence — anything shorter rounds up anyway, and       |
| matching the tick makes latency predictable.           |
|                                                        |
| Worst case: 5 retries, cumulative ~1550ms. This        |
| covers the reconnect-transient window where the G502   |
| firmware silently drops per-key writes for several     |
| hundred ms after the wireless link re-establishes.     |
\*------------------------------------------------------*/
static constexpr uint16_t HIDPP20_REPAINT_RETRY_BACKOFF_MS[] =
    { 50, 100, 200, 400, 800 };

static constexpr HIDPP20RetryPolicy HIDPP20_POLICY_RELIABLE = {
    HIDPP20_BACKOFF_RELIABLE,
    sizeof(HIDPP20_BACKOFF_RELIABLE) / sizeof(uint16_t),
    300,    // read window
    true,   // flush_before
    true,   // retry_on_busy
    "reliable"
};

/*------------------------------------------------------*\
| Probe policy: tight budget for is-this-HID++ checks    |
| during initial discovery. ~500ms worst case for dead   |
| devices, vs ~6s for reliable. One retry handles a      |
| transient hiccup on the first IRoot call (e.g. on a    |
| busy mouse), but we bail fast on truly non-responsive  |
| or non-HID++ hidraws so probe latency stays bounded.   |
\*------------------------------------------------------*/
static constexpr HIDPP20RetryPolicy HIDPP20_POLICY_PROBE = {
    HIDPP20_BACKOFF_PROBE,
    sizeof(HIDPP20_BACKOFF_PROBE) / sizeof(uint16_t),
    200,    // read window
    true,   // flush_before
    true,   // retry_on_busy
    "probe"
};

class LogitechHIDPP20Controller
{
public:
    LogitechHIDPP20Controller(hid_device* dev, const char* path,
                            uint8_t device_index, bool wireless,
                            std::shared_ptr<std::mutex> mutex_ptr,
                            uint16_t usage_page = 0xFF00);
    ~LogitechHIDPP20Controller();

    /*-------------------------------------------------*\
    | Lifecycle                                         |
    \*-------------------------------------------------*/
    bool            Probe();
    void            Initialize();
    void            Shutdown();

    /*-------------------------------------------------*\
    | Accessors                                         |
    \*-------------------------------------------------*/
    const HIDPP20DeviceCapabilities&  GetCapabilities() const;
    std::string     GetDeviceLocation();
    std::string     GetSerialString();
    uint32_t        GetInitGeneration() const;

    /*--------------------------------------------------*\
    | Per-key lighting (0x8081)                          |
    |                                                    |
    | Per-key writes are fire-and-forget at the wire     |
    | layer. SendPerKeyData enqueues an outstanding      |
    | write entry; PerKeyFrameEnd drains the response    |
    | queue, matches ACKs by FIFO, and returns which     |
    | zones the firmware actually committed plus         |
    | whether the FrameEnd itself ACKed.                 |
    \*--------------------------------------------------*/
    void              SetPerKeyColors(const std::vector<std::pair<uint16_t, RGBColor>>& zone_colors);
    void              SetAllPerKeyColor(RGBColor color);
    void              SendPerKeyData(uint8_t perkey_idx, uint8_t function,
                                     const uint8_t* data, size_t len,
                                     const std::vector<uint8_t>& zone_ids);
    PerKeyFrameResult PerKeyFrameEnd();

    /*-------------------------------------------------*\
    | Zone effects (0x8071 / 0x8070)                    |
    \*-------------------------------------------------*/
    void            SetZoneEffect(uint8_t cluster_idx, uint8_t effect_idx,
                                  uint16_t effect_id,
                                  unsigned char r, unsigned char g, unsigned char b,
                                  uint16_t period, unsigned char brightness,
                                  unsigned char direction, bool persist);

    /*---------------------------------------------------*\
    | Headset RGB hostmode (0x0620).                      |
    | Sticky-claim model: SetHostMode() claims once via   |
    | fn8, then each write is fn5 (single-value) or fn2   |
    | (individual) + fn6 FrameEnd[0x01]. 0x02 persist was |
    | tested and does not work on G522 firmware.          |
    \*---------------------------------------------------*/
    void            SetHeadsetRGBHostmodeColors(const std::vector<RGBColor>& zone_colors);

    /*-------------------------------------------------*\
    | SW control management                             |
    \*-------------------------------------------------*/
    int             SetSWControl(uint8_t mode, uint8_t flags);
    void            SetRGBPowerMode(uint8_t mode);
    void            SetHostMode();
    bool            ClaimSWControlIfNeeded();
    void            UpgradeSwControlAfterFirstPaint();

    /*-------------------------------------------------*\
    | Keyboard-family handshake (0x4522 fn3 + fn1).     |
    | G815 / G915 / G Pro send this before any mode     |
    | write. Feature-gated no-op on devices (G502 /     |
    | G515) that don't enumerate 0x4522.                |
    \*-------------------------------------------------*/
    void            DoDisableKeysByUsageHandshake();

    /*--------------------------------------------------*\
    | Keyboard-family per-key takeover prep.             |
    | Per-cluster SetEffectByIndex(effectIdx=0=Off,      |
    | persist=1) + primer key via SetIndividualRgbZones  |
    | + FrameEnd. Matches G815 / G915 InitializeDirect.  |
    | Gated on 0x4522 + per-key V2 presence.             |
    \*--------------------------------------------------*/
    void            DoKeyboardFamilyPerKeyPrep();

    /*--------------------------------------------------*\
    | Wake-repaint flag. Set by Wake() before calling    |
    | request_repaint_fn so the repaint callback knows   |
    | to invalidate sent_colors (force a full per-key    |
    | push) without triggering the claim/prep sequence.  |
    \*--------------------------------------------------*/
    bool            ConsumeWakeFullRepaint();
    bool            NeedsPrepSequence() const  { return sw_control_needs_upgrade_to_5; }

    /*--------------------------------------------------*\
    | Per-key retry scheduling. Called by the RGB        |
    | controller's DeviceUpdateLEDs on partial-commit    |
    | frames; the power thread polls and fires           |
    | request_repaint_fn when a retry deadline expires.  |
    \*--------------------------------------------------*/
    void            ScheduleRetryPaint();
    void            CancelRetryPaint();
    void            TickRetryPaintIfPending();

    /*--------------------------------------------------*\
    | Observed per-key prep sequence. Two                |
    | SetEffectByIndex calls cloned byte-for-byte from a |
    | wire capture on a G502 X PLUS.                     |
    | Used in place of the Static-pass-through prep when |
    | the device's RGBEffects enumeration matches the    |
    | G502 shape — see DeviceUpdateLEDs for gating.      |
    \*--------------------------------------------------*/
    void            DoObservedPerKeyPrep();

    /*--------------------------------------------------*\
    | Power management (idle/dim/sleep/wake)             |
    \*--------------------------------------------------*/
    void            StartPowerManager();
    void            StopPowerManager();
    void            StartEventWatcher();
    void            StartProbeWatcher();
    bool            HasBridge() const;
    void            SetRepaintCallback(std::function<void()> repaint);
    void            SetReapplyActiveModeCallback(std::function<bool()> cb);
    void            SetRegisterCallback(std::function<void(RGBController*)> cb);
    HIDPP20PowerState GetPowerState() const;
    int             GetDimBrightness() const;
    bool            IsOnline() const;
    bool            IsDeepSleep() const;
    void            SetWireless(bool w)     { wireless = w; }
    bool            QueryWirelessStatus();
    bool            QueryExternalPower();
    void            FlushResponseQueue();

private:
    hid_device*                 dev;
    std::string                 location;
    uint8_t                     device_index;
    bool                        wireless;
    std::shared_ptr<std::mutex> mutex;
    HIDPP20DeviceCapabilities     caps;
    HIDPP20Transport              transport;
    bool                        initialized;
    bool                        sw_control_claimed;
    bool                        sw_control_needs_upgrade_to_5;
    uint32_t                    frame_counter;

    /*--------------------------------------------------*\
    | Retry-paint state (partial-commit recovery).       |
    | retry_paint_deadline_ zero = no retry pending.     |
    | retry_paint_attempt_ indexes into                  |
    | HIDPP20_REPAINT_RETRY_BACKOFF_MS; once it reaches  |
    | the array length, we give up for this sequence.    |
    | Atomic so both the paint thread (RGB controller)   |
    | and the power thread can access without locks.     |
    \*--------------------------------------------------*/
    std::atomic<std::chrono::steady_clock::time_point>  retry_paint_deadline_;
    std::atomic<uint8_t>                                retry_paint_attempt_;
    std::atomic<bool>                                   wake_full_repaint_pending_;
    uint32_t                    init_generation;
    std::string                 log_tag;

    /*---------------------------------------------------*\
    | Transport-layer I/O                                 |
    |                                                     |
    | SendMessage/ReadMessage handle wire framing based   |
    | on transport.type. Upper layers pass feature index, |
    | function ID, and payload — the transport layer      |
    | wraps them in the correct report format.            |
    \*---------------------------------------------------*/
    int             SendMessage(uint8_t feat_idx, uint8_t function,
                                const uint8_t* data, size_t len);
    int             ReadMessage(uint8_t* feat_idx_out, uint8_t* function_out,
                                uint8_t* data_out, size_t data_max,
                                int timeout_ms = LOGITECH_PROTOCOL_TIMEOUT);

    /*--------------------------------------------------*\
    | Standard HID++ transport (0xFF00/0xFF43)           |
    \*--------------------------------------------------*/
    int             SendStandard(uint8_t feat_idx, uint8_t function,
                                 const uint8_t* data, size_t len);
    int             ReadStandardDirect(uint8_t* feat_idx_out, uint8_t* function_out,
                                       uint8_t* data_out, size_t data_max,
                                       int timeout_ms);

    /*--------------------------------------------------*\
    | Centurion transport (0xFFA0)                       |
    | Wraps messages in CPL framing, routes through      |
    | CentPPBridge for sub-device access.                |
    \*--------------------------------------------------*/
    int             SendCenturion(uint8_t feat_idx, uint8_t function,
                                  const uint8_t* data, size_t len);
    int             ReadCenturionDirect(uint8_t* feat_idx_out, uint8_t* function_out,
                                        uint8_t* data_out, size_t data_max,
                                        int timeout_ms);

    /*--------------------------------------------------*\
    | Reader thread dispatch layer                       |
    | ReadHIDDirect: raw HID read (used by reader thread |
    |   and during Probe before reader starts).          |
    | ReadFromQueue: waits on response queue filled by   |
    |   the reader thread.                               |
    \*--------------------------------------------------*/
    int             ReadHIDDirect(uint8_t* feat_idx_out, uint8_t* function_out,
                                  uint8_t* data_out, size_t data_max,
                                  int timeout_ms);
    int             ReadFromQueue(uint8_t* feat_idx_out, uint8_t* function_out,
                                  uint8_t* data_out, size_t data_max,
                                  int timeout_ms);

    /*---------------------------------------------------*\
    | High-level helpers                                  |
    | SendAndReceive is retained as a thin wrapper        |
    | around SendAcked with the reliable policy, for      |
    | call-site stability.                                |
    \*---------------------------------------------------*/
    int             SendAndReceive(uint8_t feat_idx, uint8_t function,
                                   const uint8_t* send_data, size_t send_len,
                                   uint8_t* recv_data, size_t recv_max);

    /*---------------------------------------------------*\
    | Unified send-and-ack primitive with retry policy.   |
    | All command paths converge here. Returns:           |
    |   >0 : bytes copied into recv_data                  |
    |    0 : timeout / BUSY exhaustion                    |
    |   -1 : non-BUSY HID++ error                         |
    |   -2 : wire error (SendMessage failed)              |
    | If hidpp20_error_out is non-null and return is -1,  |
    | the HID++ error code is stored there.               |
    \*---------------------------------------------------*/
    int             SendAcked(uint8_t feat_idx, uint8_t function,
                              const uint8_t* send_data, size_t send_len,
                              uint8_t* recv_data, size_t recv_max,
                              const HIDPP20RetryPolicy& policy = HIDPP20_POLICY_RELIABLE,
                              uint8_t* hidpp20_error_out = nullptr);

    /*--------------------------------------------------*\
    | Compatibility shim: same as SendAcked but writes   |
    | the response into a blankFAPmessage. Used by       |
    | callers that inherited the SendLong+ReadResponse   |
    | interface and inspect response.data[] downstream.  |
    \*--------------------------------------------------*/
    int             SendAckedIntoFAP(uint8_t feat_idx, uint8_t function,
                                     const uint8_t* send_data, size_t send_len,
                                     blankFAPmessage& response,
                                     const HIDPP20RetryPolicy& policy = HIDPP20_POLICY_RELIABLE);

    /*-------------------------------------------------*\
    | Feature discovery                                 |
    \*-------------------------------------------------*/
    uint8_t         GetFeatureIndex(uint16_t feature_page,
                                    const HIDPP20RetryPolicy& policy = HIDPP20_POLICY_RELIABLE);
    uint8_t         GetFeatureVersion(uint16_t feature_page) const;
    void            DiscoverTransport();
    void            DiscoverDeviceName();
    void            DiscoverDeviceType();
    void            EnumerateFeatures(uint8_t feature_set_idx);
    void            DiscoverFirmwareInfo();
    void            DiscoverRGBEffects();
    void            DiscoverEffectCards();
    void            DiscoverHeadsetRGBHostmode();
    void            DiscoverPerKeyZones();
    void            DiscoverKeyboardLayout();

    /*-------------------------------------------------*\
    | Power management internals                        |
    \*-------------------------------------------------*/
    void            ReaderThreadFunc();
    void            PowerThreadFunc();
    void            DispatchEvent(uint8_t feat, uint8_t func, const uint8_t* data);
    void            OnUserActivity(uint8_t activity_type);
    void            StartDimRamp();
    void            DimRampStep();
    void            StartSleep();
    void            Wake();
    void            ReadFirmwareTimers();
    void            ReadNvSleepRampConfig();
    void            WritePowerConfig(uint16_t idle_s, uint16_t sleep_s);
    void            ReadActiveProfileSector();
    void            ReprobeSubDevice();
    void            ReconnectDevice();
    void            FullReprobe();
    void            RediscoverFeatures();
    /*----------------------------------------------------*\
    | Platform-specific. ScanForDevice walks the OS-level  |
    | HID enumeration to find the same physical device on  |
    | a new path (USB<->wireless transitions). Linux uses  |
    | sysfs; Windows and macOS use hidapi + serial_number  |
    | matching. Bodies live in                             |
    | LogitechHIDPP20Controller_Linux.cpp and              |
    | LogitechHIDPP20Controller_Windows_MacOS.cpp.         |
    \*----------------------------------------------------*/
    bool            ScanForDevice(bool force = false);

    /*----------------------------------------------------*\
    | Platform-specific. Returns the friendly name for a   |
    | Centurion sub-device at the given hidapi path, or "" |
    | if no name is available. Linux reads HID_NAME from   |
    | sysfs; Windows uses hid_device_info::product_string. |
    \*----------------------------------------------------*/
    std::string     GetCenturionSubDeviceName(const std::string& path);

    void            SwapHIDHandle(hid_device* new_dev, const std::string& new_path);

    /*-------------------------------------------------*\
    | Reader thread + response queue                    |
    \*-------------------------------------------------*/
    std::thread*            reader_thread;
    std::atomic<bool>       reader_running;
    std::mutex              response_mutex;
    std::condition_variable response_cv;
    std::deque<HIDPP20RawMessage> response_queue;

    /*-------------------------------------------------*\
    | Power thread (state machine + command sender)     |
    \*-------------------------------------------------*/
    std::thread*            power_thread;
    std::atomic<bool>       power_thread_running;
    std::atomic<int>        pending_activity;       // -1=none, 0=idle, 1+=active
    std::atomic<int>        pending_connection;     // 0=none, +1=connected, -1=disconnected
    std::atomic<int>        pending_path_check;     // HID++1.0 DJ notification → force-scan retries remaining (0=idle)
    std::atomic<bool>       device_online;          // false when device is unreachable
    std::atomic<int>        consecutive_timeouts;   // reset on successful response
    std::atomic<bool>       watcher_mode;           // true when retrying failed probe

    /*-------------------------------------------------*\
    | Power management state                            |
    \*-------------------------------------------------*/
    HIDPP20PowerState         power_state;
    std::mutex              power_mutex;
    std::atomic<bool>       deep_sleep;             // true once device stops responding after StartSleep()
    std::atomic<int>        consecutive_frame_end_failures; // FrameEnd BUSY exhaustions while SLEEPING

    /*-------------------------------------------------*\
    | Dim ramp state                                    |
    | dim_brightness_pct is applied by DeviceUpdateLEDs |
    | to scale colors before pushing to device.         |
    | This is our own host-side animation, independent  |
    | of any firmware dim/sleep timers.                 |
    \*-------------------------------------------------*/
    static const int DIM_STEPS       = 25;
    static const int DIM_INTERVAL_MS = 200;
    static const int DIM_TARGET_PCT  = 50;
    std::atomic<int> dim_brightness_pct;    // 100=full, 50=dimmed
    int              dim_step;
    std::chrono::steady_clock::time_point next_dim_time;

    /*-------------------------------------------------*\
    | Sleep timer                                       |
    \*-------------------------------------------------*/
    std::chrono::steady_clock::time_point sleep_deadline;

    /*--------------------------------------------------*\
    | Last idle-settings re-read timestamp. Drives the   |
    | 500ms poll in PowerThreadFunc that re-reads the    |
    | LogitechHIDPP20IdleSettings JSON key so updates    |
    | from the plugin (or manual edits) apply within     |
    | about half a second without any callback plumbing. |
    \*--------------------------------------------------*/
    std::chrono::steady_clock::time_point last_idle_poll;

    /*-------------------------------------------------*\
    | Effective idle/sleep timers used by the state     |
    | machine. Populated from the firmware snapshot by  |
    | default, then possibly overridden by profile      |
    | values in ApplyPowerSavingProfile.                |
    \*-------------------------------------------------*/
    uint16_t        idle_timeout_s;
    uint16_t        sleep_timeout_s;

    /*--------------------------------------------------*\
    | Firmware-configured timer snapshot. Read at init   |
    | (and on reconnect) by ReadFirmwareTimers and       |
    | never overwritten by profile application, so       |
    | that the unconfigured fallback path and transition |
    | back from a user profile both have a clean set     |
    | of defaults to return to.                          |
    \*--------------------------------------------------*/
    uint16_t        fw_idle_timeout_s   = 60;
    uint16_t        fw_sleep_timeout_s  = 300;
    uint16_t        written_idle_s      = 0;    // last value written to device RAM (0 = not written yet)
    uint16_t        written_sleep_s     = 0;

    /*--------------------------------------------------*\
    | Host-side idle/dim/sleep state.                    |
    | Populated from LogitechHIDPP20IdleSettings on each |
    | ApplyPowerSavingProfile() invocation.              |
    \*--------------------------------------------------*/
    bool            ps_dim_enabled          = false;
    int             ps_dim_target_pct       = DIM_TARGET_PCT;
    bool            ps_sleep_enabled        = false;
    bool            ps_on_external_power    = false;
    bool            ps_last_logged_external = false;
    int             ps_last_logged_pct      = -1;
    int             ps_last_logged_idle     = -1;
    int             ps_last_logged_sleep    = -1;
    uint16_t        last_power_raw          = 0xFFFF;  // dedup for QueryExternalPower TRACE
    uint8_t         idx_unified_battery     = 0;

    void            ApplyPowerSavingProfile();
    bool            IsCurrentlyWireless() const;

    /*-------------------------------------------------*\
    | Per-key write tracking (per active frame)         |
    | Populated by SendPerKeyData, drained by           |
    | PerKeyFrameEnd. Single-threaded — only the RGB    |
    | controller thread touches per-key state.          |
    \*-------------------------------------------------*/
    std::vector<OutstandingPerKeyWrite> outstanding_writes;

    /*-------------------------------------------------*\
    | Callbacks                                         |
    \*-------------------------------------------------*/
    std::function<void()> request_repaint_fn;
    std::function<bool()> reapply_active_mode_fn;
    std::function<void(RGBController*)> register_controller_fn;
};
