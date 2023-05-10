#include "Detector.h"
#include "AOCKeyboardController.h"
#include "RGBController.h"
#include "RGBController_AOCKeyboard.h"

/*-----------------------------------------------------*\
| AOC Mousemat IDs                                      |
\*-----------------------------------------------------*/
#define AOC_VID                                     0x3938
#define AOC_GK500_PID                               0x1178

/******************************************************************************************\
*                                                                                          *
*   DetectAOCKeyboardControllers                                                           *
*                                                                                          *
*       Tests the USB address to see if an AOC Keyboard controller exists there.           *
*                                                                                          *
\******************************************************************************************/

void DetectAOCKeyboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AOCKeyboardController*     controller     = new AOCKeyboardController(dev, info->path);
        RGBController_AOCKeyboard* rgb_controller = new RGBController_AOCKeyboard(controller);
        rgb_controller->name                      = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("AOC GK500",  DetectAOCKeyboardControllers,   AOC_VID,    AOC_GK500_PID,   0xFF19, 0xFF19);
