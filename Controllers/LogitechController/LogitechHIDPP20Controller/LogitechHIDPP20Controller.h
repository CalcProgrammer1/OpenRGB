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

/*---------------------------------------------------------*\
| HID++ 2.0 Feature Page IDs                                |
\*---------------------------------------------------------*/
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

/*---------------------------------------------------------*\
| Placeholder device names. Centralized so the set          |
| sites and the is-placeholder test can't drift.            |
\*---------------------------------------------------------*/
#define HIDPP20_NAME_PLACEHOLDER_STD          "Logitech HID++ Device"
#define HIDPP20_NAME_PLACEHOLDER_CENTURION    "Logitech Centurion Device"

/*---------------------------------------------------------*\
| HID++ 2.0 Function IDs (byte 3 high nibble)               |
| Function ID is shifted left 4 bits, low nibble = swID     |
|                                                           |
| HID++ Software ID: identifies our responses.              |
| Must avoid: 0x00 (firmware), 0x01 (vendor app),           |
| 0x02-0x0F (Solaar cycles these).                          |
| There are only 16 values (4-bit field), and all are       |
| claimed. We pick a fixed value and will coordinate        |
| with Solaar to exclude it from its cycle.                 |
\*---------------------------------------------------------*/
#define HIDPP20_SW_ID                         0x07

/*---------------------------------------------------------*\
| Feature 0x8071 functions                                  |
\*---------------------------------------------------------*/
#define FN_8071_GET_INFO                    0x00
#define FN_8071_SET_EFFECT                  0x10
#define FN_8071_SET_PATTERN                 0x20
#define FN_8071_NV_CONFIG                   0x30
#define FN_8071_BIN_INFO                    0x40
#define FN_8071_SW_CONTROL                  0x50
#define FN_8071_SYNC                        0x60
#define FN_8071_PWR_CONFIG                  0x70
#define FN_8071_PWR_MODE                    0x80

/*---------------------------------------------------------*\
| Feature 0x8081 functions                                  |
\*---------------------------------------------------------*/
#define FN_8081_GET_INFO                    0x00
#define FN_8081_SET_INDIVIDUAL              0x10
#define FN_8081_SET_CONSECUTIVE             0x20
#define FN_8081_SET_DELTA_5BIT              0x30
#define FN_8081_SET_DELTA_4BIT              0x40
#define FN_8081_SET_RANGE                   0x50
#define FN_8081_SET_SINGLE_VALUE            0x60
#define FN_8081_FRAME_END                   0x70

/*---------------------------------------------------------*\
| Feature 0x8080 functions. A separate feature from 0x8081  |
| ("V2" is Logitech's feature name, not a version). Keys    |
| are (keyType u16 BE, keyId u8 = USB HID usage); fn3 rides |
| the 0x12 very-long report on a second handle, everything  |
| else rides 0x11.                                          |
\*---------------------------------------------------------*/
#define FN_8080_GET_INFO                    0x00
#define FN_8080_GET_KEY_TYPE_INFO           0x10
#define FN_8080_GET_KEY_COLORS              0x20
#define FN_8080_SET_KEY_COLORS              0x30
#define FN_8080_SET_ALL_KEYS                0x40
#define FN_8080_FLUSH_LEDS                  0x50

/*---------------------------------------------------------*\
| 0x8080 carries up to 14 (keyId,R,G,B) tuples in one       |
| very-long (report 0x12) frame, and GetKeyColors           |
| paginates its readback by the same count.                 |
\*---------------------------------------------------------*/
#define HIDPP20_8080_KEYS_PER_FRAME         14

/*---------------------------------------------------------*\
| Feature 0x0001 functions (FeatureSet)                     |
\*---------------------------------------------------------*/
#define FN_0001_GET_COUNT                   0x00
#define FN_0001_GET_FEATURE_ID              0x10

/*---------------------------------------------------------*\
| Feature 0x0620 functions (headset RGB hostmode)           |
\*---------------------------------------------------------*/
#define FN_0620_GET_INFO                    0x00
#define FN_0620_GET_RGB_ZONE_INFO           0x10
#define FN_0620_SET_INDIVIDUAL_RGB_ZONES    0x20
#define FN_0620_SET_CONSECUTIVE_RGB_ZONES   0x30
#define FN_0620_SET_RANGE_RGB_ZONES         0x40
#define FN_0620_SET_RGB_ZONES_SINGLE_VALUE  0x50
#define FN_0620_FRAME_END                   0x60
#define FN_0620_GET_HOST_MODE_STATE         0x70
#define FN_0620_SET_HOST_MODE_STATE         0x80

/*---------------------------------------------------------*\
| Feature 0x8100 functions                                  |
\*---------------------------------------------------------*/
#define FN_8100_SET_ONBOARD_MODE            0x10
#define FN_8100_GET_ONBOARD_MODE            0x20

/*---------------------------------------------------------*\
| Feature 0x8101 functions                                  |
\*---------------------------------------------------------*/
#define FN_8101_GET_SET_MODE                0x60
#define FN_8101_LOAD                        0x80    // load(partition, sector, size)
#define FN_8101_READBUFFER                  0xC0    // readBuffer(offset)

/*---------------------------------------------------------*\
| Zone cluster effect entry                                 |
\*---------------------------------------------------------*/
struct HIDPP20Effect
{
    uint8_t     index;
    uint16_t    effect_id;
    uint16_t    capabilities;
    uint16_t    default_period;
};

/*---------------------------------------------------------*\
| Zone cluster info from 0x8071 GetRgbClusterInfo           |
\*---------------------------------------------------------*/
struct HIDPP20ZoneCluster
{
    uint8_t                     index;
    uint16_t                    location;
    uint8_t                     effect_count;
    std::vector<HIDPP20Effect>    effects;
};

/*---------------------------------------------------------*\
| One confirmed 0x8080 keyType and its enumerated keyIds.   |
| keyTypes are single-bit values, fn0 typeFlags is their    |
| bitmask: KEYBOARD 0x01, CONSUMER 0x02, GKEYS 0x04,        |
| BUTTONS 0x08, LOGO 0x10, OTHER/indicators 0x40. keyIds    |
| are HID usages on KEYBOARD, vendor-local elsewhere.       |
| Populated by DiscoverPerKey8080().                        |
\*---------------------------------------------------------*/
struct HIDPP20PerKey8080Type
{
    uint16_t                    key_type;
    uint8_t                     param[3];     /* fn1 GetKeyTypeInfo bytes 2-4 (after keyCount) */
    std::vector<uint8_t>          key_ids;      /* fn2 GetKeyColors, keyId != 0 */
};

