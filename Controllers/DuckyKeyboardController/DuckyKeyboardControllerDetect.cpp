#include "Detector.h"
#include "DuckyKeyboardController.h"
#include "RGBController.h"
#include "RGBController_DuckyKeyboard.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Ducky vendor ID                                       |
\*-----------------------------------------------------*/
#define DUCKY_VID                       0x04D9

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define DUCKY_SHINE_7_ONE_2_RGB_PID     0x0348
#define DUCKY_ONE_2_RGB_TKL_PID         0x0356

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned char   usb_interface;
    const char *    name;
} ducky_device;

#define DUCKY_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const ducky_device device_list[] =
{
    /*-----------------------------------------------------------------------------------------------------*\
    | Keyboards                                                                                             |
    \*-----------------------------------------------------------------------------------------------------*/
    { DUCKY_VID,            DUCKY_SHINE_7_ONE_2_RGB_PID,        1,      "Ducky Shine 7/Ducky One 2 RGB"     },
    { DUCKY_VID,            DUCKY_ONE_2_RGB_TKL_PID,            1,      "Ducky One 2 RGB TKL"               },
};

/******************************************************************************************\
*                                                                                          *
*   DetectDuckyKeyboardControllers                                                         *
*                                                                                          *
*       Tests the USB address to see if a Ducky RGB Keyboard controller exists there.      *
*                                                                                          *
\******************************************************************************************/

void DetectDuckyKeyboardControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev;

    hid_init();

    for(std::size_t device_idx = 0; device_idx < DUCKY_NUM_DEVICES; device_idx++)
    {
        dev = NULL;

        info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        //Look for Ducky RGB Peripheral
        while(info)
        {
            if((info->vendor_id == device_list[device_idx].usb_vid)
            &&(info->product_id == device_list[device_idx].usb_pid)
            &&(info->interface_number == device_list[device_idx].usb_interface))
            {
                dev = hid_open_path(info->path);

                if( dev )
                {
                    DuckyKeyboardController* controller = new DuckyKeyboardController(dev);

                    RGBController_DuckyKeyboard* rgb_controller = new RGBController_DuckyKeyboard(controller);

                    rgb_controller->name = device_list[device_idx].name;
                    
                    rgb_controllers.push_back(rgb_controller);
                }
            }
            info = info->next;
        }
    }
}   /* DetectDuckyKeyboardControllers() */

REGISTER_DETECTOR("Ducky Keyboard", DetectDuckyKeyboardControllers);
