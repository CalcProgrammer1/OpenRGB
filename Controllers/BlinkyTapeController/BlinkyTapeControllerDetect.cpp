/*---------------------------------------------------------*\
| BlinkyTapeControllerDetect.cpp                            |
|                                                           |
|   Detector for BlinkyTape                                 |
|                                                           |
|   Matt Mets (matt@blinkinlabs.com)            01 Jul 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "DetectionManager.h"
#include "BlinkyTapeController.h"
#include "RGBController_BlinkyTape.h"

/*---------------------------------------------------------*\
| BlinkyTape VID and PID                                    |
\*---------------------------------------------------------*/
#define BLINKINLABS_VID                         0x1D50
#define BLINKYTAPE_PID                          0x605E

DetectedControllers DetectBlinkyTapeControllers(SerialDeviceInfo* port_info, const std::string& name)
{
    DetectedControllers         detected_controllers;

    BlinkyTapeController*     controller     = new BlinkyTapeController();
    controller->Initialize(port_info->port_path);

    RGBController_BlinkyTape* rgb_controller = new RGBController_BlinkyTape(controller);

    detected_controllers.push_back(rgb_controller);

    return(detected_controllers);
}

REGISTER_USB_SERIAL_DETECTOR("BlinkyTape", DetectBlinkyTapeControllers, BLINKINLABS_VID, BLINKYTAPE_PID);
