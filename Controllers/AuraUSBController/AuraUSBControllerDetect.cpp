#include "Detector.h"
#include "AuraAddressableController.h"
#include "AuraMainboardController.h"
#include "AuraMouseController.h"
#include "RGBController.h"
#include "RGBController_AuraUSB.h"
#include "RGBController_AuraMouse.h"
#include <vector>
#include <stdexcept>
#include <hidapi/hidapi.h>

#define AURA_USB_VID                            0x0B05
#define AURA_TERMINAL_PID                       0x1889
#define AURA_ADDRESSABLE_1_PID                  0x1867
#define AURA_ADDRESSABLE_2_PID                  0x1872
#define AURA_ADDRESSABLE_3_PID                  0x18A3
#define AURA_ADDRESSABLE_4_PID                  0x18A5
#define AURA_MOTHERBOARD_1_PID                  0x18F3
#define AURA_MOTHERBOARD_2_PID                  0x1939
#define AURA_ROG_GLADIUS_II_CORE_PID            0x18DD
typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    const char *    name;
} aura_device;

#define ADDRESSABLE_NUM_DEVICES (sizeof(addressable_device_list) / sizeof(addressable_device_list[ 0 ]))

static const aura_device addressable_device_list[] =
{
/*-----------------------------------------------------------------------------*\
| ASUS AURA Addressable                                                         |
\*-----------------------------------------------------------------------------*/
    { AURA_USB_VID,     AURA_TERMINAL_PID,          "ASUS ROG AURA Terminal"    },
    { AURA_USB_VID,     AURA_ADDRESSABLE_1_PID,     "ASUS Aura Addressable"     },
    { AURA_USB_VID,     AURA_ADDRESSABLE_2_PID,     "ASUS Aura Addressable"     },
    { AURA_USB_VID,     AURA_ADDRESSABLE_3_PID,     "ASUS Aura Addressable"     },
    { AURA_USB_VID,     AURA_ADDRESSABLE_4_PID,     "ASUS Aura Addressable"     },
};

#define MOTHERBOARD_NUM_DEVICES (sizeof(motherboard_device_list) / sizeof(motherboard_device_list[ 0 ]))

static const aura_device motherboard_device_list[] =
{
/*-----------------------------------------------------------------------------*\
| ASUS AURA Motherboard                                                         |
\*-----------------------------------------------------------------------------*/
    { AURA_USB_VID,     AURA_MOTHERBOARD_1_PID,     "ASUS Aura Motherboard"     },
    { AURA_USB_VID,     AURA_MOTHERBOARD_2_PID,     "ASUS Aura Motherboard"     },
};

#define MOUSE_NUM_DEVICES (sizeof(mouse_device_list) / sizeof(mouse_device_list[ 0 ]))

static const aura_device mouse_device_list[] =
{
    { AURA_USB_VID,     AURA_ROG_GLADIUS_II_CORE_PID,   "ASUS ROG Gladius II Core"  },
};

void DetectAuraUSBControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info = NULL;

    hid_init();

    /*ASUS AURA Addressable*/
    for(unsigned int pid_idx = 0; pid_idx < ADDRESSABLE_NUM_DEVICES; pid_idx++)
    {
        info = hid_enumerate(addressable_device_list[pid_idx].usb_vid, addressable_device_list[pid_idx].usb_pid);

        while(info)
        {
            hid_device* dev = NULL;

            if((info->vendor_id  == addressable_device_list[pid_idx].usb_vid)
             &&(info->product_id == addressable_device_list[pid_idx].usb_pid))
            {
                dev = hid_open_path(info->path);

                if(dev)
                {
                    AuraAddressableController* controller = new AuraAddressableController(dev, info->path);
                    RGBController_AuraUSB* rgb_controller = new RGBController_AuraUSB(controller);
                    rgb_controller->name = addressable_device_list[pid_idx].name;
                    rgb_controllers.push_back(rgb_controller);
                }
            }

            info = info->next;
        }
    }

    info = NULL;

    /*ASUS AURA Motherboard*/
    for(unsigned int pid_idx = 0; pid_idx < MOTHERBOARD_NUM_DEVICES; pid_idx++)
    {
        info = hid_enumerate(motherboard_device_list[pid_idx].usb_vid, motherboard_device_list[pid_idx].usb_pid);

        while(info)
        {
            hid_device* dev = NULL;

            if((info->vendor_id  == motherboard_device_list[pid_idx].usb_vid)
             &&(info->product_id == motherboard_device_list[pid_idx].usb_pid))
            {
                dev = hid_open_path(info->path);

                if(dev)
                {
                    try
                    {
                        AuraMainboardController* controller = new AuraMainboardController(dev, info->path);
                        RGBController_AuraUSB* rgb_controller = new RGBController_AuraUSB(controller);
                        rgb_controller->name = motherboard_device_list[pid_idx].name;
                        rgb_controllers.push_back(rgb_controller);
                    }
                    catch(std::runtime_error&)
                    {
                        // reading the config table failed
                    }
                }
            }

            info = info->next;
        }
    }

    /*ASUS AURA Mouse*/
    for(unsigned int pid_idx = 0; pid_idx < MOUSE_NUM_DEVICES; pid_idx++)
    {
        info = hid_enumerate(mouse_device_list[pid_idx].usb_vid, mouse_device_list[pid_idx].usb_pid);

        while(info)
        {
            hid_device* dev = NULL;

            if((info->vendor_id  == mouse_device_list[pid_idx].usb_vid)
             &&(info->product_id == mouse_device_list[pid_idx].usb_pid))
            {
                dev = hid_open_path(info->path);

                if(dev)
                {
                    AuraMouseController* controller = new AuraMouseController(dev, info->path);
                    RGBController_AuraMouse* rgb_controller = new RGBController_AuraMouse(controller);
                    rgb_controller->name = mouse_device_list[pid_idx].name;
                    rgb_controllers.push_back(rgb_controller);
                }
            }

            info = info->next;
        }
    }

}  /* DetectAuraUSBControllers() */

REGISTER_DETECTOR("ASUS Aura USB", DetectAuraUSBControllers);
