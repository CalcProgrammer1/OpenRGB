/*---------------------------------------------------------*\
| CMARGBGen2A1Controller.h                                  |
|                                                           |
|   Driver for Cooler Master ARGB Gen 2 A1 controller       |
|                                                           |
|   Morgan Guimard (morg)                       26 Jun 2022 |
|   Fabian R (kderazorback)                     11 Aug 2023 |
|   Daniel Clark (megadjc)                      14 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

#define CM_ARGB_GEN2_A1_PACKET_LENGTH                    65
#define CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH            64
#define CM_ARGB_GEN2_A1_CHANNEL_COUNT                    3
#define CM_ARGB_GEN2_A1_GEN2_SUBPORT_COUNT               5
#define CM_ARGB_GEN2_A1_TARGET_MAX_LEDS                  60
#define CM_ARGB_GEN2_A1_GEN1_MIN_LEDS                    6
#define CM_ARGB_GEN2_A1_EFFECT_COUNT                     10
#define CM_ARGB_GEN2_A1_OVERLAY_PHASE_COUNT              6
#define CM_ARGB_GEN2_A1_OVERLAY_TABLE_LENGTH             (CM_ARGB_GEN2_A1_GEN2_SUBPORT_COUNT * CM_ARGB_GEN2_A1_OVERLAY_PHASE_COUNT)
#define CM_ARGB_GEN2_A1_UID_LENGTH                       32
#define CM_ARGB_GEN2_A1_RESPONSE_TIMEOUT_MS              150
#define CM_ARGB_GEN2_A1_REALTIME_PERIOD_MS               40
#define CM_ARGB_GEN2_A1_REALTIME_PACKET_GAP_MS           2
#define CM_ARGB_GEN2_A1_REALTIME_PREROLL_FIRST           2
#define CM_ARGB_GEN2_A1_REALTIME_PREROLL_SECOND          5
#define CM_ARGB_GEN2_A1_REALTIME_SECOND_ON_TO_TARGET_MS  104
#define CM_ARGB_GEN2_A1_REALTIME_EXIT_QUIET_MS           10
#define CM_ARGB_GEN2_A1_REALTIME_EXIT_ATTEMPTS           2
#define CM_ARGB_GEN2_A1_EFFECT_SCOPE_SETTLE_MS           5
#define CM_ARGB_GEN2_A1_EFFECT_SETTLE_MS                 5
#define CM_ARGB_GEN2_A1_OVERLAY_INTERVAL_DEFAULT       0x32
#define CM_ARGB_GEN2_A1_OVERLAY_INTERVAL_UNIT_MS         100
#define CM_ARGB_GEN2_A1_QUERY_TO_GENERATION_DELAY_MS     4
#define CM_ARGB_GEN2_A1_GENERATION_TO_REFRESH_DELAY_MS   3
#define CM_ARGB_GEN2_A1_REFRESH_TO_TOPOLOGY_DELAY_MS     3
#define CM_ARGB_GEN2_A1_GENERATION_READY_DELAY_MS        1
#define CM_ARGB_GEN2_A1_TOPOLOGY_POLL_MS                 50
#define CM_ARGB_GEN2_A1_TOPOLOGY_POLL_COUNT              4
#define CM_ARGB_GEN2_A1_BRIGHTNESS_MAX                 0xFF
#define CM_ARGB_GEN2_A1_BRIGHTNESS_MIN                 0x00
#define CM_ARGB_GEN2_A1_SPEED_MAX                      0x04
#define CM_ARGB_GEN2_A1_SPEED_HALF                     0x02
#define CM_ARGB_GEN2_A1_SPEED_MIN                      0x00

/*---------------------------------------------------------*\
| OpenRGB mode identifiers                                  |
\*---------------------------------------------------------*/
enum
{
    CM_ARGB_GEN2_A1_DIRECT_MODE                      = 0xFF,
    CM_ARGB_GEN2_A1_SPECTRUM_MODE                    = 0x00,
    CM_ARGB_GEN2_A1_STATIC_MODE                      = 0x01,
    CM_ARGB_GEN2_A1_RELOAD_MODE                      = 0x02,
    CM_ARGB_GEN2_A1_RECOIL_MODE                      = 0x03,
    CM_ARGB_GEN2_A1_BREATHING_MODE                   = 0x04,
    CM_ARGB_GEN2_A1_REFILL_MODE                      = 0x05,
    CM_ARGB_GEN2_A1_DEMO_MODE                        = 0x06,
    CM_ARGB_GEN2_A1_FILL_FLOW_MODE                   = 0x07,
    CM_ARGB_GEN2_A1_RAINBOW_MODE                     = 0x08,
    CM_ARGB_GEN2_A1_OFF_MODE                         = 0x09,
    CM_ARGB_GEN2_A1_CUSTOM_MODE                      = 0xC0,
    CM_ARGB_GEN2_A1_MULTILAYER_MODE                  = 0xFE
};


