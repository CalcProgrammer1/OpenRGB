/*-----------------------------------------*\
|  AsusAuraKeyboardController.h             |
|                                           |
|  Definitions and types for ASUS Aura      |
|  USB RGB lighting controller              |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/19/2020   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <vector>
#include <hidapi/hidapi.h>

#pragma once

class AuraKeyboardController
{
public:
    AuraKeyboardController(hid_device* dev_handle, const char* path);
    virtual ~AuraKeyboardController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SendDirect
        (
        unsigned char       frame_count,
        unsigned char *     frame_data
        );

private:
    hid_device*                 dev;
    std::string                 location;
};
