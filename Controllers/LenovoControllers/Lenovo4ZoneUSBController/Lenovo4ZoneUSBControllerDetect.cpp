/*---------------------------------------------------------*\
| Lenovo4ZoneUSBControllerDetect.cpp                        |
|                                                           |
|   Detector for Lenovo 4-Zone devices                      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "Lenovo4ZoneUSBController.h"
#include "LenovoDevices4Zone.h"
#include "RGBController_Lenovo4ZoneUSB.h"

/*-----------------------------------------------------*\
| vendor IDs                                            |
\*-----------------------------------------------------*/
#define ITE_VID                                 0x048D

/*-----------------------------------------------------*\
| Interface, Usage, and Usage Page                      |
\*-----------------------------------------------------*/
enum
{
    LENOVO_PAGE  = 0xFF89,
    LENOVO_USAGE = 0xCC
};

void DetectLenovo4ZoneUSBControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        Lenovo4ZoneUSBController*     controller      = new Lenovo4ZoneUSBController(dev, info->path, info->product_id, name);
        RGBController_Lenovo4ZoneUSB* rgb_controller  = new RGBController_Lenovo4ZoneUSB(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("Lenovo Ideapad 3-15ach6", DetectLenovo4ZoneUSBControllers, ITE_VID, IDEAPAD_315ACH6,              LENOVO_PAGE, LENOVO_USAGE);
REGISTER_HID_DETECTOR_PU("Lenovo 5 2023",           DetectLenovo4ZoneUSBControllers, ITE_VID, LEGION_5_2023_PID,            LENOVO_PAGE, LENOVO_USAGE);
REGISTER_HID_DETECTOR_PU("Lenovo 5 2023 Ideapad",   DetectLenovo4ZoneUSBControllers, ITE_VID, LEGION_5_2023_IDEAPAD_PID,    LENOVO_PAGE, LENOVO_USAGE);
REGISTER_HID_DETECTOR_PU("Lenovo 5 2022",           DetectLenovo4ZoneUSBControllers, ITE_VID, LEGION_5_2022_PID,            LENOVO_PAGE, LENOVO_USAGE);
REGISTER_HID_DETECTOR_PU("Lenovo 5 2022 Ideapad",   DetectLenovo4ZoneUSBControllers, ITE_VID, LEGION_5_2022_IDEAPAD_PID,    LENOVO_PAGE, LENOVO_USAGE);
REGISTER_HID_DETECTOR_PU("Lenovo 5 2021",           DetectLenovo4ZoneUSBControllers, ITE_VID, LEGION_5_2021_PID,            LENOVO_PAGE, LENOVO_USAGE);
REGISTER_HID_DETECTOR_PU("Lenovo 5 2021 Ideapad",   DetectLenovo4ZoneUSBControllers, ITE_VID, LEGION_5_2021_IDEAPAD_PID,    LENOVO_PAGE, LENOVO_USAGE);
REGISTER_HID_DETECTOR_PU("Lenovo 5 2020",           DetectLenovo4ZoneUSBControllers, ITE_VID, LEGION_5_2020_PID,            LENOVO_PAGE, LENOVO_USAGE);
