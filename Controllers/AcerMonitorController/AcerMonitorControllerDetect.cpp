/*---------------------------------------------------------*\
|  AcerMonitorControllerDetect.cpp                          |
|                                                           |
|  Controller detection for 2020/2021 Acer monitors         |
|  with rear RGB strips.                                    |
|                                                           |
|  Shenghao Yang (me@shenghaoyang.info), 30/12/2021         |
|  Amended for code style by Adam Honse, 3/13/2022          |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "AcerMonitorController.h"
#include "RGBController_AcerMonitor.h"

/*-----------------------------------------------------*\
| Acer vendor ID                                        |
\*-----------------------------------------------------*/
#define ACER_VID                        0x042E

/*-----------------------------------------------------*\
| Acer monitor product IDs                              |
\*-----------------------------------------------------*/
#define ACER_XB323QK_NV_PID             0xAC0B
#define ACER_XB273U_NV_PID              0xAC0C
#define ACER_X25_PID                    0xAC0D

/*---------------------------------------------------------*\
| DetectAcerMonitorControllers                              |
|                                                           |
| Detects Acer monitor controllers.                         |
|                                                           |
| info: HID device information.                             |
\*---------------------------------------------------------*/
void DetectAcerMonitorControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AcerMonitorController*     controller     = new AcerMonitorController(dev, info->path);
        RGBController_AcerMonitor* rgb_controller = new RGBController_AcerMonitor(controller);
        rgb_controller->name                      = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR("Acer XB323QK NV", DetectAcerMonitorControllers, ACER_VID, ACER_XB323QK_NV_PID);
REGISTER_HID_DETECTOR("Acer XB273U NV",  DetectAcerMonitorControllers, ACER_VID, ACER_XB273U_NV_PID );
REGISTER_HID_DETECTOR("Acer X25",        DetectAcerMonitorControllers, ACER_VID, ACER_X25_PID       );
