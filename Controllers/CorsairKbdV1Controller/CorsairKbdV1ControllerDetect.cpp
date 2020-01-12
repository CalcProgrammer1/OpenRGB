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
static struct libusb_device *find_device(libusb_context* ctx, uint16_t vendor, uint16_t product)
{
    libusb_device** device_list;
    unsigned int count;

    count = libusb_get_device_list(ctx, &device_list);

    for(unsigned int dev_idx = 0;  dev_idx < count; dev_idx++ )
    {
        libusb_device_descriptor desc = {0};

        libusb_get_device_descriptor(device_list[dev_idx], &desc);

        if((desc.idVendor == vendor) && (desc.idProduct == product))
        {
            return device_list[dev_idx];
        }
    }

    return NULL;
}


void DetectCorsairKbdV1Controllers(std::vector<RGBController*>& rgb_controllers)
{
    libusb_context * ctx;
    libusb_init(&ctx);

    //Look for Corsair RGB Keyboard
    libusb_device * dev = find_device(ctx, CORSAIR_RGB_KEYBOARD_VID, CORSAIR_RGB_KEYBOARD_K70_PID);
    libusb_device_handle * dev_hdl;
    int error = libusb_open(dev, &dev_hdl);
    if( error == 0 )
    {
        libusb_detach_kernel_driver(dev_hdl, 1);
        libusb_claim_interface(dev_hdl, 1);

        CorsairKbdV1Controller* controller = new CorsairKbdV1Controller(dev_hdl);

        RGBController_CorsairKbdV1* rgb_controller = new RGBController_CorsairKbdV1(controller);

        rgb_controllers.push_back(rgb_controller);
    }
}
