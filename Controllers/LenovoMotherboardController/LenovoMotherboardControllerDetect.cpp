/*---------------------------------------------------------*\
| LenovoMotherboardControllerDetect.cpp                     |
|                                                           |
|   Detector for Lenovo motherboard                         |
|                                                           |
|   Morgan Guimard (morg)                       26 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "LenovoMotherboardController.h"
#include "RGBController_LenovoMotherboard.h"
#include "dmiinfo.h"

/*---------------------------------------------------------*\
| vendor ID                                                 |
\*---------------------------------------------------------*/
#define LENOVO_MB_VID                                  0x17EF

/*---------------------------------------------------------*\
| Product ID                                                |
\*---------------------------------------------------------*/
#define LENOVO_MB_PID                                  0xC955

void DetectLenovoMotherboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        DMIInfo dmi;

        LenovoMotherboardController*     controller         = new LenovoMotherboardController(dev, *info, name + " " + dmi.getMainboard());
        RGBController_LenovoMotherboard* rgb_controller     = new RGBController_LenovoMotherboard(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("Lenovo", DetectLenovoMotherboardControllers, LENOVO_MB_VID, LENOVO_MB_PID, 0xFF89, 0xCC);
