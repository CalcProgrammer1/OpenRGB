/*---------------------------------------------------------*\
| RGBController_AcerNitroHidKeyboard.cpp                    |
|                                                           |
|   Detector for Acer Nitro HID Keyboard                    |
|                                                           |
|   Marek Pokropiński                           Jul 27 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "DetectionManager.h"
#include "AcerNitroHidKeyboardController.h"
#include "RGBController_AcerNitroHidKeyboard.h"
#include "RGBController_AcerNitroHidLed.h"

/*---------------------------------------------------------*\
| HID vendor and product IDs for ENEK EC in Acer Nitro      |
\*---------------------------------------------------------*/

#define ACER_ENEC_VID           0x0CF2
#define ACER_ENEC_PID           0x5130
#define ACER_ENEC_USAGE_PAGE    0xFF5A
#define ACER_ENEC_USAGE         0x01

DetectedControllers DetectAcerHIDControllers(hid_device_info* info, const std::string&)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);
    if(dev)
    {
        /*-------------------------------------------------*\
        | Controller is shared between keyboard and onboard |
        | led                                               |
        \*-------------------------------------------------*/
        std::shared_ptr<AcerNitroHidKeyboardController> controller  = std::make_shared<AcerNitroHidKeyboardController>(dev, info->path);
        RGBController_AcerNitroHidKeyboard* rgb_kb_controller       = new RGBController_AcerNitroHidKeyboard(controller);
        RGBController_AcerNitroHidLed* rgb_led_controller           = new RGBController_AcerNitroHidLed(controller);

        detected_controllers.push_back(rgb_kb_controller);
        detected_controllers.push_back(rgb_led_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_PU("AcerNitroHidKeyboardController", DetectAcerHIDControllers, ACER_ENEC_VID, ACER_ENEC_PID, ACER_ENEC_USAGE_PAGE, ACER_ENEC_USAGE);