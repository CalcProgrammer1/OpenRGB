/*---------------------------------------------------------*\
| LuxaforControllerDetect.cpp                               |
|                                                           |
|   Detector for Luxafor devices                            |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      05 Sep 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "LuxaforController.h"
#include "RGBController_Luxafor.h"

/*---------------------------------------------------------*\
| Luxafor USB Vendor ID                                     |
\*---------------------------------------------------------*/
#define LUXAFOR_VID                                 0x04D8

/*---------------------------------------------------------*\
| Luxafor USB Product ID                                    |
\*---------------------------------------------------------*/
#define LUXAFOR_FLAG_PID                            0xF372

void DetectLuxaforControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LuxaforController*     controller     = new LuxaforController(dev, info->path, name);
        RGBController_Luxafor* rgb_controller = new RGBController_Luxafor(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR( "Luxafor Flag", DetectLuxaforControllers, LUXAFOR_VID, LUXAFOR_FLAG_PID );
