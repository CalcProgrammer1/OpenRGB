/*---------------------------------------------------------*\
| AsusLegacyUSBControllerDetect.cpp                         |
|                                                           |
|   Detector for ASUS legacy USB devices                    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "AsusCerberusKeyboardController.h"
#include "AsusSagarisKeyboardController.h"
#include "AsusStrixClawController.h"
#include "RGBController_AsusCerberusKeyboard.h"
#include "RGBController_AsusSagarisKeyboard.h"
#include "RGBController_AsusStrixClaw.h"

#define ASUS_LEGACY_USB_VID         0x195D
#define ASUS_USB_VID                0x0B05

#define ASUS_CERBERUS_MECH_PID      0x2047
#define ASUS_SAGARIS_GK1100_PID     0x1835
#define ASUS_ROG_STRIX_CLAW_PID     0x1016

void DetectAsusCerberusMech(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AsusCerberusKeyboardController*     controller          = new AsusCerberusKeyboardController(dev, info->path, info->release_number, name);
        RGBController_AsusCerberusKeyboard* rgb_controller      = new RGBController_AsusCerberusKeyboard(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusSagarisKeyboard(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AsusSagarisKeyboardController*     controller          = new AsusSagarisKeyboardController(dev, info->path, info->release_number, name);
        RGBController_AsusSagarisKeyboard* rgb_controller      = new RGBController_AsusSagarisKeyboard(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAsusStrixClaw(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        StrixClawController*     controller          = new StrixClawController(dev, info->path, name);
        RGBController_StrixClaw* rgb_controller      = new RGBController_StrixClaw(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("ASUS Cerberus Mech",  DetectAsusCerberusMech,    ASUS_LEGACY_USB_VID, ASUS_CERBERUS_MECH_PID,  1, 0xFF01, 1);
REGISTER_HID_DETECTOR_IPU("ASUS Sagaris GK1100", DetectAsusSagarisKeyboard, ASUS_USB_VID,        ASUS_SAGARIS_GK1100_PID, 1, 0xFF02, 2);
REGISTER_HID_DETECTOR_IPU("ASUS ROG Strix Claw", DetectAsusStrixClaw,       ASUS_LEGACY_USB_VID, ASUS_ROG_STRIX_CLAW_PID, 0, 0xFF01, 1);
