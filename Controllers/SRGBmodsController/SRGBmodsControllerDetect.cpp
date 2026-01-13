/*---------------------------------------------------------*\
| SRGBModsControllerDetect.cpp                              |
|                                                           |
|   Detector for SRGBmods devices                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "RGBController_SRGBmodsLEDControllerV1.h"
#include "RGBController_SRGBmodsPico.h"
#include "SRGBmodsLEDControllerV1.h"
#include "SRGBmodsPicoController.h"

#define SRGBMODS_VID                    0x16D0

#define SRGBMODS_PICO_PID               0x1123
#define SRGBMODS_LED_CONTROLLER_V1_PID  0x1205

DetectedControllers DetectSRGBmodsControllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        wchar_t product[128];
        hid_get_product_string(dev, product, 128);

        std::wstring product_str(product);

        /*-------------------------------------------------*\
        | Test the product string in case this USB ID is    |
        | reused for other Pi Pico projects                 |
        \*-------------------------------------------------*/
        if(product_str == L"SRGBmods Pico LED Controller" || product_str == L"Pico LED Controller")
        {
            SRGBmodsPicoController*     controller     = new SRGBmodsPicoController(dev, info->path, name);
            RGBController_SRGBmodsPico* rgb_controller = new RGBController_SRGBmodsPico(controller);

            detected_controllers.push_back(rgb_controller);
        }
        else if(product_str == L"LED Controller v1")
        {
            SRGBmodsLEDControllerV1*               controller     = new SRGBmodsLEDControllerV1(dev, info->path, name);
            RGBController_SRGBmodsLEDControllerV1* rgb_controller = new RGBController_SRGBmodsLEDControllerV1(controller);

            detected_controllers.push_back(rgb_controller);
        }
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR("SRGBmods Pico LED Controller", DetectSRGBmodsControllers, SRGBMODS_VID, SRGBMODS_PICO_PID             );
REGISTER_HID_DETECTOR("SRGBMods LED Controller v1",   DetectSRGBmodsControllers, SRGBMODS_VID, SRGBMODS_LED_CONTROLLER_V1_PID);
