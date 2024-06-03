/*---------------------------------------------------------*\
| PatriotViperSteelController.cpp                           |
|                                                           |
|   Driver for Patriot Viper Steel RAM                      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "PatriotViperSteelController.h"

PatriotViperSteelController::PatriotViperSteelController(i2c_smbus_interface *bus, viper_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    strcpy(device_name, "Patriot Viper Steel RGB");

    led_count = 5;
}

PatriotViperSteelController::~PatriotViperSteelController()
{
}

std::string PatriotViperSteelController::GetDeviceName()
{
    return(device_name);
}

std::string PatriotViperSteelController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

unsigned int PatriotViperSteelController::GetLEDCount()
{
    return(led_count);
}

unsigned int PatriotViperSteelController::GetSlotCount()
{
    unsigned int slot_count = 0;

    for(int slot = 0; slot < 4; slot++)
    {
        if((slots_valid & (1 << slot)) != 0)
        {
            slot_count++;
        }
    }

    return(slot_count);
}

unsigned int PatriotViperSteelController::GetMode()
{
    return(mode);
}

void PatriotViperSteelController::SetAllColors(unsigned char red, unsigned char green, unsigned char blue)
{
    ViperRegisterWrite(VIPER_STEEL_REG_LED0_DIRECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_STEEL_REG_LED1_DIRECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_STEEL_REG_LED2_DIRECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_STEEL_REG_LED3_DIRECT_COLOR, red, blue, green);
    ViperRegisterWrite(VIPER_STEEL_REG_LED4_DIRECT_COLOR, red, blue, green);
}

void PatriotViperSteelController::SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    ViperRegisterWrite(VIPER_STEEL_REG_LED0_DIRECT_COLOR + led, red, blue, green);
}

void PatriotViperSteelController::SetLEDColor(unsigned int /*slot*/, unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    ViperRegisterWrite(VIPER_STEEL_REG_LED0_DIRECT_COLOR + led, red, blue, green);
}

void PatriotViperSteelController::ViperRegisterWrite(viper_register reg, unsigned char val0, unsigned char val1, unsigned char val2)
{
    bus->i2c_smbus_write_byte_data(dev, reg, val0);
    bus->i2c_smbus_write_byte_data(dev, val2, val1);
}
