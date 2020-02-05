#include "CorsairKeyboardController.h"
#include "RGBController.h"
#include "RGBController_CorsairKeyboard.h"
#include <vector>
#include <hidapi/hidapi.h>

#define CORSAIR_RGB_KEYBOARD_VID        0x1B1C
#define CORSAIR_RGB_KEYBOARD_K70_PID    0x1B13
#define CORSAIR_RGB_KEYBOARD_K95_PID    0x1B11

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairKeyboardControllers                                                       *
*                                                                                          *
*       Tests the USB address to see if a Corsair RGB Keyboard controller exists there.    *
*                                                                                          *
\******************************************************************************************/

void DetectCorsairKeyboardControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev = NULL;

    hid_init();

    info = hid_enumerate(CORSAIR_RGB_KEYBOARD_VID, CORSAIR_RGB_KEYBOARD_K70_PID);

    //Look for Corsair RGB Keyboard, interface 1
    while(info)
    {
        if((info->vendor_id == CORSAIR_RGB_KEYBOARD_VID)
         &&(info->product_id == CORSAIR_RGB_KEYBOARD_K70_PID)
         &&(info->interface_number == 1))
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
        CorsairKeyboardController* controller = new CorsairKeyboardController(dev);

        RGBController_CorsairKeyboard* rgb_controller = new RGBController_CorsairKeyboard(controller);

        rgb_controllers.push_back(rgb_controller);
    }
}
