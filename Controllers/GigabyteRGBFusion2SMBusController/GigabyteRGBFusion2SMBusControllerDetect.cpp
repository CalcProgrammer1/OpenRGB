/*---------------------------------------------------------*\
| GigabyteRGBFusion2SMBusControllerDetect.cpp               |
|                                                           |
|   Detector for Gigabyte Aorus RGB Fusion 2 SMBus          |
|   motherboard                                             |
|                                                           |
|   Matt Harper                                 05 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string>
#include <vector>
#include "DetectionManager.h"
#include "dmiinfo.h"
#include "GigabyteRGBFusion2SMBusController.h"
#include "i2c_smbus.h"
#include "LogManager.h"
#include "pci_ids.h"
#include "RGBController_GigabyteRGBFusion2SMBus.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

#define DETECTOR_NAME                       "Gigabyte RGB Fusion 2 SMBus"
#define VENDOR_NAME                         "Gigabyte Technology Co., Ltd."
#define GIGABYTE_FOUND_MB_MESSAGE_EN        "[%s] Success - Found '%s' in the JSON list"
#define GIGABYTE_NOT_FOUND_MB_MESSAGE_EN    "[%s] FAILED  - '%s' was not found in the JSON list. Do NOT enable if this is a USB based board."
#define SMBUS_ADDRESS   0x68

typedef struct
{
    const std::string   manufacturer;
    const std::string   motherboard;
} motherboard_info;

#define RGB_FUSION_2_SMBUS_NUM_DEVICES (sizeof(rgb_fusion_2_smbus_motherboards) / sizeof(rgb_fusion_2_smbus_motherboards[ 0 ]))

json rgb_fusion_2_smbus_motherboards[] =
{
    "B450 AORUS ELITE",
    "B450 AORUS ELITE V2",
    "B450 AORUS M",
    "B450 AORUS PRO WIFI-CF",
    "B450 AORUS PRO-CF",
    "B450 AORUS PRO-CF4",
    "B450 I AORUS PRO WIFI-CF",
    "B450M DS3H-CF",
    "X299 DESIGNARE EX-CF",
    "X399 AORUS XTREME-CF",
    "X399 DESIGNARE EX-CF",
    "X470 AORUS GAMING 5 WIFI",
    "X470 AORUS GAMING 5 WIFI-CF",
    "X470 AORUS GAMING 7 WIFI-CF",
    "X470 AORUS GAMING 7 WIFI-50-CF",
    "X470 AORUS ULTRA GAMING",
    "X470 AORUS ULTRA GAMING-CF",
    "B360M AORUS Gaming 3-CF",
    "Z370 AORUS Gaming 5-CF",
    "Z370 AORUS Ultra Gaming-CF"
};

bool TestForGigabyteRGBFusion2SMBusController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    if (res >= 0)
    {
        pass = true;
    }

    return(pass);
}

DetectedControllers DetectGigabyteRGBFusion2SMBusControllers(std::vector<i2c_smbus_interface*>& busses)
{
    DetectedControllers                 detected_controllers;
    SettingsManager*                    set_man = ResourceManager::get()->GetSettingsManager();
    json                                device_settings;

    DMIInfo                             dmi;
    bool                                found = false;

    /*-----------------------------------------------------*\
    | Get Linux LED settings from settings manager          |
    \*-----------------------------------------------------*/
    device_settings = set_man->GetSettings(DETECTOR_NAME);

    if(!device_settings.contains("SupportedDevices"))
    {
        /*-------------------------------------------------*\
        | If supported devices is not found then write it   |
        | to settings                                       |
        \*-------------------------------------------------*/
        device_settings["SupportedDevices"] = rgb_fusion_2_smbus_motherboards;
        set_man->SetSettings(DETECTOR_NAME, device_settings);
        set_man->SaveSettings();
    }

    bool boolVendor                                    = ( dmi.getManufacturer() == VENDOR_NAME );
    bool boolMotherboard                               = false;
    nlohmann::detail::iter_impl<nlohmann::json> result = std::find(std::begin(device_settings["SupportedDevices"]), std::end(device_settings["SupportedDevices"]), dmi.getMainboard() );

    if(result != std::end(device_settings["SupportedDevices"]))
    {
        boolMotherboard = true;
    }
    found = ( boolVendor && boolMotherboard );

    if(found)
    {
        LOG_DEBUG(GIGABYTE_FOUND_MB_MESSAGE_EN, DETECTOR_NAME, dmi.getMainboard().c_str());
        for(unsigned int bus = 0; bus < busses.size(); bus++)
        {
            IF_MOBO_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
            {
                if(busses[bus]->pci_subsystem_vendor == GIGABYTE_SUB_VEN)
                {
                    /*-------------------------------------*\
                    | TODO - Is this necessary? Or an       |
                    | artifact of my own system?  Skip dmcd |
                    | devices                               |
                    \*-------------------------------------*/
                    std::string device_name = std::string(busses[bus]->device_name);

                    if(device_name.find("dmdc") == std::string::npos)
                    {
                        LOG_DEBUG(SMBUS_CHECK_DEVICE_MESSAGE_EN, DETECTOR_NAME, bus, VENDOR_NAME, SMBUS_ADDRESS);

                        /*---------------------------------*\
                        | Check for RGB Fusion 2 controller |
                        | at 0x68                           |
                        \*---------------------------------*/
                        if(TestForGigabyteRGBFusion2SMBusController(busses[bus], SMBUS_ADDRESS))
                        {
                            RGBFusion2SMBusController*     controller     = new RGBFusion2SMBusController(busses[bus], SMBUS_ADDRESS, dmi.getMainboard() );
                            RGBController_RGBFusion2SMBus* rgb_controller = new RGBController_RGBFusion2SMBus(controller);

                            detected_controllers.push_back(rgb_controller);
                        }
                    }
                }
                else
                {
                    LOG_DEBUG(SMBUS_CHECK_DEVICE_FAILURE_EN, DETECTOR_NAME, bus, VENDOR_NAME);
                }
            }
        }
    }
    else
    {
        LOG_DEBUG(GIGABYTE_NOT_FOUND_MB_MESSAGE_EN, DETECTOR_NAME, dmi.getMainboard().c_str());
    }

    return(detected_controllers);
}

REGISTER_I2C_DETECTOR(DETECTOR_NAME, DetectGigabyteRGBFusion2SMBusControllers);
