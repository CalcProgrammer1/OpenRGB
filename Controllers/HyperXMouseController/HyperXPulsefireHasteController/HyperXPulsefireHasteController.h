/*-----------------------------------------*\
|  HyperXPulsefireHasteController.h         |
|                                           |
|  Definitions and types for HyperX         |
|  Pulsefire FPS Pro lighting controller    |
|                                           |
|  Adam Honse (CalcProgrammer1) 8/19/2021   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    HYPERX_PULSEFIRE_HASTE_PACKET_ID_SETUP              = 0x04,     /* Direct setup packet                  */
    HYPERX_PULSEFIRE_HASTE_PACKET_ID_COLOR              = 0x81,     /* Direct color packet                  */
};

class HyperXPulsefireHasteController
{
public:
    HyperXPulsefireHasteController(hid_device* dev_handle, const char* path);
    ~HyperXPulsefireHasteController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SendDirect
            (
            RGBColor*       color_data
            );

private:
    hid_device*             dev;
    std::string             location;

    void SendDirectSetup();
    void SendDirectColor
            (
            RGBColor*       color_data
            );
};
