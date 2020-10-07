#include "Detector.h"
#include "WootingKeyboardController.h"
#include "RGBController.h"
#include "RGBController_WootingKeyboard.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Wooting vendor ID                                     |
\*-----------------------------------------------------*/
#define WOOTING_VID                       0x03EB

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define WOOTING_ONE_PID                   0xFF01
#define WOOTING_TWO_PID                   0xFF02

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    const char *    name;
} wooting_device;

#define WOOTING_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[0]))

static const wooting_device device_list[] =
{
    /*-----------------------------------------------------------------------*\
    | Keyboards                                                               |
    \*-----------------------------------------------------------------------*/
    { WOOTING_VID,              WOOTING_ONE_PID,              "Wooting One"   },
//  { WOOTING_VID,              WOOTING_TWO_PID,              "Wooting Two"   },
};

/******************************************************************************************\
*                                                                                          *
*   DetectWootingKeyboardControllers                                                       *
*                                                                                          *
*       Tests the USB address to see if a Wooting RGB Keyboard controller exists there.    *
*                                                                                          *
\******************************************************************************************/

void DetectWootingKeyboardControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev;

    hid_init();

    for(std::size_t device_idx = 0; device_idx < WOOTING_NUM_DEVICES; device_idx++)
    {
        dev = NULL;

        info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        /*-------------------------------------------------------------*\
        | The amount of interfaces is variable, so we need to look for  |
        | the configuration interface.  In the Wooting one keyboard the |
        | configuration interface is always 4 lower than the highest    |
        | number                                                        |
        \*-------------------------------------------------------------*/
        hid_device_info* hid_info_walker = info;

        unsigned char highestInterfaceNr = 0;
        while(hid_info_walker)
        {
            if(hid_info_walker->interface_number > highestInterfaceNr)
            {
                highestInterfaceNr = hid_info_walker->interface_number;
            }

            hid_info_walker = hid_info_walker->next;
        }

        unsigned char interfaceNr = highestInterfaceNr - 4;

        /*-------------------------------------------------------------*\
        | Look for Wooting keyboard                                     |
        \*-------------------------------------------------------------*/
        while(info)
        {
            if(info->interface_number == interfaceNr)
            {
                dev = hid_open_path(info->path);

                if(dev)
                {
                    WootingKeyboardController* controller = new WootingKeyboardController(dev);

                    RGBController_WootingKeyboard* rgb_controller = new RGBController_WootingKeyboard(controller);

                    rgb_controller->name = device_list[device_idx].name;
                    
                    rgb_controllers.push_back(rgb_controller);
                }
            }
            info = info->next;
        }
    }
}   /* DetectWootingKeyboardControllers() */

REGISTER_DETECTOR("Wooting Keyboard", DetectWootingKeyboardControllers);
