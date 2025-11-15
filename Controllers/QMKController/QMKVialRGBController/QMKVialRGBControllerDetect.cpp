/*---------------------------------------------------------*\
| QMKVialRGBControllerDetect.cpp                            |
|                                                           |
|   Detector for VialRGB QMK Keyboard Protocol              |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      29 Sep 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string>
#include <hidapi.h>
#include "Detector.h"
#include "QMKVialRGBController.h"
#include "RGBController_QMKVialRGB.h"
#include "SettingsManager.h"

/*-----------------------------------------------------*\
| USB IDs                                               |
\*-----------------------------------------------------*/
#define RASPBERRY_PI_VID                        0x2E8A
#define RASPBERRY_PI_500_PLUS_PID               0x0011

/*-----------------------------------------------------*\
| Usage and Usage Page                                  |
\*-----------------------------------------------------*/
#define QMK_USAGE_PAGE                          0xFF60
#define QMK_USAGE                               0x61

void DetectQMKVialRGBControllers(hid_device_info *info, const std::string&)
{
    hid_device *dev = hid_open_path(info->path);

    if(dev)
    {
        QMKVialRGBController* controller = new QMKVialRGBController(dev, info->path);

        if(controller->GetSupported())
        {
            RGBController_QMKVialRGB* rgb_controller = new RGBController_QMKVialRGB(controller);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            delete controller;
        }
    }
}

void RegisterQMKVialRGBDetectors()
{
    /*-------------------------------------------------*\
    | Get QMKVialRGB settings                           |
    \*-------------------------------------------------*/
    json vial_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("QMKVialRGBDevices");

    if(vial_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < vial_settings["devices"].size(); device_idx++)
        {
            if( vial_settings["devices"][device_idx].contains("usb_pid")
             && vial_settings["devices"][device_idx].contains("usb_vid")
             && vial_settings["devices"][device_idx].contains("name"))
            {
                std::string usb_pid_str = vial_settings["devices"][device_idx]["usb_pid"];
                std::string usb_vid_str = vial_settings["devices"][device_idx]["usb_vid"];
                std::string name        = vial_settings["devices"][device_idx]["name"];

                /*-------------------------------------*\
                | Parse hex string to integer           |
                \*-------------------------------------*/
                unsigned short usb_pid  = std::stoi(usb_pid_str, 0, 16);
                unsigned short usb_vid  = std::stoi(usb_vid_str, 0, 16);

                REGISTER_DYNAMIC_HID_DETECTOR_PU(name, DetectQMKVialRGBControllers, usb_vid, usb_pid, QMK_USAGE_PAGE, QMK_USAGE);
            }
        }
    }
}

REGISTER_DYNAMIC_DETECTOR("QMK VialRGB Devices", RegisterQMKVialRGBDetectors);

REGISTER_HID_DETECTOR_PU( "Raspberry Pi 500+", DetectQMKVialRGBControllers, RASPBERRY_PI_VID, RASPBERRY_PI_500_PLUS_PID, QMK_USAGE_PAGE, QMK_USAGE );
