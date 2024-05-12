/*---------------------------------------------------------*\
| AsusTUFLaptopDetect_Windows.cpp                           |
|                                                           |
|   Detector for ASUS TUF laptop                            |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string>
#include "Detector.h"
#include "RGBController_AsusTUFLaptop_Windows.h"
#include "wmi.h"

static void DetectAsusTUFLaptopWMIControllers()
{
    // Try to retrieve ProductID / Device name from WMI; Possibly can be rewritten to use wmi.cpp
    // IF you encounter false detection ( e.g. if your laptop keyboard backlight uses USB interface
    // instead of ACPI WMI) please add a WHITELIST by checking the
    // `name` variable for model substrings like "FX505DU"
    // For now, checking for "TUF Gaming" should suffice

    Wmi wmi;

    std::vector<QueryObj> systemProduct;
    if (wmi.query("SELECT * FROM Win32_ComputerSystemProduct", systemProduct))
    {
        return;
    }

    // There should only be one, a cycle is a precaution
    if(systemProduct.size() != 1)
    {
        return;
    }
    std::string& name = systemProduct[0]["Name"];

    if(name.find("TUF Gaming") == name.npos)
    {
        return;
    }

    AsusTUFLaptopController* controller = AsusTUFLaptopController::checkAndCreate();
    if(controller)
    {
        RGBController* new_controller = new RGBController_AsusTUFLaptopWMI(controller);

        ResourceManager::get()->RegisterRGBController(new_controller);
    }
}   /* DetectAsusTUFLaptopWMIControllers() */

REGISTER_DETECTOR("ASUS TUF Laptop", DetectAsusTUFLaptopWMIControllers);
