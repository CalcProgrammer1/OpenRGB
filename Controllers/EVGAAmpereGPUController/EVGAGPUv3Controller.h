/*---------------------------------------------------------*\
| EVGAGPUv3Controller.h                                     |
|                                                           |
|   Driver for EVGA V3 (Ampere) GPU                         |
|                                                           |
|   TheRogueZeta                                15 Jul 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

typedef unsigned char	evga_dev_id;

#define SPEED_MULTIPLIER            10
#define EVGAGPUV3_MODE_PACKET_SIZE  10
#define EVGAGPUV3_LEDS_MIN          01
#define EVGAGPUV3_LEDS_MAX          60
#define EVGAGPUV3_INIT              0xFF
#define EVGAGPUV3_CONTROLLER_NAME   "EVGAv3"

union u16_to_u8
{
    uint16_t   u16;
    struct
    {
        uint8_t lsb;
        uint8_t msb;
    };
};

struct EVGAv3_config
{
    uint8_t     brightness;
    RGBColor    colors[7];
    uint8_t     numberOfColors;
    uint16_t    speed;
    uint8_t     direction;
};

enum //Control registers and offsets
{
    EVGA_GPU_V3_REG_FIRMWARE                = 0xB1,
    EVGA_GPU_V3_REG_ENABLE                  = 0xB2,
    EVGA_GPU_V3_REG_MODE                    = 0xC0,
    EVGA_GPU_V3_OFFSET_ZONE_1               = 0x00,
    EVGA_GPU_V3_OFFSET_ZONE_2               = 0x01,
    EVGA_GPU_V3_OFFSET_ZONE_3               = 0x02,
    EVGA_GPU_V3_OFFSET_ZONE_4               = 0x03,
    EVGA_GPU_V3_REG_STATIC                  = 0xC1,
    EVGA_GPU_V3_REG_BREATHING               = 0xC5,
    EVGA_GPU_V3_REG_RAINBOW                 = 0xC9,
    EVGA_GPU_V3_REG_COLOR_CYCLE             = 0xCD,
    EVGA_GPU_V3_REG_RAINBOW_WAVE            = 0xD1,
    EVGA_GPU_V3_REG_WAVE                    = 0xD5,
    EVGA_GPU_V3_REG_STAR                    = 0xD9,
    EVGA_GPU_V3_REG_COLOR_STACK             = 0xDD,
    EVGA_GPU_V3_REG_COLOR_CYCLE_COUNT       = 0xE5,
    EVGA_GPU_V3_REG_COLOR_STACK_COLOR_COUNT = 0xE6,
    EVGA_GPU_V3_REG_COLOR_STACK_DIRECTION   = 0xEB,
    EVGA_GPU_V3_REG_SAVE                    = 0x90,
};

enum //Mode values for EVGA_GPU_V3_REG_MODE
{
    EVGA_GPU_V3_MODE_OFF                    = 0x00,
    EVGA_GPU_V3_MODE_STATIC                 = 0x01,
    EVGA_GPU_V3_MODE_BREATHING              = 0x02,
    EVGA_GPU_V3_MODE_RAINBOW                = 0x03,
    EVGA_GPU_V3_MODE_COLOR_CYCLE            = 0x04,
    EVGA_GPU_V3_MODE_RAINBOW_WAVE           = 0x05,
    EVGA_GPU_V3_MODE_WAVE                   = 0x06,
    EVGA_GPU_V3_MODE_STAR                   = 0x07,
    EVGA_GPU_V3_MODE_COLOR_STACK            = 0x08,
};

enum // Value limits for speeds
{
    EVGA_GPU_V3_BRIGHTNESS_MIN              = 0x0,
    EVGA_GPU_V3_BRIGHTNESS_DEFAULT          = 0xFF,
    EVGA_GPU_V3_BRIGHTNESS_MAX              = 0xFF,
    EVGA_GPU_V3_SPEED_GENERIC_SLOWEST       = 0x4E20, //20000
    EVGA_GPU_V3_SPEED_GENERIC_NORMAL        = 0x1388, //5000
    EVGA_GPU_V3_SPEED_GENERIC_FASTEST       = 0x03E8, //1000
    EVGA_GPU_V3_SPEED_WAVE_SLOWEST          = 0x0148, //355
    EVGA_GPU_V3_SPEED_WAVE_NORMAL           = 0x0028, //40
    EVGA_GPU_V3_SPEED_WAVE_FASTEST          = 0x000A, //10
    EVGA_GPU_V3_SPEED_STAR_SLOWEST          = 0x2710, //10000
    EVGA_GPU_V3_SPEED_STAR_NORMAL           = 0x07D0, //2000
    EVGA_GPU_V3_SPEED_STAR_FASTEST          = 0x01F4, //500
};

class EVGAGPUv3Controller
{
public:
    EVGAGPUv3Controller(i2c_smbus_interface* bus, evga_dev_id dev, std::string dev_name);
    ~EVGAGPUv3Controller();

    uint8_t         zone_led_count[4];
    uint8_t         zone_modes[4];

    std::string     GetDeviceLocation();
    std::string     GetDeviceName();
    std::string     GetFWVersion();
    std::string     ReadFWVersion();

    void            GetDeviceModes();
    uint8_t         GetZoneMode(uint8_t zone);
    EVGAv3_config   GetZoneConfig(uint8_t zone, uint8_t mode);
    void            SaveConfig();

    void            ResizeARGB(uint8_t newSize);
    void            SetAllModes(uint8_t zone0, uint8_t zone1, uint8_t zone2, uint8_t zone3, bool sync);
    void            SetZoneMode(uint8_t zone, uint8_t mode);
    void            SetZone(uint8_t zone, uint8_t mode, EVGAv3_config zone_config);

private:
    i2c_smbus_interface*    bus;
    evga_dev_id             dev;
    bool                    zone_sync;
    std::string             fwVersion;
    std::string             name;

    void                    initCard();
};
