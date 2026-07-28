/*---------------------------------------------------------*\
| ValveSteamMachineControllerDetect_Windows.cpp             |
|                                                           |
|   Detector for Valve Steam Machine LEDs                   |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      28 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <Windows.h>
#include "dmiinfo.h"
#include "LogManager.h"
#include "PawnIOLib.h"
#include "DetectionManager.h"
#include "i2c_smbus_pawnio.h"
#include "ValveSteamMachineController_Windows.h"
#include "RGBController_ValveSteamMachine_Windows_Linux.h"

DetectedControllers DetectValveSteamMachineControllers()
{
    DetectedControllers detected_controllers;
    DMIInfo             dmi;
    HANDLE              pawnio_handle;
    HRESULT             status;

    /*-----------------------------------------------------*\
    | The LedsValve PawnIO module is not capable of DMI     |
    | checking itself, so ensure the DMI info matches that  |
    | of the Steam Machine using the same DMI checks that   |
    | the official Linux leds-valve driver uses.            |
    \*-----------------------------------------------------*/
    if((dmi.getManufacturer() == "OEM"   && dmi.getProductName() == "F7F")
     ||(dmi.getManufacturer() == "Valve" && dmi.getProductName() == "Fremont"))
    {
        status = i2c_smbus_pawnio::start_pawnio("LedsValve.bin", &pawnio_handle);

        if(status == S_OK)
        {
            ValveSteamMachineController*     controller     = new ValveSteamMachineController(pawnio_handle, "Valve Steam Machine");
            RGBController_ValveSteamMachine* rgb_controller = new RGBController_ValveSteamMachine(controller);
            
            detected_controllers.push_back(rgb_controller);
        }
    }

    return(detected_controllers);
}

REGISTER_DETECTOR("Valve Steam Machine", DetectValveSteamMachineControllers);