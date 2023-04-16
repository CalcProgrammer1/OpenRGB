/*-----------------------------------------*\
|  AOCMousematController.h                  |
|                                           |
|  Definitions and types for AOC mousemat   |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/15/2023   |
\*-----------------------------------------*/

#include "RGBController.h"
#include <hidapi/hidapi.h>
#include <string>

#pragma once

class AOCMousematController
{
public:
    AOCMousematController(hid_device* dev_handle, const char* path);
    ~AOCMousematController();

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
