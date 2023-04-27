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
#define MX_BOARD_3_0S_FL_RGB_PID        0x0079
#define G80_3000_TKL_RGB_PID            0x00C5
#define G80_3000N_TKL_RGB_PID           0x00DD
#define MX_BOARD_10_0N_FL_RGB_PID       0x00DF

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
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 3.0S FL RGB",  DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_3_0S_FL_RGB_PID,         1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard G80-3000 TKL RGB",      DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   G80_3000_TKL_RGB_PID,             1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard G80-3000N TKL RGB",     DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   G80_3000N_TKL_RGB_PID,            1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 10.0N FL RGB", DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_10_0N_FL_RGB_PID,        1, CHERRY_KEYBOARD_USAGE_PAGE);

/*---------------------------------------------------------------------------------------------------------*\
| Entries for dynamic UDEV rules                                                                            |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("Cherry Keyboard MX BOARD 3.0S FL RGB", DetectCherryKeyboards, 0x046a, 0x0079 )     |
| DUMMY_DEVICE_DETECTOR("Cherry Keyboard G80-3000 TKL RGB", DetectCherryKeyboards, 0x046a, 0x00c5 )         |
| DUMMY_DEVICE_DETECTOR("Cherry Keyboard G80-3000N TKL RGB", DetectCherryKeyboards, 0x046a, 0x00dd )        |
| DUMMY_DEVICE_DETECTOR("Cherry Keyboard MX BOARD 10.0N FL RGB", DetectCherryKeyboards, 0x046a, 0x00df )    |
\*---------------------------------------------------------------------------------------------------------*/