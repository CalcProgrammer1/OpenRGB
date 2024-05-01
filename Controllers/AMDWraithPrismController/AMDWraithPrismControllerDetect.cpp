#include <hidapi/hidapi.h>
#include "Detector.h"
#include "AMDWraithPrismController.h"
#include "RGBController.h"
#include "RGBController_AMDWraithPrism.h"

/*---------------------------------------------------------*\
| AMD Wraith Prism vendor ID                                |
\*---------------------------------------------------------*/
#define AMD_WRAITH_PRISM_VID                        0x2516

/*---------------------------------------------------------*\
| AMD Wraith Prism product ID                               |
\*---------------------------------------------------------*/
#define AMD_WRAITH_PRISM_PID                        0x0051

/******************************************************************************************\
*                                                                                          *
*   DetectAMDWraithPrismControllers                                                        *
*                                                                                          *
*       Tests the USB address to see if an AMD Wraith Prism controller exists there.       *
*                                                                                          *
\******************************************************************************************/

void DetectAMDWraithPrismControllers(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        AMDWraithPrismController*     controller     = new AMDWraithPrismController(dev, info->path);
        RGBController_AMDWraithPrism* rgb_controller = new RGBController_AMDWraithPrism(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IP("AMD Wraith Prism", DetectAMDWraithPrismControllers, AMD_WRAITH_PRISM_VID, AMD_WRAITH_PRISM_PID, 1, 0xFF00);
