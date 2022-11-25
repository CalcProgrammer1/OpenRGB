/*-------------------------------------------------------------------*\
|  SteelSeriesApex3Controller.h                                       |
|                                                                     |
|  Driver for Steelseries Apex3 Controllers                           |
|       This is the base class for the Ten Zone and 8 Zone keyboards  |
|                                                                     |
|  Chris M (Dr_No)          23rd Feb 2022                             |
\*-------------------------------------------------------------------*/

#include <string>
#include <hidapi/hidapi.h>

#include "RGBController.h"
#include "SteelSeriesGeneric.h"

#pragma once

#define STEELSERIES_APEX3_BRIGHTNESS_MIN    0x00
#define STEELSERIES_APEX3_HID_TIMEOUT       100

class SteelSeriesApex3Controller
{
public:
    SteelSeriesApex3Controller(hid_device* dev_handle, const char* path);
    virtual ~SteelSeriesApex3Controller();

    std::string         GetDeviceLocation();
    std::string         GetSerialString();
    steelseries_type    GetKeyboardType();

    virtual void        SetColor(std::vector<RGBColor> colors, uint8_t mode, uint8_t brightness)    = 0;
    virtual void        Save()                                                                      = 0;
    virtual uint8_t     GetLedCount()                                                               = 0;
    virtual uint8_t     GetMaxBrightness()                                                          = 0;
    virtual bool        SupportsRainbowWave()                                                       = 0;
    virtual bool        SupportsSave()                                                              = 0;

    hid_device*         dev;

private:
    std::string         location;
};
