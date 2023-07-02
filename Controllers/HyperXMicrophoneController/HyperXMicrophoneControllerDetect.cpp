/*-----------------------------------------*\
|  HyperXMicrophoneControllerDetect.cpp     |
|                                           |
|  Implementation for the HyperX            |
|  Quadcast S RGB microphone                |
|                                           |
|  Matt Silva (thesilvanator) 2022          |
\*-----------------------------------------*/

#include "Detector.h"
#include "HyperXMicrophoneController.h"
#include "RGBController.h"
#include "RGBController_HyperXMicrophone.h"
#include <vector>
#include <LogManager.h>

#include "hidapi_wrapper.h"

/*-----------------------------------------------------*\
| HyperX microphone vendor and product IDs              |
\*-----------------------------------------------------*/
#define HYPERX_VID          0x0951
#define HYPERX_HP_VID       0x03F0

#define HYPERX_QS_PID       0x171F

#define HYPERX_QS_PID_HP_1  0x0F8B
#define HYPERX_QS_PID_HP_2  0x068C
#define HYPERX_QS_PID_HP_3  0x0294
#define HYPERX_QS_PID_HP_4  0x028C

#define HYPERX_DUOCAST_PID  0x098C

void DetectHyperXMicrophoneControllers(hidapi_wrapper wrapper, hid_device_info* info, const std::string& name)
{
    hid_device* dev = wrapper.hid_open_path(info->path);

    if(dev)
    {
        HyperXMicrophoneController* controller         = new HyperXMicrophoneController(wrapper, dev, info->path);
        RGBController_HyperXMicrophone *rgb_controller = new RGBController_HyperXMicrophone(controller);
        rgb_controller->name                          = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_WRAPPED_DETECTOR_I("HyperX Quadcast S", DetectHyperXMicrophoneControllers, HYPERX_VID,    HYPERX_QS_PID,      0);//, 0xFF90, 0xFF00);
REGISTER_HID_WRAPPED_DETECTOR_I("HyperX Quadcast S", DetectHyperXMicrophoneControllers, HYPERX_HP_VID, HYPERX_QS_PID_HP_1, 0);//, 0xFF90, 0xFF00);
REGISTER_HID_WRAPPED_DETECTOR_I("HyperX Quadcast S", DetectHyperXMicrophoneControllers, HYPERX_HP_VID, HYPERX_QS_PID_HP_2, 0);//, 0xFF90, 0xFF00);
REGISTER_HID_WRAPPED_DETECTOR_I("HyperX Quadcast S", DetectHyperXMicrophoneControllers, HYPERX_HP_VID, HYPERX_QS_PID_HP_3, 0);//, 0xFF90, 0xFF00);
REGISTER_HID_WRAPPED_DETECTOR_I("HyperX Quadcast S", DetectHyperXMicrophoneControllers, HYPERX_HP_VID, HYPERX_QS_PID_HP_4, 0);//, 0xFF90, 0xFF00);
REGISTER_HID_WRAPPED_DETECTOR_I("HyperX DuoCast",    DetectHyperXMicrophoneControllers, HYPERX_HP_VID, HYPERX_DUOCAST_PID, 0);//, 0xFF90, 0xFF00);
