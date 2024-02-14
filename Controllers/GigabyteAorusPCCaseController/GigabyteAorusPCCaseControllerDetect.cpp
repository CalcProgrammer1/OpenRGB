#include <hidapi/hidapi.h>

#include "Detector.h"
#include "RGBController.h"

#include "GigabyteAorusPCCaseController.h"
#include "RGBController_GigabyteAorusPCCase.h"

/*-----------------------------------------------------*\
| Vendor ID                                             |
\*-----------------------------------------------------*/
#define HOLTEK_VID                          0x1044

/*-----------------------------------------------------*\
| Controller product ids                                |
\*-----------------------------------------------------*/
#define C300_GLASS_PID                      0x7A30

/******************************************************************************************\
*                                                                                          *
*   DetectGigabyteAorusPCCaseControllers                                                   *
*                                                                                          *
*       Tests the USB address to see if a Gigabyte Aorus PC Case exists there.             *
*                                                                                          *
\******************************************************************************************/
void DetectGigabyteAorusPCCaseControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        GigabyteAorusPCCaseController*      controller      = new GigabyteAorusPCCaseController(dev, info->path);
        RGBController_GigabyteAorusPCCase*  rgb_controller  = new RGBController_GigabyteAorusPCCase(controller);
        rgb_controller->name                                = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Gigabyte AORUS C300 GLASS",  DetectGigabyteAorusPCCaseControllers,   HOLTEK_VID, C300_GLASS_PID, 0,  0xFF01, 0x01);
