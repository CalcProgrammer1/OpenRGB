/*---------------------------------------------------------*\
| ZotacV2GPUController.h                                    |
|                                                           |
|   Driver for Zotac V2 GPU                                 |
|                                                           |
|   Krzysztof Haładyn (krzys_h)                 16 Mar 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

enum
{
    ZOTAC_V2_GPU_REG_RGB = 0xA0,
};

enum
{
    ZOTAC_V2_GPU_CONFIG_IDLE   = 0x00,  // Config for when there is no load
    ZOTAC_V2_GPU_CONFIG_ACTIVE = 0x01,  // Config for when GPU is under load
};

enum
{
    ZOTAC_V2_GPU_SYNC_INDIVIDUAL                 = 0x00,  // Everything separated
    ZOTAC_V2_GPU_SYNC_SYNCHRONIZED               = 0x01,  // All internal zones synchronized, external is separated
    ZOTAC_V2_GPU_SYNC_SYNCHRONIZED_WITH_EXTERNAL = 0x02,  // Everything synchronized
};

enum
{
    ZOTAC_V2_GPU_MODE_STATIC  = 0x00,  // Basic static color
    ZOTAC_V2_GPU_MODE_BREATH  = 0x01,  // Single color fades on and off
    ZOTAC_V2_GPU_MODE_FADE    = 0x02,  // All colors fade through the spectrum
    ZOTAC_V2_GPU_MODE_WINK    = 0x03,  // Single color flashes on and off
    ZOTAC_V2_GPU_MODE_FLASH   = 0x04,  // Each zone flashes a different color (only supported in SYNCHRONIZED or SYNCHRONIZED_WITH_EXTERNAL mode)
    ZOTAC_V2_GPU_MODE_SHINE   = 0x05,  // (Sound activated) Single color, on and off
    ZOTAC_V2_GPU_MODE_RANDOM  = 0x06,  // Single color, random patern
    ZOTAC_V2_GPU_MODE_SLIDE   = 0x07,  // Single color, moves one side to the other
    ZOTAC_V2_GPU_MODE_RAINBOW = 0x08,  // All colors move one side to the other
    ZOTAC_V2_GPU_MODE_MARQUEE = 0x09,  // Very similar to SLIDE effect
    ZOTAC_V2_GPU_MODE_DRIP    = 0x0A,  // Similar to SLIDE as well, less color moves
    ZOTAC_V2_GPU_MODE_DANCE   = 0x0B,  // (Sound activated) Single color, equalizer effect
    ZOTAC_V2_GPU_MODE_DUET    = 0x17,  // Dual colors
    ZOTAC_V2_GPU_MODE_PATH    = 0x18,  // Very similar to RAINBOW effect
};

enum
{
    ZOTAC_V2_GPU_DIR_LEFT  = 0x00,
    ZOTAC_V2_GPU_DIR_RIGHT = 0x01,
};

enum
{
    ZOTAC_V2_GPU_CIRCUIT_ON  = 0x00,
    ZOTAC_V2_GPU_CIRCUIT_OFF = 0x01,
};

struct ZotacV2GPUConfig
{
    int numberOfZones = 0;
    bool supportsExternalLEDStrip = false;
};

struct ZotacV2GPUZone
{
    int mode = 0;
    RGBColor color1 = ToRGBColor(0, 0, 0);
    RGBColor color2 = ToRGBColor(0, 0, 0);
    unsigned int colorPreset = 0;
    unsigned int speed = 0;
    unsigned int brightness = 0;
    unsigned int direction = 0;
    unsigned int circuit = 0;
};


class ZotacV2GPUController
{
public:
    ZotacV2GPUController(i2c_smbus_interface* bus, u8 dev, std::string dev_name);
    ~ZotacV2GPUController();

    std::string             GetDeviceLocation();
    std::string             GetName();
    std::string             GetVersion();

    bool                    TurnOnOff(bool on);
    bool                    ResetToDefaults();
    bool                    GetMode(int zone, int idleActive, int& syncMode, ZotacV2GPUZone& zoneConfig, bool& on);
    bool                    SetMode(int zone, int idleActive, int syncMode, ZotacV2GPUZone zoneConfig);

private:
    i2c_smbus_interface*    bus;
    u8                      dev;
    std::string             name;
    std::string             version;

    bool                    ReadVersion();
    bool                    SendCommand(bool on, bool reset, int zone, int idleActive, int syncMode, ZotacV2GPUZone zoneConfig);
    bool                    ParseCommand(bool& on, bool& reset, int& zone, int& idleActive, int& syncMode, ZotacV2GPUZone& zoneConfig);
};
