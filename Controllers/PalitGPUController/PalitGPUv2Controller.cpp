/*---------------------------------------------------------*\
| PalitGPUv2Controller.cpp                                  |
|                                                           |
|   Driver for Palit v2 GPU (merged from Gainward v2 and    |
|   PNY controllers)                                        |
|                                                           |
|   KundaPanda                                  04 Jan 2021 |
|   KendallMorgan                               17 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "PalitGPUv2Controller.h"

PalitGPUv2Controller::PalitGPUv2Controller(i2c_smbus_interface* bus, palit_gpu_dev_id dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;
}

PalitGPUv2Controller::~PalitGPUv2Controller()
{

}

std::string PalitGPUv2Controller::GetDeviceLocation()
{
    std::string return_string(bus->info.device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string PalitGPUv2Controller::GetDeviceName()
{
    return(name);
}

unsigned char PalitGPUv2Controller::GetLEDRed()
{
    return(bus->i2c_smbus_read_byte_data(dev, PALIT_V2_RED_REGISTER));
}

unsigned char PalitGPUv2Controller::GetLEDGreen()
{
    return(bus->i2c_smbus_read_byte_data(dev, PALIT_V2_GREEN_REGISTER));
}

unsigned char PalitGPUv2Controller::GetLEDBlue()
{
    return(bus->i2c_smbus_read_byte_data(dev, PALIT_V2_BLUE_REGISTER));
}

void PalitGPUv2Controller::WriteI2CData(u8 command, u8 length, u8* data)
{
    for (u8 i = 0; i < length; i++)
    {
        bus->i2c_smbus_write_byte_data(dev, command + i, data[i]);
    }
}

void PalitGPUv2Controller::SetLEDColors(unsigned char red, unsigned char green, unsigned char blue, unsigned char color_register)
{
    switch (color_register)
    {
        default:
            bus->i2c_smbus_write_byte_data(dev, PALIT_V2_RED_REGISTER, red);
            bus->i2c_smbus_write_byte_data(dev, PALIT_V2_GREEN_REGISTER, green);
            bus->i2c_smbus_write_byte_data(dev, PALIT_V2_BLUE_REGISTER, blue);
            break;
        case PALIT_V2_COLOR_REGISTER_SECONDARY:
            bus->i2c_smbus_write_byte_data(dev, PALIT_V2_RED_SECONDARY_REGISTER, red);
            bus->i2c_smbus_write_byte_data(dev, PALIT_V2_GREEN_SECONDARY_REGISTER, green);
            bus->i2c_smbus_write_byte_data(dev, PALIT_V2_BLUE_SECONDARY_REGISTER, blue);
            break;
        case PALIT_V2_COLOR_REGISTER_TERTIARY:
            bus->i2c_smbus_write_byte_data(dev, PALIT_V2_RED_TERTIARY_REGISTER, red);
            bus->i2c_smbus_write_byte_data(dev, PALIT_V2_GREEN_TERTIARY_REGISTER, green);
            bus->i2c_smbus_write_byte_data(dev, PALIT_V2_BLUE_TERTIARY_REGISTER, blue);
            break;
    }
}

void PalitGPUv2Controller::SetMode(unsigned char mode, unsigned char speed, unsigned char control_mode)
{
    bus->i2c_smbus_write_byte_data(dev, PALIT_V2_MODE_REGISTER, mode);
    bus->i2c_smbus_write_byte_data(dev, PALIT_V2_CONTROL_REGISTER, control_mode);
    bus->i2c_smbus_write_byte_data(dev, PALIT_V2_SPEED_REGISTER, speed);
}

void PalitGPUv2Controller::SetDirection(unsigned char direction)
{
    bus->i2c_smbus_write_byte_data(dev, PALIT_V2_MODE_DIRECTION_REGISTER, direction);
}

void PalitGPUv2Controller::SetBreathingSpeed(unsigned int speed)
{
    unsigned char lower = speed & 0xFF;
    unsigned char upper = (speed >> 2 * 4) & 0xFF;

    bus->i2c_smbus_write_byte_data(dev, PALIT_V2_BREATHE_SPEED_REGISTER_A, lower);
    bus->i2c_smbus_write_byte_data(dev, PALIT_V2_BREATHE_SPEED_SECONDARY_REGISTER_A, lower);
    bus->i2c_smbus_write_byte_data(dev, PALIT_V2_BREATHE_SPEED_REGISTER_B, upper);
    bus->i2c_smbus_write_byte_data(dev, PALIT_V2_BREATHE_SPEED_SECONDARY_REGISTER_B, upper);
}

void PalitGPUv2Controller::SetOff()
{
    bus->i2c_smbus_write_byte_data(dev, PALIT_V2_MODE_REGISTER, 0);
    bus->i2c_smbus_write_byte_data(dev, PALIT_V2_CONTROL_REGISTER, 0x00);
}

void PalitGPUv2Controller::SetCycle(unsigned char speed)
{
    u8 loop[] = {
        0x01,  // Direction
        0x00,  // ??
        speed, // Speed
        0x1F   // Somehow related to speed
    };
    loop[2] = speed;
    WriteI2CData(PALIT_V2_MODE_REGISTER, sizeof(loop), loop);
}

void PalitGPUv2Controller::SetStrobe(unsigned char r, unsigned char g, unsigned char b, unsigned char speed, unsigned char brightness)
{
    u8 strobe[] = {
        0x02,       // Strobe
        0x00,       // Rise speed
        speed,      // Cycle length/2
        0x00,       // Off delay
        r,          // R
        g,          // G
        b,          // B
        brightness, // Peak brightness
        0x05        // Fade speed
    };
    WriteI2CData(PALIT_V2_MODE_REGISTER, sizeof(strobe), strobe);
}

void PalitGPUv2Controller::SetDirect(unsigned char red, unsigned char green, unsigned char blue, unsigned char brightness)
{
    bus->i2c_smbus_write_byte_data(dev, PALIT_V2_MODE_REGISTER, 0);
    bus->i2c_smbus_write_byte_data(dev, PALIT_V2_CONTROL_REGISTER, 0x01);
    bus->i2c_smbus_write_byte_data(dev, PALIT_V2_RED_REGISTER, red);
    bus->i2c_smbus_write_byte_data(dev, PALIT_V2_BLUE_REGISTER, blue);
    bus->i2c_smbus_write_byte_data(dev, PALIT_V2_GREEN_REGISTER, green);
    bus->i2c_smbus_write_byte_data(dev, PALIT_V2_BRIGHTNESS_REGISTER, brightness);
}
