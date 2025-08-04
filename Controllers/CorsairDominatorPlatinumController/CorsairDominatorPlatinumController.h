/*---------------------------------------------------------*\
| CorsairDominatorPlatinumController.h                      |
|                                                           |
|   Driver for Corsair Dominator Platinum RAM               |
|                                                           |
|   Erik Gilling (konkers)                      25 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

#define CORSAIR_DOMINATOR_PLATINUM_DATA_SIZE 38

typedef unsigned char corsair_dev_id;

class CorsairDominatorPlatinumController
{
public:
    CorsairDominatorPlatinumController(i2c_smbus_interface *bus, corsair_dev_id dev, unsigned int leds_count, std::string dev_name);
    ~CorsairDominatorPlatinumController();

    std::string GetDeviceLocation();
    std::string GetDeviceName();
    unsigned int GetLEDCount();

    void SetAllColors(unsigned char red, unsigned char green, unsigned char blue);
    void SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void ApplyColors();
    bool WaitReady();

private:
    unsigned char           led_data[CORSAIR_DOMINATOR_PLATINUM_DATA_SIZE];
    i2c_smbus_interface*    bus;
    corsair_dev_id          dev;
    unsigned int            leds_count;
    std::string             name;

    static unsigned char crc8(unsigned char init, unsigned char poly, unsigned char *data, unsigned char len);
};
