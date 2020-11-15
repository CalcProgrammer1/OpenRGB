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

typedef struct
{
    const std::string   manufacturer;
    const std::string   motherboard;
} motherboard_info;

#define RGB_FUSION_2_SMBUS_NUM_DEVICES (sizeof(rgb_fusion_2_smbus_motherboards) / sizeof(rgb_fusion_2_smbus_motherboards[ 0 ]))

const motherboard_info rgb_fusion_2_smbus_motherboards[] =
{
    { "Gigabyte Technology Co., Ltd.",      "B450 AORUS ELITE"              },
    { "Gigabyte Technology Co., Ltd.",      "B450 AORUS M"                  },
    { "Gigabyte Technology Co., Ltd.",      "B450 AORUS PRO WIFI-CF"        },
    { "Gigabyte Technology Co., Ltd.",      "B450 AORUS PRO-CF"             },
    { "Gigabyte Technology Co., Ltd.",      "B450 AORUS PRO-CF4"            },
    { "Gigabyte Technology Co., Ltd.",      "B450 I AORUS PRO WIFI-CF"      },
    { "Gigabyte Technology Co., Ltd.",      "B450M DS3H-CF"                 },
    { "Gigabyte Technology Co., Ltd.",      "X399 AORUS XTREME-CF"          },
    { "Gigabyte Technology Co., Ltd.",      "X399 DESIGNARE EX-CF"          },
    { "Gigabyte Technology Co., Ltd.",      "X470 AORUS GAMING 5 WIFI"      },
    { "Gigabyte Technology Co., Ltd.",      "X470 AORUS GAMING 7 WIFI-CF"   },
    { "Gigabyte Technology Co., Ltd.",      "X470 AORUS ULTRA GAMING"       },
    { "Gigabyte Technology Co., Ltd.",      "X470 AORUS ULTRA GAMING-CF"    },
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

void DetectGigabyteRGBFusion2SMBusControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers)
{
    RGBFusion2SMBusController* new_rgb_fusion;
    RGBController_RGBFusion2SMBus* new_controller;

    DMIInfo dmi;
    bool    found = false;

    for(unsigned int mb_idx = 0; mb_idx < RGB_FUSION_2_SMBUS_NUM_DEVICES; mb_idx++)
    {
        if( ( dmi.getManufacturer() == rgb_fusion_2_smbus_motherboards[mb_idx].manufacturer )
         && ( dmi.getMainboard()    == rgb_fusion_2_smbus_motherboards[mb_idx].motherboard  ) )
        {
            found = true;
        }
    }

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
                        new_rgb_fusion = new RGBFusion2SMBusController(busses[bus], 0x68);
                        new_controller = new RGBController_RGBFusion2SMBus(new_rgb_fusion);
                        rgb_controllers.push_back(new_controller);
                    }
                }
            }
        }
    }

}   /* DetectRGBFusion2SMBusControllers() */

REGISTER_I2C_DETECTOR("Gigabyte RGB Fusion 2 SMBus", DetectGigabyteRGBFusion2SMBusControllers);
