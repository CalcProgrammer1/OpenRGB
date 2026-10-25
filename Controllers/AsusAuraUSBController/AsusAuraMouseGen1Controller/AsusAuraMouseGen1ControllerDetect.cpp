/*---------------------------------------------------------*\
| AsusAuraMouseGen1ControllerDetect.cpp                     |
|                                                           |
|   Detector for ASUS Aura Gen1 Mouse devices               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "AsusAuraMouseGen1Controller.h"
#include "RGBController_AsusROGSpatha.h"
#include "RGBController_AsusROGStrixEvolve.h"

/*---------------------------------------------------------*\
| ASUS VID                                                  |
\*---------------------------------------------------------*/
#define AURA_USB_VID                                0x0B05

/*---------------------------------------------------------*\
| Gen1 Mouse PIDs                                           |
\*---------------------------------------------------------*/
#define AURA_ROG_STRIX_EVOLVE_PID                   0x185B
#define AURA_ROG_SPATHA_WIRED_PID                   0x181C
#define AURA_ROG_SPATHA_WIRELESS_PID                0x1824

DetectedControllers DetectAsusAuraUSBSpatha(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AsusAuraMouseGen1Controller* controller     = new AsusAuraMouseGen1Controller(dev, info->path, info->product_id, name);
        RGBController_AsusROGSpatha* rgb_controller = new RGBController_AsusROGSpatha(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectAsusAuraUSBStrixEvolve(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AsusAuraMouseGen1Controller*      controller        = new AsusAuraMouseGen1Controller(dev, info->path, info->product_id, name);
        RGBController_AsusROGStrixEvolve* rgb_controller    = new RGBController_AsusROGStrixEvolve(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| Gen1 Mouse Detectors                                      |
\*---------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("ASUS ROG Strix Evolve",       DetectAsusAuraUSBStrixEvolve,   AURA_USB_VID, AURA_ROG_STRIX_EVOLVE_PID,    1,  0x0008);
REGISTER_HID_DETECTOR_IP("ASUS ROG Spatha Wired",       DetectAsusAuraUSBSpatha,        AURA_USB_VID, AURA_ROG_SPATHA_WIRED_PID,    1,  0x0008);
REGISTER_HID_DETECTOR_IP("ASUS ROG Spatha Wireless",    DetectAsusAuraUSBSpatha,        AURA_USB_VID, AURA_ROG_SPATHA_WIRELESS_PID, 1,  0x0008);
