#include "Detector.h"
#include "CherryKeyboardController.h"
#include "RGBController.h"
#include "RGBController_CherryKeyboard.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define CHERRY_KEYBOARD_VID             0x046A
#define CHERRY_KEYBOARD_USAGE_PAGE      0xFF1C
#define G30_8000N_PID                   0x00DD

/******************************************************************************************\
*                                                                                          *
*   DetectCherryKeyboards                                                                 *
*                                                                                          *
*       Tests the USB address to see if an Cherry RGB Keyboard controller exists there.   *
*                                                                                          *
\******************************************************************************************/

void DetectCherryKeyboards(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        CherryKeyboardController* controller = new CherryKeyboardController(dev, info->path);
        RGBController_CherryKeyboard* rgb_controller = new RGBController_CherryKeyboard(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                                                                     |
\*---------------------------------------------------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Cherry Keyboard G80-3000N RGB TKL",  DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   G30_8000N_PID,            1, CHERRY_KEYBOARD_USAGE_PAGE);
/*---------------------------------------------------------------------------------------------------------*\
| Entries for dynamic UDEV rules                                                                            |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("Cherry Keyboard G80-3000N RGB TKL", DetectCherryKeyboards, 0x046a, 0x00dd )        |
\*---------------------------------------------------------------------------------------------------------*/