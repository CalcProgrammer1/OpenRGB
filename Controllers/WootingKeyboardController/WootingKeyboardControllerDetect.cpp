/*---------------------------------------------------------*\
| WootingKeyboardControllerDetect.cpp                       |
|                                                           |
|   Detector for Wooting keyboard                           |
|                                                           |
|   Diogo Trindade (diogotr7)                   25 Dec 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "WootingV1KeyboardController.h"
#include "WootingV2KeyboardController.h"
#include "WootingV3KeyboardController.h"
#include "RGBController_WootingKeyboard.h"
#include "LogManager.h"

#define WOOTING_CONFIG_USAGE_PAGE_V2          0x1337
#define WOOTING_CONFIG_USAGE_PAGE_V3          0xFF55

/*-----------------------------------------------------*\
| Wooting vendor ID                                     |
\*-----------------------------------------------------*/
#define WOOTING_OLD_VID                       0x03EB
#define WOOTING_NEW_VID                       0x31E3

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define WOOTING_ONE_LEGACY_PID                0xFF01
#define WOOTING_TWO_LEGACY_PID                0xFF02
#define WOOTING_ONE_PID                       0x1100
#define WOOTING_TWO_PID                       0x1200
#define WOOTING_TWO_LE_PID                    0x1210
#define WOOTING_TWO_HE_PID                    0x1220
#define WOOTING_TWO_HE_ARM_PID                0x1230
#define WOOTING_60HE_PID                      0x1300
#define WOOTING_60HE_ARM_PID                  0x1310
#define WOOTING_60HE_PLUS_PID                 0x1320
#define WOOTING_60HE_V2_PID                   0x1340
#define WOOTING_80HE_PID                      0x1400
#define WOOTING_UWU_RGB_PID                   0x1510

/*-----------------------------------------------------*\
| Product ID helpers                                    |
| XINP: Xbox input emulation enabled                    |
| DINP: Classic DirectInput emulation enabled           |
| NONE: Controller emulation disabled                   |
\*-----------------------------------------------------*/
#define XINP_PID(pid) (pid | 0x0000)
#define DINP_PID(pid) (pid | 0x0001)
#define NONE_PID(pid) (pid | 0x0002)

void DetectWootingControllers(hid_device_info *info, const std::string &name)
{
    static const char *controller_name = "Wooting";
    LOG_DEBUG("[%s] Interface %i\tPage %04X\tUsage %i\tPath %s", controller_name, info->interface_number, info->usage_page, info->usage, info->path);

    hid_device *dev = hid_open_path(info->path);

    if(!dev)
        return;

    WOOTING_DEVICE_TYPE wooting_type;
    uint16_t pid = info->product_id;

    if(pid == WOOTING_ONE_LEGACY_PID)
    {
        wooting_type = WOOTING_KB_TKL;
    }
    else if(pid == WOOTING_TWO_LEGACY_PID)
    {
        wooting_type = WOOTING_KB_FULL;
    }
    else
    {
        //on modern devices, mask out the last nibble to get base PID
        switch(pid & 0xFFF0)
        {
            case WOOTING_ONE_LEGACY_PID:
            case WOOTING_ONE_PID:
                wooting_type = WOOTING_KB_TKL;
                break;
            case WOOTING_TWO_LEGACY_PID:
            case WOOTING_TWO_PID:
            case WOOTING_TWO_LE_PID:
            case WOOTING_TWO_HE_PID:
            case WOOTING_TWO_HE_ARM_PID:
                wooting_type = WOOTING_KB_FULL;
                break;
            case WOOTING_60HE_PID:
            case WOOTING_60HE_ARM_PID:
            case WOOTING_60HE_PLUS_PID:
            case WOOTING_60HE_V2_PID:
                wooting_type = WOOTING_KB_60PER;
                break;
            case WOOTING_80HE_PID:
                wooting_type = WOOTING_KB_80PER;
                break;
            case WOOTING_UWU_RGB_PID:
                wooting_type = WOOTING_KB_3PAD;
                break;
            default:
                //default to largest keyboard if unknown
                wooting_type = WOOTING_KB_FULL;
                break;
        }
    }

    LOG_INFO("[%s] Detected Wooting device type %i for device at path %s", controller_name, wooting_type, info->path);

    //V1 firmware used the ATMEL VID, and uses the V1 controller
    if(info->vendor_id == WOOTING_OLD_VID && info->usage_page == WOOTING_CONFIG_USAGE_PAGE_V2)
    {
        LOG_DEBUG("[%s] Old VID detected - creating V1 Controller", controller_name);
        WootingV1KeyboardController *controller = new WootingV1KeyboardController(dev, info->path, wooting_type, name);

        LOG_DEBUG("[%s] Controller created - creating RGBController", controller_name);
        RGBController_WootingKeyboard *rgb_controller = new RGBController_WootingKeyboard(controller);

        LOG_DEBUG("[%s] Initialization complete - Registering controller\t%s", controller_name, name.c_str());
        ResourceManager::get()->RegisterRGBController(rgb_controller);
        return;
    }

    //V2-V2.11 firmware uses the V2 protocol indicated by the V2 usage page
    if(info->usage_page == WOOTING_CONFIG_USAGE_PAGE_V2)
    {
        LOG_DEBUG("[%s] V2 usage page detected - creating V2 Controller", controller_name);
        WootingV2KeyboardController *controller = new WootingV2KeyboardController(dev, info->path, wooting_type, name);

        LOG_DEBUG("[%s] Controller created - creating RGBController", controller_name);
        RGBController_WootingKeyboard *rgb_controller = new RGBController_WootingKeyboard(controller);

        LOG_DEBUG("[%s] Initialization complete - Registering controller\t%s", controller_name, name.c_str());
        ResourceManager::get()->RegisterRGBController(rgb_controller);
        return;
    }

    //V2.12+ firmware uses the new report structure indicated by the V3 usage page
    if(info->usage_page == WOOTING_CONFIG_USAGE_PAGE_V3)
    {
        LOG_DEBUG("[%s] V3 usage page detected - creating V3 Controller", controller_name);
        WootingV3KeyboardController *controller = new WootingV3KeyboardController(dev, info->path, wooting_type, name);

        LOG_DEBUG("[%s] Controller created - creating RGBController", controller_name);
        RGBController_WootingKeyboard *rgb_controller = new RGBController_WootingKeyboard(controller);

        LOG_DEBUG("[%s] Initialization complete - Registering controller\t%s", controller_name, name.c_str());
        ResourceManager::get()->RegisterRGBController(rgb_controller);
        return;
    }

    hid_close(dev);

    LOG_TRACE("[%s] No compatible Wooting controller found for device at path %s", controller_name, info->path);
}
/*-----------------------------------------------------*\
| Wooting keyboards use different PIDs based on which   |
| gamepad emulation mode is selected. We can use their  |
| base PID, and set the last nibble to get the modes.   |
\*-----------------------------------------------------*/
#define REGISTER_WOOTING_DETECTOR(name, vid, pid)                                                                                                                               \
    static HIDDeviceDetector detector_wooting_##vid##_##pid##_base(name, DetectWootingControllers, vid, NONE_PID(pid), HID_INTERFACE_ANY, HID_USAGE_PAGE_ANY, HID_USAGE_ANY);    \
    static HIDDeviceDetector detector_wooting_##vid##_##pid##_xinp(name, DetectWootingControllers, vid, XINP_PID(pid), HID_INTERFACE_ANY, HID_USAGE_PAGE_ANY, HID_USAGE_ANY);    \
    static HIDDeviceDetector detector_wooting_##vid##_##pid##_dinp(name, DetectWootingControllers, vid, DINP_PID(pid), HID_INTERFACE_ANY, HID_USAGE_PAGE_ANY, HID_USAGE_ANY)


