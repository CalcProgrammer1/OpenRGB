#include "Detector.h"
#include "HyperXPulsefireFPSProController.h"
#include "HyperXPulsefireSurgeController.h"
#include "HyperXPulsefireDartController.h"
#include "RGBController.h"
#include "RGBController_HyperXPulsefireFPSPro.h"
#include "RGBController_HyperXPulsefireSurge.h"
#include "RGBController_HyperXPulsefireDart.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| HyperX mouse vendor IDs                               |
\*-----------------------------------------------------*/
#define HYPERX_VID                          0x0951
#define HYPERX_PULSEFIRE_SURGE_PID          0x16D3
#define HYPERX_PULSEFIRE_FPS_PRO_PID        0x16D7
#define HYPERX_PULSEFIRE_DART_WIRELESS_PID  0x16E1
#define HYPERX_PULSEFIRE_DART_WIRED_PID     0x16E2

void DetectHyperXPulsefireSurgeControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        HyperXPulsefireSurgeController* controller = new HyperXPulsefireSurgeController(dev, info->path);
        RGBController_HyperXPulsefireSurge* rgb_controller = new RGBController_HyperXPulsefireSurge(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectHyperXPulsefireSurgeControllers() */

void DetectHyperXPulsefireFPSProControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        HyperXPulsefireFPSProController* controller = new HyperXPulsefireFPSProController(dev, info->path);
        RGBController_HyperXPulsefireFPSPro* rgb_controller = new RGBController_HyperXPulsefireFPSPro(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectHyperXPulsefireFPSProControllers() */

void DetectHyperXPulsefireDartControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        HyperXPulsefireDartController* controller = new HyperXPulsefireDartController(dev, info->path);
        RGBController_HyperXPulsefireDart* rgb_controller = new RGBController_HyperXPulsefireDart(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectHyperXPulsefireDartControllers() */

REGISTER_HID_DETECTOR_IP("HyperX Pulsefire Surge",   DetectHyperXPulsefireSurgeControllers,  HYPERX_VID, HYPERX_PULSEFIRE_SURGE_PID,   1, 0xFF01);
REGISTER_HID_DETECTOR_IP("HyperX Pulsefire FPS Pro", DetectHyperXPulsefireFPSProControllers, HYPERX_VID, HYPERX_PULSEFIRE_FPS_PRO_PID, 1, 0xFF01);
REGISTER_HID_DETECTOR_IP("HyperX Pulsefire Dart (Wireless)", DetectHyperXPulsefireDartControllers, HYPERX_VID, HYPERX_PULSEFIRE_DART_WIRELESS_PID, 2, 0xFF00);
REGISTER_HID_DETECTOR_IP("HyperX Pulsefire Dart (Wired)", DetectHyperXPulsefireDartControllers, HYPERX_VID, HYPERX_PULSEFIRE_DART_WIRED_PID, 1, 0xFF00);
