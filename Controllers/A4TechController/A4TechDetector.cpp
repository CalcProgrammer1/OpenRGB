/*-------------------------------------------------------------------*\
|  A4TechDetector.cpp                                                 |
|                                                                     |
|  Driver for A4Tech Devices Detector                                 |
|                                                                     |
|  Chris M (Dr_No)                              30 Jun 2022           |
|  Mohammed Julfikar Ali Mahbub (o-julfikar)    01 Apr 2024           |
|                                                                     |
\*-------------------------------------------------------------------*/

/*-----------------------------------------------------*\
| OpenRGB includes                                      |
\*-----------------------------------------------------*/
#include <hidapi/hidapi.h>
#include "Detector.h"
#include "RGBController.h"


/*-----------------------------------------------------*\
| A4 Tech specific includes                             |
\*-----------------------------------------------------*/
#include "RGBController_BloodyMouse.h"
#include "BloodyB820RController.h"
#include "RGBController_BloodyB820R.h"

/*-----------------------------------------------------*\
| A4 Tech USB vendor ID                                 |
\*-----------------------------------------------------*/
#define A4_TECH_VID                                 0x09DA

void DetectA4TechMouseControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        BloodyMouseController* controller           = new BloodyMouseController(dev, info->path, info->product_id);
        RGBController_BloodyMouse* rgb_controller   = new RGBController_BloodyMouse(controller);
        rgb_controller->name                        = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectBloodyB820R(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        BloodyB820RController*     controller     = new BloodyB820RController(dev, info->path);
        RGBController_BloodyB820R* rgb_controller = new RGBController_BloodyB820R(controller);
        rgb_controller->name                      = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}


REGISTER_HID_DETECTOR_IPU("Bloody W60 Pro",         DetectA4TechMouseControllers,       A4_TECH_VID,    BLOODY_W60_PRO_PID,     2,      0xFF33,     0x0529);
REGISTER_HID_DETECTOR_IPU("Bloody W90 Max",         DetectA4TechMouseControllers,       A4_TECH_VID,    BLOODY_W90_MAX_PID,     2,      0xFF33,     0x053D);
REGISTER_HID_DETECTOR_IPU("Bloody MP 50RS",         DetectA4TechMouseControllers,       A4_TECH_VID,    BLOODY_MP_50RS_PID,     2,      0xFFF2,     0x6009);

REGISTER_HID_DETECTOR_IPU("A4Tech Bloody B820R",    DetectBloodyB820R,                  A4_TECH_VID,    BLOODY_B820R_PID,       2,      0xFF52,     0x0210);
