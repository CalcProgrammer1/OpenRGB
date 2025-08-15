/*---------------------------------------------------------*\
| ZotacTuringGPUController.cpp                              |
|                                                           |
|   Driver for Zotac Turing GPU                             |
|                                                           |
|   David Henry                                 07 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "ZotacTuringGPUController.h"

ZotacTuringGPUController::ZotacTuringGPUController(i2c_smbus_interface* bus, u8 dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

ZotacTuringGPUController::~ZotacTuringGPUController()
{
}

std::string ZotacTuringGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string ZotacTuringGPUController::GetDeviceName()
{
    return(name);
}

void ZotacTuringGPUController::GetMode(RGBColor& color, int& mode, unsigned int& speed)
{
    u8 rdata_pkt[I2C_SMBUS_BLOCK_MAX] = { 0x00 };
    int rdata_len = sizeof(rdata_pkt);

    if(bus->i2c_read_block(dev, &rdata_len, rdata_pkt) >= 0)
    {
        mode  = rdata_pkt[0];
        color = ToRGBColor(rdata_pkt[1], rdata_pkt[2], rdata_pkt[3]);
        speed = rdata_pkt[5];
    }
}

void ZotacTuringGPUController::SetMode(RGBColor color, int mode, unsigned int speed)
{
    u8 data_pkt[] =
    {
        ZOTAC_TURING_GPU_REG_COLOR_AND_MODE,
        0x00, // Is it some zone index?
        (u8)mode,
        (u8)RGBGetRValue(color),
        (u8)RGBGetGValue(color),
        (u8)RGBGetBValue(color),
        0x00,
        (u8)speed
    };

    bus->i2c_write_block(dev, sizeof(data_pkt), data_pkt);

    /*---------------------------------------------------------*\
    | Read back color and mode. Not doing this seems to hang    |
    | the RGB controller device when switching mode...          |
    \*---------------------------------------------------------*/
    GetMode(color, mode, speed);
}
