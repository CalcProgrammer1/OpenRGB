/*---------------------------------------------------------*\
| QMKOpenRGBControllerDetect.cpp                            |
|                                                           |
|   Detector for OpenRGB QMK Keyboard Protocol              |
|                                                           |
|   Kasper                                      10 Oct 2020 |
|   Jath03                                      28 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string>
#include <hidapi.h>
#include "Detector.h"
#include "QMKOpenRGBRev9Controller.h"
#include "QMKOpenRGBRevBController.h"
#include "QMKOpenRGBRevDController.h"
#include "RGBController_QMKOpenRGBRev9.h"
#include "RGBController_QMKOpenRGBRevB.h"
#include "RGBController_QMKOpenRGBRevD.h"
#include "RGBController_QMKOpenRGBRevE.h"
#include "LogManager.h"
#include "SettingsManager.h"

/*-----------------------------------------------------*\
| Protocol version                                      |
\*-----------------------------------------------------*/
#define QMK_OPENRGB_PROTOCOL_VERSION_9          0x09
#define QMK_OPENRGB_PROTOCOL_VERSION_B          0x0B
#define QMK_OPENRGB_PROTOCOL_VERSION_C          0x0C
#define QMK_OPENRGB_PROTOCOL_VERSION_D          0x0D
#define QMK_OPENRGB_PROTOCOL_VERSION_E          0x0E

/*-----------------------------------------------------*\
| Usage and Usage Page                                  |
\*-----------------------------------------------------*/
#define QMK_USAGE_PAGE                          0xFF60
#define QMK_USAGE                               0x61

unsigned int GetProtocolVersion(hid_device *dev)
{
    unsigned char usb_buf[QMK_OPENRGB_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_OPENRGB_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_OPENRGB_GET_PROTOCOL_VERSION;

    int bytes_read = 0;
    do
    {
        hid_write(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE);
        bytes_read = hid_read_timeout(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE, QMK_OPENRGB_HID_READ_TIMEOUT);
    } while(bytes_read <= 0);

    return usb_buf[1];
}

void DetectQMKOpenRGBControllers(hid_device_info *info, const std::string&)
{
    hid_device *dev = hid_open_path(info->path);

    if(dev)
    {
        /*-----------------------------------------------------*\
        | Use Rev9 controller for getting protocol version.     |
        | Protocol version request may not change across        |
        | protocol versions                                     |
        \*-----------------------------------------------------*/
        unsigned int version                                = GetProtocolVersion(dev);

        switch(version)
        {
            case QMK_OPENRGB_PROTOCOL_VERSION_9:
                {
                QMKOpenRGBRev9Controller*     controller     = new QMKOpenRGBRev9Controller(dev, info->path);
                RGBController_QMKOpenRGBRev9* rgb_controller = new RGBController_QMKOpenRGBRev9(controller);
                ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
                break;
            case QMK_OPENRGB_PROTOCOL_VERSION_B:
                {
                QMKOpenRGBRevBController*     controller     = new QMKOpenRGBRevBController(dev, info->path);
                RGBController_QMKOpenRGBRevB* rgb_controller = new RGBController_QMKOpenRGBRevB(controller, false);
                ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
                break;
            case QMK_OPENRGB_PROTOCOL_VERSION_C:
                {
                QMKOpenRGBRevBController*     controller     = new QMKOpenRGBRevBController(dev, info->path);
                RGBController_QMKOpenRGBRevB* rgb_controller = new RGBController_QMKOpenRGBRevB(controller, true);
                ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
                break;
            case QMK_OPENRGB_PROTOCOL_VERSION_D:
                {
                QMKOpenRGBRevDController*     controller     = new QMKOpenRGBRevDController(dev, info->path);
                RGBController_QMKOpenRGBRevD* rgb_controller = new RGBController_QMKOpenRGBRevD(controller, true);
                ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
                break;
            case QMK_OPENRGB_PROTOCOL_VERSION_E:
                {
                QMKOpenRGBRevDController*     controller     = new QMKOpenRGBRevDController(dev, info->path);
                RGBController_QMKOpenRGBRevE* rgb_controller = new RGBController_QMKOpenRGBRevE(controller, true);
                ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
                break;
            default:
                if (version == 0)
                {
                    LOG_WARNING("[QMK OpenRGB] Detection failed - the detected keyboard does not have the OpenRGB protocol feature enabled! \n"
                    "Please make sure your keyboard supports RGB Matrix, add OPENRGB_ENABLE = yes to the rules.mk inside your keymap folder, compile and flash again!");
                }
                else if (version < QMK_OPENRGB_PROTOCOL_VERSION_9)
                {
                    LOG_WARNING("[QMK OpenRGB] Detection failed - the detected keyboard is using an outdated protocol version %i. Please update to to the update to the latest version of QMK-OpenRGB! \n"
                    "For officaly supported QMK boards grab <a href=\"https://github.com/Kasper24/QMK-OpenRGB\">url</a> \n"
                    "For Sonix boards grab <a href=\"https://github.com/SonixQMK/qmk_firmware/tree/sn32_openrgb\">url</a>", version);
                }
                else if (version > QMK_OPENRGB_PROTOCOL_VERSION_E)
                {
                    LOG_WARNING("[QMK OpenRGB] Detection failed - the detected keyboard is using version protocol %i which is not supported by this OpenRGB build. Please update OpenRGB!", version);
                }
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
