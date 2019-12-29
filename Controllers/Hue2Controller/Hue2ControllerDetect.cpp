#include "Hue2Controller.h"
#include "RGBController.h"
#include "RGBController_Hue2.h"
#include <vector>
#include <libusb-1.0/libusb.h>

#define NZXT_HUE_2_VID 0x1E71
#define NZXT_HUE_2_PID 0x2001

/******************************************************************************************\
*                                                                                          *
*   DetectHue2Controllers                                                                  *
*                                                                                          *
*       Detect devices supported by the Hue2 driver                                        *
*                                                                                          *                                                                                          *
\******************************************************************************************/

void DetectHue2Controllers(std::vector<RGBController*> &rgb_controllers)
{
    libusb_context * ctx;
    libusb_init(&ctx);

    //Look for NZXT Hue 2
    libusb_device_handle * dev = libusb_open_device_with_vid_pid(ctx, NZXT_HUE_2_VID, NZXT_HUE_2_PID);

    if( dev )
    {
        libusb_detach_kernel_driver(dev, 0);
        libusb_claim_interface(dev, 0);

        Hue2Controller* controller = new Hue2Controller(dev);

        RGBController_Hue2* rgb_controller = new RGBController_Hue2(controller);

        rgb_controllers.push_back(rgb_controller);
    }
}   /* DetectHuePlusControllers() */