// Legacy devices with V1 firmware
REGISTER_HID_DETECTOR_P("Wooting One (Legacy)", DetectWootingControllers, WOOTING_OLD_VID, WOOTING_ONE_LEGACY_PID, WOOTING_CONFIG_USAGE_PAGE_V2);
REGISTER_HID_DETECTOR_P("Wooting Two (Legacy)", DetectWootingControllers, WOOTING_OLD_VID, WOOTING_TWO_LEGACY_PID, WOOTING_CONFIG_USAGE_PAGE_V2);

// All other devices
REGISTER_WOOTING_DETECTOR("Wooting One",                WOOTING_NEW_VID, WOOTING_ONE_PID       );
REGISTER_WOOTING_DETECTOR("Wooting Two",                WOOTING_NEW_VID, WOOTING_TWO_PID       );
REGISTER_WOOTING_DETECTOR("Wooting Two Lekker Edition", WOOTING_NEW_VID, WOOTING_TWO_LE_PID    );
REGISTER_WOOTING_DETECTOR("Wooting Two HE",             WOOTING_NEW_VID, WOOTING_TWO_HE_PID    );
REGISTER_WOOTING_DETECTOR("Wooting Two HE (ARM)",       WOOTING_NEW_VID, WOOTING_TWO_HE_ARM_PID);
REGISTER_WOOTING_DETECTOR("Wooting 60HE",               WOOTING_NEW_VID, WOOTING_60HE_PID      );
REGISTER_WOOTING_DETECTOR("Wooting 60HE (ARM)",         WOOTING_NEW_VID, WOOTING_60HE_ARM_PID  );
REGISTER_WOOTING_DETECTOR("Wooting 60HE+",              WOOTING_NEW_VID, WOOTING_60HE_PLUS_PID );
REGISTER_WOOTING_DETECTOR("Wooting 60HEv2",             WOOTING_NEW_VID, WOOTING_60HE_V2_PID   );
REGISTER_WOOTING_DETECTOR("Wooting 80HE",               WOOTING_NEW_VID, WOOTING_80HE_PID      );
REGISTER_WOOTING_DETECTOR("Wooting UwU RGB",            WOOTING_NEW_VID, WOOTING_UWU_RGB_PID   );
