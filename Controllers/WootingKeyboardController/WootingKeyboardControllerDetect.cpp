#include "Detector.h"
#include "WootingOneKeyboardController.h"
#include "WootingTwoKeyboardController.h"
#include "RGBController.h"
#include "RGBController_WootingKeyboard.h"
#include "LogManager.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Wooting vendor ID                                     |
\*-----------------------------------------------------*/
#define WOOTING_OLD_VID                       0x03EB
#define WOOTING_NEW_VID                       0x31E3

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define WOOTING_ONE_PID                       0xFF01
#define WOOTING_TWO_PID                       0xFF02
#define WOOTING_TWO_LE_PID                    0x1210
#define WOOTING_TWO_HE_PID                    0x1220

void DetectWootingOneKeyboardControllers(hid_device_info* info, const std::string& name)
{
    static const char* controller_name = "WootingONE";
    LOG_DEBUG("[%s] Interface %i\tPage %04X\tUsage %i\tPath %s", controller_name, info->interface_number, info->usage_page, info->usage, info->path);

    hid_device* dev = hid_open_path(info->path);
    
    if(dev)
    {
        uint8_t wooting_type = (info->product_id == WOOTING_ONE_PID) ? WOOTING_KB_TKL : WOOTING_KB_FULL;

        LOG_DEBUG("[%s] Device type %i opened - creating Controller", controller_name, wooting_type);
        WootingOneKeyboardController*   controller      = new WootingOneKeyboardController(dev, info->path, wooting_type);

        LOG_DEBUG("[%s] Controller created - creating RGBController", controller_name);
        RGBController_WootingKeyboard*  rgb_controller  = new RGBController_WootingKeyboard(controller);
        rgb_controller->name = name;

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
        LOG_DEBUG("[%s] Device type %i opened - creating Controller", controller_name, WOOTING_KB_FULL);
        WootingTwoKeyboardController*  controller       = new WootingTwoKeyboardController(dev, info->path, WOOTING_KB_FULL);

        LOG_DEBUG("[%s] Controller created - creating RGBController",  controller_name);
        RGBController_WootingKeyboard* rgb_controller   = new RGBController_WootingKeyboard(controller);
        rgb_controller->name = name;

        LOG_DEBUG("[%s] Initialization complete - Registering controller\t%s", controller_name, name.c_str());
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("Wooting ONE Keyboard",     DetectWootingOneKeyboardControllers,  WOOTING_OLD_VID,  WOOTING_ONE_PID,        0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting TWO Keyboard",     DetectWootingOneKeyboardControllers,  WOOTING_OLD_VID,  WOOTING_TWO_PID,        0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting TWO Keyboard LE",  DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_LE_PID,     0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting TWO Keyboard HE",  DetectWootingTwoKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_HE_PID,     0x1337, 1);
