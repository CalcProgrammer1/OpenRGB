#include "Detector.h"
#include "IntelArcA770LEController.h"
#include "RGBController.h"
#include "RGBController_IntelArcA770LE.h"
#include <hidapi/hidapi.h>

#define INTEL_ARC_A770_LIMITED_EDITION_VID  0x2516
#define INTEL_ARC_A770_LIMITED_EDITION_PID  0x01B5

/******************************************************************************************\
*                                                                                          *
*   DetectIntelArcA770LEControllers                                                        *
*                                                                                          *
*       Tests the USB address to see if an Intel Arc A770 LE controller exists there.      *
*                                                                                          *
\******************************************************************************************/

void DetectIntelArcA770LEControllers(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        IntelArcA770LEController*     controller     = new IntelArcA770LEController(dev, info->path);
        RGBController_IntelArcA770LE* rgb_controller = new RGBController_IntelArcA770LE(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IP("Intel Arc A770 Limited Edition", DetectIntelArcA770LEControllers, INTEL_ARC_A770_LIMITED_EDITION_VID, INTEL_ARC_A770_LIMITED_EDITION_PID, 1, 0xFF00);