/*---------------------------------------------------------*\
| Per-model device quirks, behavioral differences that      |
| can't be detected via feature probing.                    |
\*---------------------------------------------------------*/
enum HIDPP20DeviceQuirks : uint32_t
{
    HIDPP20_QUIRK_NONE                = 0,
    HIDPP20_QUIRK_FADE_ACCEPTS_WRITES = (1 << 0),  // firmware accepts host frames during sleep fade without waking
    HIDPP20_QUIRK_8080_OVERREPORTS_MEDIA = (1 << 2),  // 0x8080 typeFlags over-reports media bit 0x02 (G910)
    HIDPP20_QUIRK_8080_KEYTYPE40_ID12_ONLY = (1 << 3),  // 0x8080 keyType 0x40 accepts only keyId 1 and 2 (G410)
    HIDPP20_QUIRK_KEEP_ONBOARD_MODE   = (1 << 4),  // host mode breaks the onboard-profile-mapped F-row (G915 TKL)
};

struct HIDPP20DeviceQuirkEntry
{
    uint16_t    pid_wireless;
    uint16_t    pid_wired;
    uint32_t    quirks;
};

/*---------------------------------------------------------*\
| Default: suppress frames while SLEEPING. Safe everywhere; |
| it cannot wake a device that treats writes as activity.   |
| Devices listed here opt out of suppression because their  |
| firmware accepts writes during the fade without           |
| cancelling sleep.                                         |
\*---------------------------------------------------------*/
static constexpr HIDPP20DeviceQuirkEntry HIDPP20_DEVICE_QUIRK_TABLE[] =
{
    { 0x40B4, 0xC355, HIDPP20_QUIRK_FADE_ACCEPTS_WRITES },  // G515 LS TKL
    { 0xC32B, 0xC32B, HIDPP20_QUIRK_8080_OVERREPORTS_MEDIA },  // G910 Orion Spark
    { 0xC330, 0xC330, HIDPP20_QUIRK_8080_KEYTYPE40_ID12_ONLY },  // G410 Atlas Spectrum
    { 0x408E, 0xC343, HIDPP20_QUIRK_KEEP_ONBOARD_MODE },  // G915 TKL
};

/*---------------------------------------------------------*\
| One write fired during a pipelined claim, kept so its     |
| ack can be reconciled and the write re-sent if dropped.   |
\*---------------------------------------------------------*/
struct HIDPP20PendingClaimCmd
{
    uint8_t     feat;
    uint8_t     func;
    uint8_t     data[20];
    size_t      len;
};

/*---------------------------------------------------------*\
| Device capabilities discovered via feature probing        |
\*---------------------------------------------------------*/
struct HIDPP20DeviceCapabilities
{
    std::string     device_name;
    uint8_t         device_type;
    std::string     firmware_version;
    std::string     serial_number;
    std::string     unit_id;            // stable hardware ID (from FirmwareInfo fn0)
    uint16_t        pid_wireless;       // wireless virtual PID (from FirmwareInfo fn0)
    uint16_t        pid_wired;          // wired USB PID (from FirmwareInfo fn0)
    uint16_t        pid_third;          // third modelId slot, a USB-only device's PID lands here
    uint32_t        quirks;             // resolved from HIDPP20_DEVICE_QUIRK_TABLE after PID discovery

    /*-----------------------------------------------------*\
    | Complete feature map (feature_id -> runtime index)    |
    | Built once by EnumerateFeatures, used by all          |
    | subsequent GetFeatureIndex lookups (no wire).         |
    \*-----------------------------------------------------*/
    std::map<uint16_t, uint8_t> feature_map;
    std::map<uint16_t, uint8_t> feature_versions;  /* feature_id -> version byte */
    bool        feature_map_complete;   // true after bulk enumeration

    /*-----------------------------------------------------*\
    | Feature indices (0 = not supported)                   |
    \*-----------------------------------------------------*/
    uint8_t         idx_onboard_profiles;
    uint8_t         idx_profile_management;
    uint8_t         idx_rgb_effects;
    uint8_t         idx_headset_rgb_hostmode;   /* 0x0620: Centurion headset RGB */
    uint8_t         idx_perkey_v2;
    uint8_t         idx_perkey_v1;
    uint8_t         idx_perkey_8080;            /* 0x8080: Per Key Lighting (G810/G910 keyboard family) */
    uint8_t         idx_wireless_status;
    uint8_t         idx_disable_keys_by_usage;  /* 0x4522: keyboard-family handshake */
    uint16_t        rgb_feature_page;

    /*-----------------------------------------------------*\
    | Resolved function IDs for the RGB effects feature     |
    | Varies between 0x8070, 0x8071, 0x0600                 |
    \*-----------------------------------------------------*/
    uint8_t         fn_set_effect;
    uint8_t         fn_sw_control;
    uint8_t         fn_pwr_config;
    uint8_t         fn_pwr_mode;
    bool            has_power_mgmt;
    bool            sw_control_simple;

    /*-----------------------------------------------------*\
    | Persistent NV settings read from RGBEffects fn3       |
    | (FN_8071_NV_CONFIG). Capability 0x0020 is the         |
    | sleep ramp / off-ramp transition (enabled +           |
    | ramp_seconds).                                        |
    \*-----------------------------------------------------*/
    bool            nv_sleep_ramp_known;
    bool            nv_sleep_ramp_enabled;
    uint8_t         nv_sleep_ramp_seconds;

    /*-----------------------------------------------------*\
    | Device-firmware effect cards (0x8071 fn0              |
    | GetEffectSpecificInfo). Populated by                  |
    | DiscoverEffectCards at feature-discovery time.        |
    |                                                       |
    | has_effect_cards, probe returned a valid response     |
    | for firmware card 0 page 1 (no InvalidArgument).      |
    | effect_card_template[0..1], device-wide constant      |
    | bytes read from that response at data[10..11].        |
    | Echoed back into prep1 of DoObservedPerKeyPrep        |
    | at SetEffectByIndex params[6..7].                     |
    |                                                       |
    | Gate for the observed per-key prep is now             |
    | has_effect_cards, replaces the earlier                |
    | "effects.size() < 5" heuristic, which was a proxy     |
    | that correlated with "has cards" on the devices we    |
    | happened to know but had no principled meaning.       |
    \*-----------------------------------------------------*/
    bool            has_effect_cards;
    uint8_t         effect_card_template[2];

    /*-----------------------------------------------------*\
    | Discovered zone and LED data                          |
    \*-----------------------------------------------------*/
    std::vector<HIDPP20ZoneCluster>   zone_clusters;
    std::vector<uint16_t>           perkey_zone_ids;
    std::vector<uint8_t>            headset_rgb_hostmode_zone_ids;  /* 0x0620 fn1 result */
    bool                            has_perkey;
    bool                            has_zone_effects;

