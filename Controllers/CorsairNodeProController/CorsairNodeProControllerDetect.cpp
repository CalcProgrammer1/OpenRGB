#include "CorsairNodeProController.h"
#include "RGBController.h"
#include "RGBController_CorsairNodePro.h"
#include <vector>
#include <libusb-1.0/libusb.h>

#define CORSAIR_LIGHTING_NODE_PRO_VID 0x1B1C
#define CORSAIR_LIGHTING_NODE_PRO_PID 0x0C0B

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairNodeProControllers                                                        *
*                                                                                          *
*       Detect devices supported by the Corsair Lighting Node Pro driver                   *
*                                                                                          *                                                                                          *
\******************************************************************************************/

void DetectCorsairNodeProControllers(std::vector<RGBController*> &rgb_controllers)
{
    libusb_context * ctx;
    libusb_init(&ctx);

    //Look for Corsair Lighting Node Pro
    libusb_device_handle * dev = libusb_open_device_with_vid_pid(ctx, CORSAIR_LIGHTING_NODE_PRO_VID, CORSAIR_LIGHTING_NODE_PRO_PID);

    if( dev )
    {
        libusb_detach_kernel_driver(dev, 0);
        libusb_claim_interface(dev, 0);

        CorsairNodeProController* controller = new CorsairNodeProController(dev);

        RGBController_CorsairNodePro* rgb_controller = new RGBController_CorsairNodePro(controller);

        rgb_controllers.push_back(rgb_controller);
    }
}   /* DetectCorsairNodeProControllers() */
