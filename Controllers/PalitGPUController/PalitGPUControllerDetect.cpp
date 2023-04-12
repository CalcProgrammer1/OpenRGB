#include "Detector.h"
#include "LogManager.h"
#include "PalitGPUController.h"
#include "RGBController.h"
#include "RGBController_PalitGPU.h"
#include "i2c_smbus.h"
#include "pci_ids.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
 *                                                                                        *
 *   DetectPalitGPUControllers                                                            *
 *                                                                                        *
 *       Detect Palit GPU controllers on the enumerated I2C busses at address 0x49.       *
 *                                                                                        *
 *       bus - pointer to i2c_smbus_interface where Palit GPU device is connected         *
 *       dev - I2C address of Palit GPU device                                            *
 *                                                                                        *
\******************************************************************************************/

void DetectPalitGPUControllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    if(bus->port_id != 1)
    {
        return;
    }

    const uint8_t palit[] = {'P', 'A', 'L', 'I', 'T'};
    for (int i = 0; i < sizeof(palit); i++)
    {
        int32_t letter = bus->i2c_smbus_read_byte_data(i2c_addr, 0x07 + i);
        if (palit[i] != letter)
        {
            return;
        }
    }

    PalitGPUController*   controller        = new PalitGPUController(bus, i2c_addr);
    RGBController_PalitGPU* rgb_controller  = new RGBController_PalitGPU(controller);
    rgb_controller->name                    = name;
    rgb_controller->vendor                  = name.substr(0, name.find(' '));

    ResourceManager::get()->RegisterRGBController(rgb_controller);
} /* DetectPalitGPUControllers() */

REGISTER_I2C_PCI_DETECTOR("Palit 1080", DetectPalitGPUControllers, NVIDIA_VEN, NVIDIA_GTX1080_DEV, NVIDIA_SUB_VEN, NVIDIA_GTX1080_DEV, 0x08);
