/*---------------------------------------------------------*\
| ManliGPUController.cpp                                    |
|                                                           |
|   Driver for Manli GPU RGB controllers                   |
|                                                           |
|   Based on ZotacV2GPUController                           |
|   Adapted for Manli RTX 4090 Gallardo                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "ManliGPUController.h"

ManliGPUController::ManliGPUController(i2c_smbus_interface* bus, u8 dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;

    if(dev)
    {
        ReadVersion();
    }
}

ManliGPUController::~ManliGPUController()
{
}

std::string ManliGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return ("I2C: " + return_string);
}

std::string ManliGPUController::GetName()
{
    return(name);
}

std::string ManliGPUController::GetVersion()
{
    return(version);
}

bool ManliGPUController::ReadVersion()
{
    u8 data_pkt[] = { MANLI_GPU_REG_RGB, 0xF1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    if(bus->i2c_write_block(dev, sizeof(data_pkt), data_pkt) < 0)
    {
        return false;
    }

    u8 rdata_pkt[I2C_SMBUS_BLOCK_MAX] = { 0x00 };
    int rdata_len = sizeof(rdata_pkt);
    if(bus->i2c_read_block(dev, &rdata_len, rdata_pkt) < 0)
    {
        return false;
    }

    version = std::string((char*)rdata_pkt);
    return true;
}

bool ManliGPUController::TurnOnOff(bool on)
{
    ManliGPUZone zoneConfig;
    return SendCommand(on, zoneConfig);
}

bool ManliGPUController::SetMode(ManliGPUZone zoneConfig)
{
    return SendCommand(true, zoneConfig);
}

bool ManliGPUController::SendCommand(bool on, ManliGPUZone zoneConfig)
{
    /*---------------------------------------------------------*\
    | Color Cycle: Uses breathing mode (0x01) with flag 0x07    |
    | This cycles through colors with brightness and speed      |
    \*---------------------------------------------------------*/
    if(zoneConfig.mode == MANLI_GPU_MODE_COLOR_CYCLE)
    {
        u8 data_pkt[30] = { 0x00 };
        data_pkt[0]  = on ? (u8)0x01 : (u8)0x00;
        data_pkt[6]  = 0x01;  // mode = breathing
        data_pkt[7]  = 0x00;  // R
        data_pkt[8]  = 0x00;  // G
        data_pkt[9]  = 0x00;  // B
        data_pkt[10] = (u8)zoneConfig.speed;
        data_pkt[11] = (u8)zoneConfig.brightness;
        data_pkt[13] = 0x07;  // color cycle flag

        if(bus->i2c_smbus_write_i2c_block_data(dev, MANLI_GPU_REG_RGB, sizeof(data_pkt), data_pkt) < 0)
        {
            return false;
        }
        return true;
    }
    /*---------------------------------------------------------*\
    | Standard modes (Static, Breathing, Wave, Strobing, Rainbow)|
    \*---------------------------------------------------------*/
    else
    {
        u8 data_pkt[30] = { 0x00 };
        data_pkt[0]  = on ? (u8)0x01 : (u8)0x00;
        data_pkt[6]  = (u8)zoneConfig.mode;
        data_pkt[7]  = (u8)RGBGetRValue(zoneConfig.color1);
        data_pkt[8]  = (u8)RGBGetGValue(zoneConfig.color1);
        data_pkt[9]  = (u8)RGBGetBValue(zoneConfig.color1);
        data_pkt[10] = (u8)zoneConfig.speed;
        data_pkt[11] = (u8)zoneConfig.brightness;

        if(bus->i2c_smbus_write_i2c_block_data(dev, MANLI_GPU_REG_RGB, sizeof(data_pkt), data_pkt) < 0)
        {
            return false;
        }
        return true;
    }
}
