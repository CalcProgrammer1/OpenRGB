/*---------------------------------------------------------*\
| AresonControllerDetect.cpp                                |
|                                                           |
|   Detector for Areson mice                                |
|                                                           |
|   Morgan Guimard (morg)                       29 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "AresonController.h"
#include "RGBController_Areson.h"

/*---------------------------------------------------------*\
| Areson vendor ID                                          |
\*---------------------------------------------------------*/
#define ARESON_VID                                     0x25A7

/*---------------------------------------------------------*\
| Product ID                                                |
\*---------------------------------------------------------*/
#define ZET_GAMING_EDGE_AIR_PRO_WIRELESS_PID           0xFA3F
#define ZET_GAMING_EDGE_AIR_PRO_PID                    0xFA40
#define ZET_GAMING_EDGE_AIR_ELIT_WIRELESS_PID          0xFA48
#define ZET_GAMING_EDGE_AIR_ELIT_PID                   0xFA49
#define REDRAGON_M914_PID                              0xFA7B
#define REDRAGON_M914_WIRELESS_PID                     0xFA7C

void DetectAresonControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AresonController*     controller      = new AresonController(dev, *info, name);
        RGBController_Areson* rgb_controller  = new RGBController_Areson(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("ZET GAMING Edge Air Pro (Wireless)",     DetectAresonControllers, ARESON_VID, ZET_GAMING_EDGE_AIR_PRO_WIRELESS_PID,      1, 0xFF02, 2);
REGISTER_HID_DETECTOR_IPU("ZET GAMING Edge Air Pro",                DetectAresonControllers, ARESON_VID, ZET_GAMING_EDGE_AIR_PRO_PID,               1, 0xFF02, 2);
REGISTER_HID_DETECTOR_IPU("ZET GAMING Edge Air Elit (Wireless)",    DetectAresonControllers, ARESON_VID, ZET_GAMING_EDGE_AIR_ELIT_WIRELESS_PID,     1, 0xFF02, 2);
REGISTER_HID_DETECTOR_IPU("ZET GAMING Edge Air Elit",               DetectAresonControllers, ARESON_VID, ZET_GAMING_EDGE_AIR_ELIT_PID,              1, 0xFF02, 2);
REGISTER_HID_DETECTOR_IPU("Redragon M914 NIX (Wireless)",           DetectAresonControllers, ARESON_VID, REDRAGON_M914_WIRELESS_PID,                1, 0xFF02, 2);
REGISTER_HID_DETECTOR_IPU("Redragon M914 NIX",                      DetectAresonControllers, ARESON_VID, REDRAGON_M914_PID,                         1, 0xFF02, 2);