    /*-----------------------------------------------------*\
    | Feature 0x8080 (Per Key Lighting) enumeration.        |
    | Populated by DiscoverPerKey8080() entirely from       |
    | runtime probing (fn0 GetInfo + fn1/fn2 sweep).        |
    \*-----------------------------------------------------*/
    bool                            has_perkey_8080;
    uint16_t                        perkey_8080_type_flags;
    uint16_t                        perkey_8080_type_count;     /* fn0 keyTypeCount: completeness oracle */
    uint16_t                        perkey_8080_max_key_count;
    std::vector<HIDPP20PerKey8080Type> perkey_8080_types;
    bool                            is_headset_rgb_hostmode;        /* 0x0620 path selected */
    bool                            has_numpad;
    uint8_t                         keyboard_layout_code;
};

/*---------------------------------------------------------*\
| Full snapshot of one link's discovered caps. Feature      |
| indices are per transport, and the effect-card template   |
| and zone/per-key data are cleared per link too, so we     |
| cache all of caps to restore a link without rediscovery.  |
\*---------------------------------------------------------*/
struct HIDPP20LinkIndexMap
{
    HIDPP20DeviceCapabilities   caps;
    uint8_t                     idx_unified_battery = 0;   /* lives outside caps; also per-link */
    bool                        valid               = false;
};

/*---------------------------------------------------------*\
| Transport type, determines wire framing                   |
\*---------------------------------------------------------*/
enum HIDPP20TransportType
{
    HIDPP20_TRANSPORT_STANDARD,       // 0xFF00/0xFF43: report IDs 0x10/0x11, 7/20 bytes
    HIDPP20_TRANSPORT_CENTURION       // 0xFFA0: report ID 0x51 or 0x50, 64 bytes,
                                    // with CPL framing and CentPPBridge sub-device routing
};

/*---------------------------------------------------------*\
| Transport layer, abstracts wire format differences        |
|                                                           |
| Standard HID++ and Centurion both carry the same          |
| feature/function/data payload, but with different         |
| report framing. This struct holds transport state         |
| so SendMessage/ReadMessage can adapt.                     |
\*---------------------------------------------------------*/
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

/*---------------------------------------------------------*\
| Power management state machine                            |
| Matches Solaar's RGBPowerManager states                   |
\*---------------------------------------------------------*/
enum HIDPP20PowerState
{
    HIDPP20_POWER_ACTIVE   = 0,
    HIDPP20_POWER_DIMMING  = 1,
    HIDPP20_POWER_IDLE     = 2,
    HIDPP20_POWER_SLEEPING = 3,
};

/*---------------------------------------------------------*\
| Parsed HID++ message for the response queue               |
\*---------------------------------------------------------*/
struct HIDPP20RawMessage
{
    uint8_t feat;
    uint8_t func;
    uint8_t data[60];
    int     result;
};

/*---------------------------------------------------------*\
| One pipelined write awaiting its ACK. The ACK names no    |
| packet, but its payload echoes the request (fn1: zone     |
| IDs positionally, 0xFF = rejected), so the full request   |
| is kept for the echo compare.                             |
\*---------------------------------------------------------*/
struct PerKeyPacket
{
    uint8_t              function;      // FN_8081_* (high nibble carries the type)
    uint8_t              payload[16];   // request bytes, zero-padded
    size_t               len;           // meaningful payload length
    std::vector<uint8_t> zone_ids;      // zones covered (fn1: in payload entry order)
    bool                 acked;
};

/*---------------------------------------------------------*\
| Result of a frame commit. ACKs are attributed to packets  |
| by payload echo, falling back to per-function count       |
| matching (that group commits all-or-nothing).             |
|  frame_end_acked: did the FrameEnd packet ACK             |
|  acked_zones:     zones proven committed                  |
|  attempted_zones: every zone written this frame           |
\*---------------------------------------------------------*/
struct PerKeyFrameResult
{
    bool                 frame_end_acked;
    std::vector<uint8_t> acked_zones;
    std::vector<uint8_t> attempted_zones;
};

/*---------------------------------------------------------*\
| Retry policy for one SendAcked request. Two canned        |
| policies: Reliable (probe/discovery/set/get, ~6s worst)   |
| and Probe (is-this-HID++ checks, ~500ms). Per-key frames  |
| do not come through here. backoff_ms[i] is the delay      |
| BEFORE attempt i (first = 0); the schedule mirrors the    |
| firmware's own burst pattern (63->2000ms).                |
\*---------------------------------------------------------*/
struct HIDPP20RetryPolicy
{
    const uint16_t* backoff_ms;     // schedule[i] = delay before attempt i
    uint8_t         attempts;       // length of backoff_ms (>=1)
    uint16_t        read_window_ms; // per-attempt read budget
    bool            flush_before;   // flush response queue at call start
    bool            retry_on_busy;  // BUSY (0x08) -> retry the send
    const char*     name;           // for logging ("reliable", etc.)
};

/*---------------------------------------------------------*\
| Canned backoff schedules.                                 |
\*---------------------------------------------------------*/
static constexpr uint16_t HIDPP20_BACKOFF_RELIABLE[] =
    { 0, 63, 125, 250, 500, 1000, 2000 };
static constexpr uint16_t HIDPP20_BACKOFF_PROBE[] =
    { 0, 100 };
static constexpr uint16_t HIDPP20_BACKOFF_FIRST_CONTACT[] =
    { 0, 100, 250, 500 };

/*---------------------------------------------------------*\
| SW-control reclaim backoff. Used by ReconnectDevice       |
| to retry the claim+push sequence after a wireless         |
| reconnect, racing the firmware boot animation. The        |
| vendor app typically lands control in ~50ms; this         |
| schedule fits                                             |
| 6 attempts inside ~620ms so the animation never gets      |
| a chance to become visible.                               |
\*---------------------------------------------------------*/
static constexpr uint16_t HIDPP20_RECLAIM_BACKOFF_MS[] =
    { 0, 20, 40, 80, 160, 320 };

/*---------------------------------------------------------*\
| FrameEnd BUSY (0x08) is backpressure, not failure: the    |
| not-ready window scales with keys written. Poll until     |
| ready; the wait is the frame cadence, and coalescing      |
| means the next frame sent is the newest one.              |
| NEAR: after a predicted sleep any BUSY means ready is     |
| imminent, poll tight.                                     |
| Probe: every PROBE_INTERVAL frames sleep PROBE_STEP_MS    |
| short to test whether the device got faster; a clean      |
| accept adopts the shorter window.                         |
\*---------------------------------------------------------*/
static constexpr uint16_t HIDPP20_FRAME_END_BUSY_POLL_MS      = 10;
static constexpr uint16_t HIDPP20_FRAME_END_BUSY_POLL_NEAR_MS = 3;
static constexpr uint16_t HIDPP20_FE_PROBE_STEP_MS            = 5;
static constexpr uint8_t  HIDPP20_FE_PROBE_INTERVAL           = 32;

