/******************************************************************************************\
*                                                                                          *
*   DetectRoccatControllers                                                                *
*                                                                                          *
*       Tests the USB address to see if a Roccat Kone Aimo controller exists there.        *
*                                                                                          *
\******************************************************************************************/

#include "Detector.h"
#include "RoccatKoneAimoController.h"
#include "RGBController.h"
#include "RGBController_RoccatKoneAimo.h"
#include <hidapi/hidapi.h>

#define ROCCAT_VID                  0x1E7D

void DetectRoccatControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if (dev)
    {
        RoccatKoneAimoController * controller = new RoccatKoneAimoController(dev, info->path);
        RGBController_RoccatKoneAimo * rgb_controller = new RGBController_RoccatKoneAimo(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectRoccatControllers() */

REGISTER_HID_DETECTOR_IPU("Roccat Kone Aimo 16K", DetectRoccatControllers, ROCCAT_VID, 0x2E2C, 0, 0x0B, 0);
REGISTER_HID_DETECTOR_IPU("Roccat Kone Aimo",     DetectRoccatControllers, ROCCAT_VID, 0x2E27, 0, 0x0B, 0);
