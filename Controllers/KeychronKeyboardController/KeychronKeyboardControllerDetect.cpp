#include "Detector.h"
#include "KeychronKeyboardController.h"
#include "RGBController.h"
#include "RGBController_KeychronKeyboard.h"

/*---------------------------------------------------------*\
| KeychronKeyboard vendor ID                                |
\*---------------------------------------------------------*/
#define KEYCHRON_KEYBOARD_VID                          0x05AC

/*---------------------------------------------------------*\
| Product ID                                                |
\*---------------------------------------------------------*/
#define KEYCHRON_K3_V2_OPTICAL_RGB_PID                 0x024F

void DetectKeychronKeyboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        KeychronKeyboardController*     controller      = new KeychronKeyboardController(dev, *info);
        RGBController_KeychronKeyboard* rgb_controller  = new RGBController_KeychronKeyboard(controller);
        rgb_controller->name                            = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Keychron Gaming Keyboard 1", DetectKeychronKeyboardControllers, KEYCHRON_KEYBOARD_VID, KEYCHRON_K3_V2_OPTICAL_RGB_PID, 0, 0x0001, 0x06);
