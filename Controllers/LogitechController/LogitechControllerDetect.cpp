#include "Detector.h"
#include "LogitechG203Controller.h"
#include "LogitechG203LController.h"
#include "LogitechG403Controller.h"
#include "LogitechG502PSController.h"
#include "LogitechG810Controller.h"
#include "LogitechG213Controller.h"
#include "LogitechGProWirelessController.h"
#include "LogitechGPowerPlayController.h"
#include "RGBController.h"
#include "RGBController_LogitechG203.h"
#include "RGBController_LogitechG203L.h"
#include "RGBController_LogitechG403.h"
#include "RGBController_LogitechG502PS.h"
#include "RGBController_LogitechG810.h"
#include "RGBController_LogitechG213.h"
#include "RGBController_LogitechGProWireless.h"
#include "RGBController_LogitechGPowerPlay.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Logitech vendor ID                                    |
\*-----------------------------------------------------*/
#define LOGITECH_VID                            0x046D
/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define LOGITECH_G213_PID                       0xC336
#define LOGITECH_G512_PID                       0xC342
#define LOGITECH_G512_RGB_PID                   0xC33C
#define LOGITECH_G610_PID                       0xC333
#define LOGITECH_G810_1_PID                     0xC331
#define LOGITECH_G810_2_PID                     0xC337
/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define LOGITECH_G203_PID                       0xC084
#define LOGITECH_G203L_PID                      0xC092
#define LOGITECH_G403_PID                       0xC083
#define LOGITECH_G403H_PID                      0xC08F
#define LOGITECH_G502_PS_PID                    0xC332
#define LOGITECH_G502H_PID                      0xC08B
#define LOGITECH_G_LIGHTSPEED_WIRELESS_PID      0xC539
#define LOGITECH_GPRO_WIRED_PID                 0xC085
#define LOGITECH_GPRO_HERO_WIRED_PID            0xC08C
#define LOGITECH_GPRO_WIRELESS_PID              0xC088
/*-----------------------------------------------------*\
| Mousemat product IDs                                  |
\*-----------------------------------------------------*/
#define LOGITECH_G_LIGHTSPEED_POWERPLAY_PID     0xC53A

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned char   usb_interface;
    device_type     type;
    const char *    name;
} logitech_device;

