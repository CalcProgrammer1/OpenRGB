#include "CMMP750Controller.h"
#include "RGBController.h"
#include "RGBController_CMMP750Controller.h"
#include <vector>
#include <libusb-1.0/libusb.h>

#define COOLERMASTER_VID 0x2516

#define COOLERMASTER_NUM_DEVICES (sizeof(cm_pids) / sizeof(cm_pids[ 0 ]))

static const unsigned int cm_pids[] =
{
    0x0109
};

/******************************************************************************************\
*                                                                                          *
*   DetectCoolerMasterControllers                                                          *
*                                                                                          *
*       Tests the USB address to see if any CoolerMaster controllers exists there.         *
*                                                                                          *
\******************************************************************************************/

void DetectCoolerMasterControllers(std::vector<RGBController*>& rgb_controllers)
{
    libusb_context * context;
    libusb_init(&context);

    for(int cm_pid_idx = 0; cm_pid_idx < COOLERMASTER_NUM_DEVICES; cm_pid_idx++)
    {
        //Look for the passed in cm_pids
        libusb_device_handle * dev = libusb_open_device_with_vid_pid(context, COOLERMASTER_VID, cm_pids[cm_pid_idx]);

        if(dev)
        {
            libusb_detach_kernel_driver(dev, 1);
            libusb_claim_interface(dev, 1);

            CMMP750Controller* controller = new CMMP750Controller(dev);

            RGBController_CMMP750Controller* rgb_controller = new RGBController_CMMP750Controller(controller);

            rgb_controllers.push_back(rgb_controller);
        }
    }

}
