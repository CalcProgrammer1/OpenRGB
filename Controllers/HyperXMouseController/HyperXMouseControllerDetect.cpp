#include "Detector.h"
#include "HyperXPulsefireSurgeController.h"
#include "RGBController.h"
#include "RGBController_HyperXPulsefireSurge.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| HyperX mouse vendor IDs                               |
\*-----------------------------------------------------*/
#define HYPERX_VID                  0x0951
#define HYPERX_PULSEFIRE_SURGE_PID  0x16D3

void DetectHyperXMouseControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        HyperXPulsefireSurgeController* controller = new HyperXPulsefireSurgeController(dev, info->path);
        RGBController_HyperXPulsefireSurge* rgb_controller = new RGBController_HyperXPulsefireSurge(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectHyperXMouseControllers() */

REGISTER_HID_DETECTOR_IP("HyperX Pulsefire Surge", DetectHyperXMouseControllers, HYPERX_VID, HYPERX_PULSEFIRE_SURGE_PID, 1, 0xFF01);
