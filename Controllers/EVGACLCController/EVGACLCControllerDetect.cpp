/*---------------------------------------------------------*\
| EVGACLCControllerDetect.cpp                               |
|                                                           |
|   Detector for EVGA CLC liquid coolers (Asetek 690LC)     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <libusb.h>
#include "Detector.h"
#include "EVGACLCController.h"
#include "RGBController_EVGACLC.h"

/*-----------------------------------------------------*\
| Asetek vendor ID (used by EVGA CLC coolers)           |
\*-----------------------------------------------------*/
#define ASETEK_VID                      0x2433

/*-----------------------------------------------------*\
| EVGA CLC product IDs                                  |
\*-----------------------------------------------------*/
#define EVGA_CLC_PID                    0xB200

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned char   usb_interface;
    const char *    name;
} evga_clc_device;

#define EVGA_CLC_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[0]))

static const evga_clc_device device_list[] =
{
    /*-----------------------------------------------------------------------------------------------------*\
    | EVGA CLC Coolers (Asetek 690LC)                                                                       |
    | Covers EVGA CLC 120, 240, 280, and 360                                                                |
    \*-----------------------------------------------------------------------------------------------------*/
    { ASETEK_VID,           EVGA_CLC_PID,               0,      "EVGA CLC"                              },
};

/******************************************************************************************\
*                                                                                          *
*   DetectEVGACLCControllers                                                               *
*                                                                                          *
*       Tests the USB address to see if an EVGA CLC controller exists there.               *
*                                                                                          *
\******************************************************************************************/

void DetectEVGACLCControllers()
{
    int ret = libusb_init(NULL);

    if(ret < 0)
    {
        return;
    }

    #ifdef _WIN32
    (void)libusb_set_option(NULL, LIBUSB_OPTION_USE_USBDK);
    #endif

    for(std::size_t device_idx = 0; device_idx < EVGA_CLC_NUM_DEVICES; device_idx++)
    {
        libusb_device_handle * dev = libusb_open_device_with_vid_pid(NULL, device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        if(dev)
        {
            (void)libusb_detach_kernel_driver(dev, 0);

            ret = libusb_claim_interface(dev, 0);

            if(ret < 0)
            {
                libusb_close(dev);
                continue;
            }

            EVGACLCController*     controller     = new EVGACLCController(dev, device_list[device_idx].name);
            RGBController_EVGACLC* rgb_controller = new RGBController_EVGACLC(controller, "EVGA CLC");

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}   /* DetectEVGACLCControllers() */

REGISTER_DETECTOR("EVGA CLC", DetectEVGACLCControllers);
/*---------------------------------------------------------------------------------------------------------*\
| Entries for dynamic UDEV rules                                                                            |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("EVGA CLC", DetectEVGACLCControllers, 0x2433, 0xB200 )                             |
\*---------------------------------------------------------------------------------------------------------*/
