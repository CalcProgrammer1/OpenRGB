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

RGBFusion2BlackwellGPUController::RGBFusion2BlackwellGPUController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
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

std::string RGBFusion2BlackwellGPUController::GetDeviceName()
{
    return(name);
}

void RGBFusion2BlackwellGPUController::SaveConfig()
{
    uint8_t data_pkt[64] = { 0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bus->i2c_write_block(dev, sizeof(data_pkt), data_pkt);
}

void RGBFusion2BlackwellGPUController::SetMode(uint8_t type, uint8_t zone, uint8_t mode, fusion2_config zone_config)
{
    if(zone_config.numberOfColors == 0 && zone < RGB_FUSION_2_BLACKWELL_GPU_NUMBER_OF_ZONES)
        this->zone_color[zone] = zone_config.colors[zone];

    /************************************************************************************\
    *                                                                                    *
    *       Packet (total size = 64 bytes)                                               *
    * TYPE      MODE SPD  BRT  R    G    B    0    ZONE SZ0-8                            *
    * 0x12 0x01 0x08 0x06 0x0A 0xFF 0xFF 0x00 0x00 0x00 0x08 [R] [G] [B] [R] [G] [B] ... *
    *                                                                                    *
    * SZ is the amount of colors that will be sent in the format of 3 bytes RGB          *
    *                                                                                    *
    \************************************************************************************/
    uint8_t zone_pkt[64] = {type, 0x01, mode, zone_config.speed, zone_config.brightness, (uint8_t)RGBGetRValue(this->zone_color[zone]), (uint8_t)RGBGetGValue(this->zone_color[zone]), (uint8_t)RGBGetBValue(this->zone_color[zone]), 0x00, zone, zone_config.numberOfColors, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    if(zone_config.numberOfColors > 0)
    {
        int currentPos = 12;
        for(uint8_t i = 0; i < zone_config.numberOfColors; i++)
        {
            zone_pkt[currentPos + 0] = RGBGetRValue(zone_config.colors[i]);
            zone_pkt[currentPos + 1] = RGBGetGValue(zone_config.colors[i]);
            zone_pkt[currentPos + 2] = RGBGetBValue(zone_config.colors[i]);
            currentPos += 3;
        }
    }

    bus->i2c_write_block(dev, sizeof(zone_pkt), zone_pkt);
}

void RGBFusion2BlackwellGPUController::SetZone(uint8_t zone, uint8_t mode, fusion2_config zone_config)
{
    if(mode == RGB_FUSION2_BLACKWELL_GPU_MODE_BREATHING)
        zone_config.brightness = RGB_FUSION2_BLACKWELL_GPU_BRIGHTNESS_MAX;

    uint8_t type = RGB_FUSION2_BLACKWELL_GPU_REG_COLOR;
    if(mode != RGB_FUSION2_BLACKWELL_GPU_MODE_DIRECT)
        type = RGB_FUSION2_BLACKWELL_GPU_REG_MODE;

    SetMode(type, zone, mode, zone_config);
}
