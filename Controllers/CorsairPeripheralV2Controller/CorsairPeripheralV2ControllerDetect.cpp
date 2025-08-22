/*---------------------------------------------------------*\
| CorsairPeripheralV2ControllerDetect.cpp                   |
|                                                           |
|   Detector for Corsair V2 peripherals                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

/*-----------------------------------------------------*\
| OpenRGB includes                                      |
\*-----------------------------------------------------*/
#include <hidapi.h>
#include "Detector.h"

/*-----------------------------------------------------*\
| Corsair Peripheral specific includes                  |
\*-----------------------------------------------------*/
#include "CorsairPeripheralV2Devices.h"
#include "RGBController_CorsairV2Hardware.h"
#include "RGBController_CorsairV2Software.h"

#define CORSAIR_PERIPHERAL_CONTROLLER_NAME "Corsair V2 Peripheral"

/*-----------------------------------------------------*\
| Corsair vendor ID                                     |
\*-----------------------------------------------------*/
#define CORSAIR_VID                                 0x1B1C

void DetectCorsairV2HardwareControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CorsairPeripheralV2HWController*    controller      = new CorsairPeripheralV2HWController(dev, info->path, name);
        RGBController_CorsairV2HW*          rgb_controller  = new RGBController_CorsairV2HW(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectCorsairV2HardwareControllers() */

void DetectCorsairV2SoftwareControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CorsairPeripheralV2SWController*    controller      = new CorsairPeripheralV2SWController(dev, info->path, name);
        RGBController_CorsairV2SW*          rgb_controller  = new RGBController_CorsairV2SW(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectCorsairV2SoftwareControllers() */

/*-----------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                             |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Corsair K55 RGB PRO",                     DetectCorsairV2SoftwareControllers, CORSAIR_VID,    CORSAIR_K55_RGB_PRO_PID,                1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair K60 RGB PRO",                     DetectCorsairV2SoftwareControllers, CORSAIR_VID,    CORSAIR_K60_RGB_PRO_PID,                1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair K60 RGB PRO Low Profile",         DetectCorsairV2SoftwareControllers, CORSAIR_VID,    CORSAIR_K60_RGB_PRO_LP_PID,             1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair K60 RGB PRO TKL Black",           DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_K60_RGB_PRO_TKL_B_PID,          1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair K60 RGB PRO TKL White",           DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_K60_RGB_PRO_TKL_W_PID,          1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair K70 Core RGB",                    DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_K70_CORE_RGB_PID,               1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair K70 RGB PRO",                     DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_K70_RGB_PRO_PID,                1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair K70 RGB PRO V2",                  DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_K70_RGB_PRO_V2_PID,             1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair K70 RGB TKL",                     DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_K70_RGB_TKL_PID,                1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair K70 RGB TKL Champion Series",     DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_K70_RGB_TKL_CS_PID,             1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair K95 RGB PLATINUM XT",             DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_K95_PLATINUM_XT_PID,            1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair K100 RGB Optical",                DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_K100_OPTICAL_V1_PID,            1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair K100 RGB Optical",                DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_K100_OPTICAL_V2_PID,            1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair K100 MX Red",                     DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_K100_MXRED_PID,                 1,  0xFF42);

/*-----------------------------------------------------------------------------------------------------*\
| Mice                                                                                                  |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Corsair Dark Core RGB SE (Wired)",        DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_DARK_CORE_RGB_PID,              1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair Dark Core RGB Pro SE (Wired)",    DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_DARK_CORE_RGB_PRO_PID,          1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair Harpoon Wireless (Wired)",        DetectCorsairV2SoftwareControllers, CORSAIR_VID,    CORSAIR_HARPOON_WIRELESS_PID,           1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair Ironclaw Wireless (Wired)",       DetectCorsairV2SoftwareControllers, CORSAIR_VID,    CORSAIR_IRONCLAW_WIRELESS_PID,          1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair Katar Pro",                       DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_KATAR_PRO_PID,                  1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair Katar Pro V2",                    DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_KATAR_PRO_V2_PID,               1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair Katar Pro XT",                    DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_KATAR_PRO_XT_PID,               1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair M55 RGB PRO",                     DetectCorsairV2SoftwareControllers, CORSAIR_VID,    CORSAIR_M55_RGB_PRO_PID,                1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair M65 RGB Ultra Wired",             DetectCorsairV2SoftwareControllers, CORSAIR_VID,    CORSAIR_M65_RGB_ULTRA_WIRED_PID,        1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair M65 RGB Ultra Wireless (Wired)",  DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_M65_RGB_ULTRA_WIRELESS_PID,     1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair Slipstream Wireless Receiver HW", DetectCorsairV2HardwareControllers, CORSAIR_VID,    CORSAIR_SLIPSTREAM_WIRELESS_PID1,       1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair Slipstream Wireless Receiver SW", DetectCorsairV2SoftwareControllers, CORSAIR_VID,    CORSAIR_SLIPSTREAM_WIRELESS_PID2,       1,  0xFF42);

/*-----------------------------------------------------------------------------------------------------*\
| Mousemat                                                                                              |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Corsair MM700",                           DetectCorsairV2SoftwareControllers, CORSAIR_VID,    CORSAIR_MM700_PID,                      1,  0xFF42);
REGISTER_HID_DETECTOR_IP("Corsair MM700 3XL",                       DetectCorsairV2SoftwareControllers, CORSAIR_VID,    CORSAIR_MM700_3XL_PID,                  1,  0xFF42);
