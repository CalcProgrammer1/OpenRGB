#include "Detector.h"
#include "OKSKeyboardController.h"
#include "RGBController.h"
#include "RGBController_OKSKeyboard.h"
#include <hidapi/hidapi.h>
/******************************************************************************************\
*                                                                                          *
*  DetectOKSKeyboardControllers                                                            *
*  Reference: DuckyKeyboardController                                                      *
*  Tests the USB address to see if a OKS Optical Axis RGB Keyboard controller exists there.*
*  Reference:DetectDuckyKeyboardControllers                                                *
\******************************************************************************************/

void DetectOKSKeyboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        OKSKeyboardController*     controller     = new OKSKeyboardController(dev, info->path, info->product_id);
        RGBController_OKSKeyboard* rgb_controller = new RGBController_OKSKeyboard(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectOKSKeyboardControllers() */

REGISTER_HID_DETECTOR_I("OKS Optical Axis RGB",           DetectOKSKeyboardControllers, OKS_VID, OKS_OPTICAL_RGB_PID,   1);
