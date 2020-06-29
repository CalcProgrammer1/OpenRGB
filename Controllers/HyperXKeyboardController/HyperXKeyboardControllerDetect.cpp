#include "HyperXAlloyOriginsController.h"
#include "HyperXKeyboardController.h"
#include "RGBController.h"
#include "RGBController_HyperXAlloyOrigins.h"
#include "RGBController_HyperXKeyboard.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| HyperX keyboard vendor IDs                            |
\*-----------------------------------------------------*/
#define HYPERX_KEYBOARD_VID         0x0951
#define HYPERX_ALLOY_ELITE_PID      0x16BE
#define HYPERX_ALLOY_FPS_RGB_PID    0x16DC
#define HYPERX_ALLOY_ORIGINS_PID    0x16E5

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
    | Keyboards                                                                                             |
    \*-----------------------------------------------------------------------------------------------------*/
    { HYPERX_KEYBOARD_VID,  HYPERX_ALLOY_ELITE_PID,     2,      "HyperX Alloy Elite RGB"                    },
    { HYPERX_KEYBOARD_VID,  HYPERX_ALLOY_FPS_RGB_PID,   2,      "HyperX Alloy FPS RGB"                      },
    { HYPERX_KEYBOARD_VID,  HYPERX_ALLOY_ORIGINS_PID,   3,      "HyperX Alloy Origins"                      },
};

/******************************************************************************************\
*                                                                                          *
*   DetectHyperXKeyboardControllers                                                        *
*                                                                                          *
*       Tests the USB address to see if a HyperX Keyboard controller exists there.         *
*                                                                                          *
\******************************************************************************************/

void DetectHyperXKeyboardControllers(std::vector<RGBController*>& rgb_controllers)
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
            if((device_list[device_idx].usb_pid != HYPERX_ALLOY_ORIGINS_PID)
            &&(info->vendor_id == device_list[device_idx].usb_vid)
            &&(info->product_id == device_list[device_idx].usb_pid)
#ifdef USE_HID_USAGE
            &&(info->interface_number == device_list[device_idx].usb_interface)
            &&(info->usage_page == 0xFF01))
#else
            &&(info->interface_number == device_list[device_idx].usb_interface))
#endif
            {
                dev = hid_open_path(info->path);

                if( dev )
                {
                    HyperXKeyboardController* controller = new HyperXKeyboardController(dev);

                    RGBController_HyperXKeyboard* rgb_controller = new RGBController_HyperXKeyboard(controller);

                    rgb_controller->name = device_list[device_idx].name;

                    rgb_controllers.push_back(rgb_controller);
                }
            }

            if((device_list[device_idx].usb_pid == HYPERX_ALLOY_ORIGINS_PID)
            &&(info->vendor_id == device_list[device_idx].usb_vid)
            &&(info->product_id == device_list[device_idx].usb_pid)
            &&(info->interface_number == device_list[device_idx].usb_interface))
            {
                dev = hid_open_path(info->path);

                if( dev )
                {
                    HyperXAlloyOriginsController* controller = new HyperXAlloyOriginsController(dev);

                    RGBController_HyperXAlloyOrigins* rgb_controller = new RGBController_HyperXAlloyOrigins(controller);

                    rgb_controller->name = device_list[device_idx].name;
                    
                    rgb_controllers.push_back(rgb_controller);
                }
            }

            info = info->next;
        }
    }
}
