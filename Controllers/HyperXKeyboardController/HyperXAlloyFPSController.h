/*-----------------------------------------*\
|  HyperXAlloyFPSController.h               |
|                                           |
|  Definitions and types for HyperX Alloy   |
|  Elite Keyboard lighting controller       |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/30/2020   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    HYPERX_ALLOY_FPS_PACKET_ID_DIRECT               = 0x16,     /* Direct control packet                */
};

enum
{
    HYPERX_ALLOY_FPS_COLOR_CHANNEL_RED              = 0x01,
    HYPERX_ALLOY_FPS_COLOR_CHANNEL_GREEN            = 0x02,
    HYPERX_ALLOY_FPS_COLOR_CHANNEL_BLUE             = 0x03
};

class HyperXAlloyFPSController
{
public:
    HyperXAlloyFPSController(hid_device* dev_handle, const char* path);
    ~HyperXAlloyFPSController();

    std::string     GetDeviceLocation();
    std::string     GetSerialString();

    void SetLEDsDirect(std::vector<RGBColor> colors);

private:
    hid_device*             dev;
    std::string             location;

    void    SendDirect
                (
                unsigned char   color_channel,
                unsigned char*  color_data
                );
};
