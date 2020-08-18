#include "Detector.h"
#include "NZXTKrakenController.h"
#include "RGBController.h"
#include "RGBController_NZXTKraken.h"
#include <vector>
#include <hidapi/hidapi.h>

#define NZXT_KRAKEN_VID     0x1E71
#define NZXT_KRAKEN_X2_PID  0x170E
#define NZXT_KRAKEN_M2_PID  0x1715

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    const char *    name;
} nzxt_kraken_device;

#define NZXT_KRAKEN_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const nzxt_kraken_device device_list[] =
{
    /*-----------------------------------------------------------------------------------------------------*\
    | NZXT Hue 2 devices                                                                                    |
    \*-----------------------------------------------------------------------------------------------------*/
    { NZXT_KRAKEN_VID,      NZXT_KRAKEN_X2_PID,                         "NZXT Kraken X2"                    },
    { NZXT_KRAKEN_VID,      NZXT_KRAKEN_M2_PID,                         "NZXT Kraken M2"                    },
};

/******************************************************************************************\
*                                                                                          *
*   DetectNZXTKrakenControllers                                                            *
*                                                                                          *
*       Detect devices supported by the NZXTKraken driver                                  *
*                                                                                          *
\******************************************************************************************/

void DetectNZXTKrakenControllers(std::vector<RGBController*> &rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev;

    hid_init();

    for(std::size_t device_idx = 0; device_idx < NZXT_KRAKEN_NUM_DEVICES; device_idx++)
    {
        dev = NULL;

        info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        //Look for NZXT Kraken devices
        while(info)
        {
            if((info->vendor_id == device_list[device_idx].usb_vid)
            &&(info->product_id == device_list[device_idx].usb_pid))
            {
                dev = hid_open_path(info->path);
                
                if( dev )
                {
                    NZXTKrakenController* controller = new NZXTKrakenController(dev);

                    RGBController_NZXTKraken* rgb_controller = new RGBController_NZXTKraken(controller);

                    rgb_controller->name = device_list[device_idx].name;
                    
                    rgb_controllers.push_back(rgb_controller);
                }
            }
            info = info->next;
        }
    }
}   /* DetectNZXTKrakenControllers() */

REGISTER_DETECTOR("NZXT Kraken", DetectNZXTKrakenControllers);
