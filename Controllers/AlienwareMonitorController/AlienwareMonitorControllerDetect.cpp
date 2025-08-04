/*---------------------------------------------------------*\
| AlienwareMonitorControllerDetect.cpp                      |
|                                                           |
|   Detector for Alienware monitors                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "AlienwareAW3423DWFController.h"
#include "AlienwareMonitorController.h"
#include "RGBController_AlienwareAW3423DWF.h"
#include "RGBController_AlienwareMonitor.h"
#include <hidapi.h>

/*---------------------------------------------------------*\
| Alienware Vendor ID                                       |
\*---------------------------------------------------------*/
#define ALIENWARE_VID                               0x187C

/*---------------------------------------------------------*\
| Alienware Vendor ID                                       |
\*---------------------------------------------------------*/
#define ALIENWARE_AW3423DWF_PID                     0x100E
#define ALIENWARE_AW3225QF_PID                      0x1013
#define ALIENWARE_USAGE_PAGE                        0xFFDA
#define ALIENWARE_USAGE                             0x00DA

/******************************************************************************************\
*                                                                                          *
*   AlienwareAW3423DWFControllerDetect                                                     *
*                                                                                          *
*       Tests the USB address to see if an Alienware AW3423DWF exists there.               *
*                                                                                          *
\******************************************************************************************/

void DetectAlienwareAW3423DWFControllers(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        AlienwareAW3423DWFController*     controller     = new AlienwareAW3423DWFController(dev, info->path);
        RGBController_AlienwareAW3423DWF* rgb_controller = new RGBController_AlienwareAW3423DWF(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAlienwareMonitorControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AlienwareMonitorController*     controller     = new AlienwareMonitorController(dev, info->path, name);
        RGBController_AlienwareMonitor* rgb_controller = new RGBController_AlienwareMonitor(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR("Alienware AW3423DWF", DetectAlienwareAW3423DWFControllers, ALIENWARE_VID, ALIENWARE_AW3423DWF_PID);
REGISTER_HID_DETECTOR("Alienware AW3225QF", DetectAlienwareMonitorControllers, ALIENWARE_VID, ALIENWARE_AW3225QF_PID);
