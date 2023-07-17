#include "Detector.h"
#include "LogManager.h"
#include "SeagateController.h"
#include "RGBController.h"
#include "RGBController_Seagate.h"
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <getopt.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

/******************************************************************************************\
*                                                                                          *
*   DetectSeagateControllers                                                               *
*                                                                                          *
*           Detects ENE SMBus controllers on XPG Spectrix S40G NVMe devices                *
*                                                                                          *
*           Tests for the existance of a file descriptor matching                          *
*           SCSI#Disk&Ven_NVMe&Prod_XPG_SPECTRIX_S40# on Windows machines                  *
*                                                                                          *
\******************************************************************************************/

void DetectSeagateControllers()
{
    /*---------------------------------------------------------------------*\
    | Search for /dev/sgX nodes with model matching "FireCuda HDD"          |
    \*---------------------------------------------------------------------*/
    unsigned int sg_idx = 0;

    while(1)
    {
        /*-------------------------------------------------*\
        | Create the scsi_generic class model path          |
        \*-------------------------------------------------*/
        char sg_dev_buf[1024];

        snprintf(sg_dev_buf, 1024, "/sys/class/scsi_generic/sg%d/device/model", sg_idx);

        /*-------------------------------------------------*\
        | Open the input event path to get the name         |
        \*-------------------------------------------------*/
        int sg_model_fd = open(sg_dev_buf, O_RDONLY|O_NONBLOCK);

        if(sg_model_fd < 0)
        {
            break;
        }

        memset(sg_dev_buf, 0, 1024);

        if(read(sg_model_fd, sg_dev_buf, 1024) < 0)
        {
            LOG_WARNING("[Seagate Firecuda HDD] Probing %d, failed to read NVMe model", sg_idx);
        }
        else
        {
            LOG_DEBUG("[Seagate Firecuda HDD] Probing %d, model: %s", sg_idx, sg_dev_buf);
        }

        close(sg_model_fd);

        /*-------------------------------------------------*\
        | Check if this SCSI device is a Seagate Firecuda   |
        | HDD                                               |
        \*-------------------------------------------------*/
        if(strncmp(sg_dev_buf, "FireCuda HDD", 12) == 0)
        {
            snprintf(sg_dev_buf, 1024, "/dev/sg%d", sg_idx);

            int fd = open(sg_dev_buf, O_RDWR);

            if(fd > 0)
            {
                SeagateController*     controller     = new SeagateController(fd, sg_dev_buf);
                RGBController_Seagate* rgb_controller = new RGBController_Seagate(controller);

                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
        }

        sg_idx++;
    }

}   /* DetectSpectrixS40GControllers() */

REGISTER_DETECTOR(    "Seagate Firecuda HDD", DetectSeagateControllers);
