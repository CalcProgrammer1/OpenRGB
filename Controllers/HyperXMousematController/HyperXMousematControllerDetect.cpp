#include "Detector.h"
#include "HyperXMousematController.h"
#include "RGBController.h"
#include "RGBController_HyperXMousemat.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| HyperX mousemat vendor IDs                            |
\*-----------------------------------------------------*/
#define HYPERX_VID                  0x0951
#define HYPERX_FURY_ULTRA_PID       0x1705

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
    | Mousemats                                                                                             |
    \*-----------------------------------------------------------------------------------------------------*/
    { HYPERX_VID,           HYPERX_FURY_ULTRA_PID,      0,      "HyperX Fury Ultra"                         },
};

/******************************************************************************************\
*                                                                                          *
*   DetectHyperXMousematControllers                                                        *
*                                                                                          *
*       Tests the USB address to see if a HyperX Mousemat controller exists there.         *
*                                                                                          *
\******************************************************************************************/

void DetectHyperXMousematControllers(std::vector<RGBController*>& rgb_controllers)
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
            &&(info->interface_number == device_list[device_idx].usb_interface))
            {
                dev = hid_open_path(info->path);

                if( dev )
                {
                    HyperXMousematController* controller = new HyperXMousematController(dev, info->path);

                    RGBController_HyperXMousemat* rgb_controller = new RGBController_HyperXMousemat(controller);

                    rgb_controller->name = device_list[device_idx].name;
                    
                    rgb_controllers.push_back(rgb_controller);
                }
            }

            info = info->next;
        }
    }
}   /* DetectHyperXMousematControllers() */

REGISTER_DETECTOR("HyperX Mousemat", DetectHyperXMousematControllers);
