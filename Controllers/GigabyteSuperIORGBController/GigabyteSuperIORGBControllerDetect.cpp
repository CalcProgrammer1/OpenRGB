/*---------------------------------------------------------*\
| GigabyteSuperIORGBControllerDetect.cpp                    |
|                                                           |
|   Detector for Gigabyte Aorus Super IO motherboard        |
|                                                           |
|   Ryan Frankcombe (422gRdHuX5uk)              11 Sep 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "GigabyteSuperIORGBController.h"
#include "RGBController_GigabyteSuperIORGB.h"
#include "super_io.h"
#include "dmiinfo.h"

#define NUM_COMPATIBLE_DEVICES (sizeof(compatible_devices) / sizeof(compatible_devices[0]))

typedef struct
{
    const char* name;
} gig_device;

static gig_device compatible_devices[] =
{
    {"X570 UD"}
};

void DetectGigabyteSuperIORGBControllers()
{
    int sio_addrs[2]            = {0x2E, 0x4E};

    DMIInfo board;
    std::string board_dmi       = board.getMainboard();
    std::string manufacturer    = board.getManufacturer();

    if(manufacturer != "Gigabyte Technology Co., Ltd.")
    {
        return;
    }

    for(int sioaddr_idx = 0; sioaddr_idx < 2; sioaddr_idx++)
    {
        int sioaddr = sio_addrs[sioaddr_idx];

        superio_enter(sioaddr);

        int val = (superio_inb(sioaddr, SIO_REG_DEVID) << 8) | superio_inb(sioaddr, SIO_REG_DEVID + 1);

        switch(val & SIO_ID_MASK)
        {
            case SIO_ITE8688_ID:
                for(unsigned int i = 0; i < NUM_COMPATIBLE_DEVICES; i++)
                {
                    if(board_dmi.find(std::string(compatible_devices[i].name)) != std::string::npos)
                    {
                        GigabyteSuperIORGBController*     controller     = new GigabyteSuperIORGBController(sioaddr, "Gigabyte " + board_dmi);
                        RGBController_GigabyteSuperIORGB* rgb_controller = new RGBController_GigabyteSuperIORGB(controller);

                        ResourceManager::get()->RegisterRGBController(rgb_controller);
                        break;
                    }
                }
                break;
        }
    }
}   /* DetectGigabyteSuperIORGBControllers() */

REGISTER_DETECTOR("Gigabyte RGB", DetectGigabyteSuperIORGBControllers);
