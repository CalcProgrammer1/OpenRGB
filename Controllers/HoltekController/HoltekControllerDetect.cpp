/*---------------------------------------------------------*\
| HoltekControllerDetect.cpp                                |
|                                                           |
|   Detector for Holtek devices                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "HoltekA070Controller.h"
#include "RGBController_HoltekA070.h"
#include "HoltekA1FAController.h"
#include "RGBController_HoltekA1FA.h"
#include "HoltekA09FController.h"
#include "RGBController_HoltekA09F.h"

/*-----------------------------------------------------*\
| Holtek Semiconductor Inc. vendor ID                   |
\*-----------------------------------------------------*/
#define HOLTEK_VID                    0x04D9
/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define HOLTEK_A070_PID               0xA070
/*-----------------------------------------------------*\
| Phoenix Void product ID                               |
\*-----------------------------------------------------*/
#define HOLTEK_A09F_PID               0xA09F
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

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectHoltekControllers() */

void DetectHoltekMousemats(hid_device_info *info, const std::string &name)
{
    hid_device *dev = hid_open_path(info->path);

    if(dev)
    {
        HoltekA1FAController*     controller     = new HoltekA1FAController(dev, info->path, name);
        RGBController_HoltekA1FA* rgb_controller = new RGBController_HoltekA1FA(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
} /* DetectHoltekMousemats() */

void DetectHoltekA09FControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        HoltekA09FController* controller = new HoltekA09FController(dev, info->path, name);

        if(controller->IsReady())
        {
            RGBController_HoltekA09F* rgb_controller = new RGBController_HoltekA09F(controller);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            /* Duplicate HID collection entry — physical device already registered */
            delete controller;
        }
    }
} /* DetectHoltekA09FControllers() */

REGISTER_HID_DETECTOR_IPU("Holtek USB Gaming Mouse", DetectHoltekControllers,      HOLTEK_VID, HOLTEK_A070_PID, 1, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Holtek Mousemat",         DetectHoltekMousemats,        HOLTEK_VID, HOLTEK_A1FA_PID, 2, 0xFF00, 0xFF00);
REGISTER_HID_DETECTOR_I("Phoenix Void",               DetectHoltekA09FControllers,  HOLTEK_VID, HOLTEK_A09F_PID, 1);