enum
{
    CM_ARGB_GEN2_A1_COMMAND_PAGE                     = 0x80,
    CM_ARGB_GEN2_A1_READ                             = 0x01,
    CM_ARGB_GEN2_A1_WRITE                            = 0x02,
    CM_ARGB_GEN2_A1_RESPONSE                         = 0x03
};

enum
{
    CM_ARGB_GEN2_A1_CMD_REALTIME_MODE                = 0x01,
    CM_ARGB_GEN2_A1_CMD_EFFECT                       = 0x03,
    CM_ARGB_GEN2_A1_CMD_STORED_EFFECT                = 0x04,
    CM_ARGB_GEN2_A1_CMD_GEN1_GEOMETRY                = 0x06,
    CM_ARGB_GEN2_A1_CMD_GEN2_TOPOLOGY                = 0x07,
    CM_ARGB_GEN2_A1_CMD_CUSTOM_RGB                   = 0x08,
    CM_ARGB_GEN2_A1_CMD_REALTIME_RGB                 = 0x09,
    CM_ARGB_GEN2_A1_CMD_IDENTIFY                     = 0x0A,
    CM_ARGB_GEN2_A1_CMD_GENERATION                   = 0x0B,
    CM_ARGB_GEN2_A1_CMD_EFFECT_BANK                  = 0x0C,
    CM_ARGB_GEN2_A1_CMD_GEN2_REMAP                   = 0x0D,
    CM_ARGB_GEN2_A1_CMD_OVERLAY_TABLE                = 0x10,
    CM_ARGB_GEN2_A1_CMD_OVERLAY_MODE                 = 0x11,
    CM_ARGB_GEN2_A1_CMD_SAVE                         = 0xB0,
    CM_ARGB_GEN2_A1_CMD_RESET                        = 0xB1,
    CM_ARGB_GEN2_A1_CMD_REFRESH                      = 0xB2,
    CM_ARGB_GEN2_A1_CMD_FIRMWARE                     = 0xB4,
    CM_ARGB_GEN2_A1_CMD_UID                          = 0xB5,
    CM_ARGB_GEN2_A1_CMD_CLEAR_REALTIME               = 0xB6,
    CM_ARGB_GEN2_A1_CMD_MULTILAYER                   = 0xC0
};

enum
{
    CM_ARGB_GEN2_A1_GENERATION_GEN2                  = 0x00,
    CM_ARGB_GEN2_A1_GENERATION_GEN1                  = 0x01,
    CM_ARGB_GEN2_A1_GENERATION_UNKNOWN               = 0xFF
};

enum
{
    CM_ARGB_GEN2_A1_RESET_ALL                        = 0x00,
    CM_ARGB_GEN2_A1_RESET_LED                        = 0x01,
    CM_ARGB_GEN2_A1_RESET_FAN                        = 0x02
};

enum
{
    CM_ARGB_GEN2_A1_MULTILAYER_GLOBAL                = 0x00,
    CM_ARGB_GEN2_A1_MULTILAYER_INDEPENDENT           = 0x01
};

