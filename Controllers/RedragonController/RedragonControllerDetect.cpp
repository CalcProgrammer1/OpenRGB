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
#define REDRAGON_KEYBOARD_VID           0x0C45
#define REDRAGON_KEYBOARD_USAGE_PAGE    0xFF1C
#define REDRAGON_K550_PID               0x5204
#define REDRAGON_K552_PID               0x5104
#define REDRAGON_K556_PID               0x5004
#define TECWARE_PHANTOM_ELITE_PID       0x652F
#define WARRIOR_KANE_TC235              0x8520

/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define REDRAGON_MOUSE_VID              0x04D9
#define REDRAGON_MOUSE_USAGE_PAGE       0xFFA0
#define REDRAGON_M711_PID               0xFC30
#define REDRAGON_M715_PID               0xFC39

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned char   usb_interface;
    unsigned short  usb_usage_page;
    device_type     type;
    const char *    name;
} redragon_device;

#define REDRAGON_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const redragon_device device_list[] =
{
    /*---------------------------------------------------------------------------------------------------------------------------------------------*\
    | Keyboards                                                                                                                                     |
    \*---------------------------------------------------------------------------------------------------------------------------------------------*/
    { REDRAGON_KEYBOARD_VID,    REDRAGON_K550_PID,          1,  REDRAGON_KEYBOARD_USAGE_PAGE,   DEVICE_TYPE_KEYBOARD,   "Redragon K550 Yama"        },
    { REDRAGON_KEYBOARD_VID,    REDRAGON_K552_PID,          1,  REDRAGON_KEYBOARD_USAGE_PAGE,   DEVICE_TYPE_KEYBOARD,   "Redragon K552 Kumara"      },
    { REDRAGON_KEYBOARD_VID,    REDRAGON_K556_PID,          1,  REDRAGON_KEYBOARD_USAGE_PAGE,   DEVICE_TYPE_KEYBOARD,   "Redragon K556 Devarajas"   },
    { REDRAGON_KEYBOARD_VID,    TECWARE_PHANTOM_ELITE_PID,  1,  REDRAGON_KEYBOARD_USAGE_PAGE,   DEVICE_TYPE_KEYBOARD,   "Tecware Phantom Elite"     },
    { REDRAGON_KEYBOARD_VID,    WARRIOR_KANE_TC235,         1,  REDRAGON_KEYBOARD_USAGE_PAGE,   DEVICE_TYPE_KEYBOARD,   "Warrior Kane TC235"        },
    /*---------------------------------------------------------------------------------------------------------------------------------------------*\
    | Mice                                                                                                                                          |
    \*---------------------------------------------------------------------------------------------------------------------------------------------*/
    { REDRAGON_MOUSE_VID,       REDRAGON_M711_PID,          2,  REDRAGON_MOUSE_USAGE_PAGE,      DEVICE_TYPE_MOUSE,      "Redragon M711 Cobra"       },
    { REDRAGON_MOUSE_VID,       REDRAGON_M715_PID,          2,  REDRAGON_MOUSE_USAGE_PAGE,      DEVICE_TYPE_MOUSE,      "Redragon M715 Dagger"      },
    /*---------------------------------------------------------------------------------------------------------------------------------------------*\
    | Mousemats                                                                                                                                     |
    \*---------------------------------------------------------------------------------------------------------------------------------------------*/
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
            if((info->vendor_id       == device_list[device_idx].usb_vid)
            &&(info->product_id       == device_list[device_idx].usb_pid)
#ifdef USE_HID_USAGE
            &&(info->interface_number == device_list[device_idx].usb_interface)
            &&(info->usage_page       == device_list[device_idx].usb_usage_page))
#else
            &&(info->interface_number == device_list[device_idx].usb_interface))
#endif
            {
                dev = hid_open_path(info->path);

                if( dev )
                {
                    switch(device_list[device_idx].type)
                    {
                        case DEVICE_TYPE_KEYBOARD:
                            {
                            RedragonK556Controller* controller = new RedragonK556Controller(dev);

                            RGBController_RedragonK556* rgb_controller = new RGBController_RedragonK556(controller);

                            rgb_controller->name = device_list[device_idx].name;
                            rgb_controllers.push_back(rgb_controller);
                            }
                            break;

                        case DEVICE_TYPE_MOUSE:
                            {
                            RedragonM711Controller* controller = new RedragonM711Controller(dev);

                            RGBController_RedragonM711* rgb_controller = new RGBController_RedragonM711(controller);

                            rgb_controller->name = device_list[device_idx].name;
                            rgb_controllers.push_back(rgb_controller);
                            }
                            break;
                    }
                }
            }
            info = info->next;
        }
    }
}
