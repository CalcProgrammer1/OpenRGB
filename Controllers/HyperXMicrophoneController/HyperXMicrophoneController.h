/*-----------------------------------------*\
|  HyperXMicrophoneController.h             |
|                                           |
|  Definitions and types for HyperX         |
|  microphone lighting controller           |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/16/2023   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

class HyperXMicrophoneController
{
public:
    HyperXMicrophoneController(hid_device* dev_handle, const char* path);
    ~HyperXMicrophoneController();

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
