#include "Detector.h"
#include "CorsairLightingNodeController.h"
#include "RGBController.h"
#include "RGBController_CorsairLightingNode.h"
#include <vector>
#include <hidapi/hidapi.h>

#define CORSAIR_VID                     0x1B1C
#define CORSAIR_LIGHTING_NODE_CORE_PID  0x0C1A
#define CORSAIR_LIGHTING_NODE_PRO_PID   0x0C0B
#define CORSAIR_COMMANDER_PRO_PID       0x0C10
#define CORSAIR_LS100_PID               0x0C1E
#define CORSAIR_1000D_OBSIDIAN_PID      0x1D00
#define CORSAIR_SPEC_OMEGA_RGB_PID      0x1D04

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned char   channel_count;
    const char *    name;
} corsair_node_device;

#define CORSAIR_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const corsair_node_device device_list[] =
{
    { CORSAIR_VID,          CORSAIR_LIGHTING_NODE_CORE_PID,     1,      "Corsair Lighting Node Core"    },
    { CORSAIR_VID,          CORSAIR_LIGHTING_NODE_PRO_PID,      2,      "Corsair Lighting Node Pro"     },
    { CORSAIR_VID,          CORSAIR_COMMANDER_PRO_PID,          2,      "Corsair Commander Pro"         },
    { CORSAIR_VID,          CORSAIR_LS100_PID,                  1,      "Corsair LS100 Lighting Kit"    },
    { CORSAIR_VID,          CORSAIR_1000D_OBSIDIAN_PID,         2,      "Corsair 1000D Obsidian"        },
    { CORSAIR_VID,          CORSAIR_SPEC_OMEGA_RGB_PID,         2,      "Corsair SPEC OMEGA RGB"        }
};

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairLightingNodeControllers                                                   *
*                                                                                          *
*       Detect devices supported by the Corsair Lighting Node Pro driver                   *
*                                                                                          *
\******************************************************************************************/

void DetectCorsairLightingNodeControllers(std::vector<RGBController*> &rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev;

    hid_init();

    for(std::size_t device_idx = 0; device_idx < CORSAIR_NUM_DEVICES; device_idx++)
    {
        dev = NULL;

        info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        //Look for Corsair Lighting Node Devices
        while(info)
        {
            if((info->vendor_id == device_list[device_idx].usb_vid)
            &&(info->product_id == device_list[device_idx].usb_pid))
            {
                dev = hid_open_path(info->path);

                if( dev )
                {
                    CorsairLightingNodeController* controller = new CorsairLightingNodeController(dev);

                    RGBController_CorsairLightingNode* rgb_controller = new RGBController_CorsairLightingNode(controller);

                    rgb_controller->name = device_list[device_idx].name;

                    rgb_controllers.push_back(rgb_controller);
                }
            }

            info = info->next;
        }
    }
}   /* DetectCorsairLightingNodeControllers() */

REGISTER_DETECTOR("Corsair Lighting Node", DetectCorsairLightingNodeControllers);