/*---------------------------------------------------------*\
| Per-key write flow control: past its limits the firmware  |
| silently drops responses (G515 USB: ~20 writes then the   |
| tail drops; the radio's ~3ms/packet pacing hides it       |
| there). Cap unacked in-flight packets and let the ACK     |
| stream pace the sends.                                    |
| Adaptive: an unanswered packet halves the window (floor   |
| MIN); loss-free frames grow it back (cap MAX).            |
| WAIT: full-window stall this long = link died, abort the  |
| frame. TAIL: end-of-frame quiet guard: a dropped ACK is   |
| never coming.                                             |
\*---------------------------------------------------------*/
static constexpr size_t   HIDPP20_PERKEY_WINDOW_MAX  = 8;
static constexpr size_t   HIDPP20_PERKEY_WINDOW_MIN  = 2;
static constexpr uint16_t HIDPP20_PERKEY_ACK_WAIT_MS = 60;
static constexpr uint16_t HIDPP20_PERKEY_ACK_TAIL_MS = 25;
static constexpr uint8_t  HIDPP20_PERKEY_WINDOW_GROW_AFTER = 16;

/*---------------------------------------------------------*\
| Deep-sleep detection threshold. After StartSleep()        |
| commands the firmware fade, the device eventually         |
| enters deep sleep and returns BUSY to every FrameEnd.     |
| Once this many consecutive FrameEnd attempts exhaust      |
| all BUSY retries while power_state == SLEEPING, we        |
| suppress further frame sends until Wake() fires.          |
\*---------------------------------------------------------*/
static constexpr int HIDPP20_DEEP_SLEEP_FAILURE_THRESHOLD = 5;

/*---------------------------------------------------------*\
| Per-key frame retry backoff. Used when a full             |
| DeviceUpdateLEDs pass completes with some zones           |
| unacked (partial commit). The retry re-runs a whole       |
| frame from the power thread, so the backoff is            |
| between full frames, not individual packets.              |
|                                                           |
| First value aligned to the power thread's 50ms poll       |
| cadence; anything shorter rounds up anyway, and           |
| matching the tick makes latency predictable.              |
|                                                           |
| Worst case: 5 retries, cumulative ~1550ms. This           |
| covers the reconnect-transient window where the G502      |
| firmware silently drops per-key writes for several        |
| hundred ms after the wireless link re-establishes.        |
\*---------------------------------------------------------*/
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

/*---------------------------------------------------------*\
| Probe policy: tight budget for is-this-HID++ checks       |
| during initial discovery. ~500ms worst case for dead      |
| devices, vs ~6s for reliable. One retry handles a         |
| transient hiccup on the first IRoot call (e.g. on a       |
| busy mouse), but we bail fast on truly non-responsive     |
| or non-HID++ hidraws so probe latency stays bounded.      |
\*---------------------------------------------------------*/

static constexpr HIDPP20RetryPolicy HIDPP20_POLICY_PROBE = {
    HIDPP20_BACKOFF_PROBE,
    sizeof(HIDPP20_BACKOFF_PROBE) / sizeof(uint16_t),
    200,    // read window
    true,   // flush_before
    true,   // retry_on_busy
    "probe"
};

/*---------------------------------------------------------*\
| First-contact policy: probe budget for a device named by  |
| a receiver's pairing table. A radio leaving power save    |
| can miss the first frames; worst case ~1.2s.              |
\*---------------------------------------------------------*/
static constexpr HIDPP20RetryPolicy HIDPP20_POLICY_FIRST_CONTACT = {
    HIDPP20_BACKOFF_FIRST_CONTACT,
    sizeof(HIDPP20_BACKOFF_FIRST_CONTACT) / sizeof(uint16_t),
    300,    // read window
    true,   // flush_before
    true,   // retry_on_busy
    "first-contact"
};

/*---------------------------------------------------------*\
| Rolling resync.  The per-key stream is a delta and takes  |
| an ACK as proof of paint, so a write the device answers   |
| but does not apply leaves that key wrong until its colour |
| changes again - which for a key holding a steady colour   |
| never happens.  Every frame that is not a full update     |
| also repaints the least recently sent keys, so any        |
| divergence heals within a bounded number of frames        |
| whatever caused it.  A quiet frame carries more of them;  |
| a busy frame still carries one, so a busy side of the     |
| board cannot starve an idle one.                          |
\*---------------------------------------------------------*/
#define HIDPP20_RESYNC_KEYS_MIN         1
#define HIDPP20_RESYNC_KEYS_MAX         4
#define HIDPP20_RESYNC_QUIET_ZONES      8

class LogitechHIDPP20Controller
{
public:
    LogitechHIDPP20Controller(hid_device* dev, const char* path,
                            uint8_t device_index, bool wireless,
                            std::shared_ptr<std::mutex> mutex_ptr,
                            uint16_t usage_page = 0xFF00,
                            hid_device* perkey_vl_dev = nullptr);
    ~LogitechHIDPP20Controller();

    /*-----------------------------------------------------*\
    | Lifecycle                                             |
    \*-----------------------------------------------------*/
    bool            Probe();

    /*-----------------------------------------------------*\
    | Who is this? Transport framing, an IRoot ping,        |
    | the unit id; nothing else. Changes nothing on         |
    | the device, so it is safe against hardware            |
    | another controller is driving. Empty = not            |
    | HID++ 2.0, or no unit id to identify it by.           |
    \*-----------------------------------------------------*/
    std::string     ProbeIdentity();

    /*-----------------------------------------------------*\
    | The name the device answers with, or "" when it       |
    | has none we can trust, a placeholder, or bytes        |
    | that don't read as a name. Callers persist this,      |
    | so a guess is worse than nothing. Run after           |
    | ProbeIdentity, on the same probe object.              |
    \*-----------------------------------------------------*/
    std::string     ProbeName();

    /*-----------------------------------------------------*\
    | Does this read as a name a device gave us, rather     |
    | than as whatever was left in the buffer? Public       |
    | because a receiver's stored pairing names get the     |
    | same doubt as the ones devices answer with.           |
    \*-----------------------------------------------------*/
    static bool     NameLooksReal(const std::string& name);

    void            Initialize();
    void            Shutdown();

    /*-----------------------------------------------------*\
    | Accessors                                             |
    \*-----------------------------------------------------*/
    const HIDPP20DeviceCapabilities&  GetCapabilities() const;
    const HIDPP20Transport&           GetTransport() const   { return transport; }
    std::string     GetDeviceLocation();
    std::string     GetSerialString();
    uint32_t        GetInitGeneration() const;

