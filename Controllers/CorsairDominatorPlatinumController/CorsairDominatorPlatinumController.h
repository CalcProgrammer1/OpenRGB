/*-----------------------------------------*\
|  CorsairDominatorPlatinumController.h     |
|                                           |
|  Definitions and types for Corsair        |
|  Domintator Platinum RGB RAM lighting     |
|   controller                              |
|                                           |
|  Erik Gilling (konkers)       9/25/2020   |
\*-----------------------------------------*/

#include <string>
#include "i2c_smbus.h"
#define CORSAIR_DOMINATOR_PLATINUM_DATA_SIZE 38

#pragma once

typedef unsigned char corsair_dev_id;

class CorsairDominatorPlatinumController
{
public:
    CorsairDominatorPlatinumController(i2c_smbus_interface *bus, corsair_dev_id dev, unsigned int leds_count);
    ~CorsairDominatorPlatinumController();

    std::string GetDeviceLocation();
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

    static unsigned char crc8(unsigned char init, unsigned char poly, unsigned char *data, unsigned char len);
};
