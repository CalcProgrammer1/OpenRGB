/*---------------------------------------------------------*\
| CorsairVoidControllerDetect.cpp                           |
|                                                           |
|   Detector for Corsair Void Pro headsets                  |
|                                                           |
|   Derek Ross                                  15 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "CorsairVoidController.h"
#include "RGBController_CorsairVoid.h"

/*---------------------------------------------------------*\
| Corsair vendor ID                                         |
\*---------------------------------------------------------*/
#define CORSAIR_VID                             0x1B1C

/*---------------------------------------------------------*\
| Corsair Void Pro product IDs                              |
\*---------------------------------------------------------*/
#define CORSAIR_VOID_PRO_WIRELESS_PID           0x0A14
#define CORSAIR_VOID_PRO_WIRELESS_V2_PID        0x0A1A
#define CORSAIR_VOID_PRO_USB_PID                0x0A17
#define CORSAIR_VOID_PRO_USB_V2_PID             0x0A1D

/*---------------------------------------------------------*\
| The lighting reports live on a vendor-defined collection; |
| matching on it skips the audio and status interfaces      |
\*---------------------------------------------------------*/
#define CORSAIR_VOID_LIGHTING_USAGE_PAGE        0xFFC5
#define CORSAIR_VOID_LIGHTING_USAGE             0x01

DetectedControllers DetectCorsairVoidControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        CorsairVoidController*     controller     = new CorsairVoidController(dev, *info, name);
        RGBController_CorsairVoid* rgb_controller = new RGBController_CorsairVoid(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_PU("Corsair Void Pro Wireless", DetectCorsairVoidControllers, CORSAIR_VID, CORSAIR_VOID_PRO_WIRELESS_PID,    CORSAIR_VOID_LIGHTING_USAGE_PAGE, CORSAIR_VOID_LIGHTING_USAGE);
REGISTER_HID_DETECTOR_PU("Corsair Void Pro Wireless", DetectCorsairVoidControllers, CORSAIR_VID, CORSAIR_VOID_PRO_WIRELESS_V2_PID, CORSAIR_VOID_LIGHTING_USAGE_PAGE, CORSAIR_VOID_LIGHTING_USAGE);
REGISTER_HID_DETECTOR_PU("Corsair Void Pro USB",      DetectCorsairVoidControllers, CORSAIR_VID, CORSAIR_VOID_PRO_USB_PID,         CORSAIR_VOID_LIGHTING_USAGE_PAGE, CORSAIR_VOID_LIGHTING_USAGE);
REGISTER_HID_DETECTOR_PU("Corsair Void Pro USB",      DetectCorsairVoidControllers, CORSAIR_VID, CORSAIR_VOID_PRO_USB_V2_PID,      CORSAIR_VOID_LIGHTING_USAGE_PAGE, CORSAIR_VOID_LIGHTING_USAGE);
