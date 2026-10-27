/*---------------------------------------------------------*\
| AlienwareKeyboardController.h                             |
|                                                           |
|   Driver for Alienware keyboards                          |
|                                                           |
|   Shared controller for AW410K and AW510K                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"
#include "AlienwareDevices.h"

struct SelectedKeys
{
    unsigned char idx;
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

class AlienwareKeyboardController
{
public:
    AlienwareKeyboardController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~AlienwareKeyboardController();

    std::string GetDeviceLocation();
    std::string GetDeviceName();
    std::string GetSerialString();

    void        SendInitialize();
    void        SendCommit();
    void        SendfeatureReport
                    (
                    unsigned char   first_byte,
                    unsigned char   second_byte,
                    unsigned char   third_byte,
                    unsigned char   forth_byte
                    );

    void        SendEdit();

    void        SetDirect
                    (
                    unsigned char   zone,
                    unsigned char   r,
                    unsigned char   g,
                    unsigned char   b
                    );

    void    SendDirectOn
                    (
                    std::vector<SelectedKeys>     &frame_data
                    );

    void        SetMode
                    (
                    unsigned char   mode,
                    unsigned char   speed,
                    unsigned char   direction,
                    unsigned char   colorMode,
                    unsigned char   red,
                    unsigned char   green,
                    unsigned char   blue
                    );

    void        SetMorphMode
                    (
                    unsigned char   mode,
                    unsigned char   speed,
                    unsigned char   red1,
                    unsigned char   green1,
                    unsigned char   blue1,
                    unsigned char   red2,
                    unsigned char   green2,
                    unsigned char   blue2
                    );

    void        DeviceUpdateSingleLED
                    (
                    unsigned char   led,
                    unsigned char   red,
                    unsigned char   green,
                    unsigned char   blue
                    );

private:
    hid_device*             dev;
    std::string             location;
    std::string             name;

    void        SendMode
                    (
                    unsigned char   zone,
                    unsigned char   mode,
                    unsigned char   speed,
                    unsigned char   direction,
                    unsigned char   colorMode,
                    unsigned char   red,
                    unsigned char   green,
                    unsigned char   blue
                    );
};
