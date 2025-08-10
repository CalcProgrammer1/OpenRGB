/*---------------------------------------------------------*\
| GigabyteRGBFusionGPUController.cpp                        |
|                                                           |
|   Driver for Gigabyte Aorus RGB Fusion GPU                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                20 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "GigabyteRGBFusionGPUController.h"

RGBFusionGPUController::RGBFusionGPUController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

RGBFusionGPUController::~RGBFusionGPUController()
{

}

std::string RGBFusionGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string RGBFusionGPUController::GetDeviceName()
{
    return(name);
}

void RGBFusionGPUController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte(dev, RGB_FUSION_GPU_REG_COLOR);
    bus->i2c_smbus_write_byte(dev, red);
    bus->i2c_smbus_write_byte(dev, green);
    bus->i2c_smbus_write_byte(dev, blue);

    /*-----------------------------------------------------*\
    | Pad commands with 4 zero-bytes for NVIDIA_RTX3060_DEV |
    \*-----------------------------------------------------*/
    if(dev == 0x62)
    {
        bus->i2c_smbus_write_byte(dev, 0x00);
        bus->i2c_smbus_write_byte(dev, 0x00);
        bus->i2c_smbus_write_byte(dev, 0x00);
        bus->i2c_smbus_write_byte(dev, 0x00);
    }
}

void RGBFusionGPUController::SetMode(unsigned char mode, unsigned char speed, unsigned char brightness)
{
    bus->i2c_smbus_write_byte(dev, RGB_FUSION_GPU_REG_MODE);
    bus->i2c_smbus_write_byte(dev, mode);
    bus->i2c_smbus_write_byte(dev, speed);
    bus->i2c_smbus_write_byte(dev, brightness);

    /*-----------------------------------------------------*\
    | Pad commands with 4 zero-bytes for NVIDIA_RTX3060_DEV |
    \*-----------------------------------------------------*/
    if(dev == 0x62)
    {
        bus->i2c_smbus_write_byte(dev, 0x00);
        bus->i2c_smbus_write_byte(dev, 0x00);
        bus->i2c_smbus_write_byte(dev, 0x00);
        bus->i2c_smbus_write_byte(dev, 0x00);
    }
}

void RGBFusionGPUController::Save()
{
    bus->i2c_smbus_write_byte(dev, RGB_FUSION_GPU_REG_SAVE);
    bus->i2c_smbus_write_byte(dev, 0x00);
    bus->i2c_smbus_write_byte(dev, 0x00);
    bus->i2c_smbus_write_byte(dev, 0x00);

    /*-----------------------------------------------------*\
    | Pad commands with 4 zero-bytes for NVIDIA_RTX3060_DEV |
    \*-----------------------------------------------------*/
    if(dev == 0x62)
    {
        bus->i2c_smbus_write_byte(dev, 0x00);
        bus->i2c_smbus_write_byte(dev, 0x00);
        bus->i2c_smbus_write_byte(dev, 0x00);
        bus->i2c_smbus_write_byte(dev, 0x00);
    }
}
