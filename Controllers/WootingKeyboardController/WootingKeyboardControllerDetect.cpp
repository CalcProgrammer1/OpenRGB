#include "Detector.h"
#include "WootingKeyboardController.h"
#include "RGBController.h"
#include "RGBController_WootingKeyboard.h"
#include "LogManager.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Wooting vendor ID                                     |
\*-----------------------------------------------------*/
#define WOOTING_OLD_VID                       0x03EB
#define WOOTING_NEW_VID                       0x31E3

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define WOOTING_ONE_PID                       0xFF01
#define WOOTING_TWO_PID                       0xFF02
#define WOOTING_TWO_LE_PID                    0x1210
#define WOOTING_TWO_HE_PID                    0x1220

void DetectWootingKeyboardControllers(hid_device_info* info, const std::string& name)
{
    LOG_DEBUG("[Wooting KB] Interface %i\tPage %04X\tUsage %i\tPath %s", info->interface_number, info->usage_page, info->usage, info->path);

    hid_device* dev = hid_open_path(info->path);
    
    if(dev)
    {
        WootingKeyboardController*     controller     = new WootingKeyboardController(dev, info->path);
        RGBController_WootingKeyboard* rgb_controller = new RGBController_WootingKeyboard(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}  /* DetectWootingKeyboardControllers */

REGISTER_HID_DETECTOR_PU("Wooting ONE Keyboard",     DetectWootingKeyboardControllers,  WOOTING_OLD_VID,  WOOTING_ONE_PID,        0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting TWO Keyboard",     DetectWootingKeyboardControllers,  WOOTING_OLD_VID,  WOOTING_TWO_PID,        0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting TWO Keyboard LE",  DetectWootingKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_LE_PID,     0x1337, 1);
REGISTER_HID_DETECTOR_PU("Wooting TWO Keyboard HE",  DetectWootingKeyboardControllers,  WOOTING_NEW_VID,  WOOTING_TWO_HE_PID,     0x1337, 1);
