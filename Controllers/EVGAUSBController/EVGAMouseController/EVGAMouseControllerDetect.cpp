/*---------------------------------------------------------*\
| EVGAMouseControllerDetect.cpp                             |
|                                                           |
|   Detector for EVGA mouse devices                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "DetectionManager.h"
#include "RGBController_EVGAMouse.h"

/*---------------------------------------------------------*\
| EVGA USB vendor ID                                        |
\*---------------------------------------------------------*/
#define EVGA_USB_VID                                0x3842

/*---------------------------------------------------------*\
| Mouse product IDs                                         |
\*---------------------------------------------------------*/
#define X20_WIRED_PID                               0x2420
#define X20_WIRELESS_ADAPTER_PID                    0x2402

DetectedControllers DetectEVGAMouse(hid_device_info* info, const std::string &name, int connection_type)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        EVGAMouseController*     controller     = new EVGAMouseController(dev, info->path, connection_type, name);
        RGBController_EVGAMouse* rgb_controller = new RGBController_EVGAMouse(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectWiredEVGAMouse(hid_device_info* info, const std::string &name)
{
    return(DetectEVGAMouse(info, name, EVGA_PERIPHERAL_CONNECTION_TYPE_WIRED));
}

DetectedControllers DetectWirelessEVGAMouse(hid_device_info* info, const std::string &name)
{
    return(DetectEVGAMouse(info, name, EVGA_PERIPHERAL_CONNECTION_TYPE_WIRELESS));
}

REGISTER_HID_DETECTOR_IPU("EVGA X20 Gaming Mouse",  DetectWiredEVGAMouse,           EVGA_USB_VID,   X20_WIRED_PID,              2,  0xFFFF, 0);
REGISTER_HID_DETECTOR_IPU("EVGA X20 USB Receiver",  DetectWirelessEVGAMouse,        EVGA_USB_VID,   X20_WIRELESS_ADAPTER_PID,   2,  0xFFFF, 0);
