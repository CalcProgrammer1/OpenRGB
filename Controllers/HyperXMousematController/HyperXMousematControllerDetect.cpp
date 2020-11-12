#include "Detector.h"
#include "HyperXMousematController.h"
#include "RGBController.h"
#include "RGBController_HyperXMousemat.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| HyperX mousemat vendor IDs                            |
\*-----------------------------------------------------*/
#define HYPERX_VID                  0x0951
#define HYPERX_FURY_ULTRA_PID       0x1705

/******************************************************************************************\
*                                                                                          *
*   DetectHyperXMousematControllers                                                        *
*                                                                                          *
*       Tests the USB address to see if a HyperX Mousemat controller exists there.         *
*                                                                                          *
\******************************************************************************************/

void DetectHyperXMousematControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        HyperXMousematController* controller = new HyperXMousematController(dev, info->path);
        RGBController_HyperXMousemat* rgb_controller = new RGBController_HyperXMousemat(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectHyperXMousematControllers() */

REGISTER_HID_DETECTOR_I("HyperX Fury Ultra", DetectHyperXMousematControllers, HYPERX_VID, HYPERX_FURY_ULTRA_PID, 0);
