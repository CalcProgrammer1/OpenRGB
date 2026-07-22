/*---------------------------------------------------------*\
| HyperXAlloyOrigins60and65ControllerDetect.cpp             |
|                                                           |
|   Driver for HyperX Alloy Origins 60 and 65 keyboards     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "HyperXAlloyOrigins60and65Controller.h"
#include "RGBController_HyperXAlloyOrigins60and65.h"

/*---------------------------------------------------------*\
| HyperX keyboard vendor and product IDs                    |
\*---------------------------------------------------------*/
#define HYPERX_KEYBOARD_VID                         0x0951
#define HYPERX_ALLOY_ORIGINS_60_PID                 0x1734

/*---------------------------------------------------------*\
| HyperX keyboard vendor and product IDs (HP)               |
\*---------------------------------------------------------*/
#define HP_KEYBOARD_VID                             0x03F0
#define HYPERX_ALLOY_ORIGINS_60_HP_PID              0x0C8E
#define HYPERX_ALLOY_ORIGINS_65_HP_PID              0x038F

AlloyOrigins60and65MappingLayoutType GetAlloyOrigins60and65MappingLayoutType(int pid)
{
    switch(pid)
    {
        case HYPERX_ALLOY_ORIGINS_60_PID:
        case HYPERX_ALLOY_ORIGINS_60_HP_PID:
            return ALLOY_ORIGINS_60_LAYOUT;

        case HYPERX_ALLOY_ORIGINS_65_HP_PID:
            return ALLOY_ORIGINS_65_LAYOUT;

        default:
            return ALLOY_ORIGINS_60_LAYOUT;
    }
}

DetectedControllers DetectHyperXAlloyOrigins60and65(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        HyperXAlloyOrigins60and65Controller*     controller     = new HyperXAlloyOrigins60and65Controller(dev, info->path, name);
        AlloyOrigins60and65MappingLayoutType     layout         = GetAlloyOrigins60and65MappingLayoutType(info->product_id);
        RGBController_HyperXAlloyOrigins60and65* rgb_controller = new RGBController_HyperXAlloyOrigins60and65(controller, layout);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

#ifdef _WIN32
REGISTER_HID_DETECTOR_I("HyperX Alloy Origins 60",        DetectHyperXAlloyOrigins60and65, HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ORIGINS_60_PID,      3);
REGISTER_HID_DETECTOR_I("HyperX Alloy Origins 60 (HP)",   DetectHyperXAlloyOrigins60and65, HP_KEYBOARD_VID,     HYPERX_ALLOY_ORIGINS_60_HP_PID,   3);
REGISTER_HID_DETECTOR_I("HyperX Alloy Origins 65 (HP)",   DetectHyperXAlloyOrigins60and65, HP_KEYBOARD_VID,     HYPERX_ALLOY_ORIGINS_65_HP_PID,   3);
#else
REGISTER_HID_DETECTOR_I("HyperX Alloy Origins 60",        DetectHyperXAlloyOrigins60and65, HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ORIGINS_60_PID,      0);
REGISTER_HID_DETECTOR_I("HyperX Alloy Origins 60 (HP)",   DetectHyperXAlloyOrigins60and65, HP_KEYBOARD_VID,     HYPERX_ALLOY_ORIGINS_60_HP_PID,   0);
REGISTER_HID_DETECTOR_I("HyperX Alloy Origins 65 (HP)",   DetectHyperXAlloyOrigins60and65, HP_KEYBOARD_VID,     HYPERX_ALLOY_ORIGINS_65_HP_PID,   0);
#endif
