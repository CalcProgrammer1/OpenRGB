/*---------------------------------------------------------*\
| CorsairPeripheralControllerDetect.cpp                     |
|                                                           |
|   Driver for Corsair peripherals                          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

/*-----------------------------------------------------*\
| OpenRGB includes                                      |
\*-----------------------------------------------------*/
#include <hidapi.h>
#include "Detector.h"
#include "LogManager.h"
#include "RGBController.h"

/*-----------------------------------------------------*\
| Corsair Peripheral specific includes                  |
\*-----------------------------------------------------*/
#include "RGBController_CorsairPeripheral.h"
#include "RGBController_CorsairK55RGBPROXT.h"
#include "RGBController_CorsairK65Mini.h"

#define CORSAIR_PERIPHERAL_CONTROLLER_NAME "Corsair peripheral"

/*-----------------------------------------------------*\
| Corsair vendor ID                                     |
\*-----------------------------------------------------*/
#define CORSAIR_VID                     0x1B1C

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
|   List taken from ckb-next                            |
|   Non-RGB keyboards were omitted from this list       |
\*-----------------------------------------------------*/
#define CORSAIR_K55_RGB_PID             0x1B3D

#define CORSAIR_K65_RGB_PID             0x1B17
#define CORSAIR_K65_LUX_RGB_PID         0x1B37
#define CORSAIR_K65_RGB_RAPIDFIRE_PID   0x1B39

#define CORSAIR_K68_RGB_PID             0x1B4F

#define CORSAIR_K70_RGB_PID             0x1B13
#define CORSAIR_K70_LUX_RGB_PID         0x1B33
#define CORSAIR_K70_RGB_RAPIDFIRE_PID   0x1B38
#define CORSAIR_K70_RGB_MK2_PID         0x1B49
#define CORSAIR_K70_RGB_MK2_SE_PID      0x1B6B
#define CORSAIR_K70_RGB_MK2_LP_PID      0x1B55

#define CORSAIR_K95_RGB_PID             0x1B11
#define CORSAIR_K95_PLATINUM_PID        0x1B2D
#define CORSAIR_K95_PLATINUM_SE_PID     0x1B82

#define CORSAIR_STRAFE_PID              0x1B20
#define CORSAIR_STRAFE_RED_PID          0x1B44
#define CORSAIR_STRAFE_MK2_PID          0x1B48

/*-----------------------------------------------------*\
| Non-RGB Keyboard product IDs                          |
\*-----------------------------------------------------*/
#define CORSAIR_K70_LUX_PID             0x1B36
#define CORSAIR_K68_RED_PID             0x1B3F
#define CORSAIR_K68_RED_SHADOW_PID      0x1BA5

/*-----------------------------------------------------*\
| Mouse product IDs                                     |
|   List taken from ckb-next                            |
\*-----------------------------------------------------*/
#define CORSAIR_GLAIVE_RGB_PID          0x1B34
#define CORSAIR_GLAIVE_RGB_PRO_PID      0x1B74
#define CORSAIR_HARPOON_RGB_PID         0x1B3C
#define CORSAIR_HARPOON_RGB_PRO_PID     0x1B75
#define CORSAIR_IRONCLAW_RGB_PID        0x1B5D
#define CORSAIR_M65_PID                 0x1B12
#define CORSAIR_M65_PRO_PID             0x1B2E
#define CORSAIR_M65_RGB_ELITE_PID       0x1B5A
#define CORSAIR_NIGHTSWORD_PID          0x1B5C
#define CORSAIR_SCIMITAR_RGB_PID        0x1B1E
#define CORSAIR_SCIMITAR_PRO_RGB_PID    0x1B3E
#define CORSAIR_SCIMITAR_ELITE_RGB_PID  0x1B8B
#define CORSAIR_SABRE_RGB_PID           0x1B2F

/*-----------------------------------------------------*\
| Mousepad product IDs                                  |
|   List taken from ckb-next                            |
\*-----------------------------------------------------*/
#define CORSAIR_MM800_RGB_POLARIS_PID   0x1B3B

/*-----------------------------------------------------*\
| Headset Stand product IDs                             |
|   List taken from ckb-next                            |
\*-----------------------------------------------------*/
#define CORSAIR_ST100_PID               0x0A34

/*-----------------------------------------------------*\
| Corsair K55 RGB PRO XT Keyboard product ID            |
| This keyboard uses a separate driver                  |
\*-----------------------------------------------------*/
#define CORSAIR_K55_RGB_PRO_XT_PID      0x1BA1

/*-----------------------------------------------------*\
| Corsair K65 Mini Keyboard product ID                  |
| This keyboard uses a separate driver                  |
\*-----------------------------------------------------*/
#define CORSAIR_K65_MINI_PID            0x1BAF