    /*-----------------------------------------------------*\
    | Sender thread: SubmitPerKeyFrame stores a snapshot,   |
    | replacing any unsent one, and returns; the sender     |
    | sends at the pace the link sustains. The effect       |
    | engine never blocks on the wire.                      |
    \*-----------------------------------------------------*/
    void SetPerKeyFrameSender(std::function<void(std::vector<RGBColor>&)> fn);
    void SubmitPerKeyFrame(const std::vector<RGBColor>& frame);
    bool HasPerKeyFrameSender() const;

    /*-----------------------------------------------------*\
    | True while RediscoverFeatures rebuilds the map.       |
    | The effect engine must not paint or claim during      |
    | it: has_perkey and zone/effect data flap false,       |
    | so a claim here reaches flags=5 with an empty         |
    | per-key layer.                                        |
    \*-----------------------------------------------------*/
    bool DiscoveryInProgress() const;

    /*-----------------------------------------------------*\
    | Per-key lighting (0x8081). Writes pipeline within a   |
    | frame; PerKeyFrameEnd attributes ACKs by payload echo |
    | and reports exactly which zones committed, misses     |
    | ride the next frame's delta.                          |
    \*-----------------------------------------------------*/
    void              SetPerKeyColors(const std::vector<std::pair<uint16_t, RGBColor>>& zone_colors);
    void              SetAllPerKeyColor(RGBColor color);
    void              SendPerKeyData(uint8_t perkey_idx, uint8_t function,
                                     const uint8_t* data, size_t len,
                                     const std::vector<uint8_t>& zone_ids);
    PerKeyFrameResult PerKeyFrameEnd();

    /*-----------------------------------------------------*\
    | Per-key lighting (0x8080). Addresses (keyType,        |
    | keyId); fn3 rides the 0x12 report on dev_perkey_vl,   |
    | fn5 FlushLEDS commits on dev. The key set comes       |
    | from DiscoverPerKey8080: never hardcoded.             |
    \*-----------------------------------------------------*/
    void              SetPerKey8080(const std::vector<std::pair<uint16_t, std::vector<std::pair<uint8_t, RGBColor>>>>& by_type);
    void              PerKeyCommit8080();

    /*-----------------------------------------------------*\
    | Zone effects (0x8071 / 0x8070)                        |
    \*-----------------------------------------------------*/
    void            SetZoneEffect(uint8_t cluster_idx, uint8_t effect_idx,
                                  uint16_t effect_id,
                                  unsigned char r, unsigned char g, unsigned char b,
                                  uint16_t period, unsigned char brightness,
                                  unsigned char direction, bool persist);

    /*-----------------------------------------------------*\
    | Headset RGB hostmode (0x0620).                        |
    | Sticky-claim model: SetHostMode() claims once via     |
    | fn8, then each write is fn5 (single-value) or fn2     |
    | (individual) + fn6 FrameEnd[0x01]. 0x02 persist was   |
    | tested and does not work on G522 firmware.            |
    \*-----------------------------------------------------*/
    void            SetHeadsetRGBHostmodeColors(const std::vector<RGBColor>& zone_colors);

    /*-----------------------------------------------------*\
    | SW control management                                 |
    \*-----------------------------------------------------*/
    int             SetSWControl(uint8_t mode, uint8_t flags);
    void            SetRGBPowerMode(uint8_t mode);
    void            SetHostMode();
    bool            ClaimSWControlIfNeeded();
    void            UpgradeSwControlAfterFirstPaint();

    /*-----------------------------------------------------*\
    | Keyboard-family handshake (0x4522 fn3 + fn1).         |
    | G815 / G915 / G Pro send this before any mode         |
    | write. Feature-gated no-op on devices (G502 /         |
    | G515) that don't enumerate 0x4522.                    |
    \*-----------------------------------------------------*/
    void            DoDisableKeysByUsageHandshake();

    /*-----------------------------------------------------*\
    | Keyboard-family per-key takeover prep.                |
    | Per-cluster SetEffectByIndex(effectIdx=0=Off,         |
    | persist=1) + primer key via SetIndividualRgbZones     |
    | + FrameEnd. Matches G815 / G915 InitializeDirect.     |
    | Gated on 0x4522 + per-key V2 presence.                |
    \*-----------------------------------------------------*/
    void            DoKeyboardFamilyPerKeyPrep();

    /*-----------------------------------------------------*\
    | Wake-repaint flag. Set by Wake() before calling       |
    | request_repaint_fn so the repaint callback knows      |
    | to invalidate sent_colors (force a full per-key       |
    | push) without triggering the claim/prep sequence.     |
    \*-----------------------------------------------------*/
    bool            ConsumeWakeFullRepaint();
    /*-----------------------------------------------------*\
    | The prep is a claim-time operation, not a per-frame   |
    | one: it wipes the device's per-key buffer so the      |
    | per-key layer can be laid down fresh. Running it      |
    | again on a later frame throws away everything         |
    | painted since. Once per claim, and no more.           |
    \*-----------------------------------------------------*/
    bool            NeedsPrepSequence() const  { return sw_control_needs_upgrade_to_5 && !prep_applied; }
    void            MarkPrepApplied()          { prep_applied = true; }

    /*-----------------------------------------------------*\
    | True from the claim until the per-key layer           |
    | takes the screen. This is the first-paint             |
    | window: the animation is not yet guaranteed to        |
    | drive another frame, so a partial commit has to       |
    | be retried rather than left for the next tick.        |
    \*-----------------------------------------------------*/
    bool            AwaitingSWControlUpgrade() const { return sw_control_needs_upgrade_to_5; }

    /*-----------------------------------------------------*\
    | Per-key retry scheduling. Called by the RGB           |
    | controller's DeviceUpdateLEDs on partial-commit       |
    | frames; the power thread polls and fires              |
    | request_repaint_fn when a retry deadline expires.     |
    \*-----------------------------------------------------*/
    bool            ScheduleRetryPaint();
    void            CancelRetryPaint();
    void            TickRetryPaintIfPending();

    /*-----------------------------------------------------*\
    | Observed per-key prep sequence. Two                   |
    | SetEffectByIndex calls cloned byte-for-byte from a    |
    | wire capture on a G502 X PLUS.                        |
    | Used in place of the Static-pass-through prep when    |
    | the device's RGBEffects enumeration matches the       |
    | G502 shape, see DeviceUpdateLEDs for gating.          |
    \*-----------------------------------------------------*/
    void            DoObservedPerKeyPrep();

    /*-----------------------------------------------------*\
    | Power management (idle/dim/sleep/wake)                |
    \*-----------------------------------------------------*/
    void            StartPowerManager();
    void            StopPowerManager();
    void            StartEventWatcher();
    bool            HasBridge() const;

