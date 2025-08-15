/*---------------------------------------------------------*\
| ZETKeyboardControllerDetect.cpp                           |
|                                                           |
|   Detector for ZET Blade                                  |
|                                                           |
|   Based on HyperX Alloy Elite2 implementation by          |
|   KundaPanda                                              |
|                                                           |
|   Moon_darker (Vaker)                         23 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "ZETBladeOpticalController.h"
#include "RGBController_ZETBladeOptical.h"

/*-----------------------------------------------------*\
| ZET keyboard VID/PID pairs                            |
\*-----------------------------------------------------*/
#define ZET_BLADE_OPTICAL_VID                     0x2EA8
#define ZET_BLADE_OPTICAL_PID                     0x2125

void DetectZETBladeOptical(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if (dev)
    {
        ZETBladeOpticalController*     controller     = new ZETBladeOpticalController(dev, info->path, name);
        RGBController_ZETBladeOptical* rgb_controller = new RGBController_ZETBladeOptical(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IP("ZET Blade Optical",    DetectZETBladeOptical, ZET_BLADE_OPTICAL_VID, ZET_BLADE_OPTICAL_PID,   1, 0xFF00);
