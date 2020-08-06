#include "Detector.h"
#include "SteelSeriesRivalController.h"
#include "SteelSeriesSiberiaController.h"
#include "SteelSeriesApexController.h"
#include "SteelSeriesGeneric.h"
#include "RGBController.h"
#include "RGBController_SteelSeriesRival.h"
#include "RGBController_SteelSeriesSiberia.h"
#include "RGBController_SteelSeriesApex.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Vendor ID                                             |
\*-----------------------------------------------------*/
#define STEELSERIES_VID                             0x1038
/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define STEELSERIES_RIVAL_100_PID                   0x1702
#define STEELSERIES_RIVAL_100_DOTA_PID              0x170c
#define STEELSERIES_RIVAL_105_PID                   0x1814
#define STEELSERIES_RIVAL_110_PID                   0x1729
#define STEELSERIES_RIVAL_300_PID                   0x1710
#define ACER_PREDATOR_RIVAL_300_PID                 0x1714
#define STEELSERIES_RIVAL_300_CSGO_PID              0x1394
#define STEELSERIES_RIVAL_300_CSGO_STM32_PID        0x1716
#define STEELSERIES_RIVAL_300_CSGO_HYPERBEAST_PID   0x171a
#define STEELSERIES_RIVAL_300_DOTA_PID              0x1392
#define STEELSERIES_RIVAL_300_HP_PID                0x1718
#define STEELSERIES_RIVAL_300_BLACKOPS_PID          0x1710
/*-----------------------------------------------------*\
| Headset product IDs                                   |
\*-----------------------------------------------------*/
#define STEELSERIES_SIBERIA_350_PID                 0x1229
/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define STEELSERIES_APEX_7_PID                      0x1612
#define STEELSERIES_APEX_7_TKL_PID                  0x1618

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
    /*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*\
    | Mice                                                                                                                                                              |
    \*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    { STEELSERIES_VID,      STEELSERIES_RIVAL_100_PID,                  0,  DEVICE_TYPE_MOUSE,      RIVAL_100,      "SteelSeries Rival 100"                             },
    { STEELSERIES_VID,      STEELSERIES_RIVAL_100_DOTA_PID,             0,  DEVICE_TYPE_MOUSE,      RIVAL_100,      "SteelSeries Rival 100 DotA 2 Edition"              },
    { STEELSERIES_VID,      STEELSERIES_RIVAL_105_PID,                  0,  DEVICE_TYPE_MOUSE,      RIVAL_100,      "SteelSeries Rival 105"                             },
    { STEELSERIES_VID,      STEELSERIES_RIVAL_110_PID,                  0,  DEVICE_TYPE_MOUSE,      RIVAL_100,      "SteelSeries Rival 110"                             },
    { STEELSERIES_VID,      STEELSERIES_RIVAL_300_PID,                  0,  DEVICE_TYPE_MOUSE,      RIVAL_300,      "SteelSeries Rival 300"                             },
    { STEELSERIES_VID,      ACER_PREDATOR_RIVAL_300_PID,                0,  DEVICE_TYPE_MOUSE,      RIVAL_300,      "Acer Predator Gaming Mouse (Rival 300)"            },
    { STEELSERIES_VID,      STEELSERIES_RIVAL_300_CSGO_PID,             0,  DEVICE_TYPE_MOUSE,      RIVAL_300,      "SteelSeries Rival 300 CS:GO Fade Edition"          },
    { STEELSERIES_VID,      STEELSERIES_RIVAL_300_CSGO_STM32_PID,       0,  DEVICE_TYPE_MOUSE,      RIVAL_300,      "SteelSeries Rival 300 CS:GO Fade Edition (stm32)"  },
    { STEELSERIES_VID,      STEELSERIES_RIVAL_300_CSGO_HYPERBEAST_PID,  0,  DEVICE_TYPE_MOUSE,      RIVAL_300,      "SteelSeries Rival 300 CS:GO Hyperbeast Edition"    },
    { STEELSERIES_VID,      STEELSERIES_RIVAL_300_DOTA_PID,             0,  DEVICE_TYPE_MOUSE,      RIVAL_300,      "SteelSeries Rival 300 Dota 2 Edition"              },
    { STEELSERIES_VID,      STEELSERIES_RIVAL_300_HP_PID,               0,  DEVICE_TYPE_MOUSE,      RIVAL_300,      "SteelSeries Rival 300 HP Omen Edition"             },
    { STEELSERIES_VID,      STEELSERIES_RIVAL_300_BLACKOPS_PID,         0,  DEVICE_TYPE_MOUSE,      RIVAL_300,      "SteelSeries Rival 300 Black Ops Edition"           },
    /*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*\
    | Headsets                                                                                                                                                          |
    \*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    { STEELSERIES_VID,      STEELSERIES_SIBERIA_350_PID,                3,  DEVICE_TYPE_HEADSET,    SIBERIA_350,    "SteelSeries Siberia 350"                           },
    /*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*\
    | Keyboards                                                                                                                                                         |
    \*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    { STEELSERIES_VID,      STEELSERIES_APEX_7_PID,                     1,  DEVICE_TYPE_KEYBOARD,   APEX_7,         "SteelSeries Apex 7"                                },
    { STEELSERIES_VID,      STEELSERIES_APEX_7_TKL_PID,                 1,  DEVICE_TYPE_KEYBOARD,   APEX_7_TKL,     "SteelSeries Apex 7 TKL"                            },
};

/******************************************************************************************\
*                                                                                          *
*   DetectSteelSeriesControllers                                                           *
*                                                                                          *
*       Tests the USB address to find a SteelSeries Rival Mouse, Headset or Keyboard       *
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
                    {
                    SteelSeriesApexController* controller = new SteelSeriesApexController(dev, device_list[device_idx].proto_type);
                    
                    RGBController_SteelSeriesApex* rgb_controller = new RGBController_SteelSeriesApex(controller);
                    rgb_controller->name = device_list[device_idx].name;
                    rgb_controllers.push_back(rgb_controller);
                    }
                    break;

                case DEVICE_TYPE_HEADSET:
                    {
                    SteelSeriesSiberiaController* controller = new SteelSeriesSiberiaController(dev);

                    RGBController_SteelSeriesSiberia* rgb_controller = new RGBController_SteelSeriesSiberia(controller);
                    rgb_controller->name = device_list[device_idx].name;
                    rgb_controllers.push_back(rgb_controller);
                    }
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
}   /* DetectSteelSeriesControllers() */

REGISTER_DETECTOR("SteelSeries", DetectSteelSeriesControllers);
