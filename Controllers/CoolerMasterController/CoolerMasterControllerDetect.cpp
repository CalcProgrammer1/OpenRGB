#include "CMMP750Controller.h"
#include "RGBController.h"
#include "RGBController_CMMP750Controller.h"
#include <vector>
#include <libusb-1.0/libusb.h>

#define COOLERMASTER_VID 0x2516

#define COOLERMASTER_NUM_DEVICES (sizeof(cm_pids) / sizeof(cm_pids[ 0 ]))

enum
{
    CM_PID = 0,
    CM_INADDR = 1,
    CM_OUTADDR = 2,
    CM_INTERFACE = 3
};

static const unsigned int cm_pids[][4] =
{ //PID, inAddr, outAddr, interface
    { 0x0109, 0x82, 0x03, 0x00 }        //Coolermaster MP750
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
        libusb_device_handle * dev = libusb_open_device_with_vid_pid(context, COOLERMASTER_VID, cm_pids[cm_pid_idx][CM_PID]);

        if(dev)
        {
            int status = libusb_detach_kernel_driver(dev, cm_pids[cm_pid_idx][CM_INTERFACE]);
            status = libusb_claim_interface(dev, cm_pids[cm_pid_idx][CM_INTERFACE]);

            if(status == 0)
            {
                // Success: Device has been claimed
                CMMP750Controller* controller = new CMMP750Controller(dev, cm_pids[cm_pid_idx][CM_INADDR], cm_pids[cm_pid_idx][CM_OUTADDR], cm_pids[cm_pid_idx][CM_INTERFACE]);
                RGBController_CMMP750Controller* rgb_controller = new RGBController_CMMP750Controller(controller);
                rgb_controllers.push_back(rgb_controller);
            }
        }
    }

}
