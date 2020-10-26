/*-----------------------------------------*\
|  HyperXMousematController.h               |
|                                           |
|  Definitions and types for HyperX         |
|  mousemat lighting controller             |
|                                           |
|  Adam Honse (CalcProgrammer1) 10/25/2020  |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

class HyperXMousematController
{
public:
    HyperXMousematController(hid_device* dev_handle, const char* path);
    ~HyperXMousematController();

    std::string GetDeviceLocation();

    void SendDirect
            (
            RGBColor*       color_data
            );

private:
    hid_device*             dev;
    std::string             location;
};
