/*---------------------------------------------------------*\
| CorsairHydroPlatinumControllerDetect.cpp                  |
|                                                           |
|   Detector for Corsair Hydro Platinum coolers             |
|                                                           |
|   Kasper                                      28 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "CorsairHydroPlatinumController.h"
#include "RGBController_CorsairHydroPlatinum.h"

/*-----------------------------------------------------*\
| Corsair vendor ID                                     |
\*-----------------------------------------------------*/
#define CORSAIR_VID                         0x1B1C

/*-----------------------------------------------------*\
| Product IDs                                           |
\*-----------------------------------------------------*/
#define CORSAIR_HYDRO_H100I_PLATINUM_PID    0x0C18
#define CORSAIR_HYDRO_H100I_PLATINUM_SE_PID 0x0C19
#define CORSAIR_HYDRO_H115I_PLATINUM_PID    0x0C17
#define CORSAIR_HYDRO_H60I_PRO_XT_PID       0x0C29
#define CORSAIR_HYDRO_H100I_PRO_XT_PID      0x0C20
#define CORSAIR_HYDRO_H100I_PRO_XT_V2_PID   0x0C2D
#define CORSAIR_HYDRO_H115I_PRO_XT_PID      0x0C21
#define CORSAIR_HYDRO_H150I_PRO_XT_PID      0x0C22
#define CORSAIR_HYDRO_H100I_ELITE_PID       0x0C40

void DetectCorsairHydroPlatinumControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CorsairHydroPlatinumController*     controller     = new CorsairHydroPlatinumController(dev, info->path, name);
        RGBController_CorsairHydroPlatinum* rgb_controller = new RGBController_CorsairHydroPlatinum(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR("Corsair Hydro H100i Platinum",       DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H100I_PLATINUM_PID    );
REGISTER_HID_DETECTOR("Corsair Hydro H100i Platinum SE",    DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H100I_PLATINUM_SE_PID );
REGISTER_HID_DETECTOR("Corsair Hydro H115i Platinum",       DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H115I_PLATINUM_PID    );
REGISTER_HID_DETECTOR("Corsair Hydro H60i Pro XT",          DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H60I_PRO_XT_PID       );
REGISTER_HID_DETECTOR("Corsair Hydro H100i Pro XT",         DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H100I_PRO_XT_PID      );
REGISTER_HID_DETECTOR("Corsair Hydro H100i Pro XT v2",      DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H100I_PRO_XT_V2_PID   );
REGISTER_HID_DETECTOR("Corsair Hydro H115i Pro XT",         DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H115I_PRO_XT_PID      );
REGISTER_HID_DETECTOR("Corsair Hydro H150i Pro XT",         DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H150I_PRO_XT_PID      );
REGISTER_HID_DETECTOR("Corsair Hydro H100i Elite",          DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H100I_ELITE_PID       );
