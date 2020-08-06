/*-------------------------------------------------------------------*\
|  TecknetControllerDetect.cpp                                        |
|                                                                     |
|  Driver for Tecknet Devices                                         |
|                                                                     |
|  Chris M (Dr_No)          29th Jul 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "Detector.h"
#include "TecknetController.h"
#include "RGBController.h"
#include "RGBController_Tecknet.h"
#include <hidapi/hidapi.h>

#define TECKNET_VID             0x04D9

#define TECKNET_M0008_PID       0xFC05
#define TECKNET_M0008_U         0x01        //Usage 01
#define TECKNET_M0008_UPG       0xFFA0      //Vendor Defined Usage Page

#define TECKNET_NUM_DEVICES (sizeof(tecknet_pids) / sizeof(tecknet_pids[ 0 ]))

enum
{
    TECKNET_PID         = 0,
    TECKNET_INTERFACE   = 1,
    TECKNET_USAGE       = 2,
    TECKNET_USAGE_PAGE  = 3
};

static const unsigned int tecknet_pids[][4] =
{  // PID,      Interface,      Usage,          Usage_Page
    { TECKNET_M0008_PID, 0, TECKNET_M0008_U, TECKNET_M0008_UPG }     //Tecknet M008 Mouse
};

/******************************************************************************************\
*                                                                                          *
*   DetectTecknetControllers                                                               *
*                                                                                          *
*       Tests the USB address to see if any Tecknet Controllers.                           *
*                                                                                          *
\******************************************************************************************/

void DetectTecknetControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;

    //Look for the passed in tecknet_pids
    hid_init();
    info = hid_enumerate(TECKNET_VID, 0x0);

    while(info)
    {
        hid_device* dev = NULL;
        if(info->vendor_id == TECKNET_VID)
        {
            for(int pid_idx = 0; pid_idx < TECKNET_NUM_DEVICES; pid_idx++)
            {
                if((info->vendor_id         == TECKNET_VID)
#ifdef USE_HID_USAGE
                &&(info->usage              == tecknet_pids[pid_idx][TECKNET_USAGE])
                &&(info->usage_page         == tecknet_pids[pid_idx][TECKNET_USAGE_PAGE])
                &&(info->product_id         == tecknet_pids[pid_idx][TECKNET_PID]))
#else
                &&(info->interface_number   == tecknet_pids[pid_idx][TECKNET_INTERFACE])     //Interface is only valid on Windows where there is > 1 interface
                &&(info->product_id         == tecknet_pids[pid_idx][TECKNET_PID]))
#endif
                {
                    dev = hid_open_path(info->path);
                    break;
                }
            }
        }

        if(dev)
        {
            TecknetController* controller = new TecknetController(dev, info->path);
            RGBController_Tecknet* rgb_controller = new RGBController_Tecknet(controller);
            rgb_controllers.push_back(rgb_controller);
        }
        info = info->next;
    }
    hid_free_enumeration(info);
}   /* DetectTecknetControllers) */

REGISTER_DETECTOR("Tecknet", DetectTecknetControllers);
