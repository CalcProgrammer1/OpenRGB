/*---------------------------------------------------------*\
| AsusAuraUSBControllerDetect.cpp                           |
|                                                           |
|   Detector for ASUS Aura USB devices                      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "AsusAuraAddressableController.h"
#include "AsusAuraMainboardController.h"
#include "RGBController_AsusAuraUSB.h"
#include "RGBController_AsusAuraMainboard.h"
#include "dmiinfo.h"

/*---------------------------------------------------------*\
| ASUS VID                                                  |
\*---------------------------------------------------------*/
#define AURA_USB_VID                                0x0B05

/*---------------------------------------------------------*\
| Motherboard PIDs                                          |
\*---------------------------------------------------------*/
#define AURA_ADDRESSABLE_1_PID                      0x1867
#define AURA_ADDRESSABLE_2_PID                      0x1872
#define AURA_ADDRESSABLE_3_PID                      0x18A3
#define AURA_ADDRESSABLE_4_PID                      0x18A5
#define AURA_MOTHERBOARD_1_PID                      0x18F3
#define AURA_MOTHERBOARD_2_PID                      0x1939
#define AURA_MOTHERBOARD_3_PID                      0x19AF
#define AURA_MOTHERBOARD_4_PID                      0x1AA6
#define AURA_MOTHERBOARD_5_PID                      0x1BED
#define AURA_LIQUID_COOLER_1_PID                    0x1B29

/*---------------------------------------------------------*\
| Monitor PIDs                                              |
\*---------------------------------------------------------*/
#define AURA_ROG_STRIX_XG27AQ_PID                   0x198C
#define AURA_ROG_STRIX_XG27AQM_PID                  0x19BB
#define AURA_ROG_STRIX_XG279Q_PID                   0x1919
#define AURA_ROG_STRIX_XG27W_PID                    0x1933
#define AURA_ROG_STRIX_XG32VC_PID                   0x1968
#define AURA_ROG_PG32UQ_PID                         0x19B9

/*---------------------------------------------------------*\
| Other PIDs                                                |
\*---------------------------------------------------------*/
#define AURA_TERMINAL_PID                           0x1889

DetectedControllers DetectAsusAuraUSBTerminal(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        AuraAddressableController* controller               = new AuraAddressableController(dev, info->path, name);
        RGBController_AuraUSB*     rgb_controller           = new RGBController_AuraUSB(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectAsusAuraUSBAddressable(hid_device_info* info, const std::string& /*name*/)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        DMIInfo                    dmi;
        AuraAddressableController* controller               = new AuraAddressableController(dev, info->path, "ASUS " + dmi.getMainboard() + " Addressable");
        RGBController_AuraUSB*     rgb_controller           = new RGBController_AuraUSB(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectAsusAuraUSBMotherboards(hid_device_info* info, const std::string& /*name*/)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        try
        {
            DMIInfo                      dmi;
            AuraMainboardController*     controller         = new AuraMainboardController(dev, info->path, "ASUS " + dmi.getMainboard());
            RGBController_AuraMainboard* rgb_controller     = new RGBController_AuraMainboard(controller);

            detected_controllers.push_back(rgb_controller);
        }
        catch(const std::runtime_error& ex)
        {
            // reading the config table failed
            LOG_ERROR("[AsusAuraUSB] An error occured while reading the config table: %s", ex.what());
        }
    }

    return(detected_controllers);
}

DetectedControllers DetectAsusAuraUSBLiquidCoolers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        try
        {
            /*-----------------------------------------------------*\
            | The ROG Strix LC III pump uses the same protocol as   |
            | the Aura USB mainboard controllers                    |
            \*-----------------------------------------------------*/
            AuraMainboardController*     controller         = new AuraMainboardController(dev, info->path, name);
            RGBController_AuraMainboard* rgb_controller     = new RGBController_AuraMainboard(controller);

            detected_controllers.push_back(rgb_controller);
        }
        catch(const std::runtime_error& ex)
        {
            // reading the config table failed
            LOG_ERROR("[AsusAuraUSB] An error occured while reading the config table: %s", ex.what());
        }
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
|  MOTHERBOARDS                                             |
\*---------------------------------------------------------*/
REGISTER_HID_DETECTOR   ("ASUS Aura Addressable",   DetectAsusAuraUSBAddressable,   AURA_USB_VID, AURA_ADDRESSABLE_1_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Addressable",   DetectAsusAuraUSBAddressable,   AURA_USB_VID, AURA_ADDRESSABLE_2_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Addressable",   DetectAsusAuraUSBAddressable,   AURA_USB_VID, AURA_ADDRESSABLE_3_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Addressable",   DetectAsusAuraUSBAddressable,   AURA_USB_VID, AURA_ADDRESSABLE_4_PID);
REGISTER_HID_DETECTOR   ("ASUS ROG Strix LC III",   DetectAsusAuraUSBLiquidCoolers, AURA_USB_VID, AURA_LIQUID_COOLER_1_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Motherboard",   DetectAsusAuraUSBMotherboards,  AURA_USB_VID, AURA_MOTHERBOARD_1_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Motherboard",   DetectAsusAuraUSBMotherboards,  AURA_USB_VID, AURA_MOTHERBOARD_2_PID);
REGISTER_HID_DETECTOR_PU("ASUS Aura Motherboard",   DetectAsusAuraUSBMotherboards,  AURA_USB_VID, AURA_MOTHERBOARD_3_PID,   0xFF72, 0x00A1);
REGISTER_HID_DETECTOR   ("ASUS Aura Motherboard",   DetectAsusAuraUSBMotherboards,  AURA_USB_VID, AURA_MOTHERBOARD_4_PID);
REGISTER_HID_DETECTOR   ("ASUS Aura Motherboard",   DetectAsusAuraUSBMotherboards,  AURA_USB_VID, AURA_MOTHERBOARD_5_PID);

/*---------------------------------------------------------*\
|  OTHER                                                    |
\*---------------------------------------------------------*/
REGISTER_HID_DETECTOR    ("ASUS ROG AURA Terminal", DetectAsusAuraUSBTerminal,      AURA_USB_VID, AURA_TERMINAL_PID);
