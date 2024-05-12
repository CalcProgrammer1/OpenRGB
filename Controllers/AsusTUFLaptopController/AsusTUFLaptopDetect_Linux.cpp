/*---------------------------------------------------------*\
| AsusTUFLaptopDetect_Linux.cpp                             |
|                                                           |
|   Detector for ASUS TUF laptop                            |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string>
#include <unistd.h>
#include "Detector.h"
#include "RGBController_AsusTUFLaptop_Linux.h"

static void DetectAsusTUFLaptopLinuxControllers()
{
    /*-------------------------------------------------------------------------------------*\
    | If /sys/devices/platform/asus-nb-wmi/leds/asus::kbd_backlight/kbd_rgb_mode exists,    |
    |   the kernel support TUF Laptop keyboard LED controlling.                             |
    \*-------------------------------------------------------------------------------------*/

    std::string s = "";
    s.append(ASUS_KBD_BACKLIGHT_BASE_PATH);
    s.append(ASUS_KBD_BACKLIGHT_MODE_PATH);

    if(!access(s.c_str(), F_OK))
    {
        AsusTUFLaptopLinuxController*       controller     = new AsusTUFLaptopLinuxController();
        RGBController_AsusTUFLaptopLinux*   rgb_controller = new RGBController_AsusTUFLaptopLinux(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
    return;
}

REGISTER_DETECTOR("ASUS TUF Laptop", DetectAsusTUFLaptopLinuxControllers);
