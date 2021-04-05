/*-------------------------------------------------------------------*\
|  CorsairHydroPlatinumControllerDetect.cpp                           |
|                                                                     |
|  Driver for Corsair Hydro Platinum AIO Coolers                      |
|                                                                     |
|  Kasper         28th March 2021                                     |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "Detector.h"
#include "CorsairHydroPlatinumController.h"
#include "RGBController.h"
#include "RGBController_CorsairHydroPlatinum.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Corsair vendor ID                                     |
\*-----------------------------------------------------*/
#define CORSAIR_VID 0x1B1C

/*-----------------------------------------------------*\
| Product IDs                                           |
\*-----------------------------------------------------*/
#define CORSAIR_HYDRO_H100I_PLATINUM_PID    0x0c18
#define CORSAIR_HYDRO_H100I_PLATINUM_SE_PID 0x0C19
#define CORSAIR_HYDRO_H115I_PLATINUM_PID    0x0c17
#define CORSAIR_HYDRO_H100I_PRO_XT_PID      0x0c20
#define CORSAIR_HYDRO_H115I_PRO_XT_PID      0x0c21
#define CORSAIR_HYDRO_H150I_PRO_XT_PID      0x0c22

void DetectCorsairHydroPlatinumControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if( dev )
    {
        CorsairHydroPlatinumController* controller = new CorsairHydroPlatinumController(dev, info->path);
        RGBController_CorsairHydroPlatinum* rgb_controller = new RGBController_CorsairHydroPlatinum(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR("Corsair Hydro H100i Platinum",       DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H100I_PLATINUM_PID    );
REGISTER_HID_DETECTOR("Corsair Hydro H100i Platinum SE",    DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H100I_PLATINUM_SE_PID );
REGISTER_HID_DETECTOR("Corsair Hydro H115i Platinum",       DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H115I_PLATINUM_PID    );
REGISTER_HID_DETECTOR("Corsair Hydro H100i Pro XT",         DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H100I_PRO_XT_PID      );
REGISTER_HID_DETECTOR("Corsair Hydro H115i Pro XT",         DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H115I_PRO_XT_PID      );
REGISTER_HID_DETECTOR("Corsair Hydro H150i Pro XT",         DetectCorsairHydroPlatinumControllers, CORSAIR_VID, CORSAIR_HYDRO_H150I_PRO_XT_PID      );