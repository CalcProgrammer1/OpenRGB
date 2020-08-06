#include "Detector.h"
#include "HoltekA070Controller.h"
#include "RGBController.h"
#include "RGBController_HoltekA070.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Holtek Semiconductor Inc. vendor ID                   |
\*-----------------------------------------------------*/
#define HOLTEK_VID                    0x04D9
/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define HOLTEK_A070_PID               0xA070

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned char   usb_interface;
    device_type     type;
    const char *    name;
} holtek_device;

#define HOLTEK_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const holtek_device device_list[] =
{
    /*-------------------------------------------------------------------------------------------------------------*\
    | Mice                                                                                                          |
    \*-------------------------------------------------------------------------------------------------------------*/
    { HOLTEK_VID,             HOLTEK_A070_PID,      1,  DEVICE_TYPE_MOUSE,      "Holtek USB Gaming Mouse"         },
};

void DetectHoltekControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_init();

    for(int device_idx = 0; device_idx < HOLTEK_NUM_DEVICES; device_idx++)
    {
        switch(device_list[device_idx].type)
        {
        case DEVICE_TYPE_MOUSE:
            {
                hid_device_info* info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

                while(info)
                {
                    if((info->vendor_id == device_list[device_idx].usb_vid)
                    &&(info->product_id == device_list[device_idx].usb_pid)
#ifdef USE_HID_USAGE                    
                    &&(info->interface_number == device_list[device_idx].usb_interface)
                    &&(info->usage_page == 0xFF00)
                    &&(info->usage == 2))
#else
                    &&(info->interface_number == device_list[device_idx].usb_interface))
#endif
                    {
                        hid_device* dev = hid_open_path(info->path);

                        if(dev)
                        {
                            switch(device_list[device_idx].usb_pid)
                            {
                            case HOLTEK_A070_PID:
                                {
                                    HoltekA070Controller* controller = new HoltekA070Controller(dev);

                                    RGBController_HoltekA070* rgb_controller = new RGBController_HoltekA070(controller);

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
            break;
        }

    }
}   /* DetectHoltekControllers() */

REGISTER_DETECTOR("Holtek", DetectHoltekControllers);
