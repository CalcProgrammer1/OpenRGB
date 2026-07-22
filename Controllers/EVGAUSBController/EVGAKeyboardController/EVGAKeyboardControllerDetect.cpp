/*---------------------------------------------------------*\
| EVGAKeyboardControllerDetect.cpp                          |
|                                                           |
|   Detector for EVGA keyboard devices                      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "RGBController_EVGAKeyboard.h"

/*---------------------------------------------------------*\
| EVGA USB vendor ID                                        |
\*---------------------------------------------------------*/
#define EVGA_USB_VID                                0x3842

/*---------------------------------------------------------*\
| Keyboard product IDs                                      |
\*---------------------------------------------------------*/
#define Z15_ISO_PID                                 0x260E
#define Z15_ANSI_PID                                0x2608
#define Z20_ANSI_PID                                0x260A
#define Z20_UK_PID                                  0x2610

DetectedControllers DetectEVGAKeyboardControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        EVGAKeyboardController*     controller      = new EVGAKeyboardController(dev, info->path, info->product_id, name);
        RGBController_EVGAKeyboard* rgb_controller  = new RGBController_EVGAKeyboard(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("EVGA Z15 Keyboard",     DetectEVGAKeyboardControllers,   EVGA_USB_VID,   Z15_ISO_PID,                1,  0x08,   0x4B);
REGISTER_HID_DETECTOR_IPU("EVGA Z15 Keyboard",     DetectEVGAKeyboardControllers,   EVGA_USB_VID,   Z15_ANSI_PID,               1,  0x08,   0x4B);
REGISTER_HID_DETECTOR_IPU("EVGA Z20 Keyboard",     DetectEVGAKeyboardControllers,   EVGA_USB_VID,   Z20_ANSI_PID,               1,  0x08,   0x4B);
REGISTER_HID_DETECTOR_IPU("EVGA Z20 Keyboard",     DetectEVGAKeyboardControllers,   EVGA_USB_VID,   Z20_UK_PID,                 1,  0x08,   0x4B);

