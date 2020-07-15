/*-----------------------------------------*\
|  MSIGPUControllerDetect.cpp               |
|                                           |
|  Driver for MSI GPUs                      |
|                                           |
\*-----------------------------------------*/

#include "MSIGPUController.h"
#include "RGBController.h"
#include "RGBController_MSIGPU.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   IsMSIGPUController                                                                     *
*                                                                                          *
*       Compare PCI IDs                                                                    *
*                                                                                          *
\******************************************************************************************/

bool IsMSIGPUController(i2c_smbus_interface* bus)
{
    bool pass = false;

    if (bus->port_id != 1)
    {
        return(pass);
    }

    // MSI RTX 2080S Gaming X Trio
    if (bus->pci_device == 0x1e81 &&
        bus->pci_vendor == 0x10de &&
        bus->pci_subsystem_device == 0xc724 &&
        bus->pci_subsystem_vendor == 0x1462)
    {
        pass = true;
    }

    // MSI RTX 2070S Gaming X Trio
    if (bus->pci_device == 0x1e84 &&
        bus->pci_vendor == 0x10de &&
        bus->pci_subsystem_device == 0xc726 &&
        bus->pci_subsystem_vendor == 0x1462)
    {
        pass = true;
    }

    // MSI RTX 2080 Ti Sea Hawk EK X
    if (bus->pci_device == 0x1e07 &&
        bus->pci_vendor == 0x10de &&
        bus->pci_subsystem_device == 0x3717 &&
        bus->pci_subsystem_vendor == 0x1462)
    {
        pass = true;
    }

    // MSI RTX 2070 ARMOR
    if (bus->pci_device == 0x1f02 &&
        bus->pci_vendor == 0x10de &&
        bus->pci_subsystem_device == 0x3734 &&
        bus->pci_subsystem_vendor == 0x1462)
    {
        pass = true;
    }

    return(pass);
}   /* IsMSIGPUController() */


/******************************************************************************************\
*                                                                                          *
*   DetectMSIGPUControllers                                                               *
*                                                                                          *
*       Detect MSI GPU controllers on the enumerated I2C busses.                          *
*                                                                                          *
\******************************************************************************************/

void DetectMSIGPUControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers)
{
    MSIGPUController* new_msi_gpu;
    RGBController_MSIGPU* new_controller;

    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        if (IsMSIGPUController(busses[bus]))
        {
            new_msi_gpu = new MSIGPUController(busses[bus]);
            new_controller = new RGBController_MSIGPU(new_msi_gpu);
            rgb_controllers.push_back(new_controller);
        }
    }
} /* DetectMSIGPUControllers() */
