/*-----------------------------------------*\
|  HyperXAlloyOrigins60Controller.h         |
|                                           |
|  Definitions and types for HyperX Alloy   |
|  Origins 60 RGB Keyboard lighting         |
|  controller                               |
|                                           |
|  Derek Huber 10/24/2022                   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

class HyperXAlloyOrigins60Controller
{
public:
    HyperXAlloyOrigins60Controller(hid_device* dev_handle, const char* path);
    ~HyperXAlloyOrigins60Controller();

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
