/*---------------------------------------------------------*\
| HyperXMicrophoneV2ControllerDetect.cpp                    |
|                                                           |
|   Detector for HyperX QuadCast 2 S Microphone             |
|                                                           |
|   Morgan Guimard (morg)                                   |
|   Logan Phillips (Eclipse)                    23 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "HyperXMicrophoneV2Controller.h"
#include "RGBController_HyperXMicrophoneV2.h"

/*---------------------------------------------------------*\
| HyperX microphone vendor and product IDs                  |
\*---------------------------------------------------------*/
#define HYPERX_HP_VID           0x03F0
#define HYPERX_QUADCAST_2S_PID  0x02B5

DetectedControllers DetectHyperXMicrophoneV2Controllers(hid_device_info* info, const std::string& name)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = hid_open_path(info->path);

    if(dev)
    {
        HyperXMicrophoneV2Controller* controller         = new HyperXMicrophoneV2Controller(dev, info->path, name);
        RGBController_HyperXMicrophoneV2 *rgb_controller = new RGBController_HyperXMicrophoneV2(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

REGISTER_HID_DETECTOR_IPU("HyperX QuadCast 2 S",    DetectHyperXMicrophoneV2Controllers, HYPERX_HP_VID, HYPERX_QUADCAST_2S_PID, 1, 0xFF13, 0xFF00);
