#include "Detector.h"
#include "HyperXAlloyElite2Controller.h"
#include "HyperXAlloyOriginsController.h"
#include "HyperXAlloyOriginsCoreController.h"
#include "HyperXKeyboardController.h"
#include "RGBController.h"
#include "RGBController_HyperXAlloyElite2.h"
#include "RGBController_HyperXAlloyOrigins.h"
#include "RGBController_HyperXAlloyOriginsCore.h"
#include "RGBController_HyperXKeyboard.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| HyperX keyboard vendor IDs                            |
\*-----------------------------------------------------*/
#define HYPERX_KEYBOARD_VID             0x0951
#define HYPERX_ALLOY_ELITE_PID          0x16BE
#define HYPERX_ALLOY_ELITE_2_PID        0x1711
#define HYPERX_ALLOY_FPS_RGB_PID        0x16DC
#define HYPERX_ALLOY_ORIGINS_PID        0x16E5
#define HYPERX_ALLOY_ORIGINS_CORE_PID   0x16E6

void DetectHyperXKeyboards(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        HyperXKeyboardController* controller = new HyperXKeyboardController(dev, info->path);
        RGBController_HyperXKeyboard* rgb_controller = new RGBController_HyperXKeyboard(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectHyperXAlloyOrigins(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        HyperXAlloyOriginsController* controller = new HyperXAlloyOriginsController(dev, info->path);
        RGBController_HyperXAlloyOrigins* rgb_controller = new RGBController_HyperXAlloyOrigins(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectHyperXAlloyOriginsCore(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        HyperXAlloyOriginsCoreController* controller = new HyperXAlloyOriginsCoreController(dev, info);
        RGBController_HyperXAlloyOriginsCore* rgb_controller = new RGBController_HyperXAlloyOriginsCore(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectHyperXAlloyElite2(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        HyperXAlloyElite2Controller* controller = new HyperXAlloyElite2Controller(dev, info->path);
        RGBController_HyperXAlloyElite2* rgb_controller = new RGBController_HyperXAlloyElite2(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IP("HyperX Alloy Elite RGB",      DetectHyperXKeyboards,        HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ELITE_PID,          2, 0xFF01);
REGISTER_HID_DETECTOR_IP("HyperX Alloy FPS RGB",        DetectHyperXKeyboards,        HYPERX_KEYBOARD_VID, HYPERX_ALLOY_FPS_RGB_PID,        2, 0xFF01);
REGISTER_HID_DETECTOR_I("HyperX Alloy Origins Core",    DetectHyperXAlloyOriginsCore, HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ORIGINS_CORE_PID,   2);

#ifdef _WIN32
REGISTER_HID_DETECTOR_I("HyperX Alloy Origins",         DetectHyperXAlloyOrigins,     HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ORIGINS_PID,        3);
REGISTER_HID_DETECTOR_IP("HyperX Alloy Elite 2",        DetectHyperXAlloyElite2,      HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ELITE_2_PID,        3, 0xFF90);
#else
REGISTER_HID_DETECTOR_I("HyperX Alloy Origins",         DetectHyperXAlloyOrigins,     HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ORIGINS_PID,        0);
REGISTER_HID_DETECTOR_I("HyperX Alloy Elite 2",         DetectHyperXAlloyElite2,      HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ELITE_2_PID,        0);
#endif
