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

#pragma once

typedef unsigned char corsair_dev_id;

#define CORSAIR_DOMINATOR_PLATINUM_NAME "Corsair Dominator Platinum"

class CorsairDominatorPlatinumController
{
public:
    CorsairDominatorPlatinumController(i2c_smbus_interface *bus, corsair_dev_id dev);
    ~CorsairDominatorPlatinumController();

    std::string GetDeviceName();
    std::string GetDeviceLocation();

    size_t GetLEDCount() { return CORSAIR_PLAT_LED_COUNT; }
    void SetAllColors(unsigned char red, unsigned char green, unsigned char blue);
    void SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void ApplyColors();
    bool WaitReady();

private:
    static constexpr size_t CORSAIR_PLAT_LED_COUNT = 12;

    unsigned char led_data[CORSAIR_PLAT_LED_COUNT * 3 + 2];

    i2c_smbus_interface*    bus;
    corsair_dev_id          dev;

    static unsigned char crc8(unsigned char init, unsigned char poly, unsigned char *data, unsigned char len);
};