    /*-----------------------------------------------------*\
    | The receiver watcher's connection nudge, the same     |
    | flag the reader raises, so the power thread's         |
    | handling and guards apply. Safe from any thread.      |
    \*-----------------------------------------------------*/
    void            NudgeConnection(int direction)      { pending_connection.store(direction); }
    void            SetRepaintCallback(std::function<void()> repaint);
    void            SetReapplyActiveModeCallback(std::function<bool()> cb);
    void            SetPairingName(const std::string& name);   /* receiver-stored codename; watcher label + name-read fallback */

    /*-----------------------------------------------------*\
    | One wire transaction at a time. Responses land in one |
    | queue and SendAcked drains it, discarding             |
    | unrecognized frames, only safe while nothing else has |
    | a transaction in flight. A per-key frame is a long    |
    | transaction (prep, writes, FrameEnd matching the ACKs |
    | back); the power thread sends on its own schedule and |
    | would otherwise eat the frame's ACKs mid-flight,      |
    | reading landed writes as uncommitted. Held across     |
    | SendAcked, and by the RGB controller across a whole   |
    | frame; recursive because a frame sends.               |
    \*-----------------------------------------------------*/
    std::recursive_mutex&   TransactionMutex()  { return transaction_mutex; }

    HIDPP20PowerState GetPowerState() const;
    int             GetDimBrightness() const;
    bool            IsOnline() const;
    bool            IsDeepSleep() const;
    void            SetWireless(bool w)     { wireless = w; }
    bool            QueryExternalPower();
    void            FlushResponseQueue();

private:
    hid_device*                 dev;
    hid_device*                 dev_perkey_vl;  /* 0x8080: very-long (0x12) write handle (usage 0x0604); nullptr if absent */
    std::string                 location;
    uint8_t                     device_index;

    /*-----------------------------------------------------*\
    | Set when the controller is going away. Waits,         |
    | retries and backoff sleeps check it and give up       |
    | rather than run their course.                         |
    \*-----------------------------------------------------*/
    std::atomic<bool>           teardown_pending;

    std::recursive_mutex        transaction_mutex;  /* one wire transaction at a time, see TransactionMutex() */

    bool                        wireless;
    std::shared_ptr<std::mutex> mutex;
    std::atomic<bool>           long_only;          /* collection rejected the 0x10 short report */
    std::atomic<bool>           discovery_in_progress_; /* RediscoverFeatures is rebuilding caps */
    HIDPP20DeviceCapabilities     caps;
    HIDPP20Transport              transport;

    /*-----------------------------------------------------*\
    | Per-link feature cache (this controller is one        |
    | device), keyed by command link. last_fap_error_       |
    | is the last HID++ error, for the restore              |
    | self-heal (0x06/0x07).                                |
    \*-----------------------------------------------------*/
    std::map<std::string, HIDPP20LinkIndexMap> link_index_cache_;
    uint8_t                     last_fap_error_;

    /*-----------------------------------------------------*\
    | While pipelining_claim_ is set, SendAckedIntoFAP      |
    | fires the write and records it here instead of        |
    | blocking. VerifyClaimPipeline reconciles the          |
    | batch afterward.                                      |
    \*-----------------------------------------------------*/
    bool                                pipelining_claim_;
    std::vector<HIDPP20PendingClaimCmd> claim_pipeline_;

    bool                        initialized;
    bool                        sw_control_claimed;
    bool                        sw_control_needs_upgrade_to_5;
    bool                        prep_applied;       /* prep has run for the current claim */
    uint32_t                    frame_counter;

    /*-----------------------------------------------------*\
    | Retry-paint state (partial-commit recovery).          |
    | retry_paint_deadline_ zero = no retry pending.        |
    | retry_paint_attempt_ indexes into                     |
    | HIDPP20_REPAINT_RETRY_BACKOFF_MS; once it reaches     |
    | the array length, we give up for this sequence.       |
    | Atomic so both the paint thread (RGB controller)      |
    | and the power thread can access without locks.        |
    \*-----------------------------------------------------*/
    std::atomic<std::chrono::steady_clock::time_point>  retry_paint_deadline_;
    std::atomic<uint8_t>                                retry_paint_attempt_;
    std::atomic<bool>                                   wake_full_repaint_pending_;
    uint32_t                    init_generation;
    std::string                 log_tag;

    /*-----------------------------------------------------*\
    | Transport-layer I/O                                   |
    |                                                       |
    | SendMessage/ReadMessage handle wire framing based     |
    | on transport.type. Upper layers pass feature index,   |
    | function ID, and payload, the transport layer         |
    | wraps them in the correct report format.              |
    \*-----------------------------------------------------*/
    int             SendMessage(uint8_t feat_idx, uint8_t function,
                                const uint8_t* data, size_t len);
    int             ReadMessage(uint8_t* feat_idx_out, uint8_t* function_out,
                                uint8_t* data_out, size_t data_max,
                                int timeout_ms = LOGITECH_PROTOCOL_TIMEOUT);

    /*-----------------------------------------------------*\
    | Standard HID++ transport (0xFF00/0xFF43)              |
    \*-----------------------------------------------------*/
    int             SendStandard(uint8_t feat_idx, uint8_t function,
                                 const uint8_t* data, size_t len);
    int             ReadStandardDirect(uint8_t* feat_idx_out, uint8_t* function_out,
                                       uint8_t* data_out, size_t data_max,
                                       int timeout_ms);

    /*-----------------------------------------------------*\
    | Centurion transport (0xFFA0)                          |
    | Wraps messages in CPL framing, routes through         |
    | CentPPBridge for sub-device access.                   |
    \*-----------------------------------------------------*/
    int             SendCenturion(uint8_t feat_idx, uint8_t function,
                                  const uint8_t* data, size_t len);
    int             ReadCenturionDirect(uint8_t* feat_idx_out, uint8_t* function_out,
                                        uint8_t* data_out, size_t data_max,
                                        int timeout_ms);

    /*-----------------------------------------------------*\
    | Feature 0x8080 very-long (0x12) frame transport.      |
    | Writes a 64-byte report to dev_perkey_vl; the ACK     |
    | lands on dev and is drained by the reader thread.     |
    | Fire-and-forget, do NOT route through SendStandard    |
    | (which caps at the 0x11 long report).                 |
    \*-----------------------------------------------------*/
    int             SendVeryLongFrame(uint8_t feat_idx, uint8_t function,
                                      const uint8_t* data, size_t len);

    /*-----------------------------------------------------*\
    | Reader thread dispatch layer                          |
    | ReadHIDDirect: raw HID read (used by reader thread    |
    | and during Probe before reader starts).               |
    | ReadFromQueue: waits on response queue filled by      |
    | the reader thread.                                    |
    \*-----------------------------------------------------*/
    int             ReadHIDDirect(uint8_t* feat_idx_out, uint8_t* function_out,
                                  uint8_t* data_out, size_t data_max,
                                  int timeout_ms);
    int             ReadFromQueue(uint8_t* feat_idx_out, uint8_t* function_out,
                                  uint8_t* data_out, size_t data_max,
                                  int timeout_ms);

