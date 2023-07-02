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
#define MX_BOARD_3_0S_FL_NBL_PID        0x0077
#define MX_BOARD_3_0S_FL_RGB_PID        0x0079
#define MX_BOARD_3_0S_FL_RGB_KOR_PID    0x0083
#define MX_1_0_FL_BL_PID                0x00AB
#define MX_BOARD_1_0_TKL_RGB_PID        0x00AC
#define MX_BOARD_8_0_TKL_RGB_PID        0x00B7
#define MX_BOARD_10_0_FL_RGB_PID        0x00BB
#define G80_3000_TKL_NBL_PID            0x00C3
#define MX_BOARD_2_0S_FL_RGB_PID        0x00C4
#define G80_3000_TKL_RGB_PID            0x00C5
#define MV_BOARD_3_0FL_RGB_PID          0x00C7
#define CCF_MX_8_0_TKL_BL_PID           0x00C9
#define CCF_MX_1_0_TKL_BL_PID           0x00CA
#define CCF_MX_1_0_TKL_NBL_PID          0x00CB
#define G30_3000_TKL_NBL_KOR_PID        0x00CD
#define MX_BOARD_2_0S_FL_NBL_PID        0x00CE
#define MX_1_0_FL_NBL_PID               0x00D2
#define MX_1_0_FL_RGB_PID               0x00D3
#define G80_3000N_TKL_RGB_PID           0x00DD
#define G30_3000N_FL_RGB_PID            0x00DE
#define MX_BOARD_10_0N_FL_RGB_PID       0x00DF
#define MX_BOARD_2_0S_FL_RGB_DE_PID     0x01A6

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
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 3.0S FL NBL",        DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_3_0S_FL_NBL_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 3.0S FL RGB",        DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_3_0S_FL_RGB_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 3.0S FL RGB KOREAN", DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_3_0S_FL_RGB_KOR_PID, 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX 1.0 FL BL",                DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_1_0_FL_BL_PID            , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 1.0 TKL RGB",        DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_1_0_TKL_RGB_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 8.0 TKL RGB",        DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_8_0_TKL_RGB_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 10.0 FL RGB",        DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_10_0_FL_RGB_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard G80-3000 TKL NBL",            DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   G80_3000_TKL_NBL_PID        , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 2.0S FL RGB",        DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_2_0S_FL_RGB_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard G80-3000 TKL RGB",            DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   G80_3000_TKL_RGB_PID        , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MV BOARD 3.0 FL RGB",         DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MV_BOARD_3_0FL_RGB_PID      , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard CCF MX 8.0 TKL BL",           DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   CCF_MX_8_0_TKL_BL_PID       , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard CCF MX 1.0 TKL BL",           DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   CCF_MX_1_0_TKL_BL_PID       , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard CCF MX 1.0 TKL NBL",          DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   CCF_MX_1_0_TKL_NBL_PID      , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard G80-3000 TKL NBL KOREAN",     DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   G30_3000_TKL_NBL_KOR_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 2.0S FL NBL",        DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_2_0S_FL_NBL_PID    , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX 1.0 FL NBL",               DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_1_0_FL_NBL_PID           , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX 1.0 FL RGB",               DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_1_0_FL_RGB_PID           , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard G80-3000N TKL RGB",           DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   G80_3000N_TKL_RGB_PID       , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard G80-3000N FL RGB",            DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   G30_3000N_FL_RGB_PID        , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 10.0N FL RGB",       DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_10_0N_FL_RGB_PID   , 1, CHERRY_KEYBOARD_USAGE_PAGE);
REGISTER_HID_DETECTOR_IP("Cherry Keyboard MX BOARD 2.0S FL RGB DE",     DetectCherryKeyboards, CHERRY_KEYBOARD_VID,   MX_BOARD_2_0S_FL_RGB_DE_PID , 1, CHERRY_KEYBOARD_USAGE_PAGE);
