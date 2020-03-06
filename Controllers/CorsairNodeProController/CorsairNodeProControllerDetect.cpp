#include "CorsairNodeProController.h"
#include "RGBController.h"
#include "RGBController_CorsairNodePro.h"
#include <vector>
#include <libusb-1.0/libusb.h>

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
    unsigned char   usb_endpoint;
    unsigned char   channel_count;
    const char *    name;
} corsair_node_device;

static const corsair_node_device device_list[6] =
{
    { CORSAIR_VID,          CORSAIR_LIGHTING_NODE_CORE_PID,     1,      1,      "Corsair Lighting Node Core"    },
    { CORSAIR_VID,          CORSAIR_LIGHTING_NODE_PRO_PID,      1,      2,      "Corsair Lighting Node Pro"     },
    { CORSAIR_VID,          CORSAIR_COMMANDER_PRO_PID,          2,      2,      "Corsair Commander Pro"         },
    { CORSAIR_VID,          CORSAIR_LS100_PID,                  1,      1,      "Corsair LS100 Lighting Kit"    },
    { CORSAIR_VID,          CORSAIR_1000D_OBSIDIAN_PID,         2,      2,      "Corsair 1000D Obsidian"        },
    { CORSAIR_VID,          CORSAIR_SPEC_OMEGA_RGB_PID,         1,      2,      "Corsair SPEC OMEGA RGB"        }

};

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairNodeProControllers                                                        *
*                                                                                          *
*       Detect devices supported by the Corsair Lighting Node Pro driver                   *
*                                                                                          *                                                                                          *
\******************************************************************************************/

void DetectCorsairNodeProControllers(std::vector<RGBController*> &rgb_controllers)
{
    libusb_context * ctx;
    libusb_init(&ctx);

    for(int device_idx = 0; device_idx < 6; device_idx++)
    {
        //Look for Corsair Lighting Node Device
        libusb_device_handle * dev = libusb_open_device_with_vid_pid(ctx, device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        if( dev )
        {
            libusb_detach_kernel_driver(dev, 0);
            libusb_claim_interface(dev, 0);

            CorsairNodeProController* controller = new CorsairNodeProController(dev, device_list[device_idx].usb_endpoint);

            RGBController_CorsairNodePro* rgb_controller = new RGBController_CorsairNodePro(controller);

            rgb_controller->name = device_list[device_idx].name;

            rgb_controllers.push_back(rgb_controller);
        }
    }
}   /* DetectCorsairNodeProControllers() */
