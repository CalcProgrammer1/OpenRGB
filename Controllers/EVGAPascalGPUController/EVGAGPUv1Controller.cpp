/*---------------------------------------------------------*\
| EVGAGPUv1Controller.cpp                                   |
|                                                           |
|   Driver for EVGA V1 (Pascal) GPU                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "EVGAGPUv1Controller.h"

EVGAGPUv1Controller::EVGAGPUv1Controller(i2c_smbus_interface* bus, evga_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

EVGAGPUv1Controller::~EVGAGPUv1Controller()
{

}

std::string EVGAGPUv1Controller::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string EVGAGPUv1Controller::GetDeviceName()
{
    return(name);
}

unsigned char EVGAGPUv1Controller::GetMode()
{
    return(bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V1_REG_MODE));
}

unsigned char EVGAGPUv1Controller::GetRed()
{
    return(bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V1_REG_RED));
}

unsigned char EVGAGPUv1Controller::GetGreen()
{
    return(bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V1_REG_GREEN));
}

unsigned char EVGAGPUv1Controller::GetBlue()
{
    return(bus->i2c_smbus_read_byte_data(dev, EVGA_GPU_V1_REG_BLUE));
}

void EVGAGPUv1Controller::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V1_REG_RED,   red);
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V1_REG_GREEN, green);
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V1_REG_BLUE,  blue);
}

void EVGAGPUv1Controller::SetMode(unsigned char mode)
{
    bus->i2c_smbus_write_byte_data(dev, EVGA_GPU_V1_REG_MODE, mode);
}

void EVGAGPUv1Controller::SaveSettings()
{
    bus->i2c_smbus_write_byte_data(dev, 0x23,   0xE5);
}
