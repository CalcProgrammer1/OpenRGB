#include "Detector.h"
#include "RGBController.h"
#include "ATC800Controller.h"
#include "RGBController_AorusATC800.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Vendor ID                                             |
\*-----------------------------------------------------*/
#define HOLTEK_VID                            0x1044

/*-----------------------------------------------------*\
| Controller product ids                                |
\*-----------------------------------------------------*/
#define ATC_800_CONTROLLER_PID                0x7A42

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    char            usb_interface;
    device_type     type;
    const char *    name;
} device;

#define NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const device device_list[] =
{
    { HOLTEK_VID,             ATC_800_CONTROLLER_PID,                  0,  DEVICE_TYPE_KEYBOARD,   "Aorus ATC800 CPU Cooler"                             },
};

/******************************************************************************************\
*                                                                                          *
*   DetectAorusCPUCoolerControllers                                                        *
*                                                                                          *
*       Tests the USB address to see if a Aorus RGB CPU Cooler exists there.               *
*                                                                                          *
\******************************************************************************************/

void DetectAorusCPUCoolerControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_init();

    for(unsigned int device_idx = 0; device_idx < NUM_DEVICES; device_idx++)
    {
        hid_device_info* info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        while(info)
        {
            if((info->vendor_id == device_list[device_idx].usb_vid)
            &&(info->product_id == device_list[device_idx].usb_pid)
#ifdef USE_HID_USAGE                    
            &&(info->interface_number == device_list[device_idx].usb_interface)
            &&(info->usage_page == 0xFF01)
            &&(info->usage == 1))
#else
            &&(info->interface_number == device_list[device_idx].usb_interface))
#endif
            {
                hid_device* dev = hid_open_path(info->path);

                if(dev)
                {
                    switch(device_list[device_idx].usb_pid)
                    {
                    case ATC_800_CONTROLLER_PID:
                        {
                            ATC800Controller* controller = new ATC800Controller(dev, info->path);

                            RGBController_AorusATC800* rgb_controller = new RGBController_AorusATC800(controller);

                            rgb_controller->name = device_list[device_idx].name;
                            rgb_controllers.push_back(rgb_controller);
                        }
                        break;
                    }
                }
            }
        info = info->next;
        }
        hid_free_enumeration(info);
    }
}

REGISTER_DETECTOR("Aorus CPU Coolers", DetectAorusCPUCoolerControllers);
