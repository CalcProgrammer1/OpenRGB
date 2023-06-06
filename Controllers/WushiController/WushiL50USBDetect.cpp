

#include "Detector.h"
#include "LogManager.h"
#include "RGBController.h"
#include "WushiL50USBController.h"
#include "WushiDevicesL50.h"
#include "RGBController_WushiL50USB.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| vendor IDs                                            |
\*-----------------------------------------------------*/
#define WUSHI_VID                                 0x306F

/*-----------------------------------------------------*\
| Interface, Usage, and Usage Page                      |
\*-----------------------------------------------------*/
enum
{
    WUSHI_PAGE  = 0xFF63,
    WUSHI_USAGE = 0x0C
};

void DetectWushiL50USBControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)  //WushiL50USBController
    {
        WushiL50USBController*     controller      = new WushiL50USBController(dev, info->path, info->product_id);
        RGBController_WushiL50USB* rgb_controller  = new RGBController_WushiL50USB(controller);
        rgb_controller->name                     = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("JSAUX RGB Docking Station", DetectWushiL50USBControllers, WUSHI_VID, WU_S80, WUSHI_PAGE, WUSHI_USAGE);
