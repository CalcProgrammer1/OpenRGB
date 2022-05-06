/*-----------------------------------------*\
|  SonyDS4Controller.h                      |
|                                           |
|  Detector for Sony Dualshock 4            |
|                                           |
|  Pol Rius (alpemwarrior) 24/09/2020       |
\*-----------------------------------------*/

#include <vector>
#include <hidapi/hidapi.h>

#include "RGBController_SonyDS4.h"
#include "Detector.h"

#define SONY_DUALSHOCK_VID              0x054C

#define SONY_DS4_V1_PID                 0x05C4
#define SONY_DS4_V2_PID                 0x09CC
#define SONY_DS4_RECEIVER_PID           0x0BA0

void DetectSonyDS4Controllers(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        SonyDS4Controller*      controller      = new SonyDS4Controller(dev, info->path);
        RGBController_SonyDS4*  rgb_controller  = new RGBController_SonyDS4(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR("Sony DualShock 4", DetectSonyDS4Controllers, SONY_DUALSHOCK_VID, SONY_DS4_V1_PID);
REGISTER_HID_DETECTOR("Sony DualShock 4", DetectSonyDS4Controllers, SONY_DUALSHOCK_VID, SONY_DS4_V2_PID);
REGISTER_HID_DETECTOR("Sony DualShock 4", DetectSonyDS4Controllers, SONY_DUALSHOCK_VID, SONY_DS4_RECEIVER_PID);
