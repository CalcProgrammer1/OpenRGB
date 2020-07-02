#include "CorsairPeripheralController.h"
#include "RGBController.h"
#include "RGBController_CorsairPeripheral.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Corsair vendor ID                                     |
\*-----------------------------------------------------*/
#define CORSAIR_VID                     0x1B1C

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
|   List taken from ckb-next                            |
|   Non-RGB keyboards were omitted from this list       |
\*-----------------------------------------------------*/
#define CORSAIR_K55_RGB_PID             0x1B3D

#define CORSAIR_K65_RGB_PID             0x1B17
#define CORSAIR_K65_LUX_RGB_PID         0x1B37
#define CORSAIR_K65_RGB_RAPIDFIRE_PID   0x1B39

#define CORSAIR_K68_RGB                 0x1B4F

#define CORSAIR_K70_RGB_PID             0x1B13
#define CORSAIR_K70_LUX_RGB_PID         0x1B33
#define CORSAIR_K70_RGB_RAPIDFIRE_PID   0x1B38
#define CORSAIR_K70_RGB_MK2_PID         0x1B49
#define CORSAIR_K70_RGB_MK2_SE_PID      0x1B6B
#define CORSAIR_K70_RGB_MK2_LP_PID      0x1B55

#define CORSAIR_K95_RGB_PID             0x1B11
#define CORSAIR_K95_PLATINUM_PID        0x1B2D

#define CORSAIR_STRAFE_PID              0x1B20
#define CORSAIR_STRAFE_MK2_PID          0x1B48

/*-----------------------------------------------------*\
| Mouse product IDs                                     |
|   List taken from ckb-next                            |
\*-----------------------------------------------------*/
#define CORSAIR_GLAIVE_RGB_PRO_PID      0x1B74
#define CORSAIR_HARPOON_RGB_PID         0x1B3C
#define CORSAIR_HARPOON_RGB_PRO_PID     0x1B75
#define CORSAIR_M65_PRO_PID             0x1B2E
#define CORSAIR_M65_RGB_ELITE_PID       0x1B5A

/*-----------------------------------------------------*\
| Mousepad product IDs                                  |
|   List taken from ckb-next                            |
\*-----------------------------------------------------*/
#define CORSAIR_MM800_RGB_POLARIS_PID   0x1B3B

/*-----------------------------------------------------*\
| Headset Stand product IDs                             |
|   List taken from ckb-next                            |
\*-----------------------------------------------------*/
#define CORSAIR_ST100_PID               0x0A34

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned char   usb_interface;
    const char *    name;
} corsair_node_device;

