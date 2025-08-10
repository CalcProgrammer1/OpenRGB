/*---------------------------------------------------------*\
| GigabyteRGBFusion2GPUController.cpp                       |
|                                                           |
|   Driver for Gigabyte Aorus RGB Fusion 2 GPU              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "GigabyteRGBFusion2GPUController.h"
#include "LogManager.h"

using namespace std::chrono_literals;

RGBFusion2GPUController::RGBFusion2GPUController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

RGBFusion2GPUController::~RGBFusion2GPUController()
{

}

std::string RGBFusion2GPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string RGBFusion2GPUController::GetDeviceName()
{
    return(name);
}

void RGBFusion2GPUController::SaveConfig()
{
    uint8_t data_pkt[8] = { 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bus->i2c_write_block(dev, sizeof(data_pkt), data_pkt);
}

void RGBFusion2GPUController::SetMode(uint8_t zone, uint8_t mode, fusion2_config zone_config, uint8_t mystery_flag)
{
    if(zone < 4)
    {
        this->zone_color[zone] = zone_config.colors[0];
    }

    uint8_t zone_pkt[8] = { RGB_FUSION2_GPU_REG_MODE, mode, zone_config.speed, zone_config.brightness, mystery_flag, (uint8_t)(zone + 1), 0x00, 0x00 };
    bus->i2c_write_block(dev, sizeof(zone_pkt), zone_pkt);

    uint8_t zone_pkt2[8] = { 0 };
    switch(zone)
    {
        case 0:
        case 1:
            zone_pkt2[0] = RGB_FUSION2_GPU_REG_COLOR_LEFT_MID;
            zone_pkt2[1] = mode;
            zone_pkt2[2] = (uint8_t)RGBGetRValue(this->zone_color[0]);
            zone_pkt2[3] = (uint8_t)RGBGetGValue(this->zone_color[0]);
            zone_pkt2[4] = (uint8_t)RGBGetBValue(this->zone_color[0]);
            zone_pkt2[5] = (uint8_t)RGBGetRValue(this->zone_color[1]);
            zone_pkt2[6] = (uint8_t)RGBGetGValue(this->zone_color[1]);
            zone_pkt2[7] = (uint8_t)RGBGetBValue(this->zone_color[1]);
            break;
        case 2:
            zone_pkt2[0] = RGB_FUSION2_GPU_REG_COLOR_RIGHT;
            zone_pkt2[1] = mode;
            zone_pkt2[2] = (uint8_t)RGBGetRValue(this->zone_color[2]);
            zone_pkt2[3] = (uint8_t)RGBGetGValue(this->zone_color[2]);
            zone_pkt2[4] = (uint8_t)RGBGetBValue(this->zone_color[2]);
            break;
        default:
            zone_pkt2[0] = RGB_FUSION2_GPU_REG_COLOR;
            zone_pkt2[1] = (uint8_t)RGBGetRValue(zone_config.colors[0]);
            zone_pkt2[2] = (uint8_t)RGBGetGValue(zone_config.colors[0]);
            zone_pkt2[3] = (uint8_t)RGBGetBValue(zone_config.colors[0]);
            zone_pkt2[4] = (uint8_t)(zone + 1);
            break;
    }
    bus->i2c_write_block(dev, sizeof(zone_pkt2), zone_pkt2);
}

void RGBFusion2GPUController::SetZone(uint8_t zone, uint8_t mode, fusion2_config zone_config)
{
    std::string mode_name;
    uint8_t mystery_flag = 0x00;

    switch(mode)
    {
        case RGB_FUSION2_GPU_MODE_STATIC:
            {
                SetMode(zone, mode, zone_config, mystery_flag);
            }
            break;

        case RGB_FUSION2_GPU_MODE_BREATHING:
            {
                zone_config.brightness = RGB_FUSION2_GPU_BRIGHTNESS_MAX;
                SetMode(zone, mode, zone_config, mystery_flag);
            }
            break;

        case RGB_FUSION2_GPU_MODE_COLOR_CYCLE:
            {
                uint8_t zone_pkt[8] = { RGB_FUSION2_GPU_REG_MODE, mode, zone_config.speed, zone_config.brightness, mystery_flag, (uint8_t)(zone + 1), 0x00, 0x00 };
                bus->i2c_write_block(dev, sizeof(zone_pkt), zone_pkt);
            }
            break;

        case RGB_FUSION2_GPU_MODE_GRADIENT:
            {
                mystery_flag = 0x08;
                SetMode(zone, mode, zone_config, mystery_flag);
            }
            break;

        case RGB_FUSION2_GPU_MODE_FLASHING:
            {
                SetMode(zone, mode, zone_config, mystery_flag);
            }
            break;

        case RGB_FUSION2_GPU_MODE_DUAL_FLASHING:
            {
                SetMode(zone, mode, zone_config, mystery_flag);
            }
            break;

        case RGB_FUSION2_GPU_MODE_WAVE:
            {
                SetMode(zone, mode, zone_config, mystery_flag);
            }
            break;

        case RGB_FUSION2_GPU_MODE_COLOR_SHIFT:
            {
                mystery_flag = zone_config.numberOfColors;
                uint8_t zone_pkt[8] = { RGB_FUSION2_GPU_REG_MODE, mode, zone_config.speed, zone_config.brightness, mystery_flag, (uint8_t)(zone + 1), 0x00, 0x00 };
                bus->i2c_write_block(dev, sizeof(zone_pkt), zone_pkt);

                uint8_t bank = (0xB0 + (zone * 4));
                uint8_t zone_pktA[8] = { (uint8_t)bank, mode, (uint8_t)RGBGetRValue(zone_config.colors[0]), (uint8_t)RGBGetGValue(zone_config.colors[0]), (uint8_t)RGBGetBValue(zone_config.colors[0]), (uint8_t)RGBGetRValue(zone_config.colors[1]), (uint8_t)RGBGetGValue(zone_config.colors[1]), (uint8_t)RGBGetBValue(zone_config.colors[1]) };
                bus->i2c_write_block(dev, sizeof(zone_pktA), zone_pktA);
                uint8_t zone_pktB[8] = { (uint8_t)(bank + 1), mode, (uint8_t)RGBGetRValue(zone_config.colors[2]), (uint8_t)RGBGetGValue(zone_config.colors[2]), (uint8_t)RGBGetBValue(zone_config.colors[2]), (uint8_t)RGBGetRValue(zone_config.colors[3]), (uint8_t)RGBGetGValue(zone_config.colors[3]), (uint8_t)RGBGetBValue(zone_config.colors[3]) };
                bus->i2c_write_block(dev, sizeof(zone_pktB), zone_pktB);
                uint8_t zone_pktC[8] = { (uint8_t)(bank + 2), mode, (uint8_t)RGBGetRValue(zone_config.colors[4]), (uint8_t)RGBGetGValue(zone_config.colors[4]), (uint8_t)RGBGetBValue(zone_config.colors[4]), (uint8_t)RGBGetRValue(zone_config.colors[5]), (uint8_t)RGBGetGValue(zone_config.colors[5]), (uint8_t)RGBGetBValue(zone_config.colors[5]) };
                bus->i2c_write_block(dev, sizeof(zone_pktC), zone_pktC);
                uint8_t zone_pktD[8] = { (uint8_t)(bank + 3), mode, (uint8_t)RGBGetRValue(zone_config.colors[6]), (uint8_t)RGBGetGValue(zone_config.colors[6]), (uint8_t)RGBGetBValue(zone_config.colors[6]), (uint8_t)RGBGetRValue(zone_config.colors[7]), (uint8_t)RGBGetGValue(zone_config.colors[7]), (uint8_t)RGBGetBValue(zone_config.colors[7]) };
                bus->i2c_write_block(dev, sizeof(zone_pktD), zone_pktD);
            }
            break;

        case RGB_FUSION2_GPU_MODE_TRICOLOR:
            {
                mystery_flag = 0x08;
                uint8_t zone_pkt[8] = { RGB_FUSION2_GPU_REG_MODE, mode, zone_config.speed, zone_config.brightness, mystery_flag, (uint8_t)(zone + 1), 0x00, 0x00 };
                bus->i2c_write_block(dev, sizeof(zone_pkt), zone_pkt);

                uint8_t bank = (0xB0 + (zone * 4));
                uint8_t zone_pktA[8] = { (uint8_t)bank, mode, (uint8_t)RGBGetRValue(zone_config.colors[0]), (uint8_t)RGBGetGValue(zone_config.colors[0]), (uint8_t)RGBGetBValue(zone_config.colors[0]), (uint8_t)RGBGetRValue(zone_config.colors[1]), (uint8_t)RGBGetGValue(zone_config.colors[1]), (uint8_t)RGBGetBValue(zone_config.colors[1]) };
                bus->i2c_write_block(dev, sizeof(zone_pktA), zone_pktA);
                uint8_t zone_pktB[8] = { (uint8_t)(bank + 1), mode, (uint8_t)RGBGetRValue(zone_config.colors[2]), (uint8_t)RGBGetGValue(zone_config.colors[2]), (uint8_t)RGBGetBValue(zone_config.colors[2]), (uint8_t)RGBGetRValue(zone_config.colors[3]), (uint8_t)RGBGetGValue(zone_config.colors[3]), (uint8_t)RGBGetBValue(zone_config.colors[3]) };
                bus->i2c_write_block(dev, sizeof(zone_pktB), zone_pktB);
                uint8_t zone_pktC[8] = { (uint8_t)(bank + 2), mode, (uint8_t)RGBGetRValue(zone_config.colors[4]), (uint8_t)RGBGetGValue(zone_config.colors[4]), (uint8_t)RGBGetBValue(zone_config.colors[4]), (uint8_t)RGBGetRValue(zone_config.colors[5]), (uint8_t)RGBGetGValue(zone_config.colors[5]), (uint8_t)RGBGetBValue(zone_config.colors[5]) };
                bus->i2c_write_block(dev, sizeof(zone_pktC), zone_pktC);
                uint8_t zone_pktD[8] = { (uint8_t)(bank + 3), mode, (uint8_t)RGBGetRValue(zone_config.colors[6]), (uint8_t)RGBGetGValue(zone_config.colors[6]), (uint8_t)RGBGetBValue(zone_config.colors[6]), (uint8_t)RGBGetRValue(zone_config.colors[7]), (uint8_t)RGBGetGValue(zone_config.colors[7]), (uint8_t)RGBGetBValue(zone_config.colors[7]) };
                bus->i2c_write_block(dev, sizeof(zone_pktD), zone_pktD);
            }
            break;

        default:
            {
                LOG_TRACE("[%s] Mode %02d not found", "fusion2 gpu", mode);
            }
            break;
    }
}
