#include "Detector.h"
#include "AsusAuraCoreController.h"
#include "RGBController.h"
#include "RGBController_AsusAuraCore.h"
#include <hidapi/hidapi.h>

#define AURA_CORE_VID 0x0B05

/******************************************************************************************\
*                                                                                          *
*   DetectAuraCoreControllers                                                              *
*                                                                                          *
*       Tests the USB address to see if an Asus ROG Aura Core controller exists there      *
*                                                                                          *
\******************************************************************************************/

void DetectAsusAuraCoreControllers(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        AuraCoreController*     controller     = new AuraCoreController(dev, info->path);
        RGBController_AuraCore* rgb_controller = new RGBController_AuraCore(controller);
        // Constructor sets the name
        if(rgb_controller->type != DEVICE_TYPE_UNKNOWN)
        {
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}

REGISTER_HID_DETECTOR("ASUS Aura Core", DetectAsusAuraCoreControllers, AURA_CORE_VID, 0x1854);
REGISTER_HID_DETECTOR("ASUS Aura Core", DetectAsusAuraCoreControllers, AURA_CORE_VID, 0x1866);
REGISTER_HID_DETECTOR("ASUS Aura Core", DetectAsusAuraCoreControllers, AURA_CORE_VID, 0x1869);
