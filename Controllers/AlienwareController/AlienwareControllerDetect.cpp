#include "Detector.h"
#include "AlienwareController.h"
#include "RGBController.h"
#include "RGBController_Alienware.h"

/*---------------------------------------------------------*\
| Alienware vendor ID                                       |
\*---------------------------------------------------------*/
#define ALIENWARE_VID                               0x187C

/*---------------------------------------------------------*\
| Alienware product ID                                      |
\*---------------------------------------------------------*/
#define ALIENWARE_G_SERIES_PID                         0x0550

void DetectAlienwareControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AlienwareController*     controller     = new AlienwareController(dev, *info, name);
        RGBController_Alienware* rgb_controller = new RGBController_Alienware(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR("Dell G Series LED Controller", DetectAlienwareControllers, ALIENWARE_VID, ALIENWARE_G_SERIES_PID);
