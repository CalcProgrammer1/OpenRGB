/*-----------------------------------------*\
|  AOCMouseController.h                     |
|                                           |
|  Definitions and types for AOC mouse      |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/20/2023   |
\*-----------------------------------------*/

#include "RGBController.h"
#include <hidapi/hidapi.h>
#include <string>

#pragma once

class AOCMouseController
{
public:
    AOCMouseController(hid_device* dev_handle, const char* path);
    ~AOCMouseController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SendDirect
            (
            RGBColor*       color_data
            );

private:
    hid_device*             dev;
    std::string             location;
};
