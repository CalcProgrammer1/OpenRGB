/*---------------------------------------------------------*\
| HPOmenLaptopWMIDetect_Windows.cpp                         |
|                                                           |
|   Detector for HP Omen laptop                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "HPOmenLaptopController_Windows.h"
#include "RGBController_HPOmenLaptopWMI_Windows.h"

DetectedControllers DetectHPOmenLaptopWMIControllers()
{
    DetectedControllers detected_controllers;

    HPOmenLaptopController_Windows * controller = new HPOmenLaptopController_Windows();

    if(!controller->isLightingSupported() || controller->getKeyboardType() != KeyboardType::WITHOUT_NUMPAD)
    {
        delete controller;
    }
    else
    {
        RGBController * rgb_controller = new RGBController_HPOmenLaptopWMI_Windows(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_DETECTOR("HP Omen 4-Zone Laptop Keyboard", DetectHPOmenLaptopWMIControllers);
