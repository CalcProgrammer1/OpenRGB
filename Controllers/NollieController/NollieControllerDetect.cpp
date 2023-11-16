/*-----------------------------------------*\
|  NollieControllerDetect.cpp               |
|                                           |
|  Driver for Nollie32 LED strip controller |
|                                           |
|  Name (cnn1236661)          25th Jun 2023 |
\*-----------------------------------------*/

#include "Detector.h"
#include "NollieController.h"
#include "RGBController.h"
#include "RGBController_Nollie.h"
#include <vector>
#include <hidapi/hidapi.h>


void DetectNollieControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        wchar_t product[128];
        hid_get_product_string(dev, product, 128);

        std::wstring product_str(product);

        NollieController*     controller     = new NollieController(dev, info->path,info->product_id);
        RGBController_Nollie* rgb_controller = new RGBController_Nollie(controller);
        rgb_controller->name                       = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);

    }
}

REGISTER_HID_DETECTOR("Nollie 32CH", DetectNollieControllers, NOLLIE32_VID, NOLLIE32_PID);
REGISTER_HID_DETECTOR("Nollie 16CH", DetectNollieControllers, NOLLIE16_VID, NOLLIE16_PID);
