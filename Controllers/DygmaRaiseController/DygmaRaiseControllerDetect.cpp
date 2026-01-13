/*---------------------------------------------------------*\
| DygmaRaiseControllerDetect.cpp                            |
|                                                           |
|   Detector for Dygma Raise keyboard                       |
|                                                           |
|   Timo Schlegel (@eispalast)                  Dec 12 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "DetectionManager.h"
#include "DygmaRaiseController.h"
#include "find_usb_serial_port.h"
#include "RGBController_DygmaRaise.h"

#define DYGMA_RAISE_VID 0x1209
#define DYGMA_RAISE_PID 0x2201

DetectedControllers DetectDygmaRaiseControllers()
{
    DetectedControllers         detected_controllers;
    std::vector<std::string *>  ports = find_usb_serial_port(DYGMA_RAISE_VID, DYGMA_RAISE_PID);

    for(std::size_t i = 0; i < ports.size(); i++)
    {
        if(*ports[i] != "")
        {
            DygmaRaiseController*     controller     = new DygmaRaiseController();
            controller->Initialize((char *)ports[i]->c_str());

            RGBController_DygmaRaise* rgb_controller = new RGBController_DygmaRaise(controller);

            detected_controllers.push_back(rgb_controller);
        }
    }

    return(detected_controllers);
}

REGISTER_DETECTOR("Dygma Raise", DetectDygmaRaiseControllers);
/*---------------------------------------------------------------------------------------------------------*\
| Entries for dynamic UDEV rules                                                                            |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("Dygma Raise", DetectDygmaRaiseControllers, 0x1209, 0x2201 )                        |
\*---------------------------------------------------------------------------------------------------------*/
