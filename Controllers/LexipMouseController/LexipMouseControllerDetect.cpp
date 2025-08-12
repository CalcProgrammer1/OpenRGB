/*---------------------------------------------------------*\
| LexipMouseControllerDetect.cpp                            |
|                                                           |
|   Detector for Lexip mouse                                |
|                                                           |
|   Morgan Guimard (morg)                       21 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "LexipMouseController.h"
#include "RGBController_LexipMouse.h"

/*---------------------------------------------------------*\
| Lexip vendor ID                                           |
\*---------------------------------------------------------*/
#define LEXIP_VID                                      0x04D8

/*---------------------------------------------------------*\
| Product ID                                                |
\*---------------------------------------------------------*/
#define LEXIP_NP93_ALPHA_PID                           0xFD0A

void DetectLexipMouseControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LexipMouseController*     controller        = new LexipMouseController(dev, *info, name);
        RGBController_LexipMouse* rgb_controller    = new RGBController_LexipMouse(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Np93 ALPHA - Gaming Mouse", DetectLexipMouseControllers, LEXIP_VID, LEXIP_NP93_ALPHA_PID, 0, 0x0001, 2);
