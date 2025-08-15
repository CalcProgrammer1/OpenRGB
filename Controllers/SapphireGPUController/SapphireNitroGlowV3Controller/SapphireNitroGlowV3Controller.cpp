/*---------------------------------------------------------*\
| SapphireNitroGlowV3Controller.cpp                         |
|                                                           |
|   Driver for Sapphire Nitro Glow V3                       |
|                                                           |
|   K900                                        03 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "SapphireNitroGlowV3Controller.h"

SapphireNitroGlowV3Controller::SapphireNitroGlowV3Controller(i2c_smbus_interface* bus, sapphire_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

SapphireNitroGlowV3Controller::~SapphireNitroGlowV3Controller()
{

}

std::string SapphireNitroGlowV3Controller::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string SapphireNitroGlowV3Controller::GetDeviceName()
{
    return(name);
}

unsigned char SapphireNitroGlowV3Controller::GetRed()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_RED));
}

unsigned char SapphireNitroGlowV3Controller::GetGreen()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_GREEN));
}

unsigned char SapphireNitroGlowV3Controller::GetBlue()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_BLUE));
}

void SapphireNitroGlowV3Controller::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_RED,   red);
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_GREEN, green);
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_BLUE,  blue);
}

unsigned char SapphireNitroGlowV3Controller::GetMode()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_MODE));
}

void SapphireNitroGlowV3Controller::SetMode(unsigned char mode)
{
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_MODE, mode);
}

bool SapphireNitroGlowV3Controller::GetExternalControl()
{
    return((bool)bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_EXTERNAL_CONTROL));
}

void SapphireNitroGlowV3Controller::SetExternalControl(bool enabled)
{
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_EXTERNAL_CONTROL, (unsigned char)enabled);
}

unsigned char SapphireNitroGlowV3Controller::GetBrightness()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_BRIGHTNESS));
}

void SapphireNitroGlowV3Controller::SetBrightness(unsigned char brightness)
{
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_BRIGHTNESS, brightness);
}

unsigned char SapphireNitroGlowV3Controller::GetRainbowAnimationSpeed()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_RAINBOW_ANIMATION_SPEED));
}

void SapphireNitroGlowV3Controller::SetRainbowAnimationSpeed(unsigned char speed)
{
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_RAINBOW_ANIMATION_SPEED, speed);
}

unsigned char SapphireNitroGlowV3Controller::GetRunwayAnimationSpeed()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_RUNWAY_ANIMATION_SPEED));
}

void SapphireNitroGlowV3Controller::SetRunwayAnimationSpeed(unsigned char speed)
{
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_RUNWAY_ANIMATION_SPEED, speed);
}

unsigned char SapphireNitroGlowV3Controller::GetRunwayAnimationRepeatCount()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_RUNWAY_ANIMATION_REPEAT_COUNT));
}

void SapphireNitroGlowV3Controller::SetRunwayAnimationRepeatCount(unsigned char count)
{
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_RUNWAY_ANIMATION_REPEAT_COUNT, count);
}

unsigned char SapphireNitroGlowV3Controller::GetColorCycleAnimationSpeed()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_COLOR_CYCLE_ANIMATION_SPEED));
}

void SapphireNitroGlowV3Controller::SetColorCycleAnimationSpeed(unsigned char speed)
{
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_COLOR_CYCLE_ANIMATION_SPEED, speed);
}

unsigned char SapphireNitroGlowV3Controller::GetSerialAnimationSpeed()
{
    return(bus->i2c_smbus_read_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_SERIAL_ANIMATION_SPEED));
}

void SapphireNitroGlowV3Controller::SetSerialAnimationSpeed(unsigned char speed)
{
    bus->i2c_smbus_write_byte_data(dev, SAPPHIRE_NITRO_GLOW_V3_REG_SERIAL_ANIMATION_SPEED, speed);
}
