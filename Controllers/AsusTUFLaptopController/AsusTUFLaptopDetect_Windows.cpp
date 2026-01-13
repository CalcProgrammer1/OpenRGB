/*---------------------------------------------------------*\
| AsusTUFLaptopDetect_Windows.cpp                           |
|                                                           |
|   Detector for ASUS TUF laptop                            |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string>
#include "DetectionManager.h"
#include "RGBController_AsusTUFLaptop_Windows.h"
#include "wmi.h"

DetectedControllers DetectAsusTUFLaptopWMIControllers()
{
    // Try to retrieve ProductID / Device name from WMI; Possibly can be rewritten to use wmi.cpp
    // IF you encounter false detection ( e.g. if your laptop keyboard backlight uses USB interface
    // instead of ACPI WMI) please add a WHITELIST by checking the
    // `name` variable for model substrings like "FX505DU"
    // For now, checking for "TUF Gaming" should suffice
    DetectedControllers     detected_controllers;
    Wmi                     wmi;
    std::vector<QueryObj>   systemProduct;

    if(wmi.query("SELECT * FROM Win32_ComputerSystemProduct", systemProduct) != 0)
    {
        // There should only be one, a cycle is a precaution
        if(systemProduct.size() == 1)
        {
            std::string& name = systemProduct[0]["Name"];

            if(name.find("TUF Gaming") != name.npos)
            {
                AsusTUFLaptopController* controller = AsusTUFLaptopController::checkAndCreate();
                if(controller)
                {
                    RGBController* rgb_controller = new RGBController_AsusTUFLaptopWMI(controller);

                    detected_controllers.push_back(rgb_controller);
                }
            }
        }
    }

    return(detected_controllers);
}   /* DetectAsusTUFLaptopWMIControllers() */

REGISTER_DETECTOR("ASUS TUF Laptop", DetectAsusTUFLaptopWMIControllers);
