#include "AMDWraithPrismController.h"
#include "RGBController.h"
#include "RGBController_AMDWraithPrism.h"
#include <vector>
#include <libusb-1.0/libusb.h>

#define AMD_WRAITH_PRISM_VID 0x2516
#define AMD_WRAITH_PRISM_PID 0x0051

/******************************************************************************************\
*                                                                                          *
*   DetectAMDWraithPrismControllers                                                        *
*                                                                                          *
*       Tests the USB address to see if an AMD Wraith Prism controller exists there.       *
*                                                                                          *
\******************************************************************************************/

void DetectAMDWraithPrismControllers(std::vector<RGBController*>& rgb_controllers)
{
    libusb_context * ctx;
    libusb_init(&ctx);

    //Look for AMD Wraith Prism
    libusb_device_handle * dev = libusb_open_device_with_vid_pid(ctx, AMD_WRAITH_PRISM_VID, AMD_WRAITH_PRISM_PID);

    if( dev )
    {
        libusb_detach_kernel_driver(dev, 1);
        libusb_claim_interface(dev, 1);

        AMDWraithPrismController* controller = new AMDWraithPrismController(dev);

        RGBController_AMDWraithPrism* rgb_controller = new RGBController_AMDWraithPrism(controller);

        rgb_controllers.push_back(rgb_controller);
    }
}
