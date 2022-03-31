/*-----------------------------------------*\
|  AsusAuraMonitorController.h              |
|                                           |
|  Definitions and types for ASUS Aura      |
|  USB RGB lighting controller              |
|                                           |
|  Mola19 08/03/2023                        |
\*-----------------------------------------*/

#include "RGBController.h"

#include <hidapi/hidapi.h>

#pragma once

class AuraMonitorController
{
public:
    AuraMonitorController(hid_device* dev_handle, const char* path);
    virtual ~AuraMonitorController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void UpdateLed(int led, unsigned char red, unsigned char green, unsigned char blue);
    void ApplyChanges();

private:
    hid_device*                 dev;
    std::string                 location;
};
