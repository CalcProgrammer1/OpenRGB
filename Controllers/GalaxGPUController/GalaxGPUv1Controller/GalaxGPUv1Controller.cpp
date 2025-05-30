/*---------------------------------------------------------*\
| GalaxGPUv1Controller.cpp                                  |
|                                                           |
|   Driver for Galax/KFA2 GPU                               |
|                                                           |
|   Niels Westphal (crashniels)                 12 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "GalaxGPUv1Controller.h"

GalaxGPUv1Controller::GalaxGPUv1Controller(i2c_smbus_interface* bus, galax_gpu_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    strcpy(device_name, "Galax RTX GPU");                                               // Would be nice to get the actual GPU name. Using this as a placeholder.
}

GalaxGPUv1Controller::~GalaxGPUv1Controller()
{

}

std::string GalaxGPUv1Controller::GetDeviceName()
{
    return(device_name);
}

std::string GalaxGPUv1Controller::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

unsigned char GalaxGPUv1Controller::GetLEDRed()
{
    return(GalaxGPURegisterRead(GALAX_V1_RED_REGISTER));
}

unsigned char GalaxGPUv1Controller::GetLEDGreen()
{
    return(GalaxGPURegisterRead(GALAX_V1_GREEN_REGISTER));
}

unsigned char GalaxGPUv1Controller::GetLEDBlue()
{
    return(GalaxGPURegisterRead(GALAX_V1_BLUE_REGISTER));
}

void GalaxGPUv1Controller::SetLEDColorsDirect(unsigned char red, unsigned char green, unsigned char blue)                  // Direct Mode is just Static Mode without applying color changes
{
    GalaxGPURegisterWrite(GALAX_V1_RED_REGISTER, red);
    GalaxGPURegisterWrite(GALAX_V1_GREEN_REGISTER, green);
    GalaxGPURegisterWrite(GALAX_V1_BLUE_REGISTER, blue);
}

void GalaxGPUv1Controller::SetLEDColorsEffect(unsigned char red, unsigned char green, unsigned char blue)
{
    GalaxGPURegisterWrite(GALAX_V1_RED_REGISTER, red);
    GalaxGPURegisterWrite(GALAX_V1_GREEN_REGISTER, green);
    GalaxGPURegisterWrite(GALAX_V1_BLUE_REGISTER, blue);
}

void GalaxGPUv1Controller::SetMode(unsigned char mode)
{
    switch(mode)
    {
    case 1:
        GalaxGPURegisterWrite(GALAX_V1_MODE_REGISTER_1, GALAX_V1_MODE_STATIC_VALUE_1);
        GalaxGPURegisterWrite(GALAX_V1_MODE_REGISTER_2, GALAX_V1_MODE_STATIC_VALUE_2);
        break;

    case 2:
        GalaxGPURegisterWrite(GALAX_V1_MODE_REGISTER_1, GALAX_V1_MODE_BREATHING_VALUE_1);
        GalaxGPURegisterWrite(GALAX_V1_MODE_REGISTER_2, GALAX_V1_MODE_BREATHING_VALUE_2);
        break;

    case 3:
        GalaxGPURegisterWrite(GALAX_V1_MODE_REGISTER_1, GALAX_V1_MODE_RAINBOW_VALUE_1);
        GalaxGPURegisterWrite(GALAX_V1_MODE_REGISTER_2, GALAX_V1_MODE_RAINBOW_VALUE_2);
        break;

    case 4:
        GalaxGPURegisterWrite(GALAX_V1_MODE_REGISTER_1, GALAX_V1_MODE_CYCLE_BREATHING_VALUE_1);
        GalaxGPURegisterWrite(GALAX_V1_MODE_REGISTER_2, GALAX_V1_MODE_CYCLE_BREATHING_VALUE_2);
        break;

    default:
        break;
    }
}

unsigned char GalaxGPUv1Controller::GalaxGPURegisterRead(unsigned char reg)
{
    return(bus->i2c_smbus_read_byte_data(dev, reg));
}

void GalaxGPUv1Controller::GalaxGPURegisterWrite(unsigned char reg, unsigned char val)
{
    bus->i2c_smbus_write_byte_data(dev, reg, val);
}
