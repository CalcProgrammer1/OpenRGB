/*---------------------------------------------------------*\
| ColorfulGPUController.cpp                                 |
|                                                           |
|   Driver for Colorful GPU                                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "ColorfulGPUController.h"
#include "pci_ids.h"
#include "LogManager.h"

ColorfulGPUController::ColorfulGPUController(i2c_smbus_interface* bus, colorful_gpu_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

ColorfulGPUController::~ColorfulGPUController()
{

}

std::string ColorfulGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string ColorfulGPUController::GetDeviceName()
{
    return(name);
}

void ColorfulGPUController::SetDirect(RGBColor color)
{
    uint8_t r = RGBGetRValue(color);
    uint8_t g = RGBGetGValue(color);
    uint8_t b = RGBGetBValue(color);

    if(this->bus->pci_subsystem_device == COLORFUL_IGAME_RTX_4070_VULCAN_OCV)
    {
        uint8_t data_pkt[COLORFUL_PACKET_LENGTH_V2] = { 0xAA, 0xEF, 0x01, 0x04, 0x88, 0x26 };
        for(int i=6; i < COLORFUL_PACKET_LENGTH_V2 -2; i = i + 3)
        {
            data_pkt[i] = r;
            data_pkt[i+1] = g;
            data_pkt[i+2] = b;
        }

        int crc = 0;
        for(int i = 0; i < COLORFUL_PACKET_LENGTH_V2 - 2; ++i)
        {
            crc += data_pkt[i];
        }

        data_pkt[COLORFUL_PACKET_LENGTH_V2 - 2] = crc & 0xFF;
        data_pkt[COLORFUL_PACKET_LENGTH_V2 - 1] = crc >> 8;

        bus->i2c_write_block(dev, COLORFUL_PACKET_LENGTH_V2, data_pkt);
    }
    else
    {
        uint8_t data_pkt[COLORFUL_PACKET_LENGTH_V1] = { 0xAA, 0xEF, 0x12, 0x03, 0x01, 0xFF, r, g, b};

        int crc = 0;
        for(int i = 0; i < COLORFUL_PACKET_LENGTH_V1 - 2; ++i)
        {
            crc += data_pkt[i];
        }

        data_pkt[COLORFUL_PACKET_LENGTH_V1 - 2] = crc & 0xFF;
        data_pkt[COLORFUL_PACKET_LENGTH_V1 - 1] = crc >> 8;

        bus->i2c_write_block(dev, COLORFUL_PACKET_LENGTH_V1, data_pkt);
    }
}
