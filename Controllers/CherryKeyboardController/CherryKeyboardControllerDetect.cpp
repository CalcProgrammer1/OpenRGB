/*---------------------------------------------------------*\
| CherryKeyboardControllerDetect.cpp                        |
|                                                           |
|   Detector for Cherry keyboard                            |
|                                                           |
|   Sebastian Kraus                             25 Dec 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "CherryKeyboardController.h"
#include "RGBController_CherryKeyboard.h"

/*-----------------------------------------------------*\
| Cherry keyboard VID and usage page                    |
\*-----------------------------------------------------*/
#define CHERRY_KEYBOARD_VID                     0x046A
#define CHERRY_KEYBOARD_USAGE_PAGE              0xFF1C

/******************************************************************************************\
*                                                                                          *
*   DetectCherryKeyboards                                                                 *
*                                                                                          *
*       Tests the USB address to see if an Cherry RGB Keyboard controller exists there.   *
*                                                                                          *
\******************************************************************************************/

void DetectCherryKeyboards(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        CherryKeyboardController*     controller     = new CherryKeyboardController(dev, info->path, name);
        RGBController_CherryKeyboard* rgb_controller = new RGBController_CherryKeyboard(controller, info->product_id);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                                                                     |
\*---------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 3.0S FL NBL",        DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_3_0S_FL_NBL_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 3.0S FL RGB",        DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_3_0S_FL_RGB_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 3.0S FL RGB KOREAN", DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_3_0S_FL_RGB_KOR_PID, 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX 1.0 FL BL",                DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_1_0_FL_BL_PID            , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 1.0 TKL RGB",        DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_1_0_TKL_RGB_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 8.0 TKL RGB",        DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_8_0_TKL_RGB_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 10.0 FL RGB",        DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_10_0_FL_RGB_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard G80-3000 TKL NBL",            DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   G80_3000_TKL_NBL_PID        , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 2.0S FL RGB (EU)",   DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_2_0S_FL_RGB_EU_PID , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 2.0S FL RGB (US)",   DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_2_0S_FL_RGB_US_PID , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 2.0S FL NBL",        DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_2_0S_FL_NBL_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard G80-3000 TKL RGB",            DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   G80_3000_TKL_RGB_PID        , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MV BOARD 3.0 FL RGB",         DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MV_BOARD_3_0FL_RGB_PID      , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard CCF MX 8.0 TKL BL",           DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   CCF_MX_8_0_TKL_BL_PID       , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard CCF MX 1.0 TKL BL",           DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   CCF_MX_1_0_TKL_BL_PID       , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard CCF MX 1.0 TKL NBL",          DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   CCF_MX_1_0_TKL_NBL_PID      , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard G80-3000 TKL NBL KOREAN",     DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   G80_3000_TKL_NBL_KOR_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX 1.0 FL NBL",               DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_1_0_FL_NBL_PID           , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX 1.0 FL RGB",               DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_1_0_FL_RGB_PID           , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard G80-3000N TKL RGB (EU)",      DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   G80_3000N_TKL_RGB_EU_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard G80-3000N TKL RGB (US)",      DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   G80_3000N_TKL_RGB_US_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard G80-3000N FL RGB (EU)",       DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   G80_3000N_FL_RGB_EU_PID     , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard G80-3000N FL RGB (US)",       DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   G80_3000N_FL_RGB_US_PID     , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 10.0N FL RGB (EU)",  DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_10_0N_FL_RGB_EU_PID, 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 10.0N FL RGB (US)",  DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_10_0N_FL_RGB_US_PID, 1, CHERRY_KEYBOARD_USAGE_PAGE);
