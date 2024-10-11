/*---------------------------------------------------------*\
| MSI3ZoneControllerDetect.cpp                              |
|                                                           |
|   Detector for MSI/SteelSeries 3-Zone keyboard            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "MSI3ZoneController.h"
#include "RGBController_MSI3Zone.h"

#define MSI_3_ZONE_KEYBOARD_VID 0x1770
#define MSI_3_ZONE_KEYBOARD_PID 0xFF00

/******************************************************************************************\
*                                                                                          *
*   DetectMSI3ZoneControllers                                                              *
*                                                                                          *
*       Tests the USB address to see if an MSI/SteelSeries 3-zone Keyboard controller      *
*       exists there.                                                                      *
*                                                                                          *
\******************************************************************************************/

void DetectMSI3ZoneControllers(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        MSI3ZoneController*     controller     = new MSI3ZoneController(dev, info->path);
        RGBController_MSI3Zone* rgb_controller = new RGBController_MSI3Zone(controller);
        // Constructor sets the name

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectMSI3ZoneControllers() */

REGISTER_HID_DETECTOR("MSI 3-Zone Laptop", DetectMSI3ZoneControllers, MSI_3_ZONE_KEYBOARD_VID, MSI_3_ZONE_KEYBOARD_PID);
