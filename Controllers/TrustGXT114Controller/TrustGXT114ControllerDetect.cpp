#include "Detector.h"
#include "TrustGXT114Controller.h"
#include "RGBController.h"
#include "RGBController_TrustGXT114.h"

/*---------------------------------------------------------*\
| Trust vendor ID                                           |
\*---------------------------------------------------------*/
#define TRUST_VID                                      0x145F

/*---------------------------------------------------------*\
| Product ID                                                |
\*---------------------------------------------------------*/
#define TRUST_GXT_114_PID                              0x026D

void DetectTrustGXT114ControllerControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        TrustGXT114Controller* controller               = new TrustGXT114Controller(dev, *info);

        if(controller->Test())
        {
            RGBController_TrustGXT114* rgb_controller   = new RGBController_TrustGXT114(controller);
            rgb_controller->name = name;
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            delete controller;
        }
    }
}

REGISTER_HID_DETECTOR_IPU("Trust GXT 114", DetectTrustGXT114ControllerControllers, TRUST_VID, TRUST_GXT_114_PID, 1, 0xFF00, 1);
