/*---------------------------------------------------------*\
| CorsairHydroControllerDetect.cpp                          |
|                                                           |
|   Detector for Corsair Hydro Series coolers               |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      17 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <libusb.h>
#include "Detector.h"
#include "CorsairHydroController.h"
#include "RGBController_CorsairHydro.h"

/*-----------------------------------------------------*\
| Corsair vendor ID                                     |
\*-----------------------------------------------------*/
#define CORSAIR_VID                     0x1B1C

/*-----------------------------------------------------*\
| Keyboard Hydro Series product IDs                     |
\*-----------------------------------------------------*/
#define CORSAIR_H115I_PRO_RGB_PID       0x0C13
#define CORSAIR_H100I_PRO_RGB_PID       0x0C15
#define CORSAIR_H150I_PRO_RGB_PID       0x0C12

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned char   usb_interface;
    const char *    name;
} corsair_hydro_device;

#define CORSAIR_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[ 0 ]))

static const corsair_hydro_device device_list[] =
{
    /*-----------------------------------------------------------------------------------------------------*\
    | Coolers                                                                                               |
    \*-----------------------------------------------------------------------------------------------------*/
    { CORSAIR_VID,          CORSAIR_H100I_PRO_RGB_PID,          0,      "Corsair H100i PRO RGB"             },
    { CORSAIR_VID,          CORSAIR_H115I_PRO_RGB_PID,          0,      "Corsair H115i PRO RGB"             },
    { CORSAIR_VID,          CORSAIR_H150I_PRO_RGB_PID,          0,      "Corsair H150i PRO RGB"             },
};

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairHydroControllers                                                          *
*                                                                                          *
*       Tests the USB address to see if a Corsair RGB Cooler controller exists there.      *
*                                                                                          *
\******************************************************************************************/

void DetectCorsairHydroControllers()
{
    libusb_init(NULL);

    #ifdef _WIN32
    libusb_set_option(NULL, LIBUSB_OPTION_USE_USBDK);
    #endif

    for(std::size_t device_idx = 0; device_idx < CORSAIR_NUM_DEVICES; device_idx++)
    {
        libusb_device_handle * dev = libusb_open_device_with_vid_pid(NULL, device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        //Look for Corsair RGB Peripheral
        if(dev)
        {
            libusb_detach_kernel_driver(dev, 0);
            libusb_claim_interface(dev, 0);

            CorsairHydroController*     controller     = new CorsairHydroController(dev, device_list[device_idx].name);
            RGBController_CorsairHydro* rgb_controller = new RGBController_CorsairHydro(controller);

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}   /* DetectCorsairHydroControllers() */

REGISTER_DETECTOR("Corsair Hydro Series", DetectCorsairHydroControllers);
/*---------------------------------------------------------------------------------------------------------*\
| Entries for dynamic UDEV rules                                                                            |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("Corsair Hydro Series", DetectCorsairHydroControllers, 0x1B1C, 0x0C12 )             |
| DUMMY_DEVICE_DETECTOR("Corsair Hydro Series", DetectCorsairHydroControllers, 0x1B1C, 0x0C13 )             |
| DUMMY_DEVICE_DETECTOR("Corsair Hydro Series", DetectCorsairHydroControllers, 0x1B1C, 0x0C15 )             |
\*---------------------------------------------------------------------------------------------------------*/
