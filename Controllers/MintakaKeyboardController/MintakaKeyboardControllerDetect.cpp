#include "Detector.h"
#include "MintakaKeyboardController.h"
#include "RGBController.h"
#include "RGBController_MintakaKeyboard.h"

/*---------------------------------------------------------*\
| MintakaKeyboard vendor ID                                 |
\*---------------------------------------------------------*/
#define MINTAKA_KEYBOARD_VID                          0x05AC

/*---------------------------------------------------------*\
| Product ID                                                |
\*---------------------------------------------------------*/
#define VSG_MINTAKA_PID                               0x0256

void DetectMintakaKeyboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        MintakaKeyboardController*     controller      = new MintakaKeyboardController(dev, *info);
        RGBController_MintakaKeyboard* rgb_controller  = new RGBController_MintakaKeyboard(controller);
        rgb_controller->name                            = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("VSG Mintaka", DetectMintakaKeyboardControllers, MINTAKA_KEYBOARD_VID, VSG_MINTAKA_PID, 0, 0x0001, 0x06);
