/*---------------------------------------------------------*\
| NZXTKrakenControllerDetect.cpp                            |
|                                                           |
|   Detector for NZXT Kraken                                |
|                                                           |
|   Martin Hartl (inlart)                       04 Apr 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "NZXTKrakenController.h"
#include "RGBController_NZXTKraken.h"

#define NZXT_KRAKEN_VID     0x1E71
#define NZXT_KRAKEN_X2_PID  0x170E
#define NZXT_KRAKEN_M2_PID  0x1715

/******************************************************************************************\
*                                                                                          *
*   DetectNZXTKrakenControllers                                                            *
*                                                                                          *
*       Detect devices supported by the NZXTKraken driver                                  *
*                                                                                          *
\******************************************************************************************/

void DetectNZXTKrakenControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        NZXTKrakenController*     controller     = new NZXTKrakenController(dev, info->path, name);
        RGBController_NZXTKraken* rgb_controller = new RGBController_NZXTKraken(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectNZXTKrakenControllers() */

REGISTER_HID_DETECTOR("NZXT Kraken X2", DetectNZXTKrakenControllers, NZXT_KRAKEN_VID, NZXT_KRAKEN_X2_PID);
REGISTER_HID_DETECTOR("NZXT Kraken M2", DetectNZXTKrakenControllers, NZXT_KRAKEN_VID, NZXT_KRAKEN_M2_PID);
