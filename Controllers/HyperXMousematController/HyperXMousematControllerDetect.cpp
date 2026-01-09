/*---------------------------------------------------------*\
| HyperXMousematControllerDetect.cpp                        |
|                                                           |
|   Detector for HyperX mousemat                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "HyperXMousematController.h"
#include "RGBController_HyperXMousemat.h"
#include "hidapi_wrapper.h"

/*-----------------------------------------------------*\
| HyperX mousemat vendor IDs                            |
\*-----------------------------------------------------*/
#define HYPERX_VID                  0x0951
#define HYPERX_FURY_ULTRA_PID       0x1705
#define HYPERX_FURY_A_XL_PID        0x1741

#define HYPERX_VID_2                0x03F0
#define HYPERX_PULSEFIRE_PID        0x0F8D

/******************************************************************************************\
*                                                                                          *
*   DetectHyperXMousematControllers                                                        *
*                                                                                          *
*       Tests the USB address to see if a HyperX Mousemat controller exists there.         *
*                                                                                          *
\******************************************************************************************/

void DetectHyperXMousematControllers(hidapi_wrapper wrapper, hid_device_info* info, const std::string& name)
{
    hid_device* dev = wrapper.hid_open_path(info->path);

    if(dev)
    {
        int first_zone_leds_count   = info->product_id == HYPERX_FURY_A_XL_PID ? 2 : 15;
        int second_zone_leds_count  = info->product_id == HYPERX_FURY_A_XL_PID ? 0 : 5;

        HyperXMousematController*     controller     = new HyperXMousematController(wrapper, dev, info->path, name);
        RGBController_HyperXMousemat* rgb_controller = new RGBController_HyperXMousemat(controller, first_zone_leds_count, second_zone_leds_count);

        DetectionManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectHyperXMousematControllers() */

REGISTER_HID_WRAPPED_DETECTOR_I("HyperX Fury Ultra", DetectHyperXMousematControllers, HYPERX_VID, HYPERX_FURY_ULTRA_PID, 0);
REGISTER_HID_WRAPPED_DETECTOR_IPU("HyperX Pulsefire Mat", DetectHyperXMousematControllers, HYPERX_VID_2, HYPERX_PULSEFIRE_PID, 1, 0xFF90, 0xFF00);

#ifdef _WIN32
REGISTER_HID_WRAPPED_DETECTOR_IPU("HyperX Pulsefire Mat RGB Mouse Pad XL", DetectHyperXMousematControllers, HYPERX_VID, HYPERX_FURY_A_XL_PID, 1, 0xFF90, 0xFF00);
#else
REGISTER_HID_WRAPPED_DETECTOR_IPU("HyperX Pulsefire Mat RGB Mouse Pad XL", DetectHyperXMousematControllers, HYPERX_VID, HYPERX_FURY_A_XL_PID, 0, 0x0C, 0x01);
#endif
