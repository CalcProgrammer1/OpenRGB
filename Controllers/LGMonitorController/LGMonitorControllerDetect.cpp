/*---------------------------------------------------------*\
| LGMonitorControllerDetect.cpp                             |
|                                                           |
|   Detector for LG monitor                                 |
|                                                           |
|   Morgan Guimard (morg)                       11 Oct 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "LGMonitorController.h"
#include "RGBController_LGMonitor.h"

/*---------------------------------------------------------*\
| vendor ID                                                 |
\*---------------------------------------------------------*/
#define LG_MONITOR_VID                                 0x043E

/*---------------------------------------------------------*\
| Product ID                                                |
\*---------------------------------------------------------*/
#define LG_27GN950_B_PID                               0x9A8A
#define LG_38GL950G_PID                                0x9A57

static void DetectLGMonitorControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LGMonitorController*     controller         = new LGMonitorController(dev, *info, name);
        RGBController_LGMonitor* rgb_controller     = new RGBController_LGMonitor(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("LG 27GN950-B Monitor", DetectLGMonitorControllers, LG_MONITOR_VID, LG_27GN950_B_PID, 1, 0xFF01, 0x01);

// Untested
//REGISTER_HID_DETECTOR("LG 38GL950G Monitor",  DetectLGMonitorControllers, LG_MONITOR_VID, LG_38GL950G_PID);
