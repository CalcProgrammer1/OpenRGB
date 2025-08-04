/*---------------------------------------------------------*\
| CorsairHydro2ControllerDetect.cpp                         |
|                                                           |
|   Detector for Corsair H100i v2                           |
|                                                           |
|   Tim Demand (tim.dmd)                        10 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <libusb.h>
#include "Detector.h"
#include "CorsairHydro2Controller.h"
#include "RGBController_CorsairHydro2.h"

#define CORSAIR_VID     0x1B1C
#define H100I_V2_PID    0x0C09

void DetectCorsairHydro2Controllers()
{
    libusb_init(NULL);

    #ifdef _WIN32
    libusb_set_option(NULL, LIBUSB_OPTION_USE_USBDK);
    #endif

    libusb_device_handle* dev = libusb_open_device_with_vid_pid(NULL, CORSAIR_VID, H100I_V2_PID);

    if(dev)
    {
        libusb_detach_kernel_driver(dev, 0);
        libusb_claim_interface(dev, 0);

        CorsairHydro2Controller*     controller     = new CorsairHydro2Controller(dev);
        RGBController_CorsairHydro2* rgb_controller = new RGBController_CorsairHydro2(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_DETECTOR("Corsair H100i v2", DetectCorsairHydro2Controllers);
/*---------------------------------------------------------------------------------------------------------*\
| Entries for dynamic UDEV rules                                                                            |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("Corsair H100i v2", DetectCorsairHydro2Controllers, 0x1B1C, 0x0C09 )                |
\*---------------------------------------------------------------------------------------------------------*/
