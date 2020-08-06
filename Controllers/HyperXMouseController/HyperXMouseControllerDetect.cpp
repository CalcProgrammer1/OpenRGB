#include "Detector.h"
#include "HyperXPulsefireSurgeController.h"
#include "RGBController.h"
#include "RGBController_HyperXPulsefireSurge.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| HyperX keyboard vendor IDs                            |
\*-----------------------------------------------------*/
#define HYPERX_VID                  0x0951
#define HYPERX_PULSEFIRE_SURGE_PID  0x16D3

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned char   usb_interface;
    const char *    name;
} hyperx_device;

#define HYPERX_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const hyperx_device device_list[] =
{
    /*-----------------------------------------------------------------------------------------------------*\
    | Mice                                                                                                  |
    \*-----------------------------------------------------------------------------------------------------*/
    { HYPERX_VID,           HYPERX_PULSEFIRE_SURGE_PID, 1,      "HyperX Pulsefire Surge"                    },
};

/******************************************************************************************\
*                                                                                          *
*   DetectHyperXMouseControllers                                                           *
*                                                                                          *
*       Tests the USB address to see if a HyperX Mouse controller exists there.            *
*                                                                                          *
\******************************************************************************************/

void DetectHyperXMouseControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev = NULL;

    hid_init();

    for(std::size_t device_idx = 0; device_idx < HYPERX_NUM_DEVICES; device_idx++)
    {
        dev = NULL;

        info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        //Look for HyperX RGB Peripheral
        while(info)
        {
            if((info->vendor_id        == device_list[device_idx].usb_vid)
             &&(info->product_id       == device_list[device_idx].usb_pid)
    #ifdef USE_HID_USAGE
            &&(info->interface_number == device_list[device_idx].usb_interface)
            &&(info->usage_page       == 0xFF01))
    #else
            &&(info->interface_number == device_list[device_idx].usb_interface))
    #endif
            {
                dev = hid_open_path(info->path);

                if( dev )
                {
                    HyperXPulsefireSurgeController* controller = new HyperXPulsefireSurgeController(dev);

                    RGBController_HyperXPulsefireSurge* rgb_controller = new RGBController_HyperXPulsefireSurge(controller);

                    rgb_controller->name = device_list[device_idx].name;
                    
                    rgb_controllers.push_back(rgb_controller);
                }
            }

            info = info->next;
        }
    }
}   /* DetectHyperXMouseControllers() */

REGISTER_DETECTOR("HyperX Mouse", DetectHyperXMouseControllers);
