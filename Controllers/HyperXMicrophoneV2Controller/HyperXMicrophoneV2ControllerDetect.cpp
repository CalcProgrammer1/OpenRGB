/*---------------------------------------------------------*\
| HyperXMicrophoneV2ControllerDetect.cpp                    |
|                                                           |
|   Detector for HyperX QuadCast 2s  microphone             |
|                                                           |
|   Morgan Guimard (morg)                                   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "HyperXMicrophoneV2Controller.h"
#include "RGBController_HyperXMicrophoneV2.h"

/*-----------------------------------------------------*\
| HyperX microphone vendor and product IDs              |
\*-----------------------------------------------------*/
#define HYPERX_HP_VID           0x03F0
#define HYPERX_QUADCAST_2S_PID  0x02B5

void DetectHyperXMicrophoneV2Controllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        HyperXMicrophoneV2Controller* controller         = new HyperXMicrophoneV2Controller(dev, info->path, name);
        RGBController_HyperXMicrophoneV2 *rgb_controller = new RGBController_HyperXMicrophoneV2(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("HyperX QuadCast 2S",    DetectHyperXMicrophoneV2Controllers, HYPERX_HP_VID, HYPERX_QUADCAST_2S_PID, 1, 0xFF13, 0xFF00);
