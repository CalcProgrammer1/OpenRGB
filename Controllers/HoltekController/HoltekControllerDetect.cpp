/*---------------------------------------------------------*\
| HoltekControllerDetect.cpp                                |
|                                                           |
|   Detector for Holtek devices                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "HoltekA070Controller.h"
#include "RGBController_HoltekA070.h"
#include "HoltekA1FAController.h"
#include "RGBController_HoltekA1FA.h"

/*-----------------------------------------------------*\
| Holtek Semiconductor Inc. vendor ID                   |
\*-----------------------------------------------------*/
#define HOLTEK_VID                    0x04D9
/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define HOLTEK_A070_PID               0xA070
/*-----------------------------------------------------*\
| Mousemats product IDs                                 |
\*-----------------------------------------------------*/
#define HOLTEK_A1FA_PID               0xA1FA

void DetectHoltekControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        HoltekA070Controller*     controller     = new HoltekA070Controller(dev, info->path, name);
        RGBController_HoltekA070* rgb_controller = new RGBController_HoltekA070(controller);

        DetectionManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectHoltekControllers() */

void DetectHoltekMousemats(hid_device_info *info, const std::string &name)
{
    hid_device *dev = hid_open_path(info->path);

    if(dev)
    {
        HoltekA1FAController*     controller     = new HoltekA1FAController(dev, info->path, name);
        RGBController_HoltekA1FA* rgb_controller = new RGBController_HoltekA1FA(controller);

        DetectionManager::get()->RegisterRGBController(rgb_controller);
    }
} /* DetectHoltekMousemats() */

REGISTER_HID_DETECTOR_IPU("Holtek USB Gaming Mouse", DetectHoltekControllers, HOLTEK_VID, HOLTEK_A070_PID, 1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Holtek Mousemat",         DetectHoltekMousemats,   HOLTEK_VID, HOLTEK_A1FA_PID, 2, 0xFF00, 0xFF00);
