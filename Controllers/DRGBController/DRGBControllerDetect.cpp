/*---------------------------------------------------------*\
| DRGBControllerDetect.cpp                                  |
|                                                           |
|   Driver for DRGBmods                                     |
|                                                           |
|   Zhi Yan                                     25 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "DRGBController.h"
#include "RGBController_DRGB.h"

void DetectDRGBControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        wchar_t product[128];
        hid_get_product_string(dev, product, 128);
        std::wstring product_str(product);

        DRGBController*     controller              = new DRGBController(dev, info->path, info->product_id, name);
        RGBController_DRGB* rgb_controller          = new RGBController_DRGB(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR("DeepRGB LED V4",             DetectDRGBControllers, DRGBV4_VID,  DRGB_LED_V4_PID);
REGISTER_HID_DETECTOR("DeepRGB ULTRA V4F",          DetectDRGBControllers, DRGBV4_VID,  DRGB_ULTRA_V4F_PID);
REGISTER_HID_DETECTOR("DeepRGB CORE V4F",           DetectDRGBControllers, DRGBV4_VID,  DRGB_CORE_V4F_PID);
REGISTER_HID_DETECTOR("DeepRGB SIG V4F",            DetectDRGBControllers, DRGBV4_VID,  DRGB_SIG_V4F_PID);

REGISTER_HID_DETECTOR("Airgoo AG-DRGB04",           DetectDRGBControllers, DRGBV4_VID,  DRGB_AG_04_V4F_PID);
REGISTER_HID_DETECTOR("Airgoo AG-DRGB16",           DetectDRGBControllers, DRGBV4_VID,  DRGB_AG_16_V4F_PID);
REGISTER_HID_DETECTOR("Airgoo AG-F12-DRGB16",       DetectDRGBControllers, DRGBV4_VID,  DRGB_AG_16_F12_PID);

REGISTER_HID_DETECTOR("DeepRGB L8 V5",              DetectDRGBControllers, DRGBV4_VID,  DRGB_L8_V5_PID);
REGISTER_HID_DETECTOR("DeepRGB U16 V5",             DetectDRGBControllers, DRGBV4_VID,  DRGB_U16_V5_PID);
REGISTER_HID_DETECTOR("DeepRGB U16 V5F",            DetectDRGBControllers, DRGBV4_VID,  DRGB_U16_V5F_PID);
REGISTER_HID_DETECTOR("DeepRGB C16 V5",             DetectDRGBControllers, DRGBV4_VID,  DRGB_C16_V5_PID);
REGISTER_HID_DETECTOR("DeepRGB C16 V5F",            DetectDRGBControllers, DRGBV4_VID,  DRGB_C16_V5F_PID);
REGISTER_HID_DETECTOR("DeepRGB S16 V5F",            DetectDRGBControllers, DRGBV4_VID,  DRGB_S16_V5F_PID);

REGISTER_HID_DETECTOR("DeepRGB LED",                DetectDRGBControllers, DRGBV3_VID,  DRGB_LED_V3_PID);
REGISTER_HID_DETECTOR("DeepRGB Ultra V3",           DetectDRGBControllers, DRGBV3_VID,  DRGB_Ultra_V3_PID);
REGISTER_HID_DETECTOR("DeepRGB CORE V3",            DetectDRGBControllers, DRGBV3_VID,  DRGB_CORE_V3_PID);
REGISTER_HID_DETECTOR("DeepRGB E8 F",               DetectDRGBControllers, DRGBV3_VID,  DRGB_E8_F_PID);
REGISTER_HID_DETECTOR("DeepRGB E8",                 DetectDRGBControllers, DRGBV3_VID,  DRGB_E8_PID);
REGISTER_HID_DETECTOR("DeepRGB E16",                DetectDRGBControllers, DRGBV3_VID,  DRGB_E16_PID);
REGISTER_HID_DETECTOR("NEEDMAX 10 ELITE",           DetectDRGBControllers, DRGBV3_VID,  DM_10_PID);
REGISTER_HID_DETECTOR("JPU ELITE",                  DetectDRGBControllers, DRGBV3_VID,  JPU_12_PID);

REGISTER_HID_DETECTOR("DeepRGB LED Controller",     DetectDRGBControllers, DRGBV2_VID,  DRGB_LED_PID);
REGISTER_HID_DETECTOR("DeepRGB ULTRA",              DetectDRGBControllers, DRGBV2_VID,  DRGB_ULTRA_PID);
REGISTER_HID_DETECTOR("DeepRGB SIG AB",             DetectDRGBControllers, DRGBV2_VID,  DRGB_SIG_AB_PID);
REGISTER_HID_DETECTOR("DeepRGB SIG CD",             DetectDRGBControllers, DRGBV2_VID,  DRGB_SIG_CD_PID);
REGISTER_HID_DETECTOR("DeepRGB Strimer Controller", DetectDRGBControllers, DRGBV2_VID,  DRGB_Strimer_PID);

REGISTER_HID_DETECTOR("YICO 8 ELITE",               DetectDRGBControllers, YICO_VID,    YICO_8_PID);
REGISTER_HID_DETECTOR("YICO 08 ELITE",              DetectDRGBControllers, YICO_VID,    YICO_08_PID);
REGISTER_HID_DETECTOR("YICO 08 ELITE",              DetectDRGBControllers, YICO_VID,    YICO_08_1_PID);
REGISTER_HID_DETECTOR("YICO 14 LCD",                DetectDRGBControllers, DRGBV3_VID,  YICO_14_PID);
REGISTER_HID_DETECTOR("YICO 16 ELITE",              DetectDRGBControllers, DRGBV4_VID,  YICO_16_PID);

