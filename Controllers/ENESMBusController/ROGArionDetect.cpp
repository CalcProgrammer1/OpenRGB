/*---------------------------------------------------------*\
| ROGArionDetect.cpp                                        |
|                                                           |
|   Detector for ASUS ROG Arion                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "ENESMBusController.h"
#include "ENESMBusInterface_ROGArion.h"
#include "RGBController_ENESMBus.h"
#include "scsiapi.h"

DetectedControllers DetectROGArionControllers()
{
    DetectedControllers detected_controllers;
    scsi_device_info *  info;

    info = scsi_enumerate(NULL, NULL);

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

                detected_controllers.push_back(rgb_controller);
            }
        }
        info = info->next;
    }

    scsi_free_enumeration(info);

    return(detected_controllers);
}

REGISTER_DETECTOR("ASUS ROG Arion", DetectROGArionControllers);
