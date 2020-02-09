#include "ThermaltakeRiingController.h"
#include "RGBController.h"
#include "RGBController_ThermaltakeRiing.h"
#include <vector>
#include <libusb-1.0/libusb.h>

#define THERMALTAKE_RIING_VID       0x264A
#define THERMALTAKE_RIING_PID_BEGIN 0x1FA5
#define THERMALTAKE_RIING_PID_END   0x1FB5

/******************************************************************************************\
*                                                                                          *
*   DetectThermaltakeRiingControllers                                                      *
*                                                                                          *
*       Tests the USB address to see if an AMD Wraith Prism controller exists there.       *
*                                                                                          *
\******************************************************************************************/

void DetectThermaltakeRiingControllers(std::vector<RGBController*>& rgb_controllers)
{
    libusb_context * ctx;
    libusb_init(&ctx);

    for(int pid = THERMALTAKE_RIING_PID_BEGIN; pid <= THERMALTAKE_RIING_PID_END; pid++)
    {
        //Look for Thermaltake Riing device
        libusb_device_handle * dev = libusb_open_device_with_vid_pid(ctx, THERMALTAKE_RIING_VID, pid);

        if( dev )
        {
            libusb_detach_kernel_driver(dev, 0);
            libusb_claim_interface(dev, 0);

            ThermaltakeRiingController* controller = new ThermaltakeRiingController(dev);

            RGBController_ThermaltakeRiing* rgb_controller = new RGBController_ThermaltakeRiing(controller);

            rgb_controllers.push_back(rgb_controller);
        }
    }
}
