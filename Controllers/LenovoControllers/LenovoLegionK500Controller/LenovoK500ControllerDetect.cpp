/*---------------------------------------------------------*\
| LenovoK500ControllerDetect.cpp                            |
|                                                           |
|   Detector for Lenovo Legion K500 keyboard                |
|                                                           |
|   MX1D                                        10 Aug 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "RGBController_LenovoK500.h"
#include "LenovoK500Controller.h"

#define LENOVO_VID 0x17EF
#define LEGION_K500_PID 0x60D5

DetectedControllers DetectLenovoLegionK500Controllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        LenovoK500Controller*     controller        = new LenovoK500Controller(dev, *info, name);
        RGBController_LenovoK500* rgb_controller    = new RGBController_LenovoK500(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_I("Lenovo Legion K500", DetectLenovoLegionK500Controllers, LENOVO_VID, LEGION_K500_PID, 3);
