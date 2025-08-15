/*---------------------------------------------------------*\
| SkyloongControllerDetect.cpp                              |
|                                                           |
|   Detector for Skyloong Keyboards                         |
|                                                           |
|   Givo (givowo)                               30 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "SkyloongGK104ProController.h"
#include "RGBController_SkyloongGK104Pro.h"

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define SKYLOONG_KEYBOARD_VID           0x1EA7
#define SKYLOONG_GK104_PRO_PID          0x0907
#define SKYLOONG_GK104_PRO_I            1

/******************************************************************************************\
*                                                                                          *
*   DetectSkyloongGK104Pro                                                                 *
*                                                                                          *
*       Tests the USB address to see if a Skyloong GK104 Pro controller exists there.      *
*                                                                                          *
\******************************************************************************************/
void DetectSkyloongGK104Pro(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        SkyloongGK104ProController* controller          = new SkyloongGK104ProController(dev, info->path, name);
        RGBController_SkyloongGK104Pro* rgb_controller  = new RGBController_SkyloongGK104Pro(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                                                                     |
\*---------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_I("Skyloong GK104 Pro", DetectSkyloongGK104Pro, SKYLOONG_KEYBOARD_VID, SKYLOONG_GK104_PRO_PID, SKYLOONG_GK104_PRO_I);
