#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "Detector.h"
#include "ENESMBusController.h"
#include "ENESMBusInterface_SpectrixS40G_Linux.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_ENESMBus.h"

void DetectSpectrixControllers(char * model)
{
    /*-----------------------------------------------------*\
    | Search for /dev/nvmeX nodes with given model          |
    \*-----------------------------------------------------*/
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
            LOG_WARNING("[XPG Spectrix SSD] Probing %d, failed to read NVMe model", nvme_idx);
        }
        else
        {
            LOG_DEBUG("[XPG Spectrix SSD] Probing %d, model: %s", nvme_idx, nvme_dev_buf);
        }

        close(nvme_model_fd);

        /*-------------------------------------------------*\
        | Check if this NVMe device is the right model      |
        \*-------------------------------------------------*/
        if(strncmp(nvme_dev_buf, model, strlen(model)) == 0)
        {
            snprintf(nvme_dev_buf, 1024, "/dev/nvme%d", nvme_idx);

            int nvme_fd = open(nvme_dev_buf, O_RDWR);

            if(nvme_fd > 0)
            {
                ENESMBusInterface_SpectrixS40G* interface      = new ENESMBusInterface_SpectrixS40G(nvme_fd, nvme_dev_buf);
                ENESMBusController*             controller     = new ENESMBusController(interface, 0x67);
                RGBController_ENESMBus*         rgb_controller = new RGBController_ENESMBus(controller);

                rgb_controller->name                           = "XPG Spectrix S40G";
                rgb_controller->type                           = DEVICE_TYPE_STORAGE;
                rgb_controller->vendor                         = "XPG";

                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
        }

        nvme_idx++;
    }
}   /* DetectSpectrixControllers() */

/******************************************************************************************\
*                                                                                          *
*   DetectSpectrixS20GControllers                                                          *
*                                                                                          *
*           Detects ENE SMBus controllers on XPG Spectrix S20G NVMe devices                *
*                                                                                          *
\******************************************************************************************/

void DetectSpectrixS20GControllers()
{
    DetectSpectrixControllers("XPG SPECTRIX S20G");
}

/******************************************************************************************\
*                                                                                          *
*   DetectSpectrixS40GControllers                                                          *
*                                                                                          *
*           Detects ENE SMBus controllers on XPG Spectrix S40G NVMe devices                *
*                                                                                          *
\******************************************************************************************/

void DetectSpectrixS40GControllers()
{
    DetectSpectrixControllers("XPG SPECTRIX S40G");
}

REGISTER_DETECTOR("XPG Spectrix S20G",  DetectSpectrixS20GControllers);
REGISTER_DETECTOR("XPG Spectrix S40G",  DetectSpectrixS40GControllers);
