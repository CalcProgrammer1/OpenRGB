/*---------------------------------------------------------*\
| HYTENexusControllerDetect.cpp                             |
|                                                           |
|   Detector for HYTE Nexus                                 |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      19 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include "Detector.h"
#include "HYTENexusController.h"
#include "RGBController_HYTENexus.h"
#include "find_usb_serial_port.h"

#define HYTE_VID                                    0x3402

struct hyte_nexus_type
{
    unsigned short  vid;
    unsigned short  pid;
    const char *    name;
};

#define HYTE_NEXUS_NUM_DEVICES  2

static const hyte_nexus_type hyte_nexus_devices[] =
{
    { HYTE_VID, HYTE_THICC_Q60_PID,         "HYTE THICC Q60",        },
    { HYTE_VID, HYTE_NEXUS_PORTAL_NP50_PID, "HYTE Nexus Portal NP50" },
};

/******************************************************************************************\
*                                                                                          *
*   DetectHYTENexusControllers                                                             *
*                                                                                          *
*       Detect devices supported by the HYTENexus driver                                   *
*                                                                                          *
\******************************************************************************************/

void DetectHYTENexusControllers()
{
    for(unsigned int device_id = 0; device_id < HYTE_NEXUS_NUM_DEVICES; device_id++)
    {
        std::vector<std::string *> ports = find_usb_serial_port(hyte_nexus_devices[device_id].vid, hyte_nexus_devices[device_id].pid);

        for(unsigned int i = 0; i < ports.size(); i++)
        {
            if(*ports[i] != "")
            {
                HYTENexusController *     controller     = new HYTENexusController((char *)ports[i]->c_str(), hyte_nexus_devices[device_id].pid, hyte_nexus_devices[device_id].name);
                RGBController_HYTENexus * rgb_controller = new RGBController_HYTENexus(controller);

                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
        }
    }
}   /* DetectHYTENexusControllers() */

REGISTER_DETECTOR("HYTE Nexus", DetectHYTENexusControllers);
/*---------------------------------------------------------------------------------------------------------*\
| Entries for dynamic UDEV rules                                                                            |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("HYTE THICC Q60", DetectHYTENexusControllers, 0x3402, 0x0400 )                      |
| DUMMY_DEVICE_DETECTOR("HYTE Nexus Portal NP50", DetectHYTENexusControllers, 0x3402, 0x0901 )              |
\*---------------------------------------------------------------------------------------------------------*/
