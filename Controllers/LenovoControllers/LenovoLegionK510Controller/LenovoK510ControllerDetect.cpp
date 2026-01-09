/*---------------------------------------------------------*\
| LenovoK510ControllerDetect.cpp                            |
|                                                           |
|   Detector for Lenovo Legion K510 keyboard                |
|                                                           |
|   Bnyro                                       27 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "RGBController_LenovoK510.h"
#include "LenovoK510Controller.h"

/*---------------------------------------------------------*\
| Lenovo vendor, product, usage and page IDs                |
\*---------------------------------------------------------*/
#define LENOVO_VID 0x17EF
#define LEGION_K510_PID 0x619A
#define LENOVO_IFACE_NUM 0x01
#define LENOVO_PAGE 0xFF1C
#define LENOVO_USAGE 0x0092

void DetectLenovoLegionK510Controllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LenovoK510Controller* controller            = new LenovoK510Controller(dev, *info, name);
        RGBController_LenovoK510* rgb_controller    = new RGBController_LenovoK510(controller);

        DetectionManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Lenovo Legion K510 Mini Pro", DetectLenovoLegionK510Controllers, LENOVO_VID, LEGION_K510_PID, LENOVO_IFACE_NUM, LENOVO_PAGE, LENOVO_USAGE);
