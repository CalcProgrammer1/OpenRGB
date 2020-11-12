#include "Detector.h"
#include "HoltekA070Controller.h"
#include "RGBController.h"
#include "RGBController_HoltekA070.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Holtek Semiconductor Inc. vendor ID                   |
\*-----------------------------------------------------*/
#define HOLTEK_VID                    0x04D9
/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define HOLTEK_A070_PID               0xA070

void DetectHoltekControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        HoltekA070Controller* controller = new HoltekA070Controller(dev, info->path);
        RGBController_HoltekA070* rgb_controller = new RGBController_HoltekA070(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectHoltekControllers() */

REGISTER_HID_DETECTOR_IPU("Holtek USB Gaming Mouse", DetectHoltekControllers, HOLTEK_VID, HOLTEK_A070_PID, 1, 0xFF00, 2);
