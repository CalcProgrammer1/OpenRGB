#include "CorsairCmdrProController.h"
#include "RGBController.h"
#include "RGBController_CorsairCmdrPro.h"
#include <vector>
#include <libusb-1.0/libusb.h>

#define CORSAIR_COMMANDER_PRO_VID 0x1B1C
#define CORSAIR_COMMANDER_PRO_PID 0x0C10

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairCmdrProControllers                                                        *
*                                                                                          *
*       Detect devices supported by the Corsair Commander Pro driver                       *
*                                                                                          *                                                                                          *
\******************************************************************************************/

void DetectCorsairCmdrProControllers(std::vector<RGBController*> &rgb_controllers)
{
    libusb_context * ctx;
    libusb_init(&ctx);

    //Look for Corsair Commander Pro
    libusb_device_handle * dev = libusb_open_device_with_vid_pid(ctx, CORSAIR_COMMANDER_PRO_VID, CORSAIR_COMMANDER_PRO_PID);

    if( dev )
    {
        libusb_detach_kernel_driver(dev, 0);
        libusb_claim_interface(dev, 0);

        CorsairCmdrProController* controller = new CorsairCmdrProController(dev);

        RGBController_CorsairCmdrPro* rgb_controller = new RGBController_CorsairCmdrPro(controller);

        rgb_controllers.push_back(rgb_controller);
    }
}   /* DetectCorsairCmdrProControllers() */
