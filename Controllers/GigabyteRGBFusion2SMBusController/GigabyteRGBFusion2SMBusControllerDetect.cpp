#include "Detector.h"
#include "GigabyteRGBFusion2SMBusController.h"
#include "RGBController.h"
#include "RGBController_GigabyteRGBFusion2SMBus.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "dependencies/dmiinfo.h"
#define DETECTOR_NAME "Gigabyte RGB Fusion 2 SMBus"
#define VENDOR_NAME "Gigabyte Technology Co., Ltd."

typedef struct
{
    const std::string   manufacturer;
    const std::string   motherboard;
} motherboard_info;

#define RGB_FUSION_2_SMBUS_NUM_DEVICES (sizeof(rgb_fusion_2_smbus_motherboards) / sizeof(rgb_fusion_2_smbus_motherboards[ 0 ]))

json rgb_fusion_2_smbus_motherboards[] =
{
    "B450 AORUS ELITE",
    "B450 AORUS M",
    "B450 AORUS PRO WIFI-CF",
    "B450 AORUS PRO-CF",
    "B450 AORUS PRO-CF4",
    "B450 I AORUS PRO WIFI-CF",
    "B450M DS3H-CF",
    "X399 AORUS XTREME-CF",
    "X399 DESIGNARE EX-CF",
    "X470 AORUS GAMING 5 WIFI",
    "X470 AORUS GAMING 7 WIFI-CF",
    "X470 AORUS GAMING 7 WIFI-50-CF",
    "X470 AORUS ULTRA GAMING",
    "X470 AORUS ULTRA GAMING-CF",
    "Z370 AORUS Gaming 5-CF"
};

/******************************************************************************************\
*                                                                                          *
*   TestForGigabyteRGBFusion2SMBusController                                               *
*                                                                                          *
*       Tests the given address to see if an RGB 2 Fusion controller exists there.  First  *
*       does a quick write to test for a response                                          *
*                                                                                          *
\******************************************************************************************/

bool TestForGigabyteRGBFusion2SMBusController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);
    
    if (res >= 0)
    {
        pass = true;
    }
    
    return(pass);

}   /* TestForRGBFusion2SMBusController() */

/******************************************************************************************\
*                                                                                          *
*   DetectGigabyteRGBFusion2SMBusControllers                                               *
*                                                                                          *
*       Detect RGB Fusion 2 controllers on the enumerated I2C busses at address 0x68.      *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where RGB Fusion device is connected      *
*           dev - I2C address of RGB Fusion device                                         *
*                                                                                          *
\******************************************************************************************/

void DetectGigabyteRGBFusion2SMBusControllers(std::vector<i2c_smbus_interface*>& busses)
{
    RGBFusion2SMBusController*          new_rgb_fusion;
    RGBController_RGBFusion2SMBus*      new_controller;
    SettingsManager*                    set_man = ResourceManager::get()->GetSettingsManager();
    json                                device_settings;

    DMIInfo                             dmi;
    bool                                found = false;

    /*-------------------------------------------------*\
    | Get Linux LED settings from settings manager      |
    \*-------------------------------------------------*/
    device_settings = set_man->GetSettings(DETECTOR_NAME);
    
    if (!device_settings.contains("SupportedDevices"))
    {
        //If supported devices is not found then write it to settings
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
        for (unsigned int bus = 0; bus < busses.size(); bus++)
        {
            IF_MOBO_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
            {
                // TODO - Is this necessary? Or an artifact of my own system?
                // Skip dmcd devices
                std::string device_name = std::string(busses[bus]->device_name);
                if (device_name.find("dmdc") == std::string::npos)
                {
                    // Check for RGB Fusion 2 controller at 0x68
                    if (TestForGigabyteRGBFusion2SMBusController(busses[bus], 0x68))
                    {
                        new_rgb_fusion = new RGBFusion2SMBusController(busses[bus], 0x68, dmi.getMainboard() );
                        new_controller = new RGBController_RGBFusion2SMBus(new_rgb_fusion);
                        ResourceManager::get()->RegisterRGBController(new_controller);
                    }
                }
            }
        }
    }

}   /* DetectRGBFusion2SMBusControllers() */

REGISTER_I2C_DETECTOR(DETECTOR_NAME, DetectGigabyteRGBFusion2SMBusControllers);
