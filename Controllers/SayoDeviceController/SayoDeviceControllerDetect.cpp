/*---------------------------------------------------------*\
| SayoDeviceControllerDetect.cpp                            |
|                                                           |
|   Detector for Sayo Devices                               |
|                                                           |
|   Richard Harris                              24 Jun 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "SayoDeviceController.h"
#include "RGBController_SayoDevice.h"

#define SAYO_USB_VID 0x8089
#define SAYO_USB_PID_E1 0x0007

/*----------------------------------------------------------*\
|                                                            |
|   DetectSayoDevice Controller                              |
|                                                            |
|       Detect Sayo Devices                                  |
|                                                            |
\*----------------------------------------------------------*/

void DetectSayoDeviceController
    (
    hid_device_info*    info,
    const std::string&  name
    )
{
    hid_device* dev = hid_open_path(info->path);
    if(dev != nullptr)
    {
        SayoDeviceController*     controller     = new SayoDeviceController(dev, info->path, name);
        RGBController_SayoDevice *rgb_controller = new RGBController_SayoDevice(controller);
        DetectionManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("SayoDevice E1", DetectSayoDeviceController, SAYO_USB_VID, SAYO_USB_PID_E1, 0xFF11, 0x0002);
