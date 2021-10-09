#include "Detector.h"
#include "HyperXAlloyEliteController.h"
#include "HyperXAlloyElite2Controller.h"
#include "HyperXAlloyFPSController.h"
#include "HyperXAlloyOriginsController.h"
#include "HyperXAlloyOriginsCoreController.h"
#include "RGBController.h"
#include "RGBController_HyperXAlloyElite.h"
#include "RGBController_HyperXAlloyElite2.h"
#include "RGBController_HyperXAlloyFPS.h"
#include "RGBController_HyperXAlloyOrigins.h"
#include "RGBController_HyperXAlloyOriginsCore.h"
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

#define HP_KEYBOARD_VID                 0x03F0
#define HYPERX_ALLOY_ELITE_2_HP_PID     0x058F

void DetectHyperXAlloyElite(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        HyperXAlloyEliteController*     controller     = new HyperXAlloyEliteController(dev, info->path);
        RGBController_HyperXAlloyElite* rgb_controller = new RGBController_HyperXAlloyElite(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectHyperXAlloyElite2(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        HyperXAlloyElite2Controller*     controller     = new HyperXAlloyElite2Controller(dev, info->path);
        RGBController_HyperXAlloyElite2* rgb_controller = new RGBController_HyperXAlloyElite2(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectHyperXAlloyFPS(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        HyperXAlloyFPSController* controller = new HyperXAlloyFPSController(dev, info->path);
        RGBController_HyperXAlloyFPS* rgb_controller = new RGBController_HyperXAlloyFPS(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectHyperXAlloyOrigins(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    
    if(dev)
    {
        HyperXAlloyOriginsController*     controller     = new HyperXAlloyOriginsController(dev, info->path);
        RGBController_HyperXAlloyOrigins* rgb_controller = new RGBController_HyperXAlloyOrigins(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectHyperXAlloyOriginsCore(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        HyperXAlloyOriginsCoreController*     controller     = new HyperXAlloyOriginsCoreController(dev, info);
        RGBController_HyperXAlloyOriginsCore* rgb_controller = new RGBController_HyperXAlloyOriginsCore(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IP("HyperX Alloy Elite RGB",      DetectHyperXAlloyElite,       HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ELITE_PID,          2, 0xFF01);
REGISTER_HID_DETECTOR_IP("HyperX Alloy FPS RGB",        DetectHyperXAlloyFPS,         HYPERX_KEYBOARD_VID, HYPERX_ALLOY_FPS_RGB_PID,        2, 0xFF01);
REGISTER_HID_DETECTOR_I("HyperX Alloy Origins Core",    DetectHyperXAlloyOriginsCore, HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ORIGINS_CORE_PID,   2);

#ifdef _WIN32
REGISTER_HID_DETECTOR_I("HyperX Alloy Origins",         DetectHyperXAlloyOrigins,     HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ORIGINS_PID,        3);
REGISTER_HID_DETECTOR_IP("HyperX Alloy Elite 2",        DetectHyperXAlloyElite2,      HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ELITE_2_PID,        3, 0xFF90);
REGISTER_HID_DETECTOR_IP("HyperX Alloy Elite 2 (HP)",   DetectHyperXAlloyElite2,      HP_KEYBOARD_VID,     HYPERX_ALLOY_ELITE_2_HP_PID,     3, 0xFF90);
#else
REGISTER_HID_DETECTOR_I("HyperX Alloy Origins",         DetectHyperXAlloyOrigins,     HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ORIGINS_PID,        0);
REGISTER_HID_DETECTOR_I("HyperX Alloy Elite 2",         DetectHyperXAlloyElite2,      HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ELITE_2_PID,        0);
REGISTER_HID_DETECTOR_I("HyperX Alloy Elite 2 (HP)",    DetectHyperXAlloyElite2,      HP_KEYBOARD_VID,     HYPERX_ALLOY_ELITE_2_HP_PID,     0);
#endif
