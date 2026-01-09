/*---------------------------------------------------------*\
| ThrustmasterSolControllerDetect.cpp                       |
|                                                           |
|   Detector for Thrustmaster Sol series joysticks          |
|                                                           |
|   Ken Sanislo                                 02 Apr 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "ThrustmasterSolController.h"
#include "RGBController_ThrustmasterSol.h"

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    const char*     name;
} thrustmaster_sol_device;

#define THRUSTMASTER_SOL_NUM_DEVICES (sizeof(device_list) / sizeof(device_list[0]))

static const thrustmaster_sol_device device_list[] =
{
    /*-----------------------------------------------------------------------------------------------------*\
    | Sol-R variants (tested)                                                                               |
    \*-----------------------------------------------------------------------------------------------------*/
    { THRUSTMASTER_VID,     THRUSTMASTER_SOL_R_RIGHT_PID,   "Thrustmaster Sol-R Right"                      },
    { THRUSTMASTER_VID,     THRUSTMASTER_SOL_R_LEFT_PID,    "Thrustmaster Sol-R Left"                       },
    /*-----------------------------------------------------------------------------------------------------*\
    | Sol F16/F18 variants (untested, same base protocol)                                                   |
    \*-----------------------------------------------------------------------------------------------------*/
    { THRUSTMASTER_VID,     THRUSTMASTER_SOL_F16_RIGHT_PID, "Thrustmaster Sol F16 Right"                    },
    { THRUSTMASTER_VID,     THRUSTMASTER_SOL_F18_RIGHT_PID, "Thrustmaster Sol F18 Right"                    },
    { THRUSTMASTER_VID,     THRUSTMASTER_SOL_F16_LEFT_PID,  "Thrustmaster Sol F16 Left"                     },
    { THRUSTMASTER_VID,     THRUSTMASTER_SOL_F18_LEFT_PID,  "Thrustmaster Sol F18 Left"                     },
};

/******************************************************************************************\
*                                                                                          *
*   DetectThrustmasterSolControllers                                                       *
*                                                                                          *
*       Detect Thrustmaster Sol series joysticks via libusb.                                *
*       LED control is on USB interface 1 (vendor-specific class 255, not HID).            *
*       Interface 0 (joystick HID) is not touched.                                         *
*                                                                                          *
\******************************************************************************************/

void DetectThrustmasterSolControllers()
{
    libusb_init(NULL);

    #ifdef _WIN32
    libusb_set_option(NULL, LIBUSB_OPTION_USE_USBDK);
    #endif

    libusb_device** devs;
    ssize_t num_devs = libusb_get_device_list(NULL, &devs);

    if(num_devs <= 0)
    {
        return;
    }

    for(ssize_t i = 0; i < num_devs; i++)
    {
        libusb_device_descriptor desc;

        if(libusb_get_device_descriptor(devs[i], &desc) != 0)
        {
            continue;
        }

        for(std::size_t d = 0; d < THRUSTMASTER_SOL_NUM_DEVICES; d++)
        {
            if(desc.idVendor  == device_list[d].usb_vid &&
               desc.idProduct == device_list[d].usb_pid)
            {
                libusb_device_handle* handle = NULL;

                if(libusb_open(devs[i], &handle) != LIBUSB_SUCCESS)
                {
                    continue;
                }

                libusb_set_auto_detach_kernel_driver(handle, 1);

                if(libusb_claim_interface(handle, THRUSTMASTER_SOL_INTERFACE) != LIBUSB_SUCCESS)
                {
                    libusb_close(handle);
                    continue;
                }

                uint8_t bus     = libusb_get_bus_number(devs[i]);
                uint8_t address = libusb_get_device_address(devs[i]);
                char path[32];
                snprintf(path, sizeof(path), "%d-%d", bus, address);

                ThrustmasterSolController*      controller     = new ThrustmasterSolController(handle, path, desc.idProduct, device_list[d].name);
                RGBController_ThrustmasterSol*  rgb_controller = new RGBController_ThrustmasterSol(controller);

                DetectionManager::get()->RegisterRGBController(rgb_controller);
                break;
            }
        }
    }

    libusb_free_device_list(devs, 1);
}

REGISTER_DETECTOR("Thrustmaster Sol", DetectThrustmasterSolControllers);
/*---------------------------------------------------------------------------------------------------------*\
| Entries for dynamic UDEV rules                                                                            |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("Thrustmaster Sol", DetectThrustmasterSolControllers, 0x044F, 0x0420 )              |
| DUMMY_DEVICE_DETECTOR("Thrustmaster Sol", DetectThrustmasterSolControllers, 0x044F, 0x0421 )              |
| DUMMY_DEVICE_DETECTOR("Thrustmaster Sol", DetectThrustmasterSolControllers, 0x044F, 0x0422 )              |
| DUMMY_DEVICE_DETECTOR("Thrustmaster Sol", DetectThrustmasterSolControllers, 0x044F, 0x0428 )              |
| DUMMY_DEVICE_DETECTOR("Thrustmaster Sol", DetectThrustmasterSolControllers, 0x044F, 0x0429 )              |
| DUMMY_DEVICE_DETECTOR("Thrustmaster Sol", DetectThrustmasterSolControllers, 0x044F, 0x042A )              |
\*---------------------------------------------------------------------------------------------------------*/
