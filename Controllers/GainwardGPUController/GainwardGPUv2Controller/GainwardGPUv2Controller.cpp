/*---------------------------------------------------------*\
| GainwardGPUv2Controller.cpp                               |
|                                                           |
|   Driver for Gainward v2 GPU                              |
|                                                           |
|   KundaPanda                                  04 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "GainwardGPUv2Controller.h"

GainwardGPUv2Controller::GainwardGPUv2Controller(i2c_smbus_interface* bus, gainward_gpu_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

GainwardGPUv2Controller::~GainwardGPUv2Controller() = default;

std::string GainwardGPUv2Controller::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string GainwardGPUv2Controller::GetDeviceName()
{
    return(name);
}

unsigned char GainwardGPUv2Controller::GetLEDRed()
{
    return(bus->i2c_smbus_read_byte_data(dev, GAINWARD_V2_RED_REGISTER));
}

unsigned char GainwardGPUv2Controller::GetLEDGreen()
{
    return(bus->i2c_smbus_read_byte_data(dev, GAINWARD_V2_GREEN_REGISTER));
}

unsigned char GainwardGPUv2Controller::GetLEDBlue()
{
    return(bus->i2c_smbus_read_byte_data(dev, GAINWARD_V2_BLUE_REGISTER));
}

void GainwardGPUv2Controller::SetLEDColors(unsigned char red, unsigned char green, unsigned char blue, unsigned char color_register)
{
    switch (color_register)
    {
        default:
            bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_RED_REGISTER, red);
            bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_GREEN_REGISTER, green);
            bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_BLUE_REGISTER, blue);
            break;
        case GAINWARD_V2_COLOR_REGISTER_SECONDARY:
            bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_RED_SECONDARY_REGISTER, red);
            bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_GREEN_SECONDARY_REGISTER, green);
            bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_BLUE_SECONDARY_REGISTER, blue);
            break;
        case GAINWARD_V2_COLOR_REGISTER_TERTIARY:
            bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_RED_TERTIARY_REGISTER, red);
            bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_GREEN_TERTIARY_REGISTER, green);
            bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_BLUE_TERTIARY_REGISTER, blue);
            break;
    }
}

void GainwardGPUv2Controller::SetMode(unsigned char mode, unsigned char speed, unsigned char static_mode)
{
    bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_MODE_REGISTER, mode);
    bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_STATIC_CONTROL_REGISTER, static_mode);
    bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_SPEED_REGISTER, speed);
}

void GainwardGPUv2Controller::SetDirection(unsigned char direction)
{
    bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_MODE_DIRECTION_REGISTER, direction);
}

void GainwardGPUv2Controller::SetBreathingSpeed(unsigned int speed)
{
    unsigned char lower = speed & 0xFF;
    unsigned char upper = (speed >> 2 * 4) & 0xFF;

    bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_BREATHE_SPEED_REGISTER_A, lower);
    bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_BREATHE_SPEED_SECONDARY_REGISTER_A, lower);
    bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_BREATHE_SPEED_REGISTER_B, upper);
    bus->i2c_smbus_write_byte_data(dev, GAINWARD_V2_BREATHE_SPEED_SECONDARY_REGISTER_B, upper);
}
