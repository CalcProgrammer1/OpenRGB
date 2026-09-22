/*---------------------------------------------------------*\
| HYTENexusControllerDetect.cpp                             |
|                                                           |
|   Detector for HYTE Nexus                                 |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      19 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "DetectionManager.h"
#include "HYTENexusController.h"
#include "RGBController_HYTENexus.h"

DetectedControllers DetectHYTENexusControllers(SerialDeviceInfo* port_info, const std::string& name)
{
    DetectedControllers detected_controllers;

    HYTENexusController *     controller     = new HYTENexusController((char *)port_info->port_path.c_str(), port_info->product_id, name);
    RGBController_HYTENexus * rgb_controller = new RGBController_HYTENexus(controller);

    detected_controllers.push_back(rgb_controller);

    return(detected_controllers);
}

REGISTER_USB_SERIAL_DETECTOR("HYTE THICC Q60",          DetectHYTENexusControllers, HYTE_VID, HYTE_THICC_Q60_PID);
REGISTER_USB_SERIAL_DETECTOR("HYTE Nexus Portal NP50",  DetectHYTENexusControllers, HYTE_VID, HYTE_NEXUS_PORTAL_NP50_PID);
