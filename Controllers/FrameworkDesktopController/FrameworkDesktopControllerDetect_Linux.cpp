/*---------------------------------------------------------*\
| FrameworkDesktopControllerDetect_Linux.cpp                |
|                                                           |
|   Detector for the Framework Desktop ARGB header          |
|                                                           |
|   Francis De Brabandere (francisdb@gmail.com) 16 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <fcntl.h>
#include <unistd.h>
#include "DetectionManager.h"
#include "dmiinfo.h"
#include "FrameworkDesktopController_Linux.h"
#include "RGBController_FrameworkDesktop_Linux.h"

DetectedControllers DetectFrameworkDesktopControllers()
{
    DetectedControllers detected_controllers;
    DMIInfo             dmi;

    /*-----------------------------------------------------*\
    | The cros_ec device also exists on Framework laptops   |
    | and Chromebooks, so only detect on the Desktop        |
    \*-----------------------------------------------------*/
    if(dmi.getManufacturer() == "Framework" && dmi.getProductName().rfind("Desktop", 0) == 0)
    {
        int fd = open(FRAMEWORK_DESKTOP_CROS_EC_DEV, O_RDWR);

        if(fd >= 0)
        {
            FrameworkDesktopController*     controller     = new FrameworkDesktopController(fd, FRAMEWORK_DESKTOP_CROS_EC_DEV);
            RGBController_FrameworkDesktop* rgb_controller = new RGBController_FrameworkDesktop(controller);

            detected_controllers.push_back(rgb_controller);
        }
    }

    return(detected_controllers);
}

REGISTER_DETECTOR("Framework Desktop", DetectFrameworkDesktopControllers);
REGISTER_CUSTOM_UDEV_RULE(framework_desktop, "Framework Desktop", "SUBSYSTEM==\"misc\", KERNEL==\"cros_ec\", ATTR{[dmi/id]board_vendor}==\"Framework\", ATTR{[dmi/id]product_name}==\"Desktop*\", TAG+=\"uaccess\", TAG+=\"Framework_Desktop\"");
