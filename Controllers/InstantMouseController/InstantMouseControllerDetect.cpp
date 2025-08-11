/*---------------------------------------------------------*\
| InstantMouseControllerDetect.cpp                          |
|                                                           |
|   Detector for Instant mouse                              |
|                                                           |
|   Morgan Guimard (morg)                       19 Jan 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "InstantMouseController.h"
#include "RGBController_InstantMouse.h"
#include "InstantMouseDevices.h"


void DetectInstantMouseControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        InstantMouseController*     controller         = new InstantMouseController(dev, *info, name);
        RGBController_InstantMouse* rgb_controller     = new RGBController_InstantMouse(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Advanced GTA 250 USB Gaming Mouse",  DetectInstantMouseControllers,  INSTANT_MICROELECTRONICS_VID, ADVANCED_GTA_250_PID, 1, 0xFF01, 0x01);
REGISTER_HID_DETECTOR_IPU("Anko KM43243952 USB Gaming Mouse",   DetectInstantMouseControllers,  INSTANT_MICROELECTRONICS_VID, ANKO_KM43243952_VID,  1, 0xFF01, 0x01);
REGISTER_HID_DETECTOR_IPU("Anko KM43277483 USB Gaming Mouse",   DetectInstantMouseControllers,  INSTANT_MICROELECTRONICS_VID, ANKO_KM43277483_VID,  1, 0xFF01, 0x01);
REGISTER_HID_DETECTOR_IPU("AntEsports GM600 USB Gaming Mouse",  DetectInstantMouseControllers,  INSTANT_MICROELECTRONICS_VID, ANTESPORTS_GM600_PID, 1, 0xFF01, 0001);