enum
{
    CM_ARGB_GEN2_A1_CHANNEL_ALL                      = 0xFF,
    CM_ARGB_GEN2_A1_GEN1_SUBTARGET                   = 0x00,
    CM_ARGB_GEN2_A1_SUBCHANNEL_ALL                   = 0xFF
};

struct CMARGBGen2A1EffectRecord
{
    unsigned char mode       = 0;
    unsigned char speed      = 0;
    unsigned char brightness = 0;
    unsigned char red        = 0;
    unsigned char green      = 0;
    unsigned char blue       = 0;
    unsigned char random     = 0;
    unsigned char smode      = 0;
};

struct CMARGBGen2A1Subdevice
{
    unsigned char mask      = 0;
    unsigned char led_count = 0;
};

struct CMARGBGen2A1RealtimeTarget
{
    unsigned char             outer_mask = 0;
    unsigned char             subtarget  = 0;
    std::vector<RGBColor>     colors;
};

struct CMARGBGen2A1RealtimeFrame
{
    unsigned char             outer_mask = 0;
    unsigned char             subtarget  = 0;
    unsigned char             data[CM_ARGB_GEN2_A1_TARGET_MAX_LEDS * 3] = { 0 };
};

struct CMARGBGen2A1PortInfo
{
    bool                               valid                = false;
    unsigned char                      outer_mask           = 0;
    unsigned char                      generation           = CM_ARGB_GEN2_A1_GENERATION_UNKNOWN;
    unsigned char                      gen1_device_count    = 0;
    unsigned char                      gen1_leds_per_device = 0;
    std::vector<CMARGBGen2A1Subdevice> gen2_devices;
};

class CMARGBGen2A1controller
{
public:
    CMARGBGen2A1controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~CMARGBGen2A1controller();

    std::string                                 GetDeviceLocation();
    std::string                                 GetNameString();
    std::string                                 GetSerialString();
    std::string                                 GetFirmwareVersion();

    unsigned int                                GetPortCount() const;
    const CMARGBGen2A1PortInfo&                 GetPortInfo(unsigned int port) const;
    bool                                        RefreshTopology();
    bool                                        RefreshPortTopology(unsigned int port);
    bool                                        QueryPortGeneration(unsigned int port, unsigned char& generation);

    bool                                        SetPortGeneration(unsigned int port, unsigned char generation);
    bool                                        SetGen1Geometry(unsigned int port, unsigned char device_count, unsigned char leds_per_device);

    bool                                        EnterRealtimeMode();
    bool                                        ExitRealtimeMode();
    bool                                        IsRealtimeMode() const;
    bool                                        SetRealtimeColors(const std::vector<CMARGBGen2A1RealtimeTarget>& targets);

    bool                                        GetEffect(unsigned char outer_mask, unsigned char subtarget, CMARGBGen2A1EffectRecord& effect);
    bool                                        GetStoredEffect(unsigned char outer_mask, unsigned char subtarget, unsigned char effect_id,
                                                                CMARGBGen2A1EffectRecord& effect);
    bool                                        GetEffectBank(unsigned char outer_mask, unsigned char subtarget,
                                                              CMARGBGen2A1EffectRecord effects[CM_ARGB_GEN2_A1_EFFECT_COUNT]);
    bool                                        SetEffect(unsigned char outer_mask, unsigned char subtarget, int mode_value,
                                                          unsigned int speed, unsigned int brightness, RGBColor color, bool random);

    bool                                        GetCustomColors(unsigned char outer_mask, unsigned char subtarget, std::size_t led_count,
                                                                std::vector<RGBColor>& colors);
    bool                                        SendCustomColors(unsigned char outer_mask, unsigned char subtarget, const std::vector<RGBColor>& colors);
    bool                                        Identify(unsigned char outer_mask, unsigned char subtarget);

