/*---------------------------------------------------------*\
| IntelArcA770LEController.h                                |
|                                                           |
|   Driver for Intel Arc A770 LE                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                01 Nov 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

class IntelArcA770LEController
{
public:
    IntelArcA770LEController(hid_device* dev_handle, const char* path);
    ~IntelArcA770LEController();

    std::string GetEffectChannelString(unsigned char channel);
    std::string GetFirmwareVersionString();
    std::string GetLocationString();
    std::string GetSerialString();

    void SendDirectPacket
        (
        unsigned char   size,
        unsigned char * led_ids,
        RGBColor *      colors
        );

    void SendEnableCommand();

    void SendApplyCommand();

private:
    hid_device*             dev;
    std::string             location;
};
