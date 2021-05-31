/*-------------------------------------------------------------------*\
|  QMKOpenRGBControllerDetect.cpp                                     |
|                                                                     |
|  Driver for QMK keyboards using OpenRGB Protocol                    |
|                                                                     |
|  Kasper       10th Octobber 2020                                    |
|  Jath03       28th May 2021                                         |
\*-------------------------------------------------------------------*/

#include <iostream>
#include <string>

#include "Detector.h"
#include "QMKOpenRGBController.h"
#include "RGBController.h"
#include "RGBController_QMKOpenRGB.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Protocol version                                      |
\*-----------------------------------------------------*/
#define QMK_OPENRGB_PROTOCOL_VERSION            0x09

/*-----------------------------------------------------*\
| Usage and Usage Page                                  |
\*-----------------------------------------------------*/
#define QMK_USAGE_PAGE                          0xFF60
#define QMK_USAGE                               0x61

void DetectQMKOpenRGBControllers(hid_device_info *info, const std::string&)
{
    hid_device *dev = hid_open_path(info->path);

    if(dev)
    {
        QMKOpenRGBController* controller                = new QMKOpenRGBController(dev, info->path);
        unsigned int version                            = controller->GetProtocolVersion();

        if(version == QMK_OPENRGB_PROTOCOL_VERSION)
        {
            RGBController_QMKOpenRGB* rgb_controller    = new RGBController_QMKOpenRGB(controller);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            delete controller;
        }
    }
}

void RegisterQMKDetectors()
{
    /*-------------------------------------------------*\
    | Get QMKOpenRGB settings                           |
    \*-------------------------------------------------*/
    json qmk_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("QMKOpenRGBDevices");

    if(qmk_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < qmk_settings["devices"].size(); device_idx++)
        {
            if( qmk_settings["devices"][device_idx].contains("usb_pid")
             && qmk_settings["devices"][device_idx].contains("usb_vid")
             && qmk_settings["devices"][device_idx].contains("name"))
            {
                std::string usb_pid_str = qmk_settings["devices"][device_idx]["usb_pid"];
                std::string usb_vid_str = qmk_settings["devices"][device_idx]["usb_vid"];
                std::string name        = qmk_settings["devices"][device_idx]["name"];

                /*-------------------------------------*\
                | Parse hex string to integer           |
                \*-------------------------------------*/
                unsigned short usb_pid  = std::stoi(usb_pid_str, 0, 16);
                unsigned short usb_vid  = std::stoi(usb_vid_str, 0, 16);

                REGISTER_DYNAMIC_HID_DETECTOR_IPU(name, DetectQMKOpenRGBControllers, usb_vid, usb_pid, 1, QMK_USAGE_PAGE, QMK_USAGE);
            }
        }
    }
}

REGISTER_DYNAMIC_DETECTOR("QMK OpenRGB Devices", RegisterQMKDetectors);