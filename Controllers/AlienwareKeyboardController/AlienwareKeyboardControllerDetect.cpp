/*---------------------------------------------------------*\
| AlienwareKeyboardControllerDetect.cpp                     |
|                                                           |
|   Detector for Alienware Keyboard                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "AlienwareAW510KController.h"
#include "AlienwareAW410KController.h"
#include "RGBController_AlienwareAW510K.h"
#include "RGBController_AlienwareAW410K.h"

/*-----------------------------------------------------*\
| Alienware vendor ID                                   |
\*-----------------------------------------------------*/
#define ALIENWARE_VID                           0x04F2

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define ALIENWARE_AW510K_PID                    0x1830
#define ALIENWARE_AW410K_PID                    0x1968

/******************************************************************************************\
*                                                                                          *
*   DetectAlienwareKeyboardControllers                                                     *
*                                                                                          *
*       Tests the USB address to see if a Alienware RGB Keyboard controller exists there.  *
*                                                                                          *
\******************************************************************************************/

void DetectAlienwareAW510KControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        AlienwareAW510KController*     controller     = new AlienwareAW510KController(dev, info->path, name);
        RGBController_AlienwareAW510K* rgb_controller = new RGBController_AlienwareAW510K(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectAlienwareAW410KControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        AlienwareAW410KController*     controller     = new AlienwareAW410KController(dev, info->path, name);
        RGBController_AlienwareAW410K* rgb_controller = new RGBController_AlienwareAW410K(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}/* DetectAlienwareKeyboardControllers() */


REGISTER_HID_DETECTOR_IPU("Alienware AW510K",   DetectAlienwareAW510KControllers,   ALIENWARE_VID,  ALIENWARE_AW510K_PID,   0x02,   0xFF00, 0x01);
REGISTER_HID_DETECTOR_IPU("Alienware AW410K",   DetectAlienwareAW410KControllers,   ALIENWARE_VID,  ALIENWARE_AW410K_PID,   0x02,   0xFF00, 0x01);
