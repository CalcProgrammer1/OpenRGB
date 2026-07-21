/*---------------------------------------------------------*\
| EVisionKeyboardV2ControllerDetect.cpp                     |
|                                                           |
|   Detector for EVision V2 keyboards                       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "EVisionV2KeyboardController.h"
#include "RGBController_EVisionV2Keyboard.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

/*---------------------------------------------------------*\
| EVision vendor ID                                         |
\*---------------------------------------------------------*/
#define EVISION_VID                                 0x320F
#define SPCGEAR_VID                                 0x3299

/*---------------------------------------------------------*\
| EVision Usage Page                                        |
\*---------------------------------------------------------*/
#define EVISION_KEYBOARD_USAGE_PAGE                 0xFF1C

/*---------------------------------------------------------*\
| Keyboard product IDs                                      |
\*---------------------------------------------------------*/
#define ENDORFY_OMNIS_PID                           0x0012
#define BYGG_CSB_ICL01_PID                          0x5041

DetectedControllers DetectEVisionV2Keyboards(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    json                settings    = ResourceManager::get()->GetSettingsManager()->GetSettings("EVision2Settings");
    hid_device*         dev         = hid_open_path(info->path);

    if(dev)
    {
        EVisionV2KeyboardController*     controller     = new EVisionV2KeyboardController(dev, info->path, EVISION_V2_KEYBOARD_LAYOUT, name);
        RGBController_EVisionV2Keyboard* rgb_controller = new RGBController_EVisionV2Keyboard(controller, EVISION_V2_KEYBOARD_PART_KEYBOARD);

        detected_controllers.push_back(rgb_controller);

        if(!settings.contains("AdditionalZones") || settings["AdditionalZones"] == true)
        {
            rgb_controller                             = new RGBController_EVisionV2Keyboard(controller, EVISION_V2_KEYBOARD_PART_LOGO);

            detected_controllers.push_back(rgb_controller);

            rgb_controller                             = new RGBController_EVisionV2Keyboard(controller, EVISION_V2_KEYBOARD_PART_EDGE);

            detected_controllers.push_back(rgb_controller);
        }
    }

    return(detected_controllers);
}

DetectedControllers DetectEndorfyKeyboards(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    json                settings    = ResourceManager::get()->GetSettingsManager()->GetSettings("EndorfySettings");
    hid_device*         dev         = hid_open_path(info->path);

    if(dev)
    {
        EVisionV2KeyboardController*     controller     = new EVisionV2KeyboardController(dev, info->path, ENDORFY_KEYBOARD_LAYOUT, name);
        RGBController_EVisionV2Keyboard* rgb_controller = new RGBController_EVisionV2Keyboard(controller, EVISION_V2_KEYBOARD_PART_KEYBOARD);

        detected_controllers.push_back(rgb_controller);

        if(!settings.contains("AdditionalZones") || settings["AdditionalZones"] == true)
        {
            rgb_controller                              = new RGBController_EVisionV2Keyboard(controller, ENDORFY_KEYBOARD_PART_EDGE);

            detected_controllers.push_back(rgb_controller);
        }
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| Keyboards                                                 |
\*---------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("EVision Keyboard 320F:5041",  DetectEVisionV2Keyboards,   EVISION_VID,    BYGG_CSB_ICL01_PID, 1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Endorfy Omnis",               DetectEndorfyKeyboards,     SPCGEAR_VID,    ENDORFY_OMNIS_PID,  1, EVISION_KEYBOARD_USAGE_PAGE);
