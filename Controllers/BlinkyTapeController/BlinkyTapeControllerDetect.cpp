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
#include "find_usb_serial_port.h"

/*---------------------------------------------------------*\
| BlinkyTape VID and PID                                    |
\*---------------------------------------------------------*/
#define BLINKINLABS_VID                         0x1D50
#define BLINKYTAPE_PID                          0x605E

DetectedControllers DetectBlinkyTapeControllers()
{
    DetectedControllers         detected_controllers;
    std::vector<std::string>    device_locations = find_usb_serial_port(BLINKINLABS_VID, BLINKYTAPE_PID);

    for(unsigned int device_idx = 0; device_idx < device_locations.size(); device_idx++)
    {
        BlinkyTapeController*     controller     = new BlinkyTapeController();
        controller->Initialize(device_locations[device_idx]);

        RGBController_BlinkyTape* rgb_controller = new RGBController_BlinkyTape(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_DETECTOR("BlinkyTape", DetectBlinkyTapeControllers);
REGISTER_CUSTOM_UDEV_RULE(blinky_tape, "BlinkyTape", "SUBSYSTEMS==\"serial|hidraw\", ATTRS{idVendor}==\"1d50\", ATTRS{idProduct}==\"605e\", TAG+=\"uaccess\", TAG+=\"BlinkyTape\"");
