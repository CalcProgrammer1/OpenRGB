#include "MSI3ZoneController.h"
#include "RGBController.h"
#include "RGBController_MSI3Zone.h"
#include <vector>
#include <libusb-1.0/libusb.h>

#define MSI_3_ZONE_KEYBOARD_VID 0x1770
#define MSI_3_ZONE_KEYBOARD_PID 0xFF00

/******************************************************************************************\
*                                                                                          *
*   DetectMSI3ZoneControllers                                                              *
*                                                                                          *
*       Tests the USB address to see if an MSI/SteelSeries 3-zone Keyboard controller      *
*       exists there.                                                                      *
*                                                                                          *
\******************************************************************************************/

void DetectMSI3ZoneControllers(std::vector<RGBController*>& rgb_controllers)
{
    libusb_context * ctx;
    libusb_init(&ctx);

    //Look for MSI/Steelseries 3-zone Keyboard
    libusb_device_handle * dev = libusb_open_device_with_vid_pid(ctx, MSI_3_ZONE_KEYBOARD_VID, MSI_3_ZONE_KEYBOARD_PID);

    if( dev )
    {
        libusb_detach_kernel_driver(dev, 1);
        libusb_claim_interface(dev, 1);

        MSI3ZoneController* controller = new MSI3ZoneController(dev);

        RGBController_MSI3Zone* rgb_controller = new RGBController_MSI3Zone(controller);

        rgb_controllers.push_back(rgb_controller);
    }
}
