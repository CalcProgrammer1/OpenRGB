#include "Detector.h"
#include "CMMP750Controller.h"
#include "RGBController.h"
#include "RGBController_CMMP750Controller.h"
#include <hidapi/hidapi.h>

#define COOLERMASTER_VID                0x2516

#define COOLERMASTER_MP750_XL_PID       0x0109
#define COOLERMASTER_MP750_MEDIUM_PID   0x0105

#define COOLERMASTER_NUM_DEVICES (sizeof(cm_pids) / sizeof(cm_pids[ 0 ]))

enum
{
    CM_PID = 0,
    CM_INTERFACE = 1
};

static const unsigned int cm_pids[][4] =
{  // PID,      Interface
    { COOLERMASTER_MP750_XL_PID,        0x00 },     //Coolermaster MP750 (Extra Large)
    { COOLERMASTER_MP750_MEDIUM_PID,    0x00 }      //Coolermaster MP750 (Medium)
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
    hid_device_info* info;

    //Look for the passed in cm_pids
    hid_init();
    info = hid_enumerate(COOLERMASTER_VID, 0x0);

    while(info)
    {
        hid_device* dev = NULL;
        if(info->vendor_id == COOLERMASTER_VID)
        {
            for(int cm_pid_idx = 0; cm_pid_idx < COOLERMASTER_NUM_DEVICES; cm_pid_idx++)
            {
                if((info->product_id        == cm_pids[cm_pid_idx][CM_PID])
                 &&(info->interface_number  == cm_pids[cm_pid_idx][CM_INTERFACE]))
                {
                    dev = hid_open_path(info->path);
                    break;
                }
            }
        }

        if(dev)
        {
            CMMP750Controller* controller                   = new CMMP750Controller(dev, info->manufacturer_string, info->product_string, info->path);
            RGBController_CMMP750Controller* rgb_controller = new RGBController_CMMP750Controller(controller);
            rgb_controllers.push_back(rgb_controller);
        }
        info = info->next;
    }
    hid_free_enumeration(info);
}   /* DetectCoolerMasterControllers() */

REGISTER_DETECTOR("Cooler Master", DetectCoolerMasterControllers);
