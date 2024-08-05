/*---------------------------------------------------------*\
| DasKeyboardControllerDetect.cpp                           |
|                                                           |
|   Detector for Das Keyboard keyboard                      |
|                                                           |
|   Frank Niessen (denk_mal)                    16 Dec 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "DasKeyboardController.h"
#include "RGBController.h"
#include "RGBController_DasKeyboard.h"
#include <hidapi.h>

/*-----------------------------------------------------*\
| Das Keyboard vendor ID                                |
\*-----------------------------------------------------*/
#define DAS_KEYBOARD_VID                0x24F0

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define DAS_KEYBOARD_Q4_PID             0x2037
#define DAS_KEYBOARD_Q5_PID             0x2020
#define DAS_KEYBOARD_Q5S_PID            0x209A

/******************************************************************************************\
*                                                                                          *
*   DetectDasKeyboardControllers                                                           *
*                                                                                          *
*       Tests the USB address to see if a Das Keyboard RGB controller exists there.        *
*       We need the second interface to communicate with the keyboard                      *
*                                                                                          *
\******************************************************************************************/

void DetectDasKeyboardControllers(hid_device_info *info_in, const std::string &name)
{
    hid_device_info *info = info_in;

    while(info)
    {
        if(info->vendor_id        == DAS_KEYBOARD_VID     &&
          (info->product_id       == DAS_KEYBOARD_Q4_PID  ||
           info->product_id       == DAS_KEYBOARD_Q5_PID  ||
           info->product_id       == DAS_KEYBOARD_Q5S_PID) &&
           info->interface_number == 1)
        {
            break;
        }
        info = info->next;
    }

    if(!info)
    {
        return;
    }

    hid_device *dev = hid_open_path(info->path);

    if(dev)
    {
        DasKeyboardController *controller = new DasKeyboardController(dev, info->path);

        if(controller->GetLayoutString() == "NONE")
        {
            delete controller;
        }
        else
        {
            RGBController_DasKeyboard *rgb_controller = new RGBController_DasKeyboard(controller);
            rgb_controller->SetupZones();
            rgb_controller->name = name;

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}   /* DetectDasKeyboardControllers() */

void DetectDas4QKeyboard(hid_device_info *info, const std::string &name)
{
    hid_device *dev = hid_open_path(info->path);

    if(dev)
    {
        DasKeyboardController *controller = new DasKeyboardController(dev, info->path);

        if(controller->GetLayoutString() == "NONE")
        {
            delete controller;
        }
        else
        {
            RGBController_DasKeyboard *rgb_controller = new RGBController_DasKeyboard(controller);
            rgb_controller->SetupZones();
            rgb_controller->name = name;

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}   /* DetectDas4QKeyboard() */

REGISTER_HID_DETECTOR_IPU("Das Keyboard Q4 RGB",  DetectDas4QKeyboard,          DAS_KEYBOARD_VID, DAS_KEYBOARD_Q4_PID,  1,    0x01,   0x80);
REGISTER_HID_DETECTOR_I  ("Das Keyboard Q5 RGB",  DetectDasKeyboardControllers, DAS_KEYBOARD_VID, DAS_KEYBOARD_Q5_PID,  1);
REGISTER_HID_DETECTOR_I  ("Das Keyboard Q5S RGB", DetectDasKeyboardControllers, DAS_KEYBOARD_VID, DAS_KEYBOARD_Q5S_PID, 1);
