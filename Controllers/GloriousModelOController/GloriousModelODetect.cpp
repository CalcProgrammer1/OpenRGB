#include "GloriousModelOController.h"
#include "RGBController.h"
#include "RGBController_GloriousModelO.h"
#include <vector>
#include <libusb-1.0/libusb.h>

#define Glorious_Model_O_VID 0x258A
#define Glorious_Model_O_PID 0x0036

/******************************************************************************************\
*                                                                                          *
*   DetectGloriousModelController                                                          *
*                                                                                          *
*       Tests the USB address to see if a Glorious Model O controller exists there.        *
*                                                                                          *
\******************************************************************************************/

void DetectGloriousModelOControllers(std::vector<RGBController*>& rgb_controllers)
{
    libusb_context * ctx;
    libusb_init(&ctx);

    //Look for Glorious Model O
    libusb_device_handle * dev = libusb_open_device_with_vid_pid(ctx, Glorious_Model_O_VID, Glorious_Model_O_PID);

    if( dev )
    {
        libusb_detach_kernel_driver(dev, 1);
        libusb_claim_interface(dev, 1);

        GloriousModelOController* controller = new GloriousModelOController(dev);

        RGBController_GloriousModelO* rgb_controller = new RGBController_GloriousModelO(controller);

        rgb_controllers.push_back(rgb_controller);
    }
}
