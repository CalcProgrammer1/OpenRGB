#include "Detector.h"
#include "GigabyteAorusLaptopController.h"
#include "RGBController.h"
#include "RGBController_GigabyteAorusLaptop.h"

/*---------------------------------------------------------*\
| Gigabyte vendor ID                                        |
\*---------------------------------------------------------*/
#define GIGABYTE_AORUS_LAPTOP_VID                      0x0414

/*---------------------------------------------------------*\
| AORUS Laptops PID                                         |
\*---------------------------------------------------------*/
#define AORUS_17X_BACKLIGHT_PID                        0x7A42
#define AORUS_17X_KEYBOARD_PID                         0x7A3F

void DetectGigabyteAorusLaptopControllers(hid_device_info* info, const std::string& name, GIGABYTE_AORUS_LAPTOP_DEV_TYPE dev_type)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        GigabyteAorusLaptopController*     controller       = new GigabyteAorusLaptopController(dev, *info);
        RGBController_GigabyteAorusLaptop* rgb_controller   = new RGBController_GigabyteAorusLaptop(controller, dev_type);
        rgb_controller->name                                = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectGigabyteAorusLaptopKeyboardControllers(hid_device_info* info, const std::string& name)
{
    DetectGigabyteAorusLaptopControllers(info, name, GIGABYTE_AORUS_LAPTOP_KEYBOARD_TYPE);
}

void DetectGigabyteAorusLaptopBacklightControllers(hid_device_info* info, const std::string& name)
{
    DetectGigabyteAorusLaptopControllers(info, name, GIGABYTE_AORUS_LAPTOP_BACKLIGHT_TYPE);
}

REGISTER_HID_DETECTOR_IPU("Gigabyte Aorus 17X Keyboard",  DetectGigabyteAorusLaptopKeyboardControllers,  GIGABYTE_AORUS_LAPTOP_VID, AORUS_17X_KEYBOARD_PID, 3, 0xFF01, 0x01);
REGISTER_HID_DETECTOR_IPU("Gigabyte Aorus 17X Backlight", DetectGigabyteAorusLaptopBacklightControllers, GIGABYTE_AORUS_LAPTOP_VID, AORUS_17X_BACKLIGHT_PID, 3, 0xFF01, 0x01);
