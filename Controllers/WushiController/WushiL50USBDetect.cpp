/*---------------------------------------------------------*\
| WushiL50USBControllerDetect.cpp                           |
|                                                           |
|   Detector for Wushi L50                                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "WushiL50USBController.h"
#include "RGBController_WushiL50USB.h"

/*-----------------------------------------------------*\
| Wushi vendor ID                                       |
\*-----------------------------------------------------*/
#define WUSHI_VID                               0x306F

/*-----------------------------------------------------*\
| Wushi device ID                                       |
\*-----------------------------------------------------*/
#define WUSHI_PID                               0x1234

void DetectWushiL50USBControllers(hidapi_wrapper wrapper, hid_device_info* info, const std::string& name)
{
    hid_device* dev = wrapper.hid_open_path(info->path);

    if(dev)
    {
        WushiL50USBController*     controller      = new WushiL50USBController(wrapper, dev, info->path, name);
        RGBController_WushiL50USB* rgb_controller  = new RGBController_WushiL50USB(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_WRAPPED_DETECTOR("JSAUX RGB Docking Station", DetectWushiL50USBControllers, WUSHI_VID, WUSHI_PID);
