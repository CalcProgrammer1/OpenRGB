#include "Detector.h"
#include "DasKeyboardController.h"
#include "RGBController.h"
#include "RGBController_DasKeyboard.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Das Keyboard vendor ID                                |
\*-----------------------------------------------------*/
#define DAS_KEYBOARD_VID                0x24F0

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define DAS_KEYBOARD_Q4_PID             0x2037
#define DAS_KEYBOARD_Q5_PID             0x2020

/******************************************************************************************\
*                                                                                          *
*   DetectDasKeyboardControllers                                                           *
*                                                                                          *
*       Tests the USB address to see if a Das Keyboard RGB controller exists there.        *
*       We need the second interface to communicate with the keyboard                      *
*                                                                                          *
\******************************************************************************************/

void DetectDasKeyboardControllers(hid_device_info *info_in, const std::string &name)
{
    hid_device_info *info = info_in;

    while(info)
    {
        if(info->vendor_id        == DAS_KEYBOARD_VID     &&
          (info->product_id       == DAS_KEYBOARD_Q4_PID  ||
           info->product_id       == DAS_KEYBOARD_Q5_PID) &&
           info->interface_number == 1)
        {
            break;
        }
        info = info->next;
    }

    if(!info)
    {
        return;
    }

    hid_device *dev = hid_open_path(info->path);

    if(dev)
    {
        DasKeyboardController *controller = new DasKeyboardController(dev, info->path);

        if(controller->GetLayoutString() == "NONE")
        {
            delete controller;
        }
        else
        {
            RGBController_DasKeyboard *rgb_controller = new RGBController_DasKeyboard(controller);
            rgb_controller->SetupZones();
            rgb_controller->name = name;

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}   /* DetectDasKeyboardControllers() */

REGISTER_HID_DETECTOR_I("Das Keyboard Q4 RGB", DetectDasKeyboardControllers, DAS_KEYBOARD_VID, DAS_KEYBOARD_Q4_PID, 0);
REGISTER_HID_DETECTOR_I("Das Keyboard Q5 RGB", DetectDasKeyboardControllers, DAS_KEYBOARD_VID, DAS_KEYBOARD_Q5_PID, 0);
