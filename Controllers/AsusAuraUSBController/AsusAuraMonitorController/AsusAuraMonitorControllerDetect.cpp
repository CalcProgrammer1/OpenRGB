/*---------------------------------------------------------*\
| AsusAuraMonitorControllerDetect.cpp                       |
|                                                           |
|   Detector for ASUS Aura monitor devices                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "AsusAuraMonitorController.h"
#include "RGBController_AsusAuraMonitor.h"

/*---------------------------------------------------------*\
| ASUS VID                                                  |
\*---------------------------------------------------------*/
#define AURA_USB_VID                                0x0B05

/*---------------------------------------------------------*\
| Monitor PIDs                                              |
\*---------------------------------------------------------*/
#define AURA_ROG_STRIX_XG27AQ_PID                   0x198C
#define AURA_ROG_STRIX_XG27AQM_PID                  0x19BB
#define AURA_ROG_STRIX_XG279Q_PID                   0x1919
#define AURA_ROG_STRIX_XG27W_PID                    0x1933
#define AURA_ROG_STRIX_XG32VC_PID                   0x1968
#define AURA_ROG_PG32UQ_PID                         0x19B9

DetectedControllers DetectAsusAuraUSBMonitor(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AuraMonitorController*     controller           = new AuraMonitorController(dev, info->path, info->product_id, name);
        RGBController_AuraMonitor* rgb_controller       = new RGBController_AuraMonitor(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| Monitor Detectors                                       |
\*---------------------------------------------------------*/
REGISTER_HID_DETECTOR_PU("ASUS ROG Strix XG27AQ",   DetectAsusAuraUSBMonitor,   AURA_USB_VID, AURA_ROG_STRIX_XG27AQ_PID,    0xFFA0, 1);
REGISTER_HID_DETECTOR_PU("ASUS ROG Strix XG27AQM",  DetectAsusAuraUSBMonitor,   AURA_USB_VID, AURA_ROG_STRIX_XG27AQM_PID,   0xFFA0, 1);
REGISTER_HID_DETECTOR_PU("ASUS ROG Strix XG279Q",   DetectAsusAuraUSBMonitor,   AURA_USB_VID, AURA_ROG_STRIX_XG279Q_PID,    0xFFA0, 1);
REGISTER_HID_DETECTOR_PU("ASUS ROG Strix XG27W",    DetectAsusAuraUSBMonitor,   AURA_USB_VID, AURA_ROG_STRIX_XG27W_PID,     0xFFA0, 1);
REGISTER_HID_DETECTOR_PU("ASUS ROG Strix XG32VC",   DetectAsusAuraUSBMonitor,   AURA_USB_VID, AURA_ROG_STRIX_XG32VC_PID,    0xFFA0, 1);
REGISTER_HID_DETECTOR_PU("ASUS ROG PG32UQ",         DetectAsusAuraUSBMonitor,   AURA_USB_VID, AURA_ROG_PG32UQ_PID,          0xFFA0, 1);
