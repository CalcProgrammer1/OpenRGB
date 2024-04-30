/*-----------------------------------------*\
|  HyperXPulsefireFPSProController.h        |
|                                           |
|  Definitions and types for HyperX         |
|  Pulsefire FPS Pro lighting controller    |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/26/2020  |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    HYPERX_PULSEFIRE_FPS_PRO_PACKET_ID_DIRECT           = 0x0A,     /* Direct control packet                */
};

class HyperXPulsefireFPSProController
{
public:
    HyperXPulsefireFPSProController(hid_device* dev_handle, const char* path);
    ~HyperXPulsefireFPSProController();

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
