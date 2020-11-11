#include "Detector.h"
#include "CMMP750Controller.h"
#include "CMARGBcontroller.h"
#include "RGBController.h"
#include "RGBController_CMMP750Controller.h"
#include "RGBController_CMARGBController.h"
#include <hidapi/hidapi.h>

#define COOLERMASTER_VID                0x2516

#define COOLERMASTER_MP750_XL_PID       0x0109
#define COOLERMASTER_MP750_MEDIUM_PID   0x0105
#define COOLERMASTER_ARGB_PID           0x1011

#define COOLERMASTER_NUM_DEVICES (sizeof(cm_pids) / sizeof(cm_pids[ 0 ]))

struct coolermaster_device
{
    unsigned int    product_id;
    unsigned short  interface;
    unsigned int    usage_page;
    unsigned int    usage;
    device_type     type;
};

static const coolermaster_device cm_pids[] =
{  // PID,                              Interface,  Usage_Page, Usage,  Device_Type
    { COOLERMASTER_MP750_XL_PID,        0x00,       0xFF00,     0x01,   DEVICE_TYPE_MOUSEMAT  },    //Coolermaster MP750 (Extra Large)
    { COOLERMASTER_MP750_MEDIUM_PID,    0x00,       0xFF00,     0x01,   DEVICE_TYPE_MOUSEMAT  },    //Coolermaster MP750 (Medium)
    { COOLERMASTER_ARGB_PID,            0x00,       0xFF00,     0x01,   DEVICE_TYPE_LEDSTRIP  }     //Coolermaster ARGB Controller
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
        device_type dev_type;

        if(info->vendor_id == COOLERMASTER_VID)
        {
            for(unsigned int cm_pid_idx = 0; cm_pid_idx < COOLERMASTER_NUM_DEVICES; cm_pid_idx++)
            {
                if((info->product_id        == cm_pids[cm_pid_idx].product_id)
#ifdef USE_HID_USAGE
                    &&(info->usage             == cm_pids[cm_pid_idx].usage)     //Usage and usage page required to get the correct interface
                    &&(info->usage_page        == cm_pids[cm_pid_idx].usage_page))
#else
                    &&(info->interface_number  == cm_pids[cm_pid_idx].interface))
#endif  //USE_HID_USAGE
                {
                    dev         = hid_open_path(info->path);
                    dev_type    = cm_pids[cm_pid_idx].type;
                    break;
                }
            }
        }

        if(dev)
        {
            if (dev_type == DEVICE_TYPE_MOUSEMAT)
            {
                CMMP750Controller* controller                   = new CMMP750Controller(dev, info->path);
                RGBController_CMMP750Controller* rgb_controller = new RGBController_CMMP750Controller(controller);
                rgb_controllers.push_back(rgb_controller);
            }
            else if(dev_type == DEVICE_TYPE_LEDSTRIP)
            {
                for(std::size_t i = 0; i < CM_ARGB_HEADER_DATA_SIZE; i++)
                {
                    CMARGBController* controller = new CMARGBController(dev, info->path, i);
                    RGBController_CMARGBController* rgb_controller = new RGBController_CMARGBController(controller);
                    rgb_controllers.push_back(rgb_controller);
                }
            }
        }
        info = info->next;
    }
    hid_free_enumeration(info);

}   /* DetectCoolerMasterControllers() */

REGISTER_DETECTOR("Cooler Master", DetectCoolerMasterControllers);
