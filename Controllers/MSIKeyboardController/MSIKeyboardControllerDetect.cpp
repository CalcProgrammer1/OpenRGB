/*---------------------------------------------------------*\
| MSIKeyboardControllerDetect.cpp                           |
|                                                           |
|   Detector for MSI Mystic Light MS-1565 Keyboard          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "MSIMysticLightKBController.h"
#include "RGBController_MSIMysticLightKB.h"

#define MSI_USB_VID         0x1462

/*----------------------------------------------------------*\
|                                                            |
|   DetectMSIKeyboardController                              |
|                                                            |
|       Detect MSI Mystic Light MS-1565 keyboard             |
|                                                            |
\*----------------------------------------------------------*/

void DetectMSIKeyboardController
    (
    hid_device_info*    info,
    const std::string&  /*name*/
    )
{
    hid_device* dev = hid_open_path(info->path);
    if(dev != nullptr)
    {
        MSIKeyboardController*     controller     = new MSIKeyboardController(dev, info->path);
        RGBController_MSIKeyboard* rgb_controller = new RGBController_MSIKeyboard(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("MSI Keyboard MS_1565", DetectMSIKeyboardController, MSI_USB_VID, 0x1601, 0x00FF, 0x01);