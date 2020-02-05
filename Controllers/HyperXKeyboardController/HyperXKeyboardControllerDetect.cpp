#include "HyperXKeyboardController.h"
#include "RGBController.h"
#include "RGBController_HyperXKeyboard.h"
#include <vector>
#include <hidapi/hidapi.h>

#define HYPERX_KEYBOARD_VID         0x0951
#define HYPERX_ALLOY_ELITE_PID      0x16BE

/******************************************************************************************\
*                                                                                          *
*   DetectHyperXKeyboardControllers                                                        *
*                                                                                          *
*       Tests the USB address to see if a HyperX Keyboard controller exists there.         *
*                                                                                          *
\******************************************************************************************/

void DetectHyperXKeyboardControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev = NULL;

    hid_init();

    info = hid_enumerate(HYPERX_KEYBOARD_VID, HYPERX_ALLOY_ELITE_PID);

    //Look for HyperX Keyboard, Interface 2
    while(info)
    {
        if((info->vendor_id == HYPERX_KEYBOARD_VID)
         &&(info->product_id == HYPERX_ALLOY_ELITE_PID)
         &&(info->interface_number == 2))
        {
            dev = hid_open_path(info->path);
            break;
        }
        else
        {
            info = info->next;
        }
    }

    if( dev )
    {
        HyperXKeyboardController* controller = new HyperXKeyboardController(dev);

        RGBController_HyperXKeyboard* rgb_controller = new RGBController_HyperXKeyboard(controller);

        rgb_controllers.push_back(rgb_controller);
    }
}
