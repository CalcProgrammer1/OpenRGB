/*---------------------------------------------------------*\
| PatriotViperSteelController.h                             |
|                                                           |
|   Driver for Patriot Viper Steel RAM                      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef unsigned char viper_dev_id;
typedef unsigned char viper_register;

enum
{
    VIPER_STEEL_REG_LED0_DIRECT_COLOR = 0x17, /* LED 0 Color (R, B, G)                */
    VIPER_STEEL_REG_LED1_DIRECT_COLOR = 0x18, /* LED 1 Color (R, B, G)                */
    VIPER_STEEL_REG_LED2_DIRECT_COLOR = 0x19, /* LED 2 Color (R, B, G)                */
    VIPER_STEEL_REG_LED3_DIRECT_COLOR = 0x1a, /* LED 3 Color (R, B, G)                */
    VIPER_STEEL_REG_LED4_DIRECT_COLOR = 0x1b, /* LED 4 Color (R, B, G)                */
};

class PatriotViperSteelController
{
public:
    PatriotViperSteelController(i2c_smbus_interface *bus, viper_dev_id dev);
    ~PatriotViperSteelController();

    std::string GetDeviceName();
    std::string GetDeviceLocation();
    unsigned int GetLEDCount();
    unsigned int GetSlotCount();
    unsigned int GetMode();

    void SetAllColors(unsigned char red, unsigned char green, unsigned char blue);
    void SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void SetLEDColor(unsigned int slot, unsigned int led, unsigned char red, unsigned char green, unsigned char blue);

    void ViperRegisterWrite(viper_register reg, unsigned char val0, unsigned char val1, unsigned char val2);
    bool direct;

private:
    char device_name[32];
    unsigned int led_count;
    unsigned char slots_valid;
    i2c_smbus_interface *bus;
    viper_dev_id dev;
    unsigned char mode;
};
