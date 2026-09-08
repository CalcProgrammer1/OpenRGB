/*---------------------------------------------------------*\
| PalitGPUv1Controller.cpp                                  |
|                                                           |
|   Driver for Palit v1 GPU                                 |
|                                                           |
|   TheRogueZeta                                05 Nov 2020 |
|   Manatsawin Hanmongkolchai                   11 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "PalitGPUv1Controller.h"

PalitGPUv1Controller::PalitGPUv1Controller(i2c_smbus_interface* bus, palit_gpu_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

PalitGPUv1Controller::~PalitGPUv1Controller()
{

}

std::string PalitGPUv1Controller::GetDeviceLocation()
{
    std::string return_string(bus->info.device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string PalitGPUv1Controller::GetDeviceName()
{
    return(name);
}

unsigned char PalitGPUv1Controller::GetLEDRed()
{
    /*-----------------------------------------------------*\
    | Read red color from register 0x03                     |
    \*-----------------------------------------------------*/
    return(bus->i2c_smbus_read_byte_data(dev, PALIT_V1_RED_REGISTER));
}

unsigned char PalitGPUv1Controller::GetLEDGreen()
{
    /*-----------------------------------------------------*\
    | Read green color from register 0x04                   |
    \*-----------------------------------------------------*/
    return(bus->i2c_smbus_read_byte_data(dev, PALIT_V1_RED_REGISTER + 1));
}

unsigned char PalitGPUv1Controller::GetLEDBlue()
{
    /*-----------------------------------------------------*\
    | Read blue color from register 0x05                    |
    \*-----------------------------------------------------*/
    return(bus->i2c_smbus_read_byte_data(dev, PALIT_V1_RED_REGISTER + 2));
}

void PalitGPUv1Controller::SetLEDColors(unsigned char red, unsigned char green, unsigned char blue)
{
    /*-----------------------------------------------------*\
    | Write RGB colors and control byte using individual    |
    | writes to registers 0x03, 0x04, 0x05, 0x06            |
    \*-----------------------------------------------------*/
    bus->i2c_smbus_write_byte_data(dev, PALIT_V1_RED_REGISTER, red);
    bus->i2c_smbus_write_byte_data(dev, PALIT_V1_RED_REGISTER + 1, green);
    bus->i2c_smbus_write_byte_data(dev, PALIT_V1_RED_REGISTER + 2, blue);
    bus->i2c_smbus_write_byte_data(dev, PALIT_V1_RED_REGISTER + 3, 0xFF);
}
