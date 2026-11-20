/*---------------------------------------------------------*\
| RoyuanKeyboardControllerDetect.cpp                        |
|                                                           |
|   Detector for ROYUAN-based keyboard lighting controllers |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cwchar>
#include <hidapi.h>
#include "DetectionManager.h"
#include "EpomakerController.h"
#include "RGBController_AkkoKeyboard.h"
#include "RGBController_EpomakerController.h"
#include "RGBController_SkyloongGK68HEPro.h"
#include "RoyuanKeyboardController.h"

#define ROYUAN_KEYBOARD_VID                         0x3151

#define ROYUAN_AKKO_B_SERIES_PID                    0x4003
#define ROYUAN_EPOMAKER_TH80_PRO_USB_PID            0x4010
#define ROYUAN_EPOMAKER_TH80_PRO_DONGLE_PID         0x4011
#define ROYUAN_ATTACKSHARK_K86_USB_PID              0x4015
#define ROYUAN_SKYLOONG_GK68HE_PRO_PID              0x5029

DetectedControllers DetectRoyuanAkkoKeyboardControllers(hid_device_info* info, const std::string&)
{
    DetectedControllers detected_controllers;

    if(info->interface_number == 0)
    {
        if(info->product_string != nullptr && (wcsstr(info->product_string, L"Dukharo") != nullptr || wcsstr(info->product_string, L"VN66") != nullptr))
        {
            return(detected_controllers);
        }
    }

    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        RoyuanKeyboardProfile profile = RoyuanKeyboardProfile::AkkoBSeries();

        if(info->product_string != nullptr && (wcsstr(info->product_string, L"Dukharo") != nullptr || wcsstr(info->product_string, L"VN66") != nullptr))
        {
            profile = RoyuanKeyboardProfile::DukharoVN66();
        }
        else
        {
            wchar_t prod_str[256] = { 0 };
            if(hid_get_product_string(dev, prod_str, 256) == 0)
            {
                if(wcsstr(prod_str, L"Dukharo") != nullptr || wcsstr(prod_str, L"VN66") != nullptr)
                {
                    if(info->interface_number == 0)
                    {
                        hid_close(dev);
                        return(detected_controllers);
                    }
                    profile = RoyuanKeyboardProfile::DukharoVN66();
                }
            }
        }

        RoyuanKeyboardController* controller = new RoyuanKeyboardController(dev, info->path, profile);

        if(controller->IsValid())
        {
            detected_controllers.push_back(new RGBController_AkkoKeyboard(controller));
        }
        else
        {
            delete controller;
        }
    }

    return(detected_controllers);
}

DetectedControllers DetectRoyuanGK68HEProKeyboardControllers(hid_device_info* info, const std::string&)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        RoyuanKeyboardController* controller = new RoyuanKeyboardController(dev, info->path, RoyuanKeyboardProfile::GK68HEPro());

        if(controller->IsValid())
        {
            detected_controllers.push_back(new RGBController_SkyloongGK68HEPro(controller));
        }
        else
        {
            delete controller;
        }
    }

    return(detected_controllers);
}

DetectedControllers DetectRoyuanLegacyKeyboardControllers(hid_device_info* info, const std::string&)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        EpomakerController*               controller     = new EpomakerController(dev, info->path);
        RGBController_EpomakerController* rgb_controller = new RGBController_EpomakerController(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_I("Akko 3068B Plus / PC75B Plus",              DetectRoyuanAkkoKeyboardControllers,    ROYUAN_KEYBOARD_VID, ROYUAN_AKKO_B_SERIES_PID,              0);
REGISTER_HID_DETECTOR_IP("Akko 3068B Plus / Dukharo VN66 (Vendor)", DetectRoyuanAkkoKeyboardControllers,    ROYUAN_KEYBOARD_VID, ROYUAN_AKKO_B_SERIES_PID,              1, 0xFFFF);
REGISTER_HID_DETECTOR_I("Skyloong GK68HE Pro",              DetectRoyuanGK68HEProKeyboardControllers, ROYUAN_KEYBOARD_VID, ROYUAN_SKYLOONG_GK68HE_PRO_PID,      2);
REGISTER_HID_DETECTOR_I("Epomaker TH80 Pro (USB Cable)",    DetectRoyuanLegacyKeyboardControllers,  ROYUAN_KEYBOARD_VID, ROYUAN_EPOMAKER_TH80_PRO_USB_PID,      2);
REGISTER_HID_DETECTOR_I("Epomaker TH80 Pro (USB Dongle)",   DetectRoyuanLegacyKeyboardControllers,  ROYUAN_KEYBOARD_VID, ROYUAN_EPOMAKER_TH80_PRO_DONGLE_PID,   2);
REGISTER_HID_DETECTOR_I("Attack Shark K86 (USB Cable)",     DetectRoyuanLegacyKeyboardControllers,  ROYUAN_KEYBOARD_VID, ROYUAN_ATTACKSHARK_K86_USB_PID,        2);
