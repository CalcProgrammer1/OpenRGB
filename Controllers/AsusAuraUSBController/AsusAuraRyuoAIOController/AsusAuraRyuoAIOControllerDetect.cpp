/*---------------------------------------------------------*\
| AsusAuraRyuoAIOControllerDetect.cpp                       |
|                                                           |
|   Detector for ASUS Aura Ryuo AIO devices                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "AsusAuraRyuoAIOController.h"
#include "RGBController_AsusAuraRyuoAIO.h"

/*---------------------------------------------------------*\
| ASUS VID                                                  |
\*---------------------------------------------------------*/
#define AURA_USB_VID                                0x0B05

/*---------------------------------------------------------*\
| Ryuo AIO PIDs                                             |
\*---------------------------------------------------------*/
#define AURA_RYUO_AIO_PID                           0x1887
#define AURA_RYUJIN_AIO_PID                         0x18AE

DetectedControllers DetectAsusAuraUSBRyuoAIO(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AsusAuraRyuoAIOController*     controller     = new AsusAuraRyuoAIOController(dev, info->path, name);
        RGBController_AsusAuraRyuoAIO* rgb_controller = new RGBController_AsusAuraRyuoAIO(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| Ryuo AIO Detectors                                        |
\*---------------------------------------------------------*/
REGISTER_HID_DETECTOR_PU("ASUS ROG Ryuo AIO",   DetectAsusAuraUSBRyuoAIO,   AURA_USB_VID, AURA_RYUO_AIO_PID,    0xFF72, 0x00A1);
REGISTER_HID_DETECTOR_PU("ASUS ROG Ryujin AIO", DetectAsusAuraUSBRyuoAIO,   AURA_USB_VID, AURA_RYUJIN_AIO_PID,  0xFF72, 0x00A1);
