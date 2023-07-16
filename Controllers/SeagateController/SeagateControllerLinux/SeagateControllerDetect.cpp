#include "Detector.h"
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
        int fd = open("/dev/sdb", O_RDWR);

        if(fd > 0)
        {
            SeagateController*     controller     = new SeagateController(fd, "/dev/sdb");
            RGBController_Seagate* rgb_controller = new RGBController_Seagate(controller);

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }

}   /* DetectSpectrixS40GControllers() */

REGISTER_DETECTOR(    "Seagate Firecuda HDD", DetectSeagateControllers);
