/*---------------------------------------------------------*\
| LegoDimensionsToypadBaseControllerDetect.cpp              |
|                                                           |
|   Detector for Lego Dimensions Toypad Base                |
|                                                           |
|   Morgan Guimard (morg)                       02 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "LegoDimensionsToypadBaseController.h"
#include "RGBController_LegoDimensionsToypadBase.h"

/*---------------------------------------------------------*\
| Logic3 vendor ID                                          |
\*---------------------------------------------------------*/
#define LOGIC_3_VID                                    0x0E6F

/*---------------------------------------------------------*\
| Lego Dimensions Toypad Base product ID                    |
\*---------------------------------------------------------*/
#define LEGO_DIMENSIONS_TOYPAD_BASE_PID                0x0241

void DetectLegoDimensionsToypadBaseControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LegoDimensionsToypadBaseController*     controller      = new LegoDimensionsToypadBaseController(dev, *info, name);
        RGBController_LegoDimensionsToypadBase* rgb_controller  = new RGBController_LegoDimensionsToypadBase(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR("Lego Dimensions Toypad Base", DetectLegoDimensionsToypadBaseControllers, LOGIC_3_VID, LEGO_DIMENSIONS_TOYPAD_BASE_PID);