#define LOGITECH_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const logitech_device device_list[] =
{
    /*-------------------------------------------------------------------------------------------------------------------------------------------------*\
    | Keyboards                                                                                                                                         |
    \*-------------------------------------------------------------------------------------------------------------------------------------------------*/
    { LOGITECH_VID,             LOGITECH_G213_PID,                      1,  DEVICE_TYPE_KEYBOARD,   "Logitech G213"                                     },
    { LOGITECH_VID,             LOGITECH_G512_PID,                      1,  DEVICE_TYPE_KEYBOARD,   "Logitech G512"                                     },
    { LOGITECH_VID,             LOGITECH_G512_RGB_PID,                  1,  DEVICE_TYPE_KEYBOARD,   "Logitech G512 RGB"                                 },
    { LOGITECH_VID,             LOGITECH_G610_PID,                      1,  DEVICE_TYPE_KEYBOARD,   "Logitech G610 Orion"                               },
    { LOGITECH_VID,             LOGITECH_G810_1_PID,                    1,  DEVICE_TYPE_KEYBOARD,   "Logitech G810 Orion Spectrum"                      },
    { LOGITECH_VID,             LOGITECH_G810_2_PID,                    1,  DEVICE_TYPE_KEYBOARD,   "Logitech G810 Orion Spectrum"                      },
    /*-------------------------------------------------------------------------------------------------------------------------------------------------*\
    | Mice                                                                                                                                              |
    \*-------------------------------------------------------------------------------------------------------------------------------------------------*/
    { LOGITECH_VID,             LOGITECH_G203_PID,                      1,  DEVICE_TYPE_MOUSE,      "Logitech G203 Prodigy"                             },
    { LOGITECH_VID,             LOGITECH_G203L_PID,                     1,  DEVICE_TYPE_MOUSE,      "Logitech G203 Lightsync"                           },
    { LOGITECH_VID,             LOGITECH_G403_PID,                      1,  DEVICE_TYPE_MOUSE,      "Logitech G403 Prodigy"                             },
    { LOGITECH_VID,             LOGITECH_G403H_PID,                     1,  DEVICE_TYPE_MOUSE,      "Logitech G403 Hero"                                },
    { LOGITECH_VID,             LOGITECH_G502_PS_PID,                   1,  DEVICE_TYPE_MOUSE,      "Logitech G502 Proteus Spectrum"                    },
    { LOGITECH_VID,             LOGITECH_G502H_PID,                     1,  DEVICE_TYPE_MOUSE,      "Logitech G502 Hero"                                },
    { LOGITECH_VID,             LOGITECH_GPRO_WIRED_PID,                1,  DEVICE_TYPE_MOUSE,      "Logitech G Pro Gaming Mouse"                       },
    { LOGITECH_VID,             LOGITECH_GPRO_HERO_WIRED_PID,           1,  DEVICE_TYPE_MOUSE,      "Logitech G Pro (HERO) Gaming Mouse"                },
    { LOGITECH_VID,             LOGITECH_G_LIGHTSPEED_WIRELESS_PID,     2,  DEVICE_TYPE_MOUSE,      "Logitech G Lightspeed Wireless Gaming Mouse"       },
    { LOGITECH_VID,             LOGITECH_GPRO_WIRELESS_PID,             2,  DEVICE_TYPE_MOUSE,      "Logitech G Pro Wireless Gaming Mouse (Wired)"      },
    /*-------------------------------------------------------------------------------------------------------------------------------------------------*\
    | Mousemats                                                                                                                                         |
    \*-------------------------------------------------------------------------------------------------------------------------------------------------*/
    { LOGITECH_VID,             LOGITECH_G_LIGHTSPEED_POWERPLAY_PID,    2,  DEVICE_TYPE_MOUSEMAT,   "Logitech G Powerplay Mousepad with Lightspeed"     },
};

/******************************************************************************************\
*                                                                                          *
*   DetectLogitechControllers                                                              *
*                                                                                          *
*       Tests the USB address to see if a Logitech RGB Keyboard controller exists there.   *
*                                                                                          *
\******************************************************************************************/

void DetectLogitechControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_init();

    for(unsigned int device_idx = 0; device_idx < LOGITECH_NUM_DEVICES; device_idx++)
    {
        switch(device_list[device_idx].type)
        {
        /*-------------------------------------------------------------------------------------------------*\
        | Logitech keyboards use two different usages, one for 20-byte packets and one for 64-byte packets  |
        | Usage 0x0602 for 20 byte, usage 0x0604 for 64 byte, both are on usage page 0xFF43                 |
        \*-------------------------------------------------------------------------------------------------*/
        case DEVICE_TYPE_KEYBOARD:
            {
                hid_device_info* info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

                while(info)
                {
                    if((info->vendor_id == device_list[device_idx].usb_vid)
                    &&(info->product_id == device_list[device_idx].usb_pid)
#ifdef USE_HID_USAGE
                    &&(info->interface_number == device_list[device_idx].usb_interface)
                    &&(info->usage_page == 0xFF43)
                    &&(info->usage == 0x0602))
#else
                    &&(info->interface_number == device_list[device_idx].usb_interface))
#endif
                    {
                        hid_device* dev_usage_0x0602 = hid_open_path(info->path);

                        if(dev_usage_0x0602)
                        {
#ifdef USE_HID_USAGE
                            switch(device_list[device_idx].usb_pid)
                            {
                                case LOGITECH_G213_PID:
                                    {
                                        LogitechG213Controller* controller = new LogitechG213Controller(dev_usage_0x0602);

                                        RGBController_LogitechG213* rgb_controller = new RGBController_LogitechG213(controller);

                                        rgb_controller->name = device_list[device_idx].name;
                                        rgb_controllers.push_back(rgb_controller);
                                    }
                                    break;

                                case LOGITECH_G512_PID:
                                case LOGITECH_G512_RGB_PID:
                                case LOGITECH_G610_PID:
                                case LOGITECH_G810_1_PID:
                                case LOGITECH_G810_2_PID:
                                    {
                                        hid_device_info* tmp_info_0x0604 = info;

                                        while(tmp_info_0x0604)
                                        {
                                            if((tmp_info_0x0604->vendor_id == device_list[device_idx].usb_vid)
                                            &&(tmp_info_0x0604->product_id == device_list[device_idx].usb_pid)
                                            &&(tmp_info_0x0604->interface_number == device_list[device_idx].usb_interface)
                                            &&(tmp_info_0x0604->usage_page == 0xFF43)
                                            &&(tmp_info_0x0604->usage == 0x0604))
                                            {
                                                hid_device* dev_usage_0x0604 = hid_open_path(tmp_info_0x0604->path);

                                                if(dev_usage_0x0604)
                                                {
                                                    LogitechG810Controller* controller = new LogitechG810Controller(dev_usage_0x0602, dev_usage_0x0604);

                                                    RGBController_LogitechG810* rgb_controller = new RGBController_LogitechG810(controller);

                                                    rgb_controller->name = device_list[device_idx].name;
                                                    rgb_controllers.push_back(rgb_controller);
                                                }
                                            }
                                            tmp_info_0x0604 = tmp_info_0x0604->next;
                                        }
                                    }
                                    break;
                            }
#else
                            switch(device_list[device_idx].usb_pid)
                            {
                                case LOGITECH_G213_PID:
                                    {
                                        LogitechG213Controller* controller = new LogitechG213Controller(dev_usage_0x0602);

                                        RGBController_LogitechG213* rgb_controller = new RGBController_LogitechG213(controller);

                                        rgb_controller->name = device_list[device_idx].name;
                                        rgb_controllers.push_back(rgb_controller);
                                    }
                                    break;

                                case LOGITECH_G512_PID:
                                case LOGITECH_G512_RGB_PID:
                                case LOGITECH_G610_PID:
                                case LOGITECH_G810_1_PID:
                                case LOGITECH_G810_2_PID:
                                    {
                                        LogitechG810Controller* controller = new LogitechG810Controller(dev_usage_0x0602, dev_usage_0x0602);

                                        RGBController_LogitechG810* rgb_controller = new RGBController_LogitechG810(controller);

                                        rgb_controller->name = device_list[device_idx].name;
                                        rgb_controllers.push_back(rgb_controller);
                                    }
                                    break;
                            }
#endif
                        }
                    }
                    info = info->next;
                }
                hid_free_enumeration(info);
            }
            break;

        /*-------------------------------------------------------------------------------------------------*\
        | Logitech mice use a single usage on page 0xFF00                                                   |
        \*-------------------------------------------------------------------------------------------------*/
        case DEVICE_TYPE_MOUSE:
            {
                hid_device_info* info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

                while(info)
                {
                    if((info->vendor_id == device_list[device_idx].usb_vid)
                    &&(info->product_id == device_list[device_idx].usb_pid)
#ifdef USE_HID_USAGE                    
                    &&(info->interface_number == device_list[device_idx].usb_interface)
                    &&(info->usage_page == 0xFF00)
                    &&(info->usage == 2))
#else
                    &&(info->interface_number == device_list[device_idx].usb_interface))
#endif
                    {
                        hid_device* dev = hid_open_path(info->path);

                        if(dev)
                        {
                            switch(device_list[device_idx].usb_pid)
                            {
                                case LOGITECH_G203_PID:
                                case LOGITECH_GPRO_WIRED_PID:
                                case LOGITECH_GPRO_HERO_WIRED_PID:
                                    {
                                        LogitechG203Controller* controller = new LogitechG203Controller(dev, info->path);

                                        RGBController_LogitechG203* rgb_controller = new RGBController_LogitechG203(controller);

                                        rgb_controller->name = device_list[device_idx].name;
                                        rgb_controllers.push_back(rgb_controller);
                                    }
                                    break;

                                case LOGITECH_G203L_PID:
                                    {
                                        LogitechG203LController* controller = new LogitechG203LController(dev, info->path);

                                        RGBController_LogitechG203L* rgb_controller = new RGBController_LogitechG203L(controller);

                                        rgb_controller->name = device_list[device_idx].name;
                                        rgb_controllers.push_back(rgb_controller);
                                    }
                                    break;

                                case LOGITECH_G403_PID:
                                case LOGITECH_G403H_PID:
                                    {
                                        LogitechG403Controller* controller = new LogitechG403Controller(dev, info->path);

                                        RGBController_LogitechG403* rgb_controller = new RGBController_LogitechG403(controller);

                                        rgb_controller->name = device_list[device_idx].name;
                                        rgb_controllers.push_back(rgb_controller);
                                    }
                                    break;

                                case LOGITECH_G502_PS_PID:
                                case LOGITECH_G502H_PID:
                                    {
                                        LogitechG502PSController* controller = new LogitechG502PSController(dev, info->path);

                                        RGBController_LogitechG502PS* rgb_controller = new RGBController_LogitechG502PS(controller);

                                        rgb_controller->name = device_list[device_idx].name;
                                        rgb_controllers.push_back(rgb_controller);
                                    }
                                    break;

                                case LOGITECH_GPRO_WIRELESS_PID:
                                case LOGITECH_G_LIGHTSPEED_WIRELESS_PID:
                                    {
                                        LogitechGProWirelessController* controller = new LogitechGProWirelessController(dev, info->path);

                                        RGBController_LogitechGProWireless* rgb_controller = new RGBController_LogitechGProWireless(controller);

                                        rgb_controller->name = device_list[device_idx].name;
                                        rgb_controllers.push_back(rgb_controller);
                                    }
                                    break;
                            }
                        }
                    }
                    info = info->next;
                }
                hid_free_enumeration(info);
            }
            break;
        case DEVICE_TYPE_MOUSEMAT:
            {
                hid_device_info* info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

                while(info)
                {
                    if((info->vendor_id == device_list[device_idx].usb_vid)
                    &&(info->product_id == device_list[device_idx].usb_pid)
#ifdef USE_HID_USAGE                    
                    &&(info->interface_number == device_list[device_idx].usb_interface)
                    &&(info->usage_page == 0xFF00)
                    &&(info->usage == 2))
#else
                    &&(info->interface_number == device_list[device_idx].usb_interface))
#endif
                    {
                        hid_device* dev = hid_open_path(info->path);

                        if(dev)
                        {
                            switch(device_list[device_idx].usb_pid)
                            {
                                case LOGITECH_G_LIGHTSPEED_POWERPLAY_PID:
                                    {
                                        //Add mouse
                                        LogitechGProWirelessController* mouse_controller = new LogitechGProWirelessController(dev, info->path);

                                        RGBController_LogitechGProWireless* mouse_rgb_controller = new RGBController_LogitechGProWireless(mouse_controller);

                                        mouse_rgb_controller->name = device_list[device_idx].name;
                                        rgb_controllers.push_back(mouse_rgb_controller);

                                        //Add Powerplay mousemat
                                        LogitechGPowerPlayController* mousemat_controller = new LogitechGPowerPlayController(dev, info->path);

                                        RGBController_LogitechGPowerPlay* mousemat_rgb_controller = new RGBController_LogitechGPowerPlay(mousemat_controller);

                                        mousemat_rgb_controller->name = device_list[device_idx].name;
                                        rgb_controllers.push_back(mousemat_rgb_controller);
                                    }
                                    break;
                            }
                        }
                    }
                    info = info->next;
                }
                hid_free_enumeration(info);
            }
            break;
        }

    }
}   /* DetectLogitechControllers() */

REGISTER_DETECTOR("Logitech", DetectLogitechControllers);
