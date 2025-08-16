/*---------------------------------------------------------*\
| ROGArionDetect.cpp                                        |
|                                                           |
|   Detector for ASUS ROG Arion                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "ENESMBusController.h"
#include "ENESMBusInterface_ROGArion.h"
#include "RGBController_ENESMBus.h"
#include "scsiapi.h"

/******************************************************************************************\
*                                                                                          *
*   DetectROGArionControllers                                                              *
*                                                                                          *
*           Detects ENE SMBus controllers on ASUS ROG Arion devices                        *
*                                                                                          *
\******************************************************************************************/

void DetectROGArionControllers()
{
    scsi_device_info * info = scsi_enumerate(NULL, NULL);

    while(info)
    {
        if(strncmp(info->vendor, "ROG", 3) == 0 && strncmp(info->product, "ESD-S1C", 7) == 0)
        {
            scsi_device * dev = scsi_open_path(info->path);

            if(dev)
            {
                ENESMBusInterface_ROGArion* interface      = new ENESMBusInterface_ROGArion(dev, info->path);
                ENESMBusController*         controller     = new ENESMBusController(interface, 0x67, "Asus ROG Strix Arion", DEVICE_TYPE_STORAGE);
                RGBController_ENESMBus*     rgb_controller = new RGBController_ENESMBus(controller);

                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
        }
        info = info->next;
    }

    scsi_free_enumeration(info);

}   /* DetectROGArionControllers() */

REGISTER_DETECTOR("ASUS ROG Arion", DetectROGArionControllers);
