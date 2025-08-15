/*---------------------------------------------------------*\
| ZETEdgeAirProControllerDetect.cpp                         |
|                                                           |
|   Detector for ZET Edge Air Pro                           |
|                                                           |
|   Morgan Guimard (morg)                       29 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "ZETEdgeAirProController.h"
#include "RGBController_ZETEdgeAirPro.h"

/*---------------------------------------------------------*\
| ZET GAMING vendor ID                                      |
\*---------------------------------------------------------*/
#define ZET_GAMING_VID                                 0x25A7

/*---------------------------------------------------------*\
| Product ID                                                |
\*---------------------------------------------------------*/
#define ZET_GAMING_EDGE_AIR_PRO_WIRELESS_PID           0xFA3F
#define ZET_GAMING_EDGE_AIR_PRO_PID                    0xFA40
#define ZET_GAMING_EDGE_AIR_ELIT_WIRELESS_PID          0xFA48
#define ZET_GAMING_EDGE_AIR_ELIT_PID                   0xFA49

void DetectZETGAMINGEdgeAirProControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        ZETEdgeAirProController*     controller      = new ZETEdgeAirProController(dev, *info, name);
        RGBController_ZETEdgeAirPro* rgb_controller  = new RGBController_ZETEdgeAirPro(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("ZET GAMING Edge Air Pro (Wireless)",     DetectZETGAMINGEdgeAirProControllers, ZET_GAMING_VID, ZET_GAMING_EDGE_AIR_PRO_WIRELESS_PID,     1, 0xFF02, 2);
REGISTER_HID_DETECTOR_IPU("ZET GAMING Edge Air Pro",                DetectZETGAMINGEdgeAirProControllers, ZET_GAMING_VID, ZET_GAMING_EDGE_AIR_PRO_PID,              1, 0xFF02, 2);
REGISTER_HID_DETECTOR_IPU("ZET GAMING Edge Air Elit (Wireless)",    DetectZETGAMINGEdgeAirProControllers, ZET_GAMING_VID, ZET_GAMING_EDGE_AIR_ELIT_WIRELESS_PID,    1, 0xFF02, 2);
REGISTER_HID_DETECTOR_IPU("ZET GAMING Edge Air Elit",               DetectZETGAMINGEdgeAirProControllers, ZET_GAMING_VID, ZET_GAMING_EDGE_AIR_ELIT_PID,             1, 0xFF02, 2);
