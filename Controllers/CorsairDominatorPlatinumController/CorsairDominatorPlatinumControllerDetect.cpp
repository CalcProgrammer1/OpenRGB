#include "Detector.h"
#include "CorsairDominatorPlatinumController.h"
#include "RGBController.h"
#include "RGBController_CorsairDominatorPlatinum.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include "LogManager.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std::chrono_literals;

json corsair_dominator_models =
{
    {
        "CMT",
        {
            {"name",  "Corsair Dominator Platinum"},
            {"leds",  12}
        }
    },
    {
        "CMH",
        {
            {"name",  "Corsair Vengeance Pro SL"},
            {"leds",  10}
        }
    },
    {
        "CMN",
        {
            {"name",  "Corsair Vengeance RGB RT"},
            {"leds",  10}
        }
    },
    {
        "CMG",
        {
            {"name",  "Corsair Vengeance RGB RS"},
            {"leds",  6}
        }
    }
};

#define CORSAIR_DOMINATOR_PLATINUM_NAME "Corsair Dominator Platinum"

bool TestForCorsairDominatorPlatinumController(i2c_smbus_interface *bus, unsigned char address)
{
    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    LOG_DEBUG("[%s] Trying address %02X", CORSAIR_DOMINATOR_PLATINUM_NAME, address);

    if(res < 0)
    {
        LOG_DEBUG("[%s] Failed: res was %04X", CORSAIR_DOMINATOR_PLATINUM_NAME, res);
        return false;
    }

    res = bus->i2c_smbus_read_byte_data(address, 0x43);

    if(res != 0x1b)
    {
        LOG_DEBUG("[%s] Failed: expected 0x1b, got %04X", CORSAIR_DOMINATOR_PLATINUM_NAME, res);
        return false;
    }

    res = bus->i2c_smbus_read_byte_data(address, 0x44);

    if(res != 0x04)
    {
        LOG_DEBUG("[%s] Failed: expected 0x04, got %04X", CORSAIR_DOMINATOR_PLATINUM_NAME, res);
        return false;
    }

    return true;
}

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairDominatorPlatinumControllers                                              *
*                                                                                          *
*       Detect Corsair Dominator Platinum controllers on the enumerated I2C busses.        *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

void DetectCorsairDominatorPlatinumControllers(std::vector<i2c_smbus_interface *> &busses)
{
    SettingsManager* settings_manager = ResourceManager::get()->GetSettingsManager();

    json corsair_dominator_settings = settings_manager->GetSettings("CorsairDominatorSettings");

    if(!corsair_dominator_settings.contains("model"))
    {
        // Set default value
        corsair_dominator_settings["model"] = "CMT";
        settings_manager->SetSettings("CorsairDominatorSettings", corsair_dominator_settings);
        settings_manager->SaveSettings();
    }

    std::string model = corsair_dominator_settings["model"];

    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            LOG_DEBUG("[%s] Testing bus %d", CORSAIR_DOMINATOR_PLATINUM_NAME, bus);

            std::vector<unsigned char> addresses;

            for(unsigned char addr = 0x58; addr <= 0x5F; addr++)
            {
                addresses.push_back(addr);
            }

            for(unsigned char addr = 0x18; addr <= 0x1F; addr++)
            {
                addresses.push_back(addr);
            }

            for(unsigned char addr : addresses)
            {
                if(TestForCorsairDominatorPlatinumController(busses[bus], addr))
                {
                    unsigned int leds;
                    std::string name;

                    if(corsair_dominator_models.contains(model))
                    {
                        leds = corsair_dominator_models[model]["leds"];
                        name = corsair_dominator_models[model]["name"];
                    }
                    else
                    {
                        leds = corsair_dominator_models["CMT"]["leds"];
                        name = corsair_dominator_models["CMT"]["name"];
                    }

                    LOG_DEBUG("[%s] Model: %s, Leds: %d", CORSAIR_DOMINATOR_PLATINUM_NAME, name.c_str(), leds);

                    CorsairDominatorPlatinumController*     controller    = new CorsairDominatorPlatinumController(busses[bus], addr, leds);
                    RGBController_CorsairDominatorPlatinum* rgbcontroller = new RGBController_CorsairDominatorPlatinum(controller);
                    rgbcontroller->name = name;

                    ResourceManager::get()->RegisterRGBController(rgbcontroller);
                }

                std::this_thread::sleep_for(10ms);
            }
        }
        else
        {
            LOG_DEBUG("[%s] Bus %d is not a DRAM bus", CORSAIR_DOMINATOR_PLATINUM_NAME, bus);
        }
    }
}   /* DetectCorsairDominatorPlatinumControllers() */

REGISTER_I2C_DETECTOR(CORSAIR_DOMINATOR_PLATINUM_NAME, DetectCorsairDominatorPlatinumControllers);
