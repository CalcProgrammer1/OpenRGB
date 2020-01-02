/*-----------------------------------------*\
|  PatriotViperController.h                 |
|                                           |
|  Definitions and types for Patriot Viper  |
|  RGB RAM lighting controller              |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/1/2020    |
\*-----------------------------------------*/

#include <string>
#include "i2c_smbus.h"

#pragma once

typedef unsigned char	viper_dev_id;
typedef unsigned short	viper_register;

class PatriotViperController
{
public:
    PatriotViperController(i2c_smbus_interface* bus, viper_dev_id dev, unsigned char slots);
    ~PatriotViperController();

    std::string     GetDeviceName();
    std::string     GetDeviceLocation();
    unsigned int    GetLEDCount();
    unsigned int    GetSlotCount();
    unsigned int    GetMode();
    void            SetMode(unsigned char new_mode);

    void            SetAllColors(unsigned char red, unsigned char green, unsigned char blue);
    void            SetEffectColor(unsigned char red, unsigned char green, unsigned char blue);
    void            SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void            SetLEDColor(unsigned int slot, unsigned int led, unsigned char red, unsigned char green, unsigned char blue);

private:
    char                    device_name[32];
    unsigned int            led_count;
    unsigned char           slots_valid;
    i2c_smbus_interface*    bus;
    viper_dev_id            dev;
    unsigned int            mode;
};