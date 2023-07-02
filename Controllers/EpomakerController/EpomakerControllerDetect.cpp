#include "Detector.h"
#include "EpomakerController.h"
#include "RGBController.h"
#include "RGBController_EpomakerController.h"
#include <hidapi/hidapi.h>

#define Epomaker_VID                    0x3151
#define Epomaker_TH80_Pro_USB_PID       0x4010
#define Epomaker_TH80_Pro_Dongle_PID    0x4011
#define Epomaker_TH80_Pro_BT_PID        0x4013

/******************************************************************************************\
*                                                                                          *
*   DetectEpomakerControllers                                                              *
*                                                                                          *
*       Tests the USB address to see if any Epomaker Controllers exists there.             *
*                                                                                          *
\******************************************************************************************/

void DetectEpomakerControllers(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        EpomakerController*               controller     = new EpomakerController(dev, info->path);
        RGBController_EpomakerController* rgb_controller = new RGBController_EpomakerController(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectEpomakerControllers() */

REGISTER_HID_DETECTOR_IPU("Epomaker TH80 Pro (USB Cable)", DetectEpomakerControllers, Epomaker_VID, Epomaker_TH80_Pro_USB_PID, 1, 1, 2);
REGISTER_HID_DETECTOR_IPU("Epomaker TH80 Pro (USB Dongle)", DetectEpomakerControllers, Epomaker_VID, Epomaker_TH80_Pro_Dongle_PID, 1, 1, 2);

/*---------------------------------------------------------*\
| Bluetooth Not implemented                                 |
\*---------------------------------------------------------*/
//REGISTER_HID_DETECTOR("Epomaker TH80 Pro (Bluetooth)", DetectEpomakerControllers, Epomaker_VID, Epomaker_TH80_Pro_BT_PID);
