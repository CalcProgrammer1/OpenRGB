/*---------------------------------------------------------*\
| NZXTHuePlusControllerDetect.cpp                           |
|                                                           |
|   Detector for NZXT Hue Plus                              |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      27 Aug 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include "Detector.h"
#include "NZXTHuePlusController.h"
#include "RGBController_NZXTHuePlus.h"
#include "find_usb_serial_port.h"

#define NZXT_HUE_PLUS_VID 0x04D8
#define NZXT_HUE_PLUS_PID 0x00DF

/******************************************************************************************\
*                                                                                          *
*   DetectNZXTHuePlusControllers                                                           *
*                                                                                          *
*       Detect devices supported by the NZXTHuePlus driver                                 *
*                                                                                          *
\******************************************************************************************/

void DetectNZXTHuePlusControllers()
{
    std::vector<std::string *> ports = find_usb_serial_port(NZXT_HUE_PLUS_VID, NZXT_HUE_PLUS_PID);

    for(unsigned int i = 0; i < ports.size(); i++)
    {
        if(*ports[i] != "")
        {
            HuePlusController*     controller     = new HuePlusController();
            controller->Initialize((char *)ports[i]->c_str());
            RGBController_HuePlus* rgb_controller = new RGBController_HuePlus(controller);

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}   /* DetectHuePlusControllers() */

REGISTER_DETECTOR("NZXT Hue+", DetectNZXTHuePlusControllers);
/*---------------------------------------------------------------------------------------------------------*\
| Entries for dynamic UDEV rules                                                                            |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("NZXT Hue+", DetectNZXTHuePlusControllers, 0x04D8, 0x00DF )                         |
\*---------------------------------------------------------------------------------------------------------*/
