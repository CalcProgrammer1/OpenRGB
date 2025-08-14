/*---------------------------------------------------------*\
| MSIRGBControllerDetect.cpp                                |
|                                                           |
|   Detector for MSI-RGB motherboard                        |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "MSIRGBController.h"
#include "RGBController_MSIRGB.h"
#include "super_io.h"
#include "dmiinfo.h"

/******************************************************************************************\
*                                                                                          *
*   DetectMSIRGBControllers                                                                *
*                                                                                          *
*       Detect MSI-RGB compatible Super-IO chips.                                          *
*                                                                                          *
\******************************************************************************************/

#define NUM_COMPATIBLE_DEVICES (sizeof(compatible_devices) / sizeof(compatible_devices[0]))

typedef struct
{
    const char* name;
    bool invert;
} msi_device;

static msi_device compatible_devices[] =
{
    {"7A40", false},
    {"7A34", false},
    {"7A39", false},
    {"7A38", false},
    {"7B79", false},
    {"7B73", false},
    {"7B61", false},
    {"7B54", false},
    {"7B49", false},
    {"7B48", false},
    {"7B45", false},
    {"7B44", false},
    {"7A59", false},
    {"7A57", false},
    {"7A68", false},
    {"7B40", false},
    {"7A94", false},
    {"7B06", false},
    {"7B08", false}, // B350 KRAIT GAMING (MS-7B08)
    {"7B09", false},
    {"7A58", false},
    {"7A62", false},
    {"7A69", false},
    {"7A70", false},
    {"7A72", false},
    {"7A78", false},
    {"7A79", false},
    {"7A37", false},
    {"7B89", true },
    {"7B90", true },
    {"7B19", true },
    {"7C02", true },
    {"7B75", true },
    {"7B22", true },
    {"7B23", true },
    {"7B24", true },
    {"7B27", true },
    {"7B30", true },
    {"7B31", true },
    {"7B51", true },
    {"7C04", true },
    {"7C00", true },
    {"7B98", true },
    {"7C22", true },
    {"7C24", true },
    {"7C01", true },
    {"7C39", true },
    {"7B86", true },
    {"7B87", true },
    {"7D95", false}
};

void DetectMSIRGBControllers()
{
    int sio_addrs[2] = {0x2E, 0x4E};

    DMIInfo board;
    std::string board_dmi = board.getMainboard();
    std::string manufacturer = board.getManufacturer();

    if (manufacturer != "Micro-Star International Co., Ltd." && manufacturer != "Micro-Star International Co., Ltd" && manufacturer != "MSI")
    {
        return;
    }

    for(int sioaddr_idx = 0; sioaddr_idx < 2; sioaddr_idx++)
    {
        int sioaddr = sio_addrs[sioaddr_idx];

        superio_enter(sioaddr);

        int val = (superio_inb(sioaddr, SIO_REG_DEVID) << 8) | superio_inb(sioaddr, SIO_REG_DEVID + 1);

        switch (val & SIO_ID_MASK)
        {
        case SIO_NCT6795_ID:
        case SIO_NCT6797_ID:
            for(unsigned int i = 0; i < NUM_COMPATIBLE_DEVICES; i++)
            {
                if (board_dmi.find(std::string(compatible_devices[i].name)) != std::string::npos)
                {
                    MSIRGBController*     controller     = new MSIRGBController(sioaddr, compatible_devices[i].invert, "MSI " + board_dmi);
                    RGBController_MSIRGB* rgb_controller = new RGBController_MSIRGB(controller);

                    ResourceManager::get()->RegisterRGBController(rgb_controller);
                    break;
                }
            }
            break;
        }
    }
}   /* DetectMSIRGBControllers() */

REGISTER_DETECTOR("MSI-RGB", DetectMSIRGBControllers);
