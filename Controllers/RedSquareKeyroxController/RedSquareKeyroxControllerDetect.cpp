/*---------------------------------------------------------*\
| RedSquareKeyroxControllerDetect.cpp                       |
|                                                           |
|   Detector for Red Square Keyrox                          |
|                                                           |
|   cafeed28                                    03 Nov 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "RedSquareKeyroxController.h"
#include "RedSquareKeyroxTKLClassicController.h"
#include "RGBController_RedSquareKeyrox.h"
#include "RGBController_RedSquareKeyroxTKLClassic.h"

/*-----------------------------------------------------*\
| Red Square vendor ID                                  |
\*-----------------------------------------------------*/
#define RED_SQUARE_VID                             0x1A2C
#define RED_SQUARE_KEYROX_TKL_CLASSIC_VID          0x0416

/*-----------------------------------------------------*\
| Red Square product ID                                 |
\*-----------------------------------------------------*/
#define RED_SQUARE_KEYROX_TKL_PID                  0x1511
#define RED_SQUARE_KEYROX_TKL_V2_PID               0x2511
#define RED_SQUARE_KEYROX_TKL_CLASSIC_PID          0xC345

void DetectRedSquareKeyroxTKL(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RedSquareKeyroxController*     controller     = new RedSquareKeyroxController(dev, *info, KEYROX_VARIANT_TKL, name);
        RGBController_RedSquareKeyrox* rgb_controller = new RGBController_RedSquareKeyrox(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}
void DetectRedSquareKeyroxTKLClassic(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RedSquareKeyroxTKLClassicController*     controller     = new RedSquareKeyroxTKLClassicController(dev, *info, name);
        RGBController_RedSquareKeyroxTKLClassic* rgb_controller = new RGBController_RedSquareKeyroxTKLClassic(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Red Square Keyrox TKL",          DetectRedSquareKeyroxTKL,           RED_SQUARE_VID,                     RED_SQUARE_KEYROX_TKL_PID,          3, 0xFF00, 2);
REGISTER_HID_DETECTOR_IPU("Red Square Keyrox TKL V2",       DetectRedSquareKeyroxTKL,           RED_SQUARE_VID,                     RED_SQUARE_KEYROX_TKL_V2_PID,       3, 0xFF00, 2);
REGISTER_HID_DETECTOR_I(  "Red Square Keyrox TKL Classic",  DetectRedSquareKeyroxTKLClassic,    RED_SQUARE_KEYROX_TKL_CLASSIC_VID,  RED_SQUARE_KEYROX_TKL_CLASSIC_PID,  2);
