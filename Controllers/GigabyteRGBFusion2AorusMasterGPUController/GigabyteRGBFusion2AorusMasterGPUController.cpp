/*---------------------------------------------------------*\
| GigabyteRGBFusion2AorusMasterGPUController.cpp            |
|                                                           |
|   Driver for Gigabyte AORUS MASTER GPU with fan ring LEDs |
|                                                           |
|   Protocol reverse engineered from GCC on Windows         |
|   Verified on Linux with i2ctransfer 2025-12-30           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "GigabyteRGBFusion2AorusMasterGPUController.h"
#include "LogManager.h"
#include <chrono>
#include <thread>

RGBFusion2AorusMasterGPUController::RGBFusion2AorusMasterGPUController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

RGBFusion2AorusMasterGPUController::~RGBFusion2AorusMasterGPUController()
{
}

std::string RGBFusion2AorusMasterGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string RGBFusion2AorusMasterGPUController::GetDeviceName()
{
    return(name);
}

/*---------------------------------------------------------*\
| Send mode command to select a fan zone                    |
| Format: 88 01 06 63 08 <zone_id> 00 00                    |
|   Byte 0: Mode register (0x88)                            |
|   Byte 1: Static mode (0x01)                              |
|   Byte 2: Unknown constant (0x06)                         |
|   Byte 3: Brightness (0x00-0x63)                          |
|   Byte 4: Unknown constant (0x08)                         |
|   Byte 5: Zone selector (0x02/0x05/0x06)                  |
|   Byte 6-7: Padding (0x00)                                |
\*---------------------------------------------------------*/
void RGBFusion2AorusMasterGPUController::SendModeCommand(uint8_t zone_id, uint8_t brightness)
{
    uint8_t data_pkt[8] = {
        RGB_FUSION2_AORUS_MASTER_REG_MODE,
        RGB_FUSION2_AORUS_MASTER_MODE_STATIC,
        0x06,
        brightness,
        0x08,
        zone_id,
        0x00,
        0x00
    };

    bus->i2c_write_block(dev, sizeof(data_pkt), data_pkt);
}

/*---------------------------------------------------------*\
| Send color to a register (controls 2 LEDs)                |
| Format: <reg> <mode> R1 G1 B1 R2 G2 B2                    |
|   Byte 0: Register (0xB0-0xBC)                            |
|   Byte 1: Mode byte (0x01 for fans, 0x00 for logo)        |
|   Byte 2-4: First LED RGB                                 |
|   Byte 5-7: Second LED RGB                                |
\*---------------------------------------------------------*/
void RGBFusion2AorusMasterGPUController::SendColorRegister(uint8_t reg, uint8_t mode_byte, RGBColor color1, RGBColor color2)
{
    uint8_t data_pkt[8] = {
        reg,
        mode_byte,
        (uint8_t)RGBGetRValue(color1),
        (uint8_t)RGBGetGValue(color1),
        (uint8_t)RGBGetBValue(color1),
        (uint8_t)RGBGetRValue(color2),
        (uint8_t)RGBGetGValue(color2),
        (uint8_t)RGBGetBValue(color2)
    };

    bus->i2c_write_block(dev, sizeof(data_pkt), data_pkt);
}

/*---------------------------------------------------------*\
| Set colors for a fan zone (8 LEDs = 4 registers)          |
| Must send mode command before color commands              |
|                                                           |
| fan_index: 0=left, 1=middle, 2=right                      |
| colors: Array of 8 RGB colors for the 8 LEDs              |
\*---------------------------------------------------------*/
void RGBFusion2AorusMasterGPUController::SetFanColors(uint8_t fan_index, RGBColor colors[8], uint8_t brightness)
{
    uint8_t zone_id;
    uint8_t base_reg;

    switch(fan_index)
    {
        case 0:
            zone_id  = RGB_FUSION2_AORUS_MASTER_ZONE_LEFT;
            base_reg = RGB_FUSION2_AORUS_MASTER_REG_LEFT_FAN;
            break;

        case 1:
            zone_id  = RGB_FUSION2_AORUS_MASTER_ZONE_MIDDLE;
            base_reg = RGB_FUSION2_AORUS_MASTER_REG_MIDDLE_FAN;
            break;

        case 2:
            zone_id  = RGB_FUSION2_AORUS_MASTER_ZONE_RIGHT;
            base_reg = RGB_FUSION2_AORUS_MASTER_REG_RIGHT_FAN;
            break;

        default:
            LOG_ERROR("[%s] Invalid fan index: %d", name.c_str(), fan_index);
            return;
    }

    /*---------------------------------------------------------*\
    | CRITICAL: Send mode command with zone ID first            |
    | Without this, color commands have no effect               |
    \*---------------------------------------------------------*/
    SendModeCommand(zone_id, brightness);

    /*---------------------------------------------------------*\
    | Delay after mode command - firmware needs time to process |
    | the zone selection before accepting color data            |
    \*---------------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    /*---------------------------------------------------------*\
    | Send 4 color registers (2 LEDs each)                      |
    | Register Bx+0: LEDs 0,1 (colors[0], colors[1])            |
    | Register Bx+1: LEDs 2,3 (colors[2], colors[3])            |
    | Register Bx+2: LEDs 4,5 (colors[4], colors[5])            |
    | Register Bx+3: LEDs 6,7 (colors[6], colors[7])            |
    \*---------------------------------------------------------*/
    for(int reg_offset = 0; reg_offset < 4; reg_offset++)
    {
        uint8_t reg = base_reg + reg_offset;
        int color_idx = reg_offset * 2;

        SendColorRegister(reg, 0x01, colors[color_idx], colors[color_idx + 1]);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    /*---------------------------------------------------------*\
    | Apply after each zone - required for multi-zone update    |
    \*---------------------------------------------------------*/
    ApplyChanges();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

/*---------------------------------------------------------*\
| Set logo color                                            |
| Format: BC 00 R G B 00 00 00                              |
| Note: Logo uses mode byte 0x00, not 0x01                  |
\*---------------------------------------------------------*/
void RGBFusion2AorusMasterGPUController::SetLogoColor(RGBColor color)
{
    uint8_t data_pkt[8] = {
        RGB_FUSION2_AORUS_MASTER_REG_LOGO,
        0x00,
        (uint8_t)RGBGetRValue(color),
        (uint8_t)RGBGetGValue(color),
        (uint8_t)RGBGetBValue(color),
        0x00,
        0x00,
        0x00
    };

    bus->i2c_write_block(dev, sizeof(data_pkt), data_pkt);
}

/*---------------------------------------------------------*\
| Apply all pending changes                                 |
| Format: AA 00 00 00 00 00 00 00                           |
\*---------------------------------------------------------*/
void RGBFusion2AorusMasterGPUController::ApplyChanges()
{
    uint8_t data_pkt[8] = {
        RGB_FUSION2_AORUS_MASTER_REG_APPLY,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };

    bus->i2c_write_block(dev, sizeof(data_pkt), data_pkt);
}
