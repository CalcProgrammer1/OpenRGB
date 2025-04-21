/*---------------------------------------------------------*\
| GigabyteRGBFusion2BlackwellGPUController.cpp              |
|                                                           |
|   Driver for Gigabyte RGB Fusion 2 Blackwell GPU          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "GigabyteRGBFusion2BlackwellGPUController.h"
#include "GigabyteRGBFusion2BlackwellGPUDefinitions.h"
#include "LogManager.h"

using namespace std::chrono_literals;

RGBFusion2BlackwellGPUController::RGBFusion2BlackwellGPUController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;
}

RGBFusion2BlackwellGPUController::~RGBFusion2BlackwellGPUController()
{

}

std::string RGBFusion2BlackwellGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

void RGBFusion2BlackwellGPUController::SaveConfig()
{
    uint8_t data_pkt[64] = { 0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bus->i2c_write_block(dev, sizeof(data_pkt), data_pkt);
}

void RGBFusion2BlackwellGPUController::SetMode(uint8_t zone, uint8_t mode, fusion2_config zone_config, uint8_t mystery_flag)
{
    if (zone < RGB_FUSION_2_BLACKWELL_GPU_NUMBER_OF_ZONES)
    {
        this->zone_color[zone] = zone_config.colors[0];
    }

    uint8_t zone_pkt[64] = {RGB_FUSION2_BLACKWELL_GPU_REG_COLOR, mystery_flag, mode, zone_config.speed, zone_config.brightness, RGBGetRValue(this->zone_color[zone]), RGBGetGValue(this->zone_color[zone]), RGBGetBValue(this->zone_color[zone]), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    bus->i2c_write_block(dev, sizeof(zone_pkt), zone_pkt);
}

void RGBFusion2BlackwellGPUController::SetZone(uint8_t zone, uint8_t mode, fusion2_config zone_config)
{
    std::string mode_name;
    uint8_t mystery_flag = 0x01;

    switch(mode)
    {
        case RGB_FUSION2_BLACKWELL_GPU_MODE_STATIC:
            {
                SetMode(zone, mode, zone_config, mystery_flag);
            }
            break;

        case RGB_FUSION2_BLACKWELL_GPU_MODE_BREATHING:
            {
                zone_config.brightness = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;
                SetMode(zone, mode, zone_config, mystery_flag);
            }
            break;

        case RGB_FUSION2_BLACKWELL_GPU_MODE_FLASHING:
            {
                SetMode(zone, mode, zone_config, mystery_flag);
            }
            break;

        case RGB_FUSION2_BLACKWELL_GPU_MODE_DUAL_FLASHING:
            {
                SetMode(zone, mode, zone_config, mystery_flag);
            }
            break;

        case RGB_FUSION2_BLACKWELL_GPU_MODE_COLOR_CYCLE:
            {
                SetMode(zone, mode, zone_config, mystery_flag);
            }
            break;

        default:
            {
                LOG_TRACE("[%s] Mode %02d not found", "fusion2 blackwell gpu", mode);
            }
            break;
    }
}
