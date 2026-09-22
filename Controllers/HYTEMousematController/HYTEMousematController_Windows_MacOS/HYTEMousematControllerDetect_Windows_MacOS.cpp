/*---------------------------------------------------------*\
| HYTEMousematControllerDetect_Windows_MacOS.cpp            |
|                                                           |
|   Detector for HYTE mousemat (Serial implementation for   |
|   Windows and MacOS)                                      |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      18 Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "DetectionManager.h"
#include "HYTEMousematController_Windows_MacOS.h"
#include "RGBController_HYTEMousemat.h"

#define HYTE_VID                0x3402

#define HYTE_CNVS_HW_VER_1_PID  0x0B00
#define HYTE_CNVS_HW_VER_2_PID  0x0B01

DetectedControllers DetectHYTEMousematControllers(SerialDeviceInfo* port_info, const std::string& name)
{
    DetectedControllers detected_controllers;

    HYTEMousematController *     controller     = new HYTEMousematController((char *)port_info->port_path.c_str(), name);
    RGBController_HYTEMousemat * rgb_controller = new RGBController_HYTEMousemat(controller);

    detected_controllers.push_back(rgb_controller);

    return(detected_controllers);
}

REGISTER_USB_SERIAL_DETECTOR("HYTE CNVS", DetectHYTEMousematControllers, HYTE_VID, HYTE_CNVS_HW_VER_1_PID);
REGISTER_USB_SERIAL_DETECTOR("HYTE CNVS", DetectHYTEMousematControllers, HYTE_VID, HYTE_CNVS_HW_VER_2_PID);
