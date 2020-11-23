#include "Detector.h"
#include "EKController.h"
#include "RGBController.h"
#include "RGBController_EKController.h"
#include <hidapi/hidapi.h>

#define EK_VID                  0x0483

#define EK_LOOP_CONNECT_PID     0x5750

#define EK_NUM_DEVICES (sizeof(ek_pids) / sizeof(ek_pids[ 0 ]))

struct ek_device
{
    unsigned int    product_id;
    unsigned short  interface;
    unsigned int    usage_page;
    unsigned int    usage;
    device_type     type;
};

static const ek_device ek_pids[] =
{  // PID,                  Interface,  Usage_Page, Usage,  Device_Type
   { EK_LOOP_CONNECT_PID,   0x00,       0xFFA0,     0x01,   DEVICE_TYPE_LEDSTRIP  }      //EK Loop Connect
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
    hid_device_info* info = NULL;

    //Look for the passed in ek_pids
    hid_init();
    info = hid_enumerate(0, 0x0);

    while(info)
    {
        hid_device* dev = NULL;
        if(info->vendor_id == EK_VID)
        {
            for(unsigned int ek_pid_idx = 0; ek_pid_idx < EK_NUM_DEVICES; ek_pid_idx++)
            {
                if((info->product_id            == ek_pids[ek_pid_idx].product_id)
#ifdef USE_HID_USAGE
                    &&(info->usage              == ek_pids[ek_pid_idx].usage)     //Usage and usage page required to get the correct interface
                    &&(info->usage_page         == ek_pids[ek_pid_idx].usage_page))
#else
                    &&(info->interface_number   == ek_pids[ek_pid_idx].interface))
#endif  //USE_HID_USAGE
                {
                    dev = hid_open_path(info->path);
                    break;
                }
            }
        }

        if(dev)
        {
            EKController* controller = new EKController(dev, info->path);
            RGBController_EKController* rgb_controller = new RGBController_EKController(controller);
            rgb_controllers.push_back(rgb_controller);
        }
        info = info->next;
    }
    hid_free_enumeration(info);
}   /* DetectEKControllers() */

REGISTER_DETECTOR("EK", DetectEKControllers);
