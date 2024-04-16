/*---------------------------------------------------------*\
|  NZXTMouseController.h                                    |
|                                                           |
|  Definitions for NZXT Mouse                               |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 12/13/2023       |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include <string>
#include <vector>
#include <hidapi/hidapi.h>

class NZXTMouseController
{
public:
    NZXTMouseController(hid_device* dev_handle, const char* path);
    ~NZXTMouseController();

    std::string     GetFirmwareVersion();
    std::string     GetLocation();
    std::string     GetSerialString();

    void            SetLEDs
                        (
                        RGBColor *      colors
                        );

private:
    hid_device*     dev;

    char            firmware_version[16];
    std::string     location;

    void            SendFirmwareRequest();
};
