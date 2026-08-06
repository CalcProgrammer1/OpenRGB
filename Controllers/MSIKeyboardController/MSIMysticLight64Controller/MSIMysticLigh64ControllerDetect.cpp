/*---------------------------------------------------------*\
| MSIMysticLigh64ControllerDetect.cpp                       |
|                                                           |
|   Detector for MSI Mystic Light 64 keyboards              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "MSIMysticLight64Controller.h"
#include "RGBController_MSIMysticLight64.h"

/*---------------------------------------------------------*\
| USB IDs                                                   |
\*---------------------------------------------------------*/
#define MSI_USB_VID                                 0x1462

DetectedControllers DetectMSIMysticLight64Controllers(hid_device_info* info, const std::string& /*name*/)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev != nullptr)
    {
        MSIMysticLight64Controller*     controller     = new MSIMysticLight64Controller(dev, info->path);
        RGBController_MSIMysticLight64* rgb_controller = new RGBController_MSIMysticLight64(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_1562",    DetectMSIMysticLight64Controllers, MSI_USB_VID,    0x1562,   0x00FF, 0x01);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_1563",    DetectMSIMysticLight64Controllers, MSI_USB_VID,    0x1563,   0x00FF, 0x01);
REGISTER_HID_DETECTOR_PU("MSI Mystic Light MS_1564",    DetectMSIMysticLight64Controllers, MSI_USB_VID,    0x1564,   0x00FF, 0x01);