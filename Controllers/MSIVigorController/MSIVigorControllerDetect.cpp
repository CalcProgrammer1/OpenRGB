#include "Detector.h"
#include "MSIVigorGK30Controller.h"
#include "RGBController.h"
#include "RGBController_MSIVigorGK30.h"

/*---------------------------------------------------------*\
| MSI vendor ID                                             |
\*---------------------------------------------------------*/
#define MSI_VID                                        0x0DB0

/*---------------------------------------------------------*\
| Product ID                                                |
\*---------------------------------------------------------*/
#define MSI_VIGOR_GK30_PID                             0x0B30

void DetectMSIVigorGK30Controllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        MSIVigorGK30Controller*     controller      = new MSIVigorGK30Controller(dev, *info);
        RGBController_MSIVigorGK30* rgb_controller  = new RGBController_MSIVigorGK30(controller);
        rgb_controller->name                        = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("MSI Vigor GK30 controller", DetectMSIVigorGK30Controllers, MSI_VID, MSI_VIGOR_GK30_PID, 1, 0xFF01, 1);