    /*-----------------------------------------------------*\
    | High-level helpers                                    |
    | SendAndReceive is retained as a thin wrapper          |
    | around SendAcked with the reliable policy, for        |
    | call-site stability.                                  |
    \*-----------------------------------------------------*/
    int             SendAndReceive(uint8_t feat_idx, uint8_t function,
                                   const uint8_t* send_data, size_t send_len,
                                   uint8_t* recv_data, size_t recv_max);

    /*-----------------------------------------------------*\
    | Unified send-and-ack primitive with retry policy.     |
    | All command paths converge here. Returns:             |
    | >0 : bytes copied into recv_data                      |
    | 0 : timeout / BUSY exhaustion                         |
    | -1 : non-BUSY HID++ error                             |
    | -2 : wire error (SendMessage failed)                  |
    | If hidpp20_error_out is non-null and return is -1,    |
    | the HID++ error code is stored there.                 |
    \*-----------------------------------------------------*/
    int             SendAcked(uint8_t feat_idx, uint8_t function,
                              const uint8_t* send_data, size_t send_len,
                              uint8_t* recv_data, size_t recv_max,
                              const HIDPP20RetryPolicy& policy = HIDPP20_POLICY_RELIABLE,
                              uint8_t* hidpp20_error_out = nullptr);

    /*-----------------------------------------------------*\
    | Compatibility shim: same as SendAcked but writes      |
    | the response into a blankFAPmessage. Used by          |
    | callers that inherited the SendLong+ReadResponse      |
    | interface and inspect response.data[] downstream.     |
    \*-----------------------------------------------------*/
    int             SendAckedIntoFAP(uint8_t feat_idx, uint8_t function,
                                     const uint8_t* send_data, size_t send_len,
                                     blankFAPmessage& response,
                                     const HIDPP20RetryPolicy& policy = HIDPP20_POLICY_RELIABLE);

    /*-----------------------------------------------------*\
    | Feature discovery                                     |
    \*-----------------------------------------------------*/
    uint8_t         GetFeatureIndex(uint16_t feature_page,
                                    const HIDPP20RetryPolicy& policy = HIDPP20_POLICY_RELIABLE);
    uint8_t         GetFeatureVersion(uint16_t feature_page) const;
    void            LogFeatureSummary();
    void            DiscoverTransport();
    void            DiscoverDeviceName();
    bool            ReadCenturionDeviceName0101(std::string& out);
    void            DiscoverDeviceType();
    void            EnumerateFeatures(uint8_t feature_set_idx);
    void            DiscoverFirmwareInfo();
    void            DiscoverRGBEffects();
    void            DiscoverEffectCards();
    void            DiscoverHeadsetRGBHostmode();
    void            DiscoverPerKeyZones();
    void            DiscoverPerKey8080();
    size_t          ReadPerKey8080Colors(uint8_t feat_idx, uint16_t key_type,
                                         uint16_t expected_key_count,
                                         std::vector<uint8_t>& out_ids,
                                         bool log_colors);
    void            DiscoverKeyboardLayout();

    /*-----------------------------------------------------*\
    | Power management internals                            |
    \*-----------------------------------------------------*/
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

    /*-----------------------------------------------------*\
    | Take software control and keep trying until it        |
    | sticks. A device that has just come back is booting   |
    | its onboard profile and drops a claim that arrives    |
    | too early, silently.                                  |
    \*-----------------------------------------------------*/
    bool            ReclaimSWControl(const char* reason, bool force);
    void            RediscoverFeatures();

    /*-----------------------------------------------------*\
    | Per-link feature-index cache helpers                  |
    \*-----------------------------------------------------*/
    std::string        CurrentLinkKey() const;          /* usb#idx or rx#slot for the current command link */
    HIDPP20LinkIndexMap SnapshotLinkIndexMap() const;   /* pull the link-dependent indices out of caps */
    void               RestoreLinkIndexMap(const HIDPP20LinkIndexMap& m); /* push them back */
    void               CacheCurrentLinkIndexMap();       /* snapshot the current link if discovery is complete */
    void               HealStaleMapIfNeeded(const char* reclaim_reason); /* rediscover if a reclaim hit 0x06/0x07 */
    void               VerifyClaimPipeline(); /* reconcile a pipelined claim's ACKs, blocking-retry any miss */

    /*-----------------------------------------------------*\
    | Platform-specific. Returns the friendly name for a    |
    | Centurion sub-device at the given hidapi path, or ""  |
    | if no name is available. Linux reads HID_NAME from    |
    | sysfs; Windows uses hid_device_info::product_string.  |
    \*-----------------------------------------------------*/
    std::string     GetCenturionSubDeviceName(const std::string& path);

    /*-----------------------------------------------------*\
    | Reader thread + response queue                        |
    \*-----------------------------------------------------*/
    std::thread*            reader_thread;
    std::atomic<bool>       reader_running;
    std::mutex              response_mutex;
    std::condition_variable response_cv;
    std::deque<HIDPP20RawMessage> response_queue;

    /*-----------------------------------------------------*\
    | Power thread (state machine + command sender)         |
    \*-----------------------------------------------------*/
    std::thread*            power_thread;
    std::atomic<bool>       power_thread_running;
    std::atomic<int>        pending_activity;       // -1=none, 0=idle, 1+=active
    std::atomic<int>        pending_connection;     // 0=none, +1=connected, -1=disconnected
    std::atomic<bool>       pending_power_check;    // device broadcast 0x1004, re-read its status
    std::atomic<bool>       device_online;          // false when device is unreachable
    std::atomic<int>        consecutive_timeouts;   // reset on successful response

    /*-----------------------------------------------------*\
    | Power management state                                |
    \*-----------------------------------------------------*/
    HIDPP20PowerState         power_state;
    std::mutex              power_mutex;
    std::atomic<bool>       deep_sleep;             // true once device stops responding after StartSleep()
    std::atomic<int>        consecutive_frame_end_failures; // FrameEnd BUSY exhaustions while SLEEPING

    /*-----------------------------------------------------*\
    | Dim ramp state                                        |
    | dim_brightness_pct is applied by DeviceUpdateLEDs     |
    | to scale colors before pushing to device.             |
    | This is our own host-side animation, independent      |
    | of any firmware dim/sleep timers.                     |
    \*-----------------------------------------------------*/
    #define DIM_STEPS                           25
    #define DIM_INTERVAL_MS                     200
    #define DIM_TARGET_PCT                      50
    std::atomic<int> dim_brightness_pct;    // 100=full, 50=dimmed
    int              dim_step;
    std::chrono::steady_clock::time_point next_dim_time;

