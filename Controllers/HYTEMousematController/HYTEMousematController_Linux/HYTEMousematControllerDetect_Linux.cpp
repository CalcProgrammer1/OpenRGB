/*---------------------------------------------------------*\
| HYTEMousematControllerDetect_Linux.cpp                    |
|                                                           |
|   Detector for HYTE mousemat (libusb implementation for   |
|   Linux)                                                  |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      18 Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <libusb.h>
#include "Detector.h"
#include "RGBController_HYTEMousemat.h"

/*-----------------------------------------------------*\
| HYTE vendor ID                                        |
\*-----------------------------------------------------*/
#define HYTE_VID                        0x3402

/*-----------------------------------------------------*\
| HYTE CNVS product IDs                                 |
\*-----------------------------------------------------*/
#define HYTE_CNVS_HW_VER_1_PID          0x0B00
#define HYTE_CNVS_HW_VER_2_PID          0x0B01

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned char   usb_interface;
    const char *    name;
} hyte_mousemat_device;

#define HYTE_MOUSEMAT_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const hyte_mousemat_device device_list[] =
{
    /*-----------------------------------------------------------------------------------------------------*\
    | Mousemats                                                                                              |
    \*-----------------------------------------------------------------------------------------------------*/
    { HYTE_VID, HYTE_CNVS_HW_VER_1_PID, 0,  "HYTE CNVS" },
    { HYTE_VID, HYTE_CNVS_HW_VER_2_PID, 0,  "HYTE CNVS" },
};

/******************************************************************************************\
*                                                                                          *
*   DetectHYTEMousematControllers                                                          *
*                                                                                          *
*       Detect devices supported by the HyteMousemat driver                                *
*                                                                                          *
\******************************************************************************************/

void DetectHYTEMousematControllers()
{
    libusb_init(NULL);

    #ifdef _WIN32
    libusb_set_option(NULL, LIBUSB_OPTION_USE_USBDK);
    #endif

    for(std::size_t device_idx = 0; device_idx < HYTE_MOUSEMAT_NUM_DEVICES; device_idx++)
    {
        libusb_device_handle * dev = libusb_open_device_with_vid_pid(NULL, device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        //Look for HYTE CNVS
        if(dev)
        {
            libusb_detach_kernel_driver(dev, 0);
            libusb_claim_interface(dev, 0);

            HYTEMousematController *     controller     = new HYTEMousematController(dev, device_list[device_idx].name);
            RGBController_HYTEMousemat * rgb_controller = new RGBController_HYTEMousemat(controller);

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }

}   /* DetectHYTEMousematControllers() */

REGISTER_DETECTOR("HYTE Mousemat", DetectHYTEMousematControllers);
/*---------------------------------------------------------------------------------------------------------*\
| Entries for dynamic UDEV rules                                                                            |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("HYTE Mousemat", DetectHYTEMousematControllers, 0x3402, 0x0B00 )                    |
| DUMMY_DEVICE_DETECTOR("HYTE Mousemat", DetectHYTEMousematControllers, 0x3402, 0x0B01 )                    |
\*---------------------------------------------------------------------------------------------------------*/
