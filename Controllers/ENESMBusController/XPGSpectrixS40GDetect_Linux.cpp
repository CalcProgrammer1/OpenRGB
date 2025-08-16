/*---------------------------------------------------------*\
| XPGSpectrixS40GDetect_Linux.cpp                           |
|                                                           |
|   Detector for XPG Spectrix S40G (Linux)                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include "Detector.h"
#include "ENESMBusController.h"
#include "ENESMBusInterface_SpectrixS40G_Linux.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_ENESMBus.h"

/******************************************************************************************\
*                                                                                          *
*   DetectSpectrixS40GControllers                                                          *
*                                                                                          *
*           Detects ENE SMBus controllers on XPG Spectrix S40G NVMe devices                *
*                                                                                          *
\******************************************************************************************/

void DetectSpectrixS40GControllers()
{
    /*---------------------------------------------------------------------*\
    | Search for /dev/nvmeX nodes with model matching "XPG SPECTRIX S40G"   |
    \*---------------------------------------------------------------------*/
    unsigned int nvme_idx = 0;

    while(1)
    {
        /*-------------------------------------------------*\
        | Create the nvme class model path                  |
        \*-------------------------------------------------*/
        char nvme_dev_buf[1024];

        snprintf(nvme_dev_buf, 1024, "/sys/class/nvme/nvme%d/model", nvme_idx);

        /*-------------------------------------------------*\
        | Open the input event path to get the name         |
        \*-------------------------------------------------*/
        int nvme_model_fd = open(nvme_dev_buf, O_RDONLY|O_NONBLOCK);

        if(nvme_model_fd < 0)
        {
            break;
        }

        memset(nvme_dev_buf, 0, 1024);

        if(read(nvme_model_fd, nvme_dev_buf, 1024) < 0)
        {
            LOG_WARNING("[XPG Spectrix S40G] Probing %d, failed to read NVMe model", nvme_idx);
        }
        else
        {
            LOG_DEBUG("[XPG Spectrix S40G] Probing %d, model: %s", nvme_idx, nvme_dev_buf);
        }

        close(nvme_model_fd);

        /*-------------------------------------------------*\
        | Check if this NVMe device is a SPECTRIX S40G      |
        \*-------------------------------------------------*/
        if(strncmp(nvme_dev_buf, "XPG SPECTRIX S40G", 17) == 0)
        {
            snprintf(nvme_dev_buf, 1024, "/dev/nvme%d", nvme_idx);

            int nvme_fd = open(nvme_dev_buf, O_RDWR);

            if(nvme_fd > 0)
            {
                ENESMBusInterface_SpectrixS40G* interface      = new ENESMBusInterface_SpectrixS40G(nvme_fd, nvme_dev_buf);
                ENESMBusController*             controller     = new ENESMBusController(interface, 0x67, "XPG Spectrix S40G", DEVICE_TYPE_STORAGE);
                RGBController_ENESMBus*         rgb_controller = new RGBController_ENESMBus(controller);

                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
        }

        nvme_idx++;
    }
}   /* DetectSpectrixS40GControllers() */

REGISTER_DETECTOR(    "XPG Spectrix S40G",              DetectSpectrixS40GControllers);
