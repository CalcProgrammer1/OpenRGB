/*---------------------------------------------------------*\
| HYTEMousematController_Linux.h                            |
|                                                           |
|   Driver for HYTE mousemat (libusb implementation for     |
|   Linux)                                                  |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      18 Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <libusb.h>
#include "RGBController.h"

/*---------------------------------------------------------*\
| HYTE CNVS endpoint values                                 |
\*---------------------------------------------------------*/
#define HYTE_CNVS_EP_IN     0x81
#define HYTE_CNVS_EP_OUT    0x01

class HYTEMousematController
{
public:
    HYTEMousematController(libusb_device_handle* dev_handle, std::string dev_name);
    ~HYTEMousematController();

    std::string     GetLocation();
    std::string     GetName();

    void            FirmwareAnimationControl(bool enabled);
    void            StreamingCommand(RGBColor* colors);

private:
    libusb_device_handle*   dev;
    std::string             location;
    std::string             name;
};
