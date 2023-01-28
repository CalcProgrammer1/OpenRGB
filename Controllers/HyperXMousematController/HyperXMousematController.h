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

#include "hidapi_wrapper.h"

#pragma once

class HyperXMousematController
{
public:
    HyperXMousematController(hidapi_wrapper hid_wrapper, hid_device* dev_handle, const char* path);
    ~HyperXMousematController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SendDirect
            (
            RGBColor*       color_data
            );

private:
    hidapi_wrapper          wrapper;
    hid_device*             dev;
    std::string             location;
};
