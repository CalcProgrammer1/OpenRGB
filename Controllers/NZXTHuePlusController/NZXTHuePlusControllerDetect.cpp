/*---------------------------------------------------------*\
| NZXTHuePlusControllerDetect.cpp                           |
|                                                           |
|   Detector for NZXT Hue Plus                              |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      27 Aug 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "DetectionManager.h"
#include "find_usb_serial_port.h"
#include "NZXTHuePlusController.h"
#include "RGBController_NZXTHuePlus.h"

/*---------------------------------------------------------*\
| NZXT USB IDs                                              |
\*---------------------------------------------------------*/
#define NZXT_HUE_PLUS_VID 0x04D8
#define NZXT_HUE_PLUS_PID 0x00DF

DetectedControllers DetectNZXTHuePlusControllers()
{
    DetectedControllers         detected_controllers;
    std::vector<std::string>    ports;

    ports = find_usb_serial_port(NZXT_HUE_PLUS_VID, NZXT_HUE_PLUS_PID);

    for(std::size_t i = 0; i < ports.size(); i++)
    {
        if(ports[i] != "")
        {
            HuePlusController*     controller     = new HuePlusController();
            controller->Initialize((char *)ports[i].c_str());
            RGBController_HuePlus* rgb_controller = new RGBController_HuePlus(controller);

            detected_controllers.push_back(rgb_controller);
        }
    }

    return(detected_controllers);
}

REGISTER_DETECTOR("NZXT Hue+", DetectNZXTHuePlusControllers);
REGISTER_CUSTOM_UDEV_RULE(nzxt_hue_plus, "NZXT Hue+", "SUBSYSTEMS==\"serial|hidraw\", ATTRS{idVendor}==\"04d8\", ATTRS{idProduct}==\"00df\", TAG+=\"uaccess\", TAG+=\"NZXT_Hue\"");
