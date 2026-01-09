/*---------------------------------------------------------*\
| AsusMonitorControllerDetect.cpp                           |
|                                                           |
|   Detector for Asus monitors                              |
|                                                           |
|   Morgan Guimard (morg)                       19 oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "AsusMonitorController.h"
#include "RGBController_AsusMonitor.h"

/*---------------------------------------------------------*\
| Asus vendor ID                                            |
\*---------------------------------------------------------*/
#define ASUS_VID                                       0x0B05

/*---------------------------------------------------------*\
| Product ID                                                |
\*---------------------------------------------------------*/
#define ASUS_ROG_STRIX_XG27AQDMG_PID                   0x1BA3
#define ASUS_ROG_SWIFT_XG27UCG_PID                     0x1BB4
#define ASUS_ROG_SWIFT_PG32UCDM_PID                    0x1B2B

void DetectAsusMonitorControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AsusMonitorController*     controller      = new AsusMonitorController(dev, *info, name);
        RGBController_AsusMonitor* rgb_controller  = new RGBController_AsusMonitor(controller);

        DetectionManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Asus ROG STRIX XG27AQDMG",   DetectAsusMonitorControllers, ASUS_VID, ASUS_ROG_STRIX_XG27AQDMG_PID,   1, 0xFF72, 0x00A1);
REGISTER_HID_DETECTOR_IPU("Asus ROG STRIX XG27UCG",     DetectAsusMonitorControllers, ASUS_VID, ASUS_ROG_SWIFT_XG27UCG_PID,     1, 0xFF72, 0x00A1);
REGISTER_HID_DETECTOR_IPU("Asus ROG STRIX PG32UCDM",    DetectAsusMonitorControllers, ASUS_VID, ASUS_ROG_SWIFT_PG32UCDM_PID,    1, 0xFF72, 0x00A1);
