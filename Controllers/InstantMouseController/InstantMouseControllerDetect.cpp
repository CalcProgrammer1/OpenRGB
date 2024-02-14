#include "Detector.h"
#include "InstantMouseController.h"
#include "RGBController.h"
#include "RGBController_InstantMouse.h"

#define INSTANT_MICROELECTRONICS_VID            0x30FA
#define ADVANCED_GTA_250_PID                    0x1030
#define ANKO_KM43243952_VID                     0x1440
#define ANKO_KM43277483_VID                     0x1540

void DetectInstantMouseControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        InstantMouseController*     controller         = new InstantMouseController(dev, *info);
        RGBController_InstantMouse* rgb_controller     = new RGBController_InstantMouse(controller);
        rgb_controller->name                           = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Advanced GTA 250 USB Gaming Mouse",  DetectInstantMouseControllers,  INSTANT_MICROELECTRONICS_VID, ADVANCED_GTA_250_PID, 1, 0xFF01, 0x01);
REGISTER_HID_DETECTOR_IPU("Anko KM43243952 USB Gaming Mouse",   DetectInstantMouseControllers,  INSTANT_MICROELECTRONICS_VID, ANKO_KM43243952_VID,  1, 0xFF01, 0x01);
REGISTER_HID_DETECTOR_IPU("Anko KM43277483 USB Gaming Mouse",   DetectInstantMouseControllers,  INSTANT_MICROELECTRONICS_VID, ANKO_KM43277483_VID,  1, 0xFF01, 0x01);
