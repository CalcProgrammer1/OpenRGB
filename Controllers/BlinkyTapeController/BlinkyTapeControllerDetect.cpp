/*---------------------------------------------------------*\
| BlinkyTapeControllerDetect.cpp                            |
|                                                           |
|   Detector for BlinkyTape                                 |
|                                                           |
|   Matt Mets (matt@blinkinlabs.com)            01 Jul 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include "Detector.h"
#include "BlinkyTapeController.h"
#include "RGBController_BlinkyTape.h"
#include "find_usb_serial_port.h"

/*-----------------------------------------------------*\
| BlinkyTape VID and PID                                |
\*-----------------------------------------------------*/
#define BLINKINLABS_VID                         0x1D50
#define BLINKYTAPE_PID                          0x605E

/******************************************************************************************\
*                                                                                          *
*   DetectBlinkyTapeControllers                                                            *
*                                                                                          *
*       Detect BlinkyTape devices                                                          *
*                                                                                          *
\******************************************************************************************/

void DetectBlinkyTapeControllers()
{
    std::vector<std::string *> device_locations = find_usb_serial_port(BLINKINLABS_VID, BLINKYTAPE_PID);

    for(unsigned int device_idx = 0; device_idx < device_locations.size(); device_idx++)
    {
        BlinkyTapeController*     controller     = new BlinkyTapeController();
        controller->Initialize(*device_locations[device_idx]);

        RGBController_BlinkyTape* rgb_controller = new RGBController_BlinkyTape(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_DETECTOR("BlinkyTape", DetectBlinkyTapeControllers);
/*---------------------------------------------------------------------------------------------------------*\
| Entries for dynamic UDEV rules                                                                            |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("BlinkyTape", DetectBlinkyTapeControllers, 0x1D50, 0x605E )                         |
\*---------------------------------------------------------------------------------------------------------*/
