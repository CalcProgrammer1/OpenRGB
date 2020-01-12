#include "CorsairKbdV1Controller.h"
#include "RGBController.h"
#include "RGBController_CorsairKbdV1.h"
#include <vector>
#include <hidapi/hidapi.h>

#define CORSAIR_RGB_KEYBOARD_VID        0x1B1C
#define CORSAIR_RGB_KEYBOARD_K70_PID    0x1B13
#define CORSAIR_RGB_KEYBOARD_K95_PID    0x1B11

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairKbdV1Controllers                                                          *
*                                                                                          *
*       Tests the USB address to see if a Corsair RGB Keyboard controller exists there.    *
*                                                                                          *
\******************************************************************************************/

void DetectCorsairKbdV1Controllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev;

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
        CorsairKbdV1Controller* controller = new CorsairKbdV1Controller(dev);

        RGBController_CorsairKbdV1* rgb_controller = new RGBController_CorsairKbdV1(controller);

        rgb_controllers.push_back(rgb_controller);
    }
}
