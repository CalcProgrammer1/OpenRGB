/*---------------------------------------------------------*\
| LenovoM300ControllerDetect.cpp                            |
|                                                           |
|   Detector for Lenovo Legion M300 mouse                   |
|                                                           |
|   Wayne Riordan                               09 Jan 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "RGBController_LenovoM300.h"
#include "LenovoM300Controller.h"

/*---------------------------------------------------------*\
| Lenovo vendor, product, usage and page IDs                |
\*---------------------------------------------------------*/
#define LENOVO_VID 0x17EF
#define LEGION_M300_PID 0x60E4
#define LENOVO_USAGE 0X01
#define LENOVO_PAGE 0XFF01

void DetectLenovoLegionM300Controllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LenovoM300Controller* controller            = new LenovoM300Controller(dev, *info, name);
        RGBController_LenovoM300* rgb_controller    = new RGBController_LenovoM300(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("Lenovo Legion M300", DetectLenovoLegionM300Controllers, LENOVO_VID, LEGION_M300_PID, LENOVO_PAGE, LENOVO_USAGE);
