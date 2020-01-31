#include "Hue2Controller.h"
#include "RGBController.h"
#include "RGBController_Hue2.h"
#include <vector>
#include <libusb-1.0/libusb.h>

#define NZXT_HUE_2_VID 0x1E71
#define NZXT_HUE_2_PID 0x2001
#define NZXT_SMART_DEVICE_V2_PID 0x2006

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
    libusb_device_handle * hue_2_dev = libusb_open_device_with_vid_pid(ctx, NZXT_HUE_2_VID, NZXT_HUE_2_PID);

    if( hue_2_dev )
    {
        libusb_detach_kernel_driver(hue_2_dev, 0);
        libusb_claim_interface(hue_2_dev, 0);

        Hue2Controller* controller = new Hue2Controller(hue_2_dev);

        RGBController_Hue2* rgb_controller = new RGBController_Hue2(controller);

        rgb_controllers.push_back(rgb_controller);
    }

    //Look for NZXT Smart Device V2
    libusb_device_handle * smart_dev_2_dev = libusb_open_device_with_vid_pid(ctx, NZXT_HUE_2_VID, NZXT_SMART_DEVICE_V2_PID);

    if( smart_dev_2_dev )
    {
        libusb_detach_kernel_driver(smart_dev_2_dev, 0);
        libusb_claim_interface(smart_dev_2_dev, 0);

        Hue2Controller* controller = new Hue2Controller(smart_dev_2_dev);

        RGBController_Hue2* rgb_controller = new RGBController_Hue2(controller);

        rgb_controllers.push_back(rgb_controller);
    }
}   /* DetectHuePlusControllers() */
