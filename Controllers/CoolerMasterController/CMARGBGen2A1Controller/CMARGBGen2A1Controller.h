/*---------------------------------------------------------*\
| CMARGBGen2A1Controller.h                                  |
|                                                           |
|   Driver for Cooler Master ARGB Gen 2 A1 controller       |
|                                                           |
|   Morgan Guimard (morg)                       26 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define CM_ARGB_GEN2_A1_PACKET_LENGTH 65
#define CM_ARGB_GEN2_A1_CHANNEL_MAX_SIZE 48
#define CM_ARGB_GEN2_A1_CHANNEL_COUNT 3
#define CM_ARGB_GEN2_A1_PACKETS_PER_CHANNEL 3

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
    CM_ARGB_GEN2_A1_SPEED_MIN           = 0x00,
};

enum
{
    CM_ARGB_GEN2_A1_COMMAND             = 0x80,
    CM_ARGB_GEN2_A1_READ                = 0x01,
    CM_ARGB_GEN2_A1_WRITE               = 0x02,
    CM_ARGB_GEN2_A1_RESPONSE            = 0x03
};

enum
{
    CM_ARGB_GEN2_A1_SIZES               = 0x06,
    CM_ARGB_GEN2_A1_FLASH               = 0x0B,
    CM_ARGB_GEN2_A1_IDENTIFY            = 0x0A,
    CM_ARGB_GEN2_A1_LIGHTNING_CONTROL   = 0x01,
    CM_ARGB_GEN2_A1_HW_MODE_SETUP       = 0x03,
    CM_ARGB_GEN2_A1_CUSTOM_SEQUENCES    = 0x10,
    CM_ARGB_GEN2_A1_CUSTOM_SPEED        = 0x11

};

class CMARGBGen2A1controller
{
public:
    CMARGBGen2A1controller(hid_device* dev_handle, const hid_device_info& info);
    ~CMARGBGen2A1controller();

    std::string                 GetSerialString();
    std::string                 GetDeviceLocation();

    void                        SendDirectChannel(unsigned int zone_id, std::vector<RGBColor> colors);
    void                        SetupZoneSize(unsigned int zone_id, unsigned int size);
    void                        SetupDirectMode();
    void                        SetMode(unsigned int mode_value, unsigned char speed, unsigned char brightness, RGBColor color, bool random);
    void                        SetCustomColors(unsigned int zone_id, std::vector<RGBColor> colors);
    void                        SaveToFlash();

private:
    std::string                 location;
    bool                        software_mode_activated = false;
    hid_device*                 dev;

    void                        SetCustomSequence(unsigned int zone_id);
    std::vector<unsigned char>  CreateColorData(std::vector<RGBColor> colors);
};
