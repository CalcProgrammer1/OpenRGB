/*---------------------------------------------------------------*\
| GaiZhongGaiKeyboardControllerDetect.cpp                         |
|                                                                 |
| https://oshwlab.com/yangdsada/GaiZhongGai-Keyboard-68-4PRO      |
|                                                                 |
| An Yang     2022/6/12                                           |
\*---------------------------------------------------------------*/

#include "Detector.h"
#include "GaiZhongGaiKeyboardController.h"
#include "RGBController.h"
#include "RGBController_GaiZhongGaiKeyboard.h"
#include <hidapi/hidapi.h>

/******************************************************************************************\
*                                                                                          *
*   DetectGaiZhongGaiKeyboardControllers                                                   *
*                                                                                          *
*       Tests the USB address to see if a GaiZhongGai RGB Keyboard controller exists there.*
*                                                                                          *
\******************************************************************************************/

void DetectGaiZhongGaiKeyboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        GaiZhongGaiKeyboardController* controller           = new GaiZhongGaiKeyboardController(dev, info);
        RGBController_GaiZhongGaiKeyboard* rgb_controller   = new RGBController_GaiZhongGaiKeyboard(controller);
        rgb_controller->name                                = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectGaiZhongGaiKeyboardControllers() */

REGISTER_HID_DETECTOR_I("GaiZhongGai 68+4 PRO",         DetectGaiZhongGaiKeyboardControllers, GAIZHONGGAI_VID, GAIZHONGGAI_68_PRO_PID,          3);
REGISTER_HID_DETECTOR_I("GaiZhongGai 17+4+Touch PRO",   DetectGaiZhongGaiKeyboardControllers, GAIZHONGGAI_VID, GAIZHONGGAI_17_TOUCH_PRO_PID,    3);
REGISTER_HID_DETECTOR_I("GaiZhongGai 17 PRO",           DetectGaiZhongGaiKeyboardControllers, GAIZHONGGAI_VID, GAIZHONGGAI_17_PRO_PID,          3);
REGISTER_HID_DETECTOR_I("GaiZhongGai 20 PRO",           DetectGaiZhongGaiKeyboardControllers, GAIZHONGGAI_VID, GAIZHONGGAI_20_PRO_PID,          3);