void DetectCorsairK55RGBPROXTControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CorsairK55RGBPROXTController*   controller       = new CorsairK55RGBPROXTController(dev, info->path, name);
        RGBController_CorsairK55RGBPROXT* rgb_controller = new RGBController_CorsairK55RGBPROXT(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectCorsairK55RGBPROXTControllers() */

void DetectCorsairK65MiniControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CorsairK65MiniController*     controller        = new CorsairK65MiniController(dev, info->path, name);
        RGBController_CorsairK65Mini* rgb_controller    = new RGBController_CorsairK65Mini(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectCorsairK65MiniControllers() */

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairPeripheralControllers                                                     *
*                                                                                          *
*       Tests the USB address to see if a Corsair RGB Keyboard controller exists there.    *
*                                                                                          *
\******************************************************************************************/
void DetectCorsairPeripheralControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LOG_DEBUG("[%s] Device opened. VID/PID %02X:%02X", CORSAIR_PERIPHERAL_CONTROLLER_NAME, info->vendor_id , info->product_id);

        CorsairPeripheralController* controller = new CorsairPeripheralController(dev, info->path, name);

        if(controller->GetDeviceType() != DEVICE_TYPE_UNKNOWN)
        {
            bool supports_hardware_modes =
                    (info->product_id == CORSAIR_K70_RGB_MK2_PID) ||
                    (info->product_id == CORSAIR_K70_RGB_MK2_LP_PID);

            RGBController_CorsairPeripheral* rgb_controller = new RGBController_CorsairPeripheral(controller, supports_hardware_modes);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            LOG_DEBUG("[%s] Device type is unknown", CORSAIR_PERIPHERAL_CONTROLLER_NAME);
            delete controller;
        }
    }
}   /* DetectCorsairPeripheralControllers() */

/*-----------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                             |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Corsair K55 RGB",                 DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K55_RGB_PID,            1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K65 RGB",                 DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K65_RGB_PID,            1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K65 LUX RGB",             DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K65_LUX_RGB_PID,        1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K65 RGB RAPIDFIRE",       DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K65_RGB_RAPIDFIRE_PID,  1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K68 RGB",                 DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K68_RGB_PID,            1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K68 RED",                 DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K68_RED_PID,            1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K68 RED SHADOW",          DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K68_RED_SHADOW_PID,     1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K70 RGB",                 DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K70_RGB_PID,            1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K70 LUX",                 DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K70_LUX_PID,            1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K70 LUX RGB",             DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K70_LUX_RGB_PID,        1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K70 RGB RAPIDFIRE",       DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K70_RGB_RAPIDFIRE_PID,  1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K70 RGB MK.2",            DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K70_RGB_MK2_PID,        1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K70 RGB MK.2 SE",         DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K70_RGB_MK2_SE_PID,     1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K70 RGB MK.2 Low Profile",DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K70_RGB_MK2_LP_PID,     1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K95 RGB",                 DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K95_RGB_PID,            1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K95 RGB PLATINUM",        DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K95_PLATINUM_PID,       1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K95 RGB PLATINUM SE",     DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K95_PLATINUM_SE_PID,    1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Strafe",                  DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_STRAFE_PID,             1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Strafe Red",              DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_STRAFE_RED_PID,         1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Strafe MK.2",             DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_STRAFE_MK2_PID,         1, 0xFFC2);
/*-----------------------------------------------------------------------------------------------------*\
| Mice                                                                                                  |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Corsair Glaive RGB",              DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_GLAIVE_RGB_PID,         1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Glaive RGB PRO",          DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_GLAIVE_RGB_PRO_PID,     1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Harpoon RGB",             DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_HARPOON_RGB_PID,        1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Harpoon RGB PRO",         DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_HARPOON_RGB_PRO_PID,    1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Ironclaw RGB",            DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_IRONCLAW_RGB_PID,       1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair M65",                     DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_M65_PID,                1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair M65 PRO",                 DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_M65_PRO_PID,            1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair M65 RGB Elite",           DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_M65_RGB_ELITE_PID,      1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Nightsword",              DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_NIGHTSWORD_PID,         1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Scimitar RGB",            DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_SCIMITAR_RGB_PID,       1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Scimitar PRO RGB",        DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_SCIMITAR_PRO_RGB_PID,   1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Scimitar Elite RGB",      DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_SCIMITAR_ELITE_RGB_PID, 1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Sabre RGB",               DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_SABRE_RGB_PID,          1, 0xFFC2);

/*-----------------------------------------------------------------------------------------------------*\
| Mousemats                                                                                             |
\*-----------------------------------------------------------------------------------------------------*/
#ifdef USE_HID_USAGE
REGISTER_HID_DETECTOR_P("Corsair MM800 RGB Polaris",        DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_MM800_RGB_POLARIS_PID,  0xFFC2);
#else
REGISTER_HID_DETECTOR_I("Corsair MM800 RGB Polaris",        DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_MM800_RGB_POLARIS_PID,  0);
#endif
/*-----------------------------------------------------------------------------------------------------*\
| Headset Stands                                                                                        |
\*-----------------------------------------------------------------------------------------------------*/
#ifdef USE_HID_USAGE
REGISTER_HID_DETECTOR_P("Corsair ST100 RGB",                DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_ST100_PID,              0xFFC2);
#else
REGISTER_HID_DETECTOR_I("Corsair ST100 RGB",                DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_ST100_PID,              0);
#endif

/*-----------------------------------------------------------------------------------------------------*\
| Corsair K65 Mini Keyboard                                                                             |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_I("Corsair K65 Mini",                 DetectCorsairK65MiniControllers,    CORSAIR_VID, CORSAIR_K65_MINI_PID,           1);

/*-----------------------------------------------------------------------------------------------------*\
| Corsair K55 RGB PRO XT Keyboard                                                                       |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Corsair K55 RGB PRO XT",          DetectCorsairK55RGBPROXTControllers, CORSAIR_VID, CORSAIR_K55_RGB_PRO_XT_PID,    1, 0xFF42);
