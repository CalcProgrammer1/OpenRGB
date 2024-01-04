#include "Detector.h"
#include "ValkyrieKeyboardController.h"
#include "RGBController.h"
#include "RGBController_ValkyrieKeyboard.h"
#include <hidapi/hidapi.h>

void DetectValkyrieKeyboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        ValkyrieKeyboardController*     controller     = new ValkyrieKeyboardController(dev, info->path, info->product_id);
        RGBController_ValkyrieKeyboard* rgb_controller = new RGBController_ValkyrieKeyboard(controller);
        rgb_controller->name                           = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectValkyrieKeyboardControllers() */

REGISTER_HID_DETECTOR_IPU("valkyrie-99pro",DetectValkyrieKeyboardControllers,VALKYRIE_VID,VALKYRIE_99_PRO_PID,3,0xFF13,0x0001);
