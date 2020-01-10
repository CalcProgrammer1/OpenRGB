#include "CorsairKbdV1Controller.h"
#include "RGBController.h"
#include "RGBController_CorsairKbdV1.h"
#include <vector>
#include <libusb-1.0/libusb.h>

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
    libusb_context * ctx;
    libusb_init(&ctx);

    //Look for Corsair RGB Keyboard
    libusb_device_handle * dev = libusb_open_device_with_vid_pid(ctx, CORSAIR_RGB_KEYBOARD_VID, CORSAIR_RGB_KEYBOARD_K70_PID);

    if( dev )
    {
        libusb_detach_kernel_driver(dev, 1);
        libusb_claim_interface(dev, 1);

        CorsairKbdV1Controller* controller = new CorsairKbdV1Controller(dev);

        RGBController_CorsairKbdV1* rgb_controller = new RGBController_CorsairKbdV1(controller);

        rgb_controllers.push_back(rgb_controller);
    }
}
