/*---------------------------------------------------------*\
| RealtekARGBControllerDetect.cpp                           |
|                                                           |
|   Detector for Realtek USB ARGB ICs                       |
|                                                           |
|   Jerry Fan (JerryFan0612)                    13 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "RGBController.h"
#include "RGBController_RealtekARGB.h"

/******************************************************************************************\
*                                                                                          *
*   DetectRealtekARGBControllers                                                           *
*                                                                                          *
*       Tests the USB address to see if an Realtek ARGB controller exists there            *
*                                                                                          *
\******************************************************************************************/
void DetectRealtekARGBControllers(hid_device_info* info, const std::string& /*name*/)
{
    RealtekARGBController* controller = NULL;
    RGBController_RealtekARGB* rgb_controller = NULL;
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        controller = new RealtekARGBController(dev, info);
        if(controller->get_support_openrgb())
        {
            rgb_controller = new RGBController_RealtekARGB(controller);
            if(rgb_controller->GetDeviceType() != DEVICE_TYPE_UNKNOWN)
            {
                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
            else
            {
                delete rgb_controller;
            }
        }
        else
        {
            delete controller;
        }
    }
    return;
}

REGISTER_HID_DETECTOR_PU("RTL9209", DetectRealtekARGBControllers, REALTEK_ARGB_VID, REALTEK_ARGB_PID, REALTEK_ARGB_HID2SCSI_PG, REALTEK_ARGB_HID2SCSI_USAGE);
