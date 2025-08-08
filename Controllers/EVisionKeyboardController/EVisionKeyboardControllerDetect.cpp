/*---------------------------------------------------------*\
| EVisionKeyboardControllerDetect.cpp                       |
|                                                           |
|   Detector for EVision keyboards                          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "EVisionKeyboardController.h"
#include "EVisionV2KeyboardController.h"
#include "RGBController_EVisionKeyboard.h"
#include "RGBController_EVisionV2Keyboard.h"
#include "SettingsManager.h"

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define EVISION_KEYBOARD_VID        0x0C45
#define EVISION_KEYBOARD2_VID       0x320F
#define EVISION_KEYBOARD3_VID       0x3299
#define EVISION_KEYBOARD_USAGE_PAGE 0xFF1C
#define ENDORFY_OMNIS_PID           0x0012
#define DEXP_BLAZE_PID              0x5084
#define GLORIOUS_GMMK_TKL_PID       0x5064
#define REDRAGON_K550_PID           0x5204
#define MARS_GAMING_MKMINI_PID      0x5078
#define SKILLKORP_K5_PID            0x505B
#define REDRAGON_K552_PID           0x5104
#define REDRAGON_K552_V2_PID        0x5000
#define REDRAGON_K556_PID           0x5004
#define TECWARE_PHANTOM_ELITE_PID   0x652F
#define WARRIOR_KANE_TC235          0x8520
#define WOMIER_K87_PID              0x502A
#define WOMIER_K66_PID              0x7698
#define BYGG_CSB_ICL01_PID          0x5041
#define GAMEPOWER_OGRE_RGB_PID      0x7672

/******************************************************************************************\
*                                                                                          *
*   DetectEVisionKeyboards                                                                 *
*                                                                                          *
*       Tests the USB address to see if an EVision RGB Keyboard controller exists there.   *
*                                                                                          *
\******************************************************************************************/

void DetectEVisionKeyboards(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        EVisionKeyboardController*     controller     = new EVisionKeyboardController(dev, info->path, name);
        RGBController_EVisionKeyboard* rgb_controller = new RGBController_EVisionKeyboard(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectEVisionV2Keyboards(hid_device_info* info, const std::string& name)
{
    json settings   = ResourceManager::get()->GetSettingsManager()->GetSettings("EVision2Settings");
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        EVisionV2KeyboardController*     controller     = new EVisionV2KeyboardController(dev, info->path, EVISION_V2_KEYBOARD_LAYOUT, name);
        RGBController_EVisionV2Keyboard* rgb_controller = new RGBController_EVisionV2Keyboard(controller, EVISION_V2_KEYBOARD_PART_KEYBOARD);

        ResourceManager::get()->RegisterRGBController(rgb_controller);

        if(!settings.contains("AdditionalZones") || settings["AdditionalZones"] == true)
        {
            rgb_controller                             = new RGBController_EVisionV2Keyboard(controller, EVISION_V2_KEYBOARD_PART_LOGO);

            ResourceManager::get()->RegisterRGBController(rgb_controller);

            rgb_controller                             = new RGBController_EVisionV2Keyboard(controller, EVISION_V2_KEYBOARD_PART_EDGE);

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}

void DetectEndorfyKeyboards(hid_device_info* info, const std::string& name)
{
    json settings   = ResourceManager::get()->GetSettingsManager()->GetSettings("EndorfySettings");
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        EVisionV2KeyboardController*     controller     = new EVisionV2KeyboardController(dev, info->path, ENDORFY_KEYBOARD_LAYOUT, name);
        RGBController_EVisionV2Keyboard* rgb_controller = new RGBController_EVisionV2Keyboard(controller, EVISION_V2_KEYBOARD_PART_KEYBOARD);

        ResourceManager::get()->RegisterRGBController(rgb_controller);

        if(!settings.contains("AdditionalZones") || settings["AdditionalZones"] == true)
        {
            rgb_controller                              = new RGBController_EVisionV2Keyboard(controller, ENDORFY_KEYBOARD_PART_EDGE);

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                                                                     |
\*---------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("EVision Keyboard 320F:5078",      DetectEVisionKeyboards,   EVISION_KEYBOARD2_VID, MARS_GAMING_MKMINI_PID,    1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 0C45:5204",      DetectEVisionKeyboards,   EVISION_KEYBOARD_VID,  REDRAGON_K550_PID,         1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 0C45:5104",      DetectEVisionKeyboards,   EVISION_KEYBOARD_VID,  REDRAGON_K552_PID,         1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 320F:5000",      DetectEVisionKeyboards,   EVISION_KEYBOARD2_VID, REDRAGON_K552_V2_PID,      1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 0C45:5004",      DetectEVisionKeyboards,   EVISION_KEYBOARD_VID,  REDRAGON_K556_PID,         1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 0C45:652F",      DetectEVisionKeyboards,   EVISION_KEYBOARD_VID,  TECWARE_PHANTOM_ELITE_PID, 1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 0C45:8520",      DetectEVisionKeyboards,   EVISION_KEYBOARD_VID,  WARRIOR_KANE_TC235,        1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 320F:502A",      DetectEVisionKeyboards,   EVISION_KEYBOARD2_VID, WOMIER_K87_PID,            1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 0C45:7698",      DetectEVisionKeyboards,   EVISION_KEYBOARD_VID,  WOMIER_K66_PID,            1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 320F:5064",      DetectEVisionKeyboards,   EVISION_KEYBOARD2_VID, GLORIOUS_GMMK_TKL_PID,     1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 320F:5084",      DetectEVisionKeyboards,   EVISION_KEYBOARD2_VID, DEXP_BLAZE_PID,            1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 320F:505B",      DetectEVisionKeyboards,   EVISION_KEYBOARD2_VID, SKILLKORP_K5_PID,          1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Endorfy Omnis",                   DetectEndorfyKeyboards,   EVISION_KEYBOARD3_VID, ENDORFY_OMNIS_PID,         1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("CSB/ICL01 Keyboard",              DetectEVisionV2Keyboards, EVISION_KEYBOARD2_VID, BYGG_CSB_ICL01_PID,        1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Gamepower Ogre RGB 0C45:7672",    DetectEVisionKeyboards,   EVISION_KEYBOARD_VID,  GAMEPOWER_OGRE_RGB_PID,    1, EVISION_KEYBOARD_USAGE_PAGE);
