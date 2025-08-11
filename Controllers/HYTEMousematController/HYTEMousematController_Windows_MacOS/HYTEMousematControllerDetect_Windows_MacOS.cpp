/*---------------------------------------------------------*\
| HYTEMousematControllerDetect_Windows_MacOS.cpp            |
|                                                           |
|   Detector for HYTE mousemat (Serial implementation for   |
|   Windows and MacOS)                                      |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      18 Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include "Detector.h"
#include "HYTEMousematController_Windows_MacOS.h"
#include "RGBController_HYTEMousemat.h"
#include "find_usb_serial_port.h"

#define HYTE_VID                0x3402

#define HYTE_CNVS_HW_VER_1_PID  0x0B00
#define HYTE_CNVS_HW_VER_2_PID  0x0B01

struct hyte_mousemat_type
{
    unsigned short  vid;
    unsigned short  pid;
    const char *    name;
};

#define HYTE_MOUSEMAT_NUM_DEVICES   2

static const hyte_mousemat_type hyte_mousemat_devices[] =
{
    { HYTE_VID, HYTE_CNVS_HW_VER_1_PID, "HYTE CNVS" },
    { HYTE_VID, HYTE_CNVS_HW_VER_2_PID, "HYTE CNVS" },
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
    for(unsigned int device_id = 0; device_id < HYTE_MOUSEMAT_NUM_DEVICES; device_id++)
    {
        std::vector<std::string *> ports = find_usb_serial_port(hyte_mousemat_devices[device_id].vid, hyte_mousemat_devices[device_id].pid);

        for(unsigned int i = 0; i < ports.size(); i++)
        {
            if(*ports[i] != "")
            {
                HYTEMousematController *     controller     = new HYTEMousematController((char *)ports[i]->c_str(), hyte_mousemat_devices[device_id].name);
                RGBController_HYTEMousemat * rgb_controller = new RGBController_HYTEMousemat(controller);

                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
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