    bool                                        RemapGen2Subports(unsigned int port,
                                                                  const unsigned char old_masks[CM_ARGB_GEN2_A1_GEN2_SUBPORT_COUNT],
                                                                  const unsigned char new_masks[CM_ARGB_GEN2_A1_GEN2_SUBPORT_COUNT]);
    bool                                        GetOverlayTable(unsigned int port, unsigned char table[CM_ARGB_GEN2_A1_OVERLAY_TABLE_LENGTH]);
    bool                                        SetOverlayTable(unsigned int port, const unsigned char table[CM_ARGB_GEN2_A1_OVERLAY_TABLE_LENGTH]);
    bool                                        GetOverlayMode(bool& enabled, unsigned char& interval);
    bool                                        SetOverlayMode(bool enabled, unsigned char interval);
    bool                                        ResetConfiguration(unsigned char reset_type);
    bool                                        GetUID(unsigned char uid[CM_ARGB_GEN2_A1_UID_LENGTH]);
    bool                                        SetUID(const unsigned char uid[CM_ARGB_GEN2_A1_UID_LENGTH]);
    bool                                        GetMultilayerMode(unsigned char& mode);
    bool                                        SetMultilayerMode(unsigned char mode);
    bool                                        SaveToFlash();

private:
    std::string                                 location;
    std::string                                 name;
    hid_device*                                 dev                 = nullptr;
    unsigned int                                port_count          = CM_ARGB_GEN2_A1_CHANNEL_COUNT;
    std::atomic<bool>                           realtime_mode       = false;
    std::atomic<bool>                           realtime_stop       = false;
    std::mutex                                  io_mutex;
    std::mutex                                  realtime_mutex;
    std::condition_variable                     realtime_cv;
    std::thread                                 realtime_thread;
    std::vector<CMARGBGen2A1RealtimeFrame>      realtime_frames;
    CMARGBGen2A1PortInfo                        ports[CM_ARGB_GEN2_A1_CHANNEL_COUNT];

    static unsigned char                       PortMask(unsigned int port);
    bool                                        RefreshPort(unsigned int port);
    bool                                        ReadPortGeneration(unsigned int port, unsigned char& generation);
    bool                                        ReadGen1Geometry(unsigned int port, unsigned char& device_count, unsigned char& leds_per_device);
    bool                                        ReadGen2Topology(unsigned int port, std::vector<CMARGBGen2A1Subdevice>& devices);
    bool                                        ReadGen2TopologyStable(unsigned int port, std::vector<CMARGBGen2A1Subdevice>& devices);

    void                                        StartRealtimeThread();
    void                                        StopRealtimeThread();
    void                                        RealtimeWorker();
    bool                                        SendRealtimeFrame(const CMARGBGen2A1RealtimeFrame& frame);
    bool                                        SendRealtimeFrameLocked(const CMARGBGen2A1RealtimeFrame& frame);
    bool                                        SendRealtimeCycle(const std::vector<CMARGBGen2A1RealtimeFrame>& frames);
    bool                                        SendRealtimePreamble();
    void                                        ClearRealtimeFrames();

    bool                                        SendCommandPage(unsigned char page, unsigned char command, unsigned char operation,
                                                                const std::vector<unsigned char>& payload,
                                                                unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = nullptr,
                                                                int timeout_ms = CM_ARGB_GEN2_A1_RESPONSE_TIMEOUT_MS,
                                                                const std::vector<unsigned char>& expected_response_prefix = {});
    bool                                        SendRegularCommand(unsigned char command, unsigned char operation,
                                                                   const std::vector<unsigned char>& payload,
                                                                   unsigned char response[CM_ARGB_GEN2_A1_LOGICAL_PACKET_LENGTH] = nullptr,
                                                                   int timeout_ms = CM_ARGB_GEN2_A1_RESPONSE_TIMEOUT_MS,
                                                                   const std::vector<unsigned char>& expected_response_prefix = {});
    bool                                        WriteReport(const unsigned char report[CM_ARGB_GEN2_A1_PACKET_LENGTH]);
    void                                        DrainPendingResponses();
};
