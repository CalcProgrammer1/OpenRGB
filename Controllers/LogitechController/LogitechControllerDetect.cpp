#include "LogitechG203Controller.h"
#include "RGBController.h"
#include "RGBController_LogitechG203.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/

/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define LOGITECH_MOUSE_VID              0x046D
#define LOGITECH_G203_PID               0xC084

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned char   usb_interface;
    device_type     type;
    const char *    name;
} logitech_device;

#define LOGITECH_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const logitech_device device_list[] =
{
    /*-----------------------------------------------------------------------------------------------------*\
    | Keyboards                                                                                             |
    \*-----------------------------------------------------------------------------------------------------*/
    /*-----------------------------------------------------------------------------------------------------*\
    | Mice                                                                                                  |
    \*-----------------------------------------------------------------------------------------------------*/
    { LOGITECH_MOUSE_VID,       LOGITECH_G203_PID,  1,  DEVICE_TYPE_MOUSE,      "Logitech G203 Prodigy"     },
    /*-----------------------------------------------------------------------------------------------------*\
    | Mousemats                                                                                             |
    \*-----------------------------------------------------------------------------------------------------*/
};

/******************************************************************************************\
*                                                                                          *
*   DetectLogitechControllers                                                              *
*                                                                                          *
*       Tests the USB address to see if a Logitech RGB Keyboard controller exists there.   *
*                                                                                          *
\******************************************************************************************/

void DetectLogitechControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev;

    hid_init();

    for(int device_idx = 0; device_idx < LOGITECH_NUM_DEVICES; device_idx++)
    {
        dev = NULL;

        info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        //Look for Logitech RGB Peripheral
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
                    // {
                    // RedragonK556Controller* controller = new RedragonK556Controller(dev);

                    // RGBController_RedragonK556* rgb_controller = new RGBController_RedragonK556(controller);

                    // rgb_controller->name = device_list[device_idx].name;
                    // rgb_controllers.push_back(rgb_controller);
                    // }
                    break;

                case DEVICE_TYPE_MOUSE:
                    {
                    LogitechG203Controller* controller = new LogitechG203Controller(dev);

                    RGBController_LogitechG203* rgb_controller = new RGBController_LogitechG203(controller);

                    rgb_controller->name = device_list[device_idx].name;
                    rgb_controllers.push_back(rgb_controller);
                    }
                    break;
            }
        }
    }
}
