/*---------------------------------------------------------*\
| NollieControllerDetect.cpp                                |
|                                                           |
|   Detector for Nollie                                     |
|                                                           |
|   Name (cnn1236661)                           25 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "NollieController.h"
#include "RGBController_Nollie.h"

void DetectNollieControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        wchar_t product[128];
        hid_get_product_string(dev, product, 128);

        std::wstring product_str(product);

        NollieController*     controller     = new NollieController(dev, info->path, info->vendor_id, info->product_id, name);
        RGBController_Nollie* rgb_controller = new RGBController_Nollie(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);

    }
}

REGISTER_HID_DETECTOR("Nollie 32CH", DetectNollieControllers, NOLLIE32_VID, NOLLIE32_PID);
REGISTER_HID_DETECTOR("Nollie 16CH", DetectNollieControllers, NOLLIE16_VID, NOLLIE16_PID);
REGISTER_HID_DETECTOR("Nollie 8CH", DetectNollieControllers, NOLLIE8_VID, NOLLIE8_PID);
REGISTER_HID_DETECTOR("Nollie 1CH", DetectNollieControllers, NOLLIE1_VID, NOLLIE1_PID);
REGISTER_HID_DETECTOR("Nollie 28 12", DetectNollieControllers, NOLLIE28_12_VID, NOLLIE28_12_PID);
REGISTER_HID_DETECTOR("Nollie 28 L1", DetectNollieControllers, NOLLIE28_12_VID, NOLLIE28_L1_PID);
REGISTER_HID_DETECTOR("Nollie 28 L2", DetectNollieControllers, NOLLIE28_12_VID, NOLLIE28_L2_PID);
//Nollie OS2 Firmware
REGISTER_HID_DETECTOR("Nollie 32_OS2", DetectNollieControllers, NOLLIERGBOS_2_VID, NOLLIE32_PID);
REGISTER_HID_DETECTOR("Nollie 16_OS2", DetectNollieControllers, NOLLIERGBOS_2_VID, NOLLIE16_PID);
REGISTER_HID_DETECTOR("Nollie 8_OS2", DetectNollieControllers, NOLLIERGBOS_2_VID, NOLLIE8_PID);
REGISTER_HID_DETECTOR("Nollie 1_OS2", DetectNollieControllers, NOLLIERGBOS_2_VID, NOLLIE1_PID);
