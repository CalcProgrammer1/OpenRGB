/*---------------------------------------------------------*\
| GigabyteRGBFusion2AorusMasterGPUController.h              |
|                                                           |
|   Driver for Gigabyte AORUS MASTER GPU with fan ring LEDs |
|                                                           |
|   Protocol reverse engineered from GCC on Windows         |
|   Fan LEDs only illuminate when fans are spinning         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

typedef unsigned char   rgb_fusion_dev_id;

/*---------------------------------------------------------*\
| Fan zone configuration                                    |
| Each fan has 8 LEDs controlled by 4 registers             |
| Each register controls 2 LEDs with dual RGB values        |
\*---------------------------------------------------------*/
struct aorus_master_zone_colors
{
    RGBColor    led_colors[8];  // 8 LEDs per fan (or 1 for logo)
};

/*---------------------------------------------------------*\
| Register definitions                                      |
\*---------------------------------------------------------*/
enum
{
    RGB_FUSION2_AORUS_MASTER_REG_MODE        = 0x88,
    RGB_FUSION2_AORUS_MASTER_REG_APPLY       = 0xAA,
    RGB_FUSION2_AORUS_MASTER_REG_LEFT_FAN    = 0xB0,  // B0-B3
    RGB_FUSION2_AORUS_MASTER_REG_MIDDLE_FAN  = 0xB4,  // B4-B7
    RGB_FUSION2_AORUS_MASTER_REG_RIGHT_FAN   = 0xB8,  // B8-BB
    RGB_FUSION2_AORUS_MASTER_REG_LOGO        = 0xBC,
};

/*---------------------------------------------------------*\
| Fan zone ID values for mode command (byte 5)              |
\*---------------------------------------------------------*/
enum
{
    RGB_FUSION2_AORUS_MASTER_ZONE_LEFT       = 0x02,  // IO side fan
    RGB_FUSION2_AORUS_MASTER_ZONE_MIDDLE     = 0x05,  // Center fan
    RGB_FUSION2_AORUS_MASTER_ZONE_RIGHT      = 0x06,  // Far side fan
};

/*---------------------------------------------------------*\
| Mode definitions                                          |
\*---------------------------------------------------------*/
enum
{
    RGB_FUSION2_AORUS_MASTER_MODE_STATIC     = 0x01,
};

/*---------------------------------------------------------*\
| Speed and brightness                                      |
\*---------------------------------------------------------*/
enum
{
    RGB_FUSION2_AORUS_MASTER_BRIGHTNESS_MIN  = 0x00,
    RGB_FUSION2_AORUS_MASTER_BRIGHTNESS_MAX  = 0x63,  // 99
};

/*---------------------------------------------------------*\
| Zone count: 3 fans x 8 LEDs + 1 logo = 25 zones           |
\*---------------------------------------------------------*/
#define RGB_FUSION_2_AORUS_MASTER_FAN_LEDS           8
#define RGB_FUSION_2_AORUS_MASTER_TOTAL_ZONES        25

class RGBFusion2AorusMasterGPUController
{
public:
    RGBFusion2AorusMasterGPUController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev, std::string dev_name);
    ~RGBFusion2AorusMasterGPUController();

    std::string     GetDeviceLocation();
    std::string     GetDeviceName();

    void            SetFanColors(uint8_t fan_index, RGBColor colors[8], uint8_t brightness);
    void            SetLogoColor(RGBColor color);
    void            ApplyChanges();

private:
    i2c_smbus_interface*    bus;
    rgb_fusion_dev_id       dev;
    std::string             name;

    void            SendModeCommand(uint8_t zone_id, uint8_t brightness);
    void            SendColorRegister(uint8_t reg, uint8_t mode_byte, RGBColor color1, RGBColor color2);
};
