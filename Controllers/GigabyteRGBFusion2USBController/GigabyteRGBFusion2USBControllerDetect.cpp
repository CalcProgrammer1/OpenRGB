/*---------------------------------------------------------*\
| GigabyteRGBFusion2USBControllerDetect.cpp                 |
|                                                           |
|   Detector for Gigabyte Aorus RGB Fusion 2 USB            |
|   motherboard                                             |
|                                                           |
|   jackun                                      08 Jan 2020 |
|   megadjc                                     31 Jul 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "dmiinfo.h"
#include "GigabyteRGBFusion2USBController.h"
#include "RGBController_GigabyteRGBFusion2USB.h"

#define DETECTOR_NAME "Gigabyte RGB Fusion 2 USB"

#define IT8297_VID              0x048D
#define IT8297_IFC              0
#define IT8297_U                0xCC
#define IT8297_UPG              0xFF89

/*---------------------------------------------------------*\
| Detector for Gigabyte RGB Fusion USB controllers          |
\*---------------------------------------------------------*/
DetectedControllers DetectGigabyteRGBFusion2USBControllers(hid_device_info* info, const std::string&)
{
    DetectedControllers detected_controllers;
    DMIInfo             MB_info;
    hid_device*         dev = hid_open_path(info->path);

    if(dev)
    {
        RGBFusion2USBController*     controller     = new RGBFusion2USBController(dev, info->path, MB_info.getMainboard(), info->product_id);
        RGBController_RGBFusion2USB* rgb_controller = new RGBController_RGBFusion2USB(controller, DETECTOR_NAME);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_PU(DETECTOR_NAME, DetectGigabyteRGBFusion2USBControllers, IT8297_VID, 0x8297, IT8297_UPG, IT8297_U);
REGISTER_HID_DETECTOR_PU(DETECTOR_NAME, DetectGigabyteRGBFusion2USBControllers, IT8297_VID, 0x8950, IT8297_UPG, IT8297_U);
REGISTER_HID_DETECTOR_PU(DETECTOR_NAME, DetectGigabyteRGBFusion2USBControllers, IT8297_VID, 0x5702, IT8297_UPG, IT8297_U);
REGISTER_HID_DETECTOR_PU(DETECTOR_NAME, DetectGigabyteRGBFusion2USBControllers, IT8297_VID, 0x5711, IT8297_UPG, IT8297_U);
