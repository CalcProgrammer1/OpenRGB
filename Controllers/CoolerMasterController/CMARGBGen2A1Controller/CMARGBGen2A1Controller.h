/*---------------------------------------------------------*\
| CMARGBGen2A1Controller.h                                  |
|                                                           |
|   Driver for Cooler Master ARGB Gen 2 A1 controller       |
|                                                           |
|   Morgan Guimard (morg)                       26 Jun 2022 |
|   Fabian R (kderazorback)                     11 Aug 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define CM_ARGB_GEN2_A1_PACKET_LENGTH 65
#define CM_ARGB_GEN2_A1_CHANNEL_MAX_SIZE 72
#define CM_ARGB_GEN2_A1_CHANNEL_COUNT 3
#define CM_ARGB_GEN2_A1_PACKETS_PER_CHANNEL 2

#define CM_ARGB_GEN2_A1_SLEEP_SHORT     5
#define CM_ARGB_GEN2_A1_SLEEP_MEDIUM    45
#define CM_ARGB_GEN2_A1_SLEEP_LONG      70

enum
{
    CM_ARGB_GEN2_A1_DIRECT_MODE         = 0xFF,
    CM_ARGB_GEN2_A1_SPECTRUM_MODE       = 0x00,
    CM_ARGB_GEN2_A1_STATIC_MODE         = 0x01,
    CM_ARGB_GEN2_A1_RELOAD_MODE         = 0x02,
    CM_ARGB_GEN2_A1_RECOIL_MODE         = 0x03,
    CM_ARGB_GEN2_A1_BREATHING_MODE      = 0x04,
    CM_ARGB_GEN2_A1_REFILL_MODE         = 0x05,
    CM_ARGB_GEN2_A1_DEMO_MODE           = 0x06,
    CM_ARGB_GEN2_A1_FILL_FLOW_MODE      = 0x07,
    CM_ARGB_GEN2_A1_RAINBOW_MODE        = 0x08,
    CM_ARGB_GEN2_A1_CUSTOM_MODE         = 0xC0,
    CM_ARGB_GEN2_A1_OFF_MODE            = 0x09
};

enum
{
    CM_ARGB_GEN2_A1_BRIGHTNESS_MAX      = 0xFF,
    CM_ARGB_GEN2_A1_BRIGHTNESS_MIN      = 0x00,
    CM_ARGB_GEN2_A1_SPEED_MAX           = 0x04,
    CM_ARGB_GEN2_A1_SPEED_HALF          = 0x02,
    CM_ARGB_GEN2_A1_SPEED_MIN           = 0x00,
};

enum
{
    CM_ARGB_GEN2_A1_COMMAND             = 0x80,
    CM_ARGB_GEN2_A1_COMMAND_EXTRA_1     = 0x81,
    CM_ARGB_GEN2_A1_COMMAND_EXTRA_2     = 0x82,
    CM_ARGB_GEN2_A1_READ                = 0x01,
    CM_ARGB_GEN2_A1_WRITE               = 0x02,
    CM_ARGB_GEN2_A1_RESPONSE            = 0x03
};

enum
{
    CM_ARGB_GEN2_A1_SIZES               = 0x06,
    CM_ARGB_GEN2_A1_SET_RGB_VALUES      = 0x08,
    CM_ARGB_GEN2_A1_FLASH               = 0x0B,
    CM_ARGB_GEN2_A1_IDENTIFY            = 0x0A,
    CM_ARGB_GEN2_A1_LIGHTNING_CONTROL   = 0x01,
    CM_ARGB_GEN2_A1_HW_MODE_SETUP       = 0x03,
    CM_ARGB_GEN2_A1_CUSTOM_SEQUENCES    = 0x10,
    CM_ARGB_GEN2_A1_CUSTOM_SPEED        = 0x11,
    CM_ARGB_GEN2_A1_RESET               = 0xC0,
    CM_ARGB_GEN2_A1_APPLY_CHANGES       = 0xB0
};

enum
{
    CM_ARGB_GEN2_A1_CHANNEL_A           = 0x01,
    CM_ARGB_GEN2_A1_CHANNEL_B           = 0x02,
    CM_ARGB_GEN2_A1_CHANNEL_C           = 0x04,
    CM_ARGB_GEN2_A1_CHANNEL_ALL         = 0xFF,
    CM_ARGB_GEN2_A1_SUBCHANNEL_ALL      = 0xFF
};

class CMARGBGen2A1controller
{
public:
    CMARGBGen2A1controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~CMARGBGen2A1controller();

    std::string                 GetDeviceLocation();
    std::string                 GetNameString();
    std::string                 GetSerialString();

    void                        SendChannelColors(unsigned int zone_id, unsigned int subchannel_id, std::vector<RGBColor> colors);
    void                        SetupZoneSize(unsigned int zone_id, unsigned int size);
    void                        SetupDirectMode();
    void                        SetMode(unsigned int mode_value, unsigned char speed, unsigned char brightness, RGBColor color, bool random);
    void                        SetCustomColors(unsigned int zone_id, std::vector<RGBColor> colors);
    void                        SaveToFlash();

private:
    std::string                 location;
    std::string                 name;
    bool                        software_mode_activated = false;
    hid_device*                 dev;

    void                        SetCustomSequence(unsigned int zone_id);
    void                        SetPipelineStaticSequence(unsigned int zone_id);
    std::vector<unsigned char>  CreateColorData(std::vector<RGBColor> colors);
    void                        ResetDevice();
};
