#include "SteelSeriesRivalController.h"
#include "SteelSeriesGeneric.h"
#include "RGBController.h"
#include "RGBController_SteelSeriesRival.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define STEELSERIES_RIVAL_VID                       0x1038
#define STEELSERIES_RIVAL_100_PID                   0x1702
#define STEELSERIES_RIVAL_100_DOTA_PID              0x170c
#define STEELSERIES_RIVAL_105_PID                   0x1814
#define STEELSERIES_RIVAL_110_PID                   0x1729
#define STEELSERIES_RIVAL_300_PID                   0x1384
#define ACER_PREDATOR_RIVAL_300_PID                 0x1714
#define STEELSERIES_RIVAL_300_CSGO_PID              0x1394
#define STEELSERIES_RIVAL_300_CSGO_STM32_PID        0x1716
#define STEELSERIES_RIVAL_300_CSGO_HYPERBEAST_PID   0x171a
#define STEELSERIES_RIVAL_300_DOTA_PID              0x1392
#define STEELSERIES_RIVAL_300_HP_PID                0x1718
#define STEELSERIES_RIVAL_300_BLACKOPS_PID          0x1710

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned char   usb_interface;
    device_type     type;
    steelseries_type    proto_type;
    const char *    name;
} steelseries_device;

#define STEELSERIES_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const steelseries_device device_list[] = 
{
    /*-------------------------------------------------------------------------------------------------------------------------------------------------------------*\
    | Mice                                                                                                                                                          |
    \*-------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    { STEELSERIES_RIVAL_VID,    STEELSERIES_RIVAL_100_PID,                  0,  DEVICE_TYPE_MOUSE,  RIVAL_100,  "SteelSeries Rival 100"                             },
    { STEELSERIES_RIVAL_VID,    STEELSERIES_RIVAL_100_DOTA_PID,             0,  DEVICE_TYPE_MOUSE,  RIVAL_100,  "SteelSeries Rival 100 DotA 2 Edition"              },
    { STEELSERIES_RIVAL_VID,    STEELSERIES_RIVAL_105_PID,                  0,  DEVICE_TYPE_MOUSE,  RIVAL_100,  "SteelSeries Rival 105"                             },
    { STEELSERIES_RIVAL_VID,    STEELSERIES_RIVAL_110_PID,                  0,  DEVICE_TYPE_MOUSE,  RIVAL_100,  "SteelSeries Rival 110"                             },
    { STEELSERIES_RIVAL_VID,    STEELSERIES_RIVAL_300_PID,                  0,  DEVICE_TYPE_MOUSE,  RIVAL_300,  "SteelSeries Rival 300"                             },
    { STEELSERIES_RIVAL_VID,    ACER_PREDATOR_RIVAL_300_PID,                0,  DEVICE_TYPE_MOUSE,  RIVAL_300,  "Acer Predator Gaming Mouse (Rival 300)"            },
    { STEELSERIES_RIVAL_VID,    STEELSERIES_RIVAL_300_CSGO_PID,             0,  DEVICE_TYPE_MOUSE,  RIVAL_300,  "SteelSeries Rival 300 CS:GO Fade Edition"          },
    { STEELSERIES_RIVAL_VID,    STEELSERIES_RIVAL_300_CSGO_STM32_PID,       0,  DEVICE_TYPE_MOUSE,  RIVAL_300,  "SteelSeries Rival 300 CS:GO Fade Edition (stm32)"  },
    { STEELSERIES_RIVAL_VID,    STEELSERIES_RIVAL_300_CSGO_HYPERBEAST_PID,  0,  DEVICE_TYPE_MOUSE,  RIVAL_300,  "SteelSeries Rival 300 CS:GO Hyperbeast Edition"    },
    { STEELSERIES_RIVAL_VID,    STEELSERIES_RIVAL_300_DOTA_PID,             0,  DEVICE_TYPE_MOUSE,  RIVAL_300,  "SteelSeries Rival 300 Dota 2 Edition"              },
    { STEELSERIES_RIVAL_VID,    STEELSERIES_RIVAL_300_HP_PID,               0,  DEVICE_TYPE_MOUSE,  RIVAL_300,  "SteelSeries Rival 300 HP Omen Edition"             },
    { STEELSERIES_RIVAL_VID,    STEELSERIES_RIVAL_300_BLACKOPS_PID,         0,  DEVICE_TYPE_MOUSE,  RIVAL_300,  "SteelSeries Rival 300 Black Ops Edition"           },
};

/******************************************************************************************\
*                                                                                          *
*   DetectSteelSeriesControllers                                                           *
*                                                                                          *
*       Tests the USB address to find a SteelSeries Rival Mouse                            *
*                                                                                          *
\******************************************************************************************/

void DetectSteelSeriesControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev;

    hid_init();

    for(int device_idx = 0; device_idx < STEELSERIES_NUM_DEVICES; device_idx++)
    {
        dev = NULL;

        info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        //Look for SteelSeries RGB Peripheral
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

        if(dev)
        {
            switch(device_list[device_idx].type)
            {
                case DEVICE_TYPE_KEYBOARD:
                    /* Not yet supported */
                    break;

                case DEVICE_TYPE_HEADSET:
                    /* Not yet supported */
                    break;

                case DEVICE_TYPE_MOUSE:
                    {
                    SteelSeriesRivalController* controller = new SteelSeriesRivalController(dev, device_list[device_idx].proto_type);

                    RGBController_SteelSeriesRival* rgb_controller = new RGBController_SteelSeriesRival(controller);
                    rgb_controller->name = device_list[device_idx].name;
                    rgb_controllers.push_back(rgb_controller);
                    }
                    break;

                case DEVICE_TYPE_MOUSEMAT:
                    /* Not yet supported */
                    break;
            }
        }
    }
}
