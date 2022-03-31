#include "Detector.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_EVGAKeyboard.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| EVGA USB vendor ID                                    |
\*-----------------------------------------------------*/
#define EVGA_USB_VID                            0x3842

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define Z15_ISO_PID                             0x260E
#define Z15_ANSI_PID                            0x2608
#define Z20_ANSI_PID                            0x260A

void DetectEVGAKeyboardControllers(hid_device_info* info, const std::string& name)
{
    static const char* controller_name = "EVGA Keyboard Controller";

    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        EVGAKeyboardController* controller          = new EVGAKeyboardController(dev, info->path, info->product_id);
        RGBController_EVGAKeyboard* rgb_controller  = new RGBController_EVGAKeyboard(controller);
        rgb_controller->name                        = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("EVGA Z15 Keyboard",     DetectEVGAKeyboardControllers,  EVGA_USB_VID,  Z15_ISO_PID,      1,  0x08, 0x4B);
REGISTER_HID_DETECTOR_IPU("EVGA Z15 Keyboard",     DetectEVGAKeyboardControllers,  EVGA_USB_VID,  Z15_ANSI_PID,     1,  0x08, 0x4B);
REGISTER_HID_DETECTOR_IPU("EVGA Z20 Keyboard",     DetectEVGAKeyboardControllers,  EVGA_USB_VID,  Z20_ANSI_PID,     1,  0x08, 0x4B);
