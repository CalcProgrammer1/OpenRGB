#include "Detector.h"
#include "NZXTHue1Controller.h"
#include "RGBController.h"
#include "RGBController_NZXTHue1.h"
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| NZXT USB IDs                                          |
\*-----------------------------------------------------*/
#define NZXT_VID                        0x1E71
#define NZXT_SMART_DEVICE_V1_PID        0x1714

/******************************************************************************************\
*                                                                                          *
*   DetectNZXTHue1Controllers                                                              *
*                                                                                          *
*       Detect devices supported by the NZXTHue1 driver                                    *
*                                                                                          *
\******************************************************************************************/

void DetectNZXTHue1Controllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        NZXTHue1Controller*     controller     = new NZXTHue1Controller(dev, 3, info->path);
        RGBController_NZXTHue1* rgb_controller = new RGBController_NZXTHue1(controller);
        rgb_controller->name                   = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectNZXTHue1Controllers() */

REGISTER_HID_DETECTOR("NZXT Smart Device V1",   DetectNZXTHue1Controllers,  NZXT_VID,   NZXT_SMART_DEVICE_V1_PID);
