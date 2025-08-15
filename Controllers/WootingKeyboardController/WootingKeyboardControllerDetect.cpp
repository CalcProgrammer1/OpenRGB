/*---------------------------------------------------------*\
| WootingKeyboardControllerDetect.cpp                       |
|                                                           |
|   Detector for Wooting keyboard                           |
|                                                           |
|   Diogo Trindade (diogotr7)                   04 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "WootingOneKeyboardController.h"
#include "WootingTwoKeyboardController.h"
#include "RGBController_WootingKeyboard.h"
#include "LogManager.h"

/*-----------------------------------------------------*\
| Wooting vendor ID                                     |
\*-----------------------------------------------------*/
#define WOOTING_OLD_VID                       0x03EB
#define WOOTING_NEW_VID                       0x31E3

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
| Gamepad Mode: X = Xbox | C = Classic | N = None       |
\*-----------------------------------------------------*/
#define WOOTING_ONE_OLD_PID                   0xFF01
#define WOOTING_TWO_OLD_PID                   0xFF02
#define WOOTING_ONE_X_PID                     0x1100
#define WOOTING_ONE_C_PID                     0x1101
#define WOOTING_ONE_N_PID                     0x1102
#define WOOTING_TWO_X_PID                     0x1200
#define WOOTING_TWO_C_PID                     0x1201
#define WOOTING_TWO_N_PID                     0x1202
#define WOOTING_TWO_LE_X_PID                  0x1210
#define WOOTING_TWO_LE_C_PID                  0x1211
#define WOOTING_TWO_LE_N_PID                  0x1212
#define WOOTING_TWO_HE_X_PID                  0x1220
#define WOOTING_TWO_HE_C_PID                  0x1221
#define WOOTING_TWO_HE_N_PID                  0x1222
#define WOOTING_TWO_HE_ARM_X_PID              0x1230
#define WOOTING_TWO_HE_ARM_C_PID              0x1231
#define WOOTING_TWO_HE_ARM_N_PID              0x1232
#define WOOTING_TWO_60_X_PID                  0x1300
#define WOOTING_TWO_60_C_PID                  0x1301
#define WOOTING_TWO_60_N_PID                  0x1302
#define WOOTING_TWO_60_HE_ARM_X_PID           0x1310
#define WOOTING_TWO_60_HE_ARM_C_PID           0x1311
#define WOOTING_TWO_60_HE_ARM_N_PID           0x1312
#define WOOTING_TWO_UWU_RGB_X_PID             0x1510
#define WOOTING_TWO_UWU_RGB_C_PID             0x1511
#define WOOTING_TWO_UWU_RGB_N_PID             0x1512
#define WOOTING_TWO_80HE_PID                  0x1400

void DetectWootingOneKeyboardControllers(hid_device_info* info, const std::string& name)
{
    static const char* controller_name = "WootingONE";
    LOG_DEBUG("[%s] Interface %i\tPage %04X\tUsage %i\tPath %s", controller_name, info->interface_number, info->usage_page, info->usage, info->path);

    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        // TODO: 60% Keymap for Wooting Two 60HE
        uint8_t wooting_type = (info->product_id == WOOTING_ONE_OLD_PID) ? WOOTING_KB_TKL : WOOTING_KB_FULL;

        LOG_DEBUG("[%s] Device type %i opened - creating Controller", controller_name, wooting_type);
        WootingOneKeyboardController*   controller      = new WootingOneKeyboardController(dev, info->path, wooting_type, name);

        LOG_DEBUG("[%s] Controller created - creating RGBController", controller_name);
        RGBController_WootingKeyboard*  rgb_controller  = new RGBController_WootingKeyboard(controller);

        LOG_DEBUG("[%s] Initialization complete - Registering controller\t%s", controller_name, name.c_str());
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectWootingTwoKeyboardControllers(hid_device_info* info, const std::string& name)
{
    static const char* controller_name = "WootingTWO";
    LOG_DEBUG("[%s] Interface %i\tPage %04X\tUsage %i\tPath %s", controller_name, info->interface_number, info->usage_page, info->usage, info->path);

    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        uint8_t wooting_type;
        switch(info->product_id)
        {
            case WOOTING_TWO_80HE_PID   :       wooting_type = WOOTING_80HE;    break;
            default                     :       wooting_type = WOOTING_KB_FULL; break;
        }

        LOG_DEBUG("[%s] Device type %i opened - creating Controller", controller_name, wooting_type);
        WootingTwoKeyboardController*  controller       = new WootingTwoKeyboardController(dev, info->path, wooting_type, name);

        LOG_DEBUG("[%s] Controller created - creating RGBController",  controller_name);
        RGBController_WootingKeyboard* rgb_controller   = new RGBController_WootingKeyboard(controller);

        LOG_DEBUG("[%s] Initialization complete - Registering controller\t%s", controller_name, name.c_str());
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("Wooting One (Legacy)",             DetectWootingOneKeyboardControllers,  WOOTING_OLD_VID,  WOOTING_ONE_OLD_PID,             0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two (Legacy)",             DetectWootingOneKeyboardControllers,  WOOTING_OLD_VID,  WOOTING_TWO_OLD_PID,             0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting One (Xbox)",               DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_ONE_X_PID,               0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting One (Classic)",            DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_ONE_C_PID,               0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting One (None)",               DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_ONE_N_PID,               0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two (Xbox)",               DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_X_PID,               0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two (Classic)",            DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_C_PID,               0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two (None)",               DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_N_PID,               0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two LE (Xbox)",            DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_LE_X_PID,            0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two LE (Classic)",         DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_LE_C_PID,            0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two LE (None)",            DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_LE_N_PID,            0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two HE (Xbox)",            DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_HE_X_PID,            0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two HE (Classic)",         DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_HE_C_PID,            0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two HE (None)",            DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_HE_N_PID,            0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two HE (ARM) (Xbox)",      DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_HE_ARM_X_PID,        0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two HE (ARM) (Classic)",   DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_HE_ARM_C_PID,        0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two HE (ARM) (None)",      DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_HE_ARM_N_PID,        0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two 60HE (Xbox)",          DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_60_X_PID,            0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two 60HE (Classic)",       DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_60_C_PID,            0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two 60HE (None)",          DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_60_N_PID,            0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two 60HE (ARM) (Xbox)",    DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_60_HE_ARM_X_PID,     0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two 60HE (ARM) (Classic)", DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_60_HE_ARM_C_PID,     0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two 60HE (ARM) (None)",    DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_60_HE_ARM_N_PID,     0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two UwU RGB (Xbox)",       DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_UWU_RGB_X_PID,       0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two UwU RGB (Classic)",    DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_UWU_RGB_C_PID,       0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two UwU RGB (None)",       DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_UWU_RGB_N_PID,       0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting Two 80HE",                 DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_80HE_PID,            0x1337, 1);
