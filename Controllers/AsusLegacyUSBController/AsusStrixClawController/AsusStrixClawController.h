/*-----------------------------------------*\
|  AsusStrixClawController.h                |
|                                           |
|  Definitions and types for ASUS           |
|  Legacy USB RGB lighting controller       |
|                                           |
|  Mola19 08/06/2022                        |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

#define HID_MAX_STR 255

class StrixClawController
{
public:
    StrixClawController(hid_device* dev_handle, const char* path);
    virtual ~StrixClawController();

    std::string GetDeviceLocation();
    std::string GetSerialString();
    std::string GetVersion();

    void SetScrollWheelLED(bool OnOff);
    void SetLogoLED(uint8_t brightness);

private:
    hid_device*                 dev;
    std::string                 location;
};
