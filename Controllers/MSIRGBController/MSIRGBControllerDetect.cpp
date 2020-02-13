#include "MSIRGBController.h"
#include "RGBController.h"
#include "RGBController_MSIRGB.h"
#include "super_io.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************\
*                                                                                          *
*   DetectMSIRGBControllers                                                                *
*                                                                                          *
*       Detect MSI-RGB compatible Super-IO chips.                                          *
*                                                                                          *
\******************************************************************************************/

void DetectMSIRGBControllers(std::vector<RGBController*> &rgb_controllers)
{
    int sioaddr = 0x4E;
    superio_enter(sioaddr);

    int val = (superio_inb(sioaddr, SIO_REG_DEVID) << 8) | superio_inb(sioaddr, SIO_REG_DEVID + 1);

    switch (val & SIO_ID_MASK)
    {
    case SIO_NCT6795_ID:
    case SIO_NCT6797_ID:
        MSIRGBController* new_msi = new MSIRGBController(sioaddr);
        RGBController_MSIRGB* new_rgb = new RGBController_MSIRGB(new_msi);

        rgb_controllers.push_back(new_rgb);
        break;
    }
}   /* DetectMSIRGBControllers() */
