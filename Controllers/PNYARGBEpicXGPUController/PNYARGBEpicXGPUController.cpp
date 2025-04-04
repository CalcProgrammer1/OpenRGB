/*---------------------------------------------------------*\
| PNYARGBEpicXGPUController.cpp                             |
|                                                           |
|   Driver for PNY ARGB Epic-X GPU                          |
|                                                           |
|   Peter Berendi                               27 Apr 2025 |
|   Adam Honse <calcprogrammer1@gmail.com>      01 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "PNYARGBEpicXGPUController.h"

PNYARGBEpicXGPUController::PNYARGBEpicXGPUController(i2c_smbus_interface* bus, unsigned char init_i2c_addr, std::string name)
{
    this->bus       = bus;
    this->i2c_addr  = init_i2c_addr;
    this->name      = name;
}

PNYARGBEpicXGPUController::~PNYARGBEpicXGPUController()
{

}

std::string PNYARGBEpicXGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", i2c_addr);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string PNYARGBEpicXGPUController::GetDeviceName()
{
    return(name);
}

void PNYARGBEpicXGPUController::SetZoneMode(unsigned char zone, unsigned char mode, unsigned char speed, unsigned char brightness, unsigned char subcmd, RGBColor color)
{
    unsigned char data[7] = 
    {
        mode,
        brightness,
        speed,
        subcmd,
        (unsigned char)RGBGetRValue(color),
        (unsigned char)RGBGetGValue(color),
        (unsigned char)RGBGetBValue(color)
    };

    bus->i2c_smbus_write_i2c_block_data(i2c_addr, zone, sizeof(data), data);
}

void PNYARGBEpicXGPUController::SetLEDDirect(unsigned char zone, unsigned char led, unsigned char mode, RGBColor color)
{
    unsigned char data[7] =
    {
        mode,
        0xFF,
        led,
        0x00,
        (unsigned char)RGBGetRValue(color),
        (unsigned char)RGBGetGValue(color),
        (unsigned char)RGBGetBValue(color)
    };

    bus->i2c_smbus_write_i2c_block_data(i2c_addr, zone, sizeof(data), data);
}