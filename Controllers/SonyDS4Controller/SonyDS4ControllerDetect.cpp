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

const unsigned short vendor_id     = 0x054C;
const unsigned short product_id_v1 = 0x05C4;
const unsigned short product_id_v2 = 0x09CC;
const unsigned short adapter_id    = 0x0ba0;

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

REGISTER_HID_DETECTOR("Sony DualShock 4", DetectSonyDS4Controllers, vendor_id, product_id_v1);
REGISTER_HID_DETECTOR("Sony DualShock 4", DetectSonyDS4Controllers, vendor_id, product_id_v2);
REGISTER_HID_DETECTOR("Sony DualShock 4", DetectSonyDS4Controllers, vendor_id, adapter_id);