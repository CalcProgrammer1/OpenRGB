/*---------------------------------------*\
|  HyperXAlloyOrigins60and65Controller.h  |
|                                         |
|  Definitions and types for HyperX       |
|  Alloy Origins 60 and 65 RGB Keyboards  |
|  lighting controller                    |
|                                         |
|  Derek Huber 03/18/2023                 |
\*---------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

class HyperXAlloyOrigins60and65Controller
{
public:
    HyperXAlloyOrigins60and65Controller(hid_device* dev_handle, const char* path);
    ~HyperXAlloyOrigins60and65Controller();

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
