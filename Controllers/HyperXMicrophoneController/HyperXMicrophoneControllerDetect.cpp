#include "Detector.h"
#include "HyperXMicrophoneController.h"
#include "RGBController.h"
#include "RGBController_HyperXMicrophone.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| HyperX microphone vendor IDs                          |
\*-----------------------------------------------------*/
#define HYPERX_VID_2                0x03F0
#define HYPERX_QUADCAST_S_PID       0x0294
#define HYPERX_QUADCAST_S_PID2      0x068C

/******************************************************************************************\
*                                                                                          *
*   DetectHyperXMicrophoneControllers                                                      *
*                                                                                          *
*       Tests the USB address to see if a HyperX Microphone controller exists there.       *
*                                                                                          *
\******************************************************************************************/

void DetectHyperXMicrophoneControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        HyperXMicrophoneController*     controller     = new HyperXMicrophoneController(dev, info->path);
        RGBController_HyperXMicrophone* rgb_controller = new RGBController_HyperXMicrophone(controller);
        rgb_controller->name                           = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectHyperXMicrophoneControllers() */

REGISTER_HID_DETECTOR("HyperX Quadcast S", DetectHyperXMicrophoneControllers, HYPERX_VID_2, HYPERX_QUADCAST_S_PID);
