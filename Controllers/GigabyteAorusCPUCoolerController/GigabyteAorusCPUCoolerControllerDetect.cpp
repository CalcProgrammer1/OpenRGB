#include "Detector.h"
#include "RGBController.h"
#include "ATC800Controller.h"
#include "RGBController_AorusATC800.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Vendor ID                                             |
\*-----------------------------------------------------*/
#define HOLTEK_VID                            0x1044

/*-----------------------------------------------------*\
| Controller product ids                                |
\*-----------------------------------------------------*/
#define ATC_800_CONTROLLER_PID                0x7A42

/******************************************************************************************\
*                                                                                          *
*   DetectAorusCPUCoolerControllers                                                        *
*                                                                                          *
*       Tests the USB address to see if a Aorus RGB CPU Cooler exists there.               *
*                                                                                          *
\******************************************************************************************/

void DetectGigabyteAorusCPUCoolerControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        ATC800Controller*          controller     = new ATC800Controller(dev, info->path);
        RGBController_AorusATC800* rgb_controller = new RGBController_AorusATC800(controller);
        rgb_controller->name                      = name;
        
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Aorus CPU Coolers", DetectGigabyteAorusCPUCoolerControllers, HOLTEK_VID, ATC_800_CONTROLLER_PID, 0, 0xFF01, 1);
