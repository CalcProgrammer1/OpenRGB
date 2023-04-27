#include "Detector.h"
#include "AOCMousematController.h"
#include "RGBController.h"
#include "RGBController_AOCMousemat.h"

/*-----------------------------------------------------*\
| AOC Mousemat IDs                                      |
\*-----------------------------------------------------*/
#define AOC_VID                                     0x3938
#define AOC_AMM700_PID                              0x1162

/******************************************************************************************\
*                                                                                          *
*   DetectAOCMousematControllers                                                           *
*                                                                                          *
*       Tests the USB address to see if an AOC Mousemat controller exists there.           *
*                                                                                          *
\******************************************************************************************/

void DetectAOCMousematControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AOCMousematController*     controller     = new AOCMousematController(dev, info->path);
        RGBController_AOCMousemat* rgb_controller = new RGBController_AOCMousemat(controller);
        rgb_controller->name                      = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("AOC AGON AMM700",    DetectAOCMousematControllers,   AOC_VID,    AOC_AMM700_PID, 1,  0xFF19,  0xFF19);