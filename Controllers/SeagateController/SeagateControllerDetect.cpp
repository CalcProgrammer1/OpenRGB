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
*           Detects Seagate FireCuda HDD devices                                           *
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

            if(dev)
            {
                SeagateController*     controller     = new SeagateController(dev, info->path);
                RGBController_Seagate* rgb_controller = new RGBController_Seagate(controller);

                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
        }
        info = info->next;
    }

    scsi_free_enumeration(info);

}   /* DetectSeagateControllers() */

REGISTER_DETECTOR("Seagate Firecuda HDD", DetectSeagateControllers);
