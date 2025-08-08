/*---------------------------------------------------------*\
| GalaxGPUv2Controller.cpp                                  |
|                                                           |
|   RGBController for Galax GPUs (Xtreme Tuner)             |
|                                                           |
|   Daniel Stuart  (daniel.stuart14)            26 may 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "GalaxGPUv2Controller.h"

GalaxGPUv2Controller::GalaxGPUv2Controller(i2c_smbus_interface* bus, galax_gpu_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

GalaxGPUv2Controller::~GalaxGPUv2Controller()
{

}

std::string GalaxGPUv2Controller::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string GalaxGPUv2Controller::GetDeviceName()
{
    return(name);
}

unsigned char GalaxGPUv2Controller::GetLEDRed()
{
    return(GalaxGPURegisterRead(GALAX_V2_RED_REGISTER));
}

unsigned char GalaxGPUv2Controller::GetLEDGreen()
{
    return(GalaxGPURegisterRead(GALAX_V2_GREEN_REGISTER));
}

unsigned char GalaxGPUv2Controller::GetLEDBlue()
{
    return(GalaxGPURegisterRead(GALAX_V2_BLUE_REGISTER));
}

unsigned char GalaxGPUv2Controller::GetMode()
{
    return GalaxGPURegisterRead(GALAX_V2_MODE_REGISTER);
}

unsigned char GalaxGPUv2Controller::GetSync()
{
    return GalaxGPURegisterRead(GALAX_V2_SYNC_REGISTER);
}

unsigned char GalaxGPUv2Controller::GetSpeed()
{
    return GalaxGPURegisterRead(GALAX_V2_SPEED_REGISTER_A);
}

unsigned char GalaxGPUv2Controller::GetBrightness()
{
    return GalaxGPURegisterRead(GALAX_V2_BRIGHTNESS_REGISTER);
}

void GalaxGPUv2Controller::SetLEDColors(unsigned char red, unsigned char green, unsigned char blue)
{
    GalaxGPURegisterWrite(GALAX_V2_RED_REGISTER, red);
    GalaxGPURegisterWrite(GALAX_V2_GREEN_REGISTER, green);
    GalaxGPURegisterWrite(GALAX_V2_BLUE_REGISTER, blue);
}

void GalaxGPUv2Controller::SetMode(unsigned char value)
{
    GalaxGPURegisterWrite(GALAX_V2_MODE_REGISTER, value);
}

void GalaxGPUv2Controller::SetSync(unsigned char value)
{
    GalaxGPURegisterWrite(GALAX_V2_SYNC_REGISTER, value);
}

void GalaxGPUv2Controller::SetSpeed(unsigned char value)
{
    // We just duplicate the value to both speed registers
    GalaxGPURegisterWrite(GALAX_V2_SPEED_REGISTER_A, value);
    GalaxGPURegisterWrite(GALAX_V2_SPEED_REGISTER_B, value);
}

void GalaxGPUv2Controller::SetBrightness(unsigned char value)
{
    GalaxGPURegisterWrite(GALAX_V2_BRIGHTNESS_REGISTER, value);
}

void GalaxGPUv2Controller::SaveMode()
{
    GalaxGPURegisterWrite(GALAX_V2_SAVE_REGISTER, GALAX_V2_SAVE_VALUE);
}

unsigned char GalaxGPUv2Controller::GalaxGPURegisterRead(unsigned char reg)
{
    return(bus->i2c_smbus_read_byte_data(dev, reg));
}

void GalaxGPUv2Controller::GalaxGPURegisterWrite(unsigned char reg, unsigned char val)
{
    bus->i2c_smbus_write_byte_data(dev, reg, val);
}
