/*---------------------------------------------------------*\
| RaspberryPiControllerDetect_Linux.cpp                     |
|                                                           |
|   Detector for Raspberry Pi GPIO ARGB                     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Jul 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include "Detector.h"
#include "RaspberryPiController_Linux.h"
#include "RGBController_RaspberryPi_Linux.h"

/******************************************************************************************\
 *                                                                                        *
 *   DetectRaspberryPiControllers                                                         *
 *                                                                                        *
 *       Detect Raspberry Pi controllers                                                  *
 *                                                                                        *
\******************************************************************************************/

void DetectRaspberryPiControllers()
{
    RaspberryPiController* controller = new RaspberryPiController();

    if(controller->Initialize())
    {
        RGBController_RaspberryPi* rgb_controller = new RGBController_RaspberryPi(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_DETECTOR("Raspberry Pi", DetectRaspberryPiControllers);
