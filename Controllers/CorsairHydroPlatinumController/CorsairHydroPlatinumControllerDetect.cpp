/*---------------------------------------------------------*\
| CorsairHydroPlatinumControllerDetect.cpp                  |
|                                                           |
|   Detector for Corsair Hydro Platinum coolers             |
|                                                           |
|   Kasper                                      28 Mar 2021 |
|   Nikola Jurkovic (jurkovic.nikola)           13 Aug 2025 |
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
#define CORSAIR_HYDRO_H100I_PLATINUM_PID    	0x0C18
#define CORSAIR_HYDRO_H100I_PLATINUM_SE_PID 	0x0C19
#define CORSAIR_HYDRO_H115I_PLATINUM_PID    	0x0C17
#define CORSAIR_HYDRO_H60I_PRO_XT_PID       	0x0C29
#define CORSAIR_HYDRO_H100I_PRO_XT_PID      	0x0C20
#define CORSAIR_HYDRO_H100I_PRO_XT_V2_PID   	0x0C2D
#define CORSAIR_HYDRO_H115I_PRO_XT_PID      	0x0C21
#define CORSAIR_HYDRO_H150I_PRO_XT_PID      	0x0C22
#define CORSAIR_HYDRO_H100I_ELITE_RGB_PID       0x0C35
#define CORSAIR_HYDRO_H115I_ELITE_RGB_PID       0x0C36
#define CORSAIR_HYDRO_H150I_ELITE_RGB_PID       0x0C37
#define CORSAIR_HYDRO_H100I_ELITE_RGB_PID_WHITE	0x0C40
#define CORSAIR_HYDRO_H150I_ELITE_RGB_PID_WHITE 0x0C41

void DetectCorsairHydroPlatinumControllers(hid_device_info* info, const std::string& name)
{
    uint16_t no_rgb_fan_models[] =
    {
        CORSAIR_HYDRO_H100I_ELITE_RGB_PID,
        CORSAIR_HYDRO_H115I_ELITE_RGB_PID,
        CORSAIR_HYDRO_H150I_ELITE_RGB_PID,
        CORSAIR_HYDRO_H100I_ELITE_RGB_PID_WHITE,
        CORSAIR_HYDRO_H150I_ELITE_RGB_PID_WHITE
    };

    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        bool dev_rgb_fan = true;
        for(uint16_t pid : no_rgb_fan_models)
        {
            if(info->product_id == pid)
            {
                dev_rgb_fan = false;
                break;
            }
        }

        CorsairHydroPlatinumController*     controller     = new CorsairHydroPlatinumController(dev, info->path, dev_rgb_fan, name);
        RGBController_CorsairHydroPlatinum* rgb_controller = new RGBController_CorsairHydroPlatinum(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR("Corsair Hydro H100i Platinum",       DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H100I_PLATINUM_PID    	);
REGISTER_HID_DETECTOR("Corsair Hydro H100i Platinum SE",    DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H100I_PLATINUM_SE_PID 	);
REGISTER_HID_DETECTOR("Corsair Hydro H115i Platinum",       DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H115I_PLATINUM_PID    	);
REGISTER_HID_DETECTOR("Corsair Hydro H60i Pro XT",          DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H60I_PRO_XT_PID       	);
REGISTER_HID_DETECTOR("Corsair Hydro H100i Pro XT",         DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H100I_PRO_XT_PID      	);
REGISTER_HID_DETECTOR("Corsair Hydro H100i Pro XT v2",      DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H100I_PRO_XT_V2_PID   	);
REGISTER_HID_DETECTOR("Corsair Hydro H115i Pro XT",         DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H115I_PRO_XT_PID      	);
REGISTER_HID_DETECTOR("Corsair Hydro H150i Pro XT",         DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H150I_PRO_XT_PID      	);
REGISTER_HID_DETECTOR("Corsair Hydro H100i Elite",          DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H100I_ELITE_RGB_PID       );
REGISTER_HID_DETECTOR("Corsair Hydro H115i Elite",          DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H115I_ELITE_RGB_PID       );
REGISTER_HID_DETECTOR("Corsair Hydro H150i Elite",          DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H150I_ELITE_RGB_PID       );
REGISTER_HID_DETECTOR("Corsair Hydro H100i Elite White",    DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H100I_ELITE_RGB_PID_WHITE );
REGISTER_HID_DETECTOR("Corsair Hydro H150i Elite White",    DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H150I_ELITE_RGB_PID_WHITE );
