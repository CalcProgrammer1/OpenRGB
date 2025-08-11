/*---------------------------------------------------------*\
| HYTEKeyboardControllerDetect.cpp                          |
|                                                           |
|   Detector for HYTE keyboard                              |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      30 Oct 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "HYTEKeyboardController.h"
#include "RGBController_HYTEKeyboard.h"

/*---------------------------------------------------------*\
| HYTE vendor ID                                            |
\*---------------------------------------------------------*/
#define HYTE_VID                                    0x3402

/*---------------------------------------------------------*\
| HYTE keyboard product IDs                                 |
\*---------------------------------------------------------*/
#define HYTE_KEEB_TKL_PID                           0x0300

void DetectHYTEKeyboard(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        HYTEKeyboardController*     controller     = new HYTEKeyboardController(dev, info->path, name);
        RGBController_HYTEKeyboard* rgb_controller = new RGBController_HYTEKeyboard(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("HYTE Keeb TKL", DetectHYTEKeyboard, HYTE_VID, HYTE_KEEB_TKL_PID, 0xFF11, 0xF0);
