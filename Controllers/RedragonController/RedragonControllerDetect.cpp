#include "RedragonK556Controller.h"
#include "RedragonM711Controller.h"
#include "RGBController.h"
#include "RGBController_RedragonK556.h"
#include "RGBController_RedragonM711.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define REDRAGON_K556_VID               0x0C45
#define REDRAGON_K556_PID               0x5004

/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define REDRAGON_M711_VID               0x04D9
#define REDRAGON_M711_PID               0xFC30

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned char   usb_interface;
    device_type     type;
    const char *    name;
} redragon_device;

#define REDRAGON_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const redragon_device device_list[] =
{
    /*-----------------------------------------------------------------------------------------------------*\
    | Keyboards                                                                                             |
    \*-----------------------------------------------------------------------------------------------------*/
    { REDRAGON_K556_VID,    REDRAGON_K556_PID,  1,  DEVICE_TYPE_KEYBOARD,   "Redragon K556 Devarajas"       },
    /*-----------------------------------------------------------------------------------------------------*\
    | Mice                                                                                                  |
    \*-----------------------------------------------------------------------------------------------------*/
    { REDRAGON_M711_VID,    REDRAGON_M711_PID,  2,  DEVICE_TYPE_MOUSE,      "Redragon M711 Cobra"           },
    /*-----------------------------------------------------------------------------------------------------*\
    | Mousemats                                                                                             |
    \*-----------------------------------------------------------------------------------------------------*/
};

/******************************************************************************************\
*                                                                                          *
*   DetectRedragonControllers                                                              *
*                                                                                          *
*       Tests the USB address to see if a Redragon RGB Keyboard controller exists there.   *
*                                                                                          *
\******************************************************************************************/

void DetectRedragonControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev;

    hid_init();

    for(int device_idx = 0; device_idx < REDRAGON_NUM_DEVICES; device_idx++)
    {
        dev = NULL;

        info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        //Look for Redragon RGB Peripheral
        while(info)
        {
            if((info->vendor_id == device_list[device_idx].usb_vid)
            &&(info->product_id == device_list[device_idx].usb_pid)
            &&(info->interface_number == device_list[device_idx].usb_interface))
            {
                dev = hid_open_path(info->path);
                break;
            }
            else
            {
                info = info->next;
            }
        }

        if( dev )
        {
            switch(device_list[device_idx].type)
            {
                case DEVICE_TYPE_KEYBOARD:
                    {
                    RedragonK556Controller* controller = new RedragonK556Controller(dev);

                    RGBController_RedragonK556* rgb_controller = new RGBController_RedragonK556(controller);
                    rgb_controllers.push_back(rgb_controller);
                    }
                    break;

                case DEVICE_TYPE_MOUSE:
                    {
                    RedragonM711Controller* controller = new RedragonM711Controller(dev);

                    RGBController_RedragonM711* rgb_controller = new RGBController_RedragonM711(controller);
                    rgb_controllers.push_back(rgb_controller);
                    }
                    break;
            }
        }
    }
}
