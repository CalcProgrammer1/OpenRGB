/*---------------------------------------------------------*\
| RobobloqLightStripControllerDetect.cpp                    |
|                                                           |
|   Detector for Robobloq RGB Light Strips                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "RobobloqLightStripController.h"
#include "RGBController_RobobloqLightStrip.h"

#define ROBOBLOQ_USB_VID 0x1A86

/*----------------------------------------------------------*\
|                                                            |
|   DetectRobobloqLightStripController                       |
|                                                            |
|       Detect Robobloq RGB Light Strips                     |
|                                                            |
\*----------------------------------------------------------*/

void DetectRobobloqLightStripController
    (
    hid_device_info*    info,
    const std::string&  name
    )
{
    hid_device* dev = hid_open_path(info->path);
    if(dev != nullptr)
    {
        RobobloqLightStripController*     controller     = new RobobloqLightStripController(dev, info->path, name);
        RGBController_RobobloqLightStrip* rgb_controller = new RGBController_RobobloqLightStrip(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("Robobloq Monitor Light Strip", DetectRobobloqLightStripController, ROBOBLOQ_USB_VID, 0xFE07, 0xFF00, 0x01);
