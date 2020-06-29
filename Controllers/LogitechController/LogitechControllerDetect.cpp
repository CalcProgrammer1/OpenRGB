#include "LogitechG203Controller.h"
#include "LogitechG403Controller.h"
#include "LogitechG810Controller.h"
#include "RGBController.h"
#include "RGBController_LogitechG203.h"
#include "RGBController_LogitechG403.h"
#include "RGBController_LogitechG810.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Logitech vendor ID                                    |
\*-----------------------------------------------------*/
#define LOGITECH_VID                    0x046D
/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define LOGITECH_G810_1_PID             0xC337
#define LOGITECH_G810_2_PID             0xC331
#define LOGITECH_G512_PID               0xC342
/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define LOGITECH_G203_PID               0xC084
#define LOGITECH_G403_PID               0xC083
#define LOGITECH_G403H_PID              0xC08F

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
    /*-------------------------------------------------------------------------------------------------------------*\
    | Keyboards                                                                                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
    { LOGITECH_VID,             LOGITECH_G810_1_PID,    1,  DEVICE_TYPE_KEYBOARD,   "Logitech G810 Orion Spectrum"  },
    { LOGITECH_VID,             LOGITECH_G810_2_PID,    1,  DEVICE_TYPE_KEYBOARD,   "Logitech G810 Orion Spectrum"  },
    { LOGITECH_VID,             LOGITECH_G512_PID,      1,  DEVICE_TYPE_KEYBOARD,   "Logitech G512"                 },
    /*-------------------------------------------------------------------------------------------------------------*\
    | Mice                                                                                                          |
    \*-------------------------------------------------------------------------------------------------------------*/
    { LOGITECH_VID,             LOGITECH_G203_PID,      1,  DEVICE_TYPE_MOUSE,      "Logitech G203 Prodigy"         },
    { LOGITECH_VID,             LOGITECH_G403_PID,      1,  DEVICE_TYPE_MOUSE,      "Logitech G403 Prodigy"         },
    { LOGITECH_VID,             LOGITECH_G403H_PID,     1,  DEVICE_TYPE_MOUSE,      "Logitech G403 Hero"            },
    /*-------------------------------------------------------------------------------------------------------------*\
    | Mousemats                                                                                                     |
    \*-------------------------------------------------------------------------------------------------------------*/
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

    for(int device_idx = 0; device_idx < LOGITECH_NUM_DEVICES; device_idx++)
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
#else
                            LogitechG810Controller* controller = new LogitechG810Controller(dev_usage_0x0602, dev_usage_0x0602);

                            RGBController_LogitechG810* rgb_controller = new RGBController_LogitechG810(controller);

                            rgb_controller->name = device_list[device_idx].name;
                            rgb_controllers.push_back(rgb_controller);
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
                                {
                                    LogitechG203Controller* controller = new LogitechG203Controller(dev);

                                    RGBController_LogitechG203* rgb_controller = new RGBController_LogitechG203(controller);

                                    rgb_controller->name = device_list[device_idx].name;
                                    rgb_controllers.push_back(rgb_controller);
                                }
                                break;

                            case LOGITECH_G403_PID:
                            case LOGITECH_G403H_PID:
                                {
                                    LogitechG403Controller* controller = new LogitechG403Controller(dev);

                                    RGBController_LogitechG403* rgb_controller = new RGBController_LogitechG403(controller);

                                    rgb_controller->name = device_list[device_idx].name;
                                    rgb_controllers.push_back(rgb_controller);
                                }
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
}
