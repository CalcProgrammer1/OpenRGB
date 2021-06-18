#include "Detector.h"
#include "EVisionKeyboardController.h"
#include "RGBController.h"
#include "RGBController_EVisionKeyboard.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define EVISION_KEYBOARD_VID            0x0C45
#define WOMIER_K87_VID                  0x320F
#define EVISION_KEYBOARD_USAGE_PAGE     0xFF1C
#define REDRAGON_K550_PID               0x5204
#define REDRAGON_K552_PID               0x5104
#define REDRAGON_K556_PID               0x5004
#define TECWARE_PHANTOM_ELITE_PID       0x652F
#define WARRIOR_KANE_TC235              0x8520
#define WOMIER_K87_PID                  0x502A
#define WOMIER_K66_PID                  0x7698

/******************************************************************************************\
*                                                                                          *
*   DetectEVisionKeyboards                                                                 *
*                                                                                          *
*       Tests the USB address to see if an EVision RGB Keyboard controller exists there.   *
*                                                                                          *
\******************************************************************************************/

void DetectEVisionKeyboards(hid_device_info* info, const std::string& /*name*/)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        EVisionKeyboardController* controller = new EVisionKeyboardController(dev, info->path);
        RGBController_EVisionKeyboard* rgb_controller = new RGBController_EVisionKeyboard(controller);
        rgb_controller->name = "EVision Keyboard";
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                                                                     |
\*---------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("EVision Keyboard 0C45:5204",  DetectEVisionKeyboards, EVISION_KEYBOARD_VID,   REDRAGON_K550_PID,         1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 0C45:5104",  DetectEVisionKeyboards, EVISION_KEYBOARD_VID,   REDRAGON_K552_PID,         1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 0C45:5004",  DetectEVisionKeyboards, EVISION_KEYBOARD_VID,   REDRAGON_K556_PID,         1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 0C45:652F",  DetectEVisionKeyboards, EVISION_KEYBOARD_VID,   TECWARE_PHANTOM_ELITE_PID, 1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 0C45:8520",  DetectEVisionKeyboards, EVISION_KEYBOARD_VID,   WARRIOR_KANE_TC235,        1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 320F:502A",  DetectEVisionKeyboards, WOMIER_K87_VID,         WOMIER_K87_PID,            1, EVISION_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("EVision Keyboard 0C45:7698",  DetectEVisionKeyboards, EVISION_KEYBOARD_VID,   WOMIER_K66_PID,            1, EVISION_KEYBOARD_USAGE_PAGE);