#define CORSAIR_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const corsair_node_device device_list[] =
{
    /*-----------------------------------------------------------------------------------------------------*\
    | Keyboards                                                                                             |
    \*-----------------------------------------------------------------------------------------------------*/
//  { CORSAIR_VID,          CORSAIR_K55_RGB_PID,                1,      "Corsair K55 RGB"                   }, //Not per-key, disabled for now
    { CORSAIR_VID,          CORSAIR_K65_RGB_PID,                1,      "Corsair K65 RGB"                   },
    { CORSAIR_VID,          CORSAIR_K65_LUX_RGB_PID,            1,      "Corsair K65 LUX RGB"               },
    { CORSAIR_VID,          CORSAIR_K65_RGB_RAPIDFIRE_PID,      1,      "Corsair K65 RGB RAPIDFIRE"         },
    { CORSAIR_VID,          CORSAIR_K68_RGB,                    1,      "Corsair K68 RGB"                   },
    { CORSAIR_VID,          CORSAIR_K70_RGB_PID,                1,      "Corsair K70 RGB"                   },
    { CORSAIR_VID,          CORSAIR_K70_LUX_RGB_PID,            1,      "Corsair K70 LUX RGB"               },
    { CORSAIR_VID,          CORSAIR_K70_RGB_RAPIDFIRE_PID,      1,      "Corsair K70 RGB RAPIDFIRE"         },
    { CORSAIR_VID,          CORSAIR_K70_RGB_MK2_PID,            1,      "Corsair K70 RGB MK.2"              },
    { CORSAIR_VID,          CORSAIR_K70_RGB_MK2_SE_PID,         1,      "Corsair K70 RGB MK.2 SE"           },
    { CORSAIR_VID,          CORSAIR_K70_RGB_MK2_LP_PID,         1,      "Corsair K70 RGB MK.2 Low Profile"  },
    { CORSAIR_VID,          CORSAIR_K95_RGB_PID,                1,      "Corsair K95 RGB"                   },
    { CORSAIR_VID,          CORSAIR_K95_PLATINUM_PID,           1,      "Corsair K95 RGB PLATINUM"          },
    { CORSAIR_VID,          CORSAIR_STRAFE_PID,                 1,      "Corsair Strafe"                    },
    { CORSAIR_VID,          CORSAIR_STRAFE_MK2_PID,             1,      "Corsair Strafe MK.2"               },
    /*-----------------------------------------------------------------------------------------------------*\
    | Mice                                                                                                  |
    \*-----------------------------------------------------------------------------------------------------*/
    { CORSAIR_VID,          CORSAIR_GLAIVE_RGB_PRO_PID,         1,      "Corsair Glaive RGB PRO"            },
    { CORSAIR_VID,          CORSAIR_HARPOON_RGB_PID,            1,      "Corsair Harpoon RGB"               },
    { CORSAIR_VID,          CORSAIR_HARPOON_RGB_PRO_PID,        1,      "Corsair Harpoon RGB PRO"           },
    { CORSAIR_VID,          CORSAIR_M65_PRO_PID,                1,      "Corsair M65 PRO"                   },
    { CORSAIR_VID,          CORSAIR_M65_RGB_ELITE_PID,          1,      "Corsair M65 RGB Elite"             },
    /*-----------------------------------------------------------------------------------------------------*\
    | Mousemats                                                                                             |
    \*-----------------------------------------------------------------------------------------------------*/
    { CORSAIR_VID,          CORSAIR_MM800_RGB_POLARIS_PID,      0,      "Corsair MM800 RGB Polaris"         },
    /*-----------------------------------------------------------------------------------------------------*\
    | Headset Stands                                                                                        |
    \*-----------------------------------------------------------------------------------------------------*/
    { CORSAIR_VID,          CORSAIR_ST100_PID,                  0,      "Corsair ST100 RGB"                 }
};

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairPeripheralControllers                                                     *
*                                                                                          *
*       Tests the USB address to see if a Corsair RGB Keyboard controller exists there.    *
*                                                                                          *
\******************************************************************************************/

void DetectCorsairPeripheralControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev;

    hid_init();

    for(std::size_t device_idx = 0; device_idx < CORSAIR_NUM_DEVICES; device_idx++)
    {
        dev = NULL;

        info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        //Look for Corsair RGB Peripheral
        while(info)
        {
            if((info->vendor_id == device_list[device_idx].usb_vid)
#ifdef USE_HID_USAGE
            &&(info->product_id == device_list[device_idx].usb_pid)
            &&(info->usage_page == 0xFFC2))
#else
            &&(info->product_id == device_list[device_idx].usb_pid)
            &&(info->interface_number == device_list[device_idx].usb_interface))
#endif
            {
                dev = hid_open_path(info->path);

                if( dev )
                {
                    CorsairPeripheralController* controller = new CorsairPeripheralController(dev);

                    if(controller->GetDeviceType() != DEVICE_TYPE_UNKNOWN)
                    {
                        RGBController_CorsairPeripheral* rgb_controller = new RGBController_CorsairPeripheral(controller);

                        rgb_controller->name = device_list[device_idx].name;

                        rgb_controllers.push_back(rgb_controller);
                    }
                }
            }
            info = info->next;
        }
    }
}
