#include "Detector.h"
#include "LogManager.h"
#include "SeagateController.h"
#include "RGBController.h"
#include "RGBController_Seagate.h"
#include <vector>

#include "scsiapi.h"

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
    scsi_device_info * info = scsi_enumerate(NULL, NULL);

    while(info)
    {
        if(strncmp(info->vendor, "Seagate", 7) == 0 && strncmp(info->product, "FireCuda HDD", 12) == 0)
        {
            scsi_device * dev = scsi_open_path(info->path);

            SeagateController*     controller     = new SeagateController(dev, info->path);
            RGBController_Seagate* rgb_controller = new RGBController_Seagate(controller);

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        info = info->next;
    }

    scsi_free_enumeration(info);

}   /* DetectSpectrixS40GControllers() */

REGISTER_DETECTOR(    "Seagate Firecuda HDD", DetectSeagateControllers);