    /*-----------------------------------------------------*\
    | Sleep timer                                           |
    \*-----------------------------------------------------*/
    std::chrono::steady_clock::time_point sleep_deadline;

    /*-----------------------------------------------------*\
    | Last idle-settings re-read timestamp. Drives the      |
    | 500ms poll in PowerThreadFunc that re-reads the       |
    | LogitechHIDPP20IdleSettings JSON key so updates       |
    | from the plugin (or manual edits) apply within        |
    | about half a second without any callback plumbing.    |
    \*-----------------------------------------------------*/
    std::chrono::steady_clock::time_point last_idle_poll;

    /*-----------------------------------------------------*\
    | Last power-source read. Reads are driven by the       |
    | device's 0x1004 broadcast; this interval is the       |
    | backstop, see PowerThreadFunc.                        |
    \*-----------------------------------------------------*/
    #define POWER_POLL_INTERVAL_MS              60000
    std::chrono::steady_clock::time_point last_power_poll;

    /*-----------------------------------------------------*\
    | Effective idle/sleep timers used by the state         |
    | machine. Populated from the firmware snapshot by      |
    | default, then possibly overridden by profile          |
    | values in ApplyPowerSavingProfile.                    |
    \*-----------------------------------------------------*/
    uint16_t        idle_timeout_s;
    uint16_t        sleep_timeout_s;

    /*-----------------------------------------------------*\
    | Firmware-configured timer snapshot. Read at init      |
    | (and on reconnect) by ReadFirmwareTimers and          |
    | never overwritten by profile application, so          |
    | that the unconfigured fallback path and transition    |
    | back from a user profile both have a clean set        |
    | of defaults to return to.                             |
    \*-----------------------------------------------------*/
    uint16_t        fw_idle_timeout_s   = 60;
    uint16_t        fw_sleep_timeout_s  = 300;
    uint16_t        written_idle_s      = 0;    // last value written to device RAM (0 = not written yet)
    uint16_t        written_sleep_s     = 0;

    /*-----------------------------------------------------*\
    | Host-side idle/dim/sleep state.                       |
    | Populated from LogitechHIDPP20IdleSettings on each    |
    | ApplyPowerSavingProfile() invocation.                 |
    \*-----------------------------------------------------*/
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

    /*-----------------------------------------------------*\
    | Per-key frame accumulators. Populated by              |
    | SendPerKeyData as packets go out and ACKs are         |
    | attributed, settled by PerKeyFrameEnd. Guarded by     |
    | transaction_mutex, which every frame path holds.      |
    \*-----------------------------------------------------*/
    std::vector<uint8_t>      frame_attempted_zones;
    std::vector<PerKeyPacket> frame_packets;
    std::vector<uint8_t>      frame_acked_zones;
    std::map<uint8_t, size_t> frame_unmatched_acks;
    size_t                    frame_responses_seen = 0;
    size_t                    frame_exact_acks     = 0;
    bool                      frame_aborted        = false;
    std::chrono::steady_clock::time_point frame_first_write;

    /*-----------------------------------------------------*\
    | Learned FrameEnd busy window, us per zone: the        |
    | commit's not-ready time scales with update size, so   |
    | predict it, sleep the prediction before the first     |
    | FrameEnd attempt (link stays free for input), keep    |
    | the BUSY poll as corrector. BUSY -> pull toward the   |
    | measurement; clean first-try accept -> decay slowly   |
    | to re-probe the floor. Starts at 0 (first frames      |
    | measure); converges within seconds, per device.       |
    \*-----------------------------------------------------*/
    float           fe_busy_us_per_zone = 0.0f;
    uint8_t         fe_frames_since_probe = 0;

    /*-----------------------------------------------------*\
    | Zones staged on the device since the last             |
    | successful FrameEnd. This: not the current            |
    | frame's zone count, is what the commit has to         |
    | chew through: an aborted frame skips its FrameEnd     |
    | and its landed writes latch with the next one.        |
    \*-----------------------------------------------------*/
    size_t          fe_staged_zones = 0;

    /*-----------------------------------------------------*\
    | Adaptive write window (AIMD on ACK loss).             |
    \*-----------------------------------------------------*/
    size_t          perkey_window       = HIDPP20_PERKEY_WINDOW_MAX;
    uint8_t         perkey_clean_frames = 0;

    void            ResetPerKeyFrameState();

    /*-----------------------------------------------------*\
    | Read and process one per-key response within          |
    | timeout_ms: attribute a write ACK, count a write      |
    | error, ignore anything else. Returns 1 when a         |
    | message was consumed, 0 on timeout, -1 on wire        |
    | error.                                                |
    \*-----------------------------------------------------*/
    int             ProcessOnePerKeyResponse(int timeout_ms, uint8_t perkey_idx);

    /*-----------------------------------------------------*\
    | Read until the wire goes quiet. Called after an       |
    | incomplete frame: late ACKs may still be in           |
    | flight and must be consumed before the next           |
    | frame's matching can trust what it reads.             |
    \*-----------------------------------------------------*/
    void            DrainStaleResponses();

    /*-----------------------------------------------------*\
    | Attribute one write ACK to an outstanding packet      |
    | by payload echo. Appends the zones it proves to       |
    | acked_zones_out; returns the packet index or -1.      |
    \*-----------------------------------------------------*/
    int             MatchPerKeyAck(uint8_t resp_func_hi,
                                   const uint8_t* resp_data,
                                   std::vector<uint8_t>& acked_zones_out);

    /*-----------------------------------------------------*\
    | Per-key frame sender thread state. pending_frame      |
    | holds the newest submitted snapshot; submitting       |
    | over an unsent one replaces it (the skip counter      |
    | records how many).                                    |
    \*-----------------------------------------------------*/
    void            SenderThreadFunc();
    void            StopSenderThread();

    /*-----------------------------------------------------*\
    | Sliced sleep that wakes early on a pending link       |
    | change or offline; false when interrupted.            |
    \*-----------------------------------------------------*/
    bool            InterruptibleBackoff(uint16_t delay_ms);

    std::function<void(std::vector<RGBColor>&)> perkey_frame_fn;
    std::thread*            sender_thread          = nullptr;
    std::atomic<bool>       sender_running{false};
    std::mutex              pending_frame_mutex;
    std::condition_variable pending_frame_cv;
    std::vector<RGBColor>   pending_frame;
    bool                    pending_frame_valid    = false;
    uint32_t                pending_frames_skipped = 0;

    /*-----------------------------------------------------*\
    | Callbacks                                             |
    \*-----------------------------------------------------*/
    std::function<void()> request_repaint_fn;
    std::function<bool()> reapply_active_mode_fn;
    std::string                 pairing_name;   /* receiver-stored codename, "" if unknown */
};
