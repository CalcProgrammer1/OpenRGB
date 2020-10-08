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

void DetectSonyDS4Controllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_init();

    info = hid_enumerate(0x00, 0x00);

    while (info)
    {
        if(info->vendor_id == vendor_id)
        {
            if(info->product_id == product_id_v1 || info->product_id == product_id_v2)
            {
                hid_device* dev = hid_open_path(info->path);

                if (dev != NULL)
                {
                    SonyDS4Controller*      controller      = new SonyDS4Controller(dev, info->path);
                    RGBController_SonyDS4*  rgb_controller  = new RGBController_SonyDS4(controller);
                    rgb_controllers.push_back(rgb_controller);
                }
            }
        }
        info = info->next;
    }

    hid_free_enumeration(info);
}

REGISTER_DETECTOR("Sony DualShock 4", DetectSonyDS4Controllers);
