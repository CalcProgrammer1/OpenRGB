/*---------------------------------------------------------*\
| ElgatoStreamDeckControllerDetect.cpp                      |
|                                                           |
|   Detector for Elgato Stream Deck MK.2                    |
|                                                           |
|   Ferréol DUBOIS COLI (Fefe_du_973)           23 Jan 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "ElgatoStreamDeckController.h"
#include "RGBController_ElgatoStreamDeck.h"

#define ELGATO_VID        0x0FD9
#define STREAMDECK_MK2_PID 0x0080

void DetectElgatoStreamDeckControllers(hid_device_info* info, const std::string&)
{
    if(info->interface_number == 0)
    {
        hid_device* dev = hid_open_path(info->path);
        if(dev)
        {
            ElgatoStreamDeckController* controller         = new ElgatoStreamDeckController(dev, info->path);
            RGBController_ElgatoStreamDeck* rgb_controller = new RGBController_ElgatoStreamDeck(controller);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}

REGISTER_HID_DETECTOR("Elgato Stream Deck MK.2", DetectElgatoStreamDeckControllers, ELGATO_VID, STREAMDECK_MK2_PID);
