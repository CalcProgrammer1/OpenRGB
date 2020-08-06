#include "Detector.h"
#include "EKController.h"
#include "RGBController.h"
#include "RGBController_EKController.h"
#include <hidapi/hidapi.h>

#define EK_VID                0x0483

#define EK_LOOP_CONNECT       0x5750

#define EK_NUM_DEVICES (sizeof(ek_pids) / sizeof(ek_pids[ 0 ]))

enum
{
    EK_PID = 0,
    EK_INTERFACE = 1
};

static const unsigned int ek_pids[][2] =
{  // PID,      Interface
    { EK_LOOP_CONNECT,        0x00 }     //EK Loop Connect
};

/******************************************************************************************\
*                                                                                          *
*   DetectEKControllers                                                                    *
*                                                                                          *
*       Tests the USB address to see if any EK Controllers exists there.                   *
*                                                                                          *
\******************************************************************************************/

void DetectEKControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;

    //Look for the passed in cm_pids
    hid_init();
    info = hid_enumerate(0x0, 0x0);

    while(info)
    {
        hid_device* dev = NULL;
        if(info->vendor_id == EK_VID)
        {
            for(int ek_pid_idx = 0; ek_pid_idx < EK_NUM_DEVICES; ek_pid_idx++)
            {
                if((info->product_id        == ek_pids[ek_pid_idx][EK_PID])
                 &&(info->interface_number  == ek_pids[ek_pid_idx][EK_INTERFACE]))
                {
                    dev = hid_open_path(info->path);
                    break;
                }
            }
        }

        if(dev)
        {
            EKController* controller = new EKController(dev, info->manufacturer_string, info->product_string, info->path);
            RGBController_EKController* rgb_controller = new RGBController_EKController(controller);
            rgb_controllers.push_back(rgb_controller);
        }
        info = info->next;
    }
    hid_free_enumeration(info);
}   /* DetectEKControllers() */

REGISTER_DETECTOR("EK", DetectEKControllers);
