#include "Detector.h"
#include "NZXTHue2Controller.h"
#include "RGBController.h"
#include "RGBController_NZXTHue2.h"
#include <vector>
#include <hidapi/hidapi.h>

#define NZXT_VID                    0x1E71
#define NZXT_HUE_2_PID              0x2001
#define NZXT_HUE_2_AMBIENT_PID      0x2002
#define NZXT_SMART_DEVICE_V2_PID    0x2006
#define NZXT_RGB_FAN_CONTROLLER_PID 0x2009

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned int    num_rgb_channels;
    unsigned int    num_fan_channels;
    const char *    name;
} nzxt_hue_2_device;

#define NZXT_HUE_2_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const nzxt_hue_2_device device_list[] =
{
    /*-----------------------------------------------------------------------------------------------------*\
    | NZXT Hue 2 devices                                                                                    |
    \*-----------------------------------------------------------------------------------------------------*/
    { NZXT_VID,             NZXT_HUE_2_PID,                 4,  0,      "NZXT Hue 2"                        },
    { NZXT_VID,             NZXT_HUE_2_AMBIENT_PID,         2,  0,      "NZXT Hue 2 Ambient"                },
    { NZXT_VID,             NZXT_SMART_DEVICE_V2_PID,       2,  3,      "NZXT Smart Device V2"              },
    { NZXT_VID,             NZXT_RGB_FAN_CONTROLLER_PID,    2,  3,      "NZXT RGB & Fan Controller"         },
};

/******************************************************************************************\
*                                                                                          *
*   DetectNZXTHue2Controllers                                                              *
*                                                                                          *
*       Detect devices supported by the NZXT Hue 2 driver                                  *
*                                                                                          *
\******************************************************************************************/

void DetectNZXTHue2Controllers(std::vector<RGBController*> &rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev;

    hid_init();

    for(std::size_t device_idx = 0; device_idx < NZXT_HUE_2_NUM_DEVICES; device_idx++)
    {
        dev = NULL;

        info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        //Look for NZXT Hue 2 devices
        while(info)
        {
            if((info->vendor_id == device_list[device_idx].usb_vid)
            &&(info->product_id == device_list[device_idx].usb_pid))
            {
                dev = hid_open_path(info->path);

                if( dev )
                {
                    NZXTHue2Controller* controller = new NZXTHue2Controller(dev, device_list[device_idx].num_rgb_channels, device_list[device_idx].num_fan_channels);

                    RGBController_NZXTHue2* rgb_controller = new RGBController_NZXTHue2(controller);

                    rgb_controller->name = device_list[device_idx].name;
                    
                    rgb_controllers.push_back(rgb_controller);
                }
            }
            info = info->next;
        }
    }
}   /* DetectNZXTHue2Controllers() */

REGISTER_DETECTOR("NZXT Hue 2", DetectNZXTHue2Controllers);
