#include "Detector.h"
#include "NvidiaESAController.h"
#include "RGBController.h"
#include "RGBController_NvidiaESA.h"

/*---------------------------------------------------------*\
| NVIDIA ESA vendor ID                                      |
\*---------------------------------------------------------*/
#define NVIDIA_ESA_VID                                 0x0955

/*---------------------------------------------------------*\
| NVIDIA ESA product ID                                     |
\*---------------------------------------------------------*/
#define NVIDIA_ESA_DELL_XPS_730X_PID                   0x000A

void DetectNvidiaESAControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        NvidiaESAController*     controller      = new NvidiaESAController(dev, *info);
        RGBController_NvidiaESA* rgb_controller  = new RGBController_NvidiaESA(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("Nvidia ESA - Dell XPS 730x", DetectNvidiaESAControllers, NVIDIA_ESA_VID, NVIDIA_ESA_DELL_XPS_730X_PID, 0xFFDE, 0x02);
