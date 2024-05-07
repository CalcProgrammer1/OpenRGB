/*---------------------------------------------------------*\
| HIDLampArrayControllerDetect.cpp                          |
|                                                           |
|   Detector for HID LampArray Devices                      |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      26 Mar 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "HIDLampArrayController.h"
#include "RGBController.h"
#include "RGBController_HIDLampArray.h"
#include <vector>
#include <hidapi/hidapi.h>

static lamparray_hid_devs devs;

void DetectHIDLampArrayControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        if(info->usage_page == 0x59 && info->usage == 0x01)
        {
            devs.hid_dev_LampArray = dev;
        }

        if(info->usage_page == 0x59 && info->usage == 0x02)
        {
            devs.hid_dev_LampArrayAttributesReport = dev;
        }

        if(info->usage_page == 0x59 && info->usage == 0x20)
        {
            devs.hid_dev_LampAttributesRequestReport = dev;
        }

        if(info->usage_page == 0x59 && info->usage == 0x22)
        {
            devs.hid_dev_LampAttributesResponseReport = dev;
        }

        if(info->usage_page == 0x59 && info->usage == 0x50)
        {
            devs.hid_dev_LampMultiUpdateReport = dev;
        }

        if(info->usage_page == 0x59 && info->usage == 0x60)
        {
            devs.hid_dev_LampRangeUpdateReport = dev;
        }

        if(info->usage_page == 0x59 && info->usage == 0x70)
        {
            devs.hid_dev_LampArrayControlReport = dev;
        }
    }

    if(devs.hid_dev_LampArray
    && devs.hid_dev_LampArrayAttributesReport
    && devs.hid_dev_LampAttributesRequestReport
    && devs.hid_dev_LampAttributesResponseReport
    && devs.hid_dev_LampMultiUpdateReport
    && devs.hid_dev_LampRangeUpdateReport
    && devs.hid_dev_LampArrayControlReport)
    {
        HIDLampArrayController*     controller     = new HIDLampArrayController(&devs);
        RGBController_HIDLampArray* rgb_controller = new RGBController_HIDLampArray(controller);
        rgb_controller->name                       = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectHIDLampArrayControllers() */

REGISTER_HID_DETECTOR("Arduino Zero HID Lamp Array Demo", DetectHIDLampArrayControllers, 0x2341, 0x804D);
