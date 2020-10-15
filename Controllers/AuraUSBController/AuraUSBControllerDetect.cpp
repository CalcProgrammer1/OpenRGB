#include "Detector.h"
#include "AuraAddressableController.h"
#include "AuraMainboardController.h"
#include "RGBController.h"
#include "RGBController_AuraUSB.h"
#include <vector>
#include <stdexcept>
#include <hidapi/hidapi.h>

#define AURA_USB_VID                            0x0B05
#define AURA_TERMINAL_PID                       0x1889
#define AURA_PRODUCT_1_PID                      0x1867
#define AURA_PRODUCT_2_PID                      0x1872
#define AURA_PRODUCT_3_PID                      0x18A3
#define AURA_MOTHERBOARD_1_PID                  0x18F3
#define AURA_MOTHERBOARD_2_PID                  0x1939

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
    { AURA_USB_VID,     AURA_PRODUCT_1_PID,         "ASUS Aura Addressable"     },
    { AURA_USB_VID,     AURA_PRODUCT_2_PID,         "ASUS Aura Addressable"     },
    { AURA_USB_VID,     AURA_PRODUCT_3_PID,         "ASUS Aura Addressable"     },
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

}  /* DetectAuraUSBControllers() */

REGISTER_DETECTOR("ASUS Aura USB", DetectAuraUSBControllers);
