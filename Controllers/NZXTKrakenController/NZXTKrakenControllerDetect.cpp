#include "NZXTKrakenController.h"
#include "RGBController.h"
#include "RGBController_NZXTKraken.h"
#include <vector>
#include <libusb-1.0/libusb.h>

#define NZXT_KRAKEN_VID 0x1E71
#define NZXT_KRAKEN_PID 0x170E

/******************************************************************************************\
*                                                                                          *
*   DetectNZXTKrakenControllers                                                            *
*                                                                                          *
*       Detect devices supported by the NZXTKraken driver                                  *
*                                                                                          *                                                                                          *
\******************************************************************************************/

void DetectNZXTKrakenControllers(std::vector<RGBController*> &rgb_controllers)
{
    libusb_context * ctx;
    libusb_init(&ctx);

    libusb_device_handle * dev = libusb_open_device_with_vid_pid(ctx, NZXT_KRAKEN_VID, NZXT_KRAKEN_PID);

    if( dev )
    {
        libusb_detach_kernel_driver(dev, 0);
        libusb_claim_interface(dev, 0);

        NZXTKrakenController* controller = new NZXTKrakenController(dev);

        RGBController_NZXTKraken* rgb_controller = new RGBController_NZXTKraken(controller);

        rgb_controllers.push_back(rgb_controller);
    }

}
