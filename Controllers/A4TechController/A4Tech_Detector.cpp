/*-----------------------------------------------------*\
| OpenRGB includes                                      |
\*-----------------------------------------------------*/
#include <hidapi/hidapi.h>
#include "Detector.h"
#include "LogManager.h"
#include "RGBController.h"

/*-----------------------------------------------------*\
| A4 Tech specific includes                             |
\*-----------------------------------------------------*/
#include "RGBController_BloodyMouse.h"

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

REGISTER_HID_DETECTOR_IPU("Bloody W60 Pro",         DetectA4TechMouseControllers,       A4_TECH_VID,    BLOODY_W60_PRO_PID,     2,      0xFF33,     0x0529);
REGISTER_HID_DETECTOR_IPU("Bloody MP 50RS",         DetectA4TechMouseControllers,       A4_TECH_VID,    BLOODY_MP_50RS_PID,     2,      0xFFF2,     0x6009);
