/*---------------------------------------------------------*\
| ThingMControllerDetect.cpp                                |
|                                                           |
|   Detector for ThingM Blink                               |
|                                                           |
|   Eric S (edbgon)                             01 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "BlinkController.h"
#include "RGBController_BlinkController.h"

#define THINGM_VID                              0x27B8

#define THINGM_BLINK_PID                        0x01ED

/******************************************************************************************\
*                                                                                          *
*   DetectThingMControllers                                                          *
*                                                                                          *
*       Tests the USB address to see if any CoolerMaster controllers exists there.         *
*                                                                                          *
\******************************************************************************************/

void DetectThingMBlink(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        BlinkController* controller = new BlinkController(dev, info->path);
        RGBController_BlinkController* rgb_controller = new RGBController_BlinkController(controller);
        // Constructor sets the name
        DetectionManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("ThingM blink(1) mk2",          DetectThingMBlink,          THINGM_VID,   THINGM_BLINK_PID,    0xFF00,     0x01);
