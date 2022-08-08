#include "Detector.h"
#include "AsusStrixClawController.h"
#include "RGBController.h"
#include "RGBController_AsusStrixClaw.h"
#include <stdexcept>
#include <hidapi/hidapi.h>
#include "dependencies/dmiinfo.h"

#define ASUS_LEGACY_USB_VID         0x195D

#define ASUS_ROG_STRIX_CLAW_PID     0x1016

void DetectAsusStrixClaw(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        StrixClawController*     controller          = new StrixClawController(dev, info->path);
        RGBController_StrixClaw* rgb_controller      = new RGBController_StrixClaw(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("ASUS ROG Strix Claw", DetectAsusStrixClaw,  ASUS_LEGACY_USB_VID, ASUS_ROG_STRIX_CLAW_PID, 0, 0xFF01, 1);
