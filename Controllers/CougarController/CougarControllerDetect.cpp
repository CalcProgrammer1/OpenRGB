#include "Detector.h"
#include "CougarRevengerSTController.h"
#include "RGBController.h"
#include "RGBController_CougarRevengerST.h"

/*---------------------------------------------------------*\
| Cougar vendor ID                                          |
\*---------------------------------------------------------*/
#define COUGAR_VID                                     0x12CF

/*---------------------------------------------------------*\
| Product ID                                                |
\*---------------------------------------------------------*/
#define COUGAR_REVENGER_ST_PID                         0x0412

void DetectCougarRevengerSTControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CougarRevengerSTController*     controller      = new CougarRevengerSTController(dev, *info);
        RGBController_CougarRevengerST* rgb_controller  = new RGBController_CougarRevengerST(controller);
        rgb_controller->name                            = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Cougar Revenger ST", DetectCougarRevengerSTControllers, COUGAR_VID, COUGAR_REVENGER_ST_PID, 0, 0x0001, 0x02);
