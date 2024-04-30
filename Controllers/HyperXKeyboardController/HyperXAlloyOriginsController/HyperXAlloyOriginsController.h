/*-----------------------------------------*\
|  HyperXAlloyOriginsController.h           |
|                                           |
|  Definitions and types for HyperX Alloy   |
|  Origins RGB Keyboard lighting controller |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/11/2020   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

class HyperXAlloyOriginsController
{
public:
    HyperXAlloyOriginsController(hid_device* dev_handle, const char* path);
    ~HyperXAlloyOriginsController();

    std::string     GetDeviceLocation();
    std::string     GetSerialString();

    void SetLEDsDirect(std::vector<RGBColor> colors);

private:
    hid_device*             dev;
    std::string             location;

    void    SendDirectInitialization();
    void    SendDirectColorPacket
                (
                RGBColor*       color_data,
                unsigned int    color_count
                );
};
