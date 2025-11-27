/*---------------------------------------------------------*\
| FnaticStreakControllerDetect.cpp                          |
|                                                           |
|   Detector for Fnatic Streak and miniStreak keyboards     |
|                                                           |
|   Based on leddy project by Hanna Czenczek                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "FnaticStreakController.h"
#include "RGBController_FnaticStreak.h"

/*-----------------------------------------------------*\
| Fnatic keyboard vendor and product IDs                |
| Based on leddy project keyboard.rs                    |
| VID: 0x2f0e                                           |
| PID: 0x0101 (Streak full)                             |
| PID: 0x0102 (miniStreak)                              |
| Interface: 1                                          |
\*-----------------------------------------------------*/
#define FNATIC_VID                                  0x2F0E

#define FNATIC_STREAK_PID                           0x0101
#define FNATIC_MINISTREAK_PID                       0x0102

void DetectFnaticStreakKeyboard(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        FnaticStreakType kb_type;

        if(info->product_id == FNATIC_MINISTREAK_PID)
        {
            kb_type = FNATIC_STREAK_TYPE_MINI;
        }
        else
        {
            kb_type = FNATIC_STREAK_TYPE_FULL;
        }

        FnaticStreakController*     controller     = new FnaticStreakController(dev, info, name, kb_type);
        RGBController_FnaticStreak* rgb_controller = new RGBController_FnaticStreak(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_I("Fnatic Streak",      DetectFnaticStreakKeyboard, FNATIC_VID, FNATIC_STREAK_PID,      1);
REGISTER_HID_DETECTOR_I("Fnatic miniStreak",  DetectFnaticStreakKeyboard, FNATIC_VID, FNATIC_MINISTREAK_PID,  1);
