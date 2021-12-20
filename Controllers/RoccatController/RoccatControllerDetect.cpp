/******************************************************************************************\
*                                                                                          *
*   DetectRoccatControllers                                                                *
*                                                                                          *
*       Tests the USB address to see if a Roccat Kone Aimo controller exists there.        *
*                                                                                          *
\******************************************************************************************/

#include "Detector.h"
#include "RoccatKoneAimoController.h"
#include "RoccatVulcanAimoController.h"
#include "RGBController.h"
#include "RGBController_RoccatKoneAimo.h"
#include "RGBController_RoccatVulcanAimo.h"
#include <hidapi/hidapi.h>

#define ROCCAT_VID                  0x1E7D

#define ROCCAT_KONE_AIMO_PID        0x2E27
#define ROCCAT_KONE_AIMO_16K_PID    0x2E2C
#define ROCCAT_VULCAN_120_AIMO_PID  0x3098

void DetectRoccatMouseControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RoccatKoneAimoController *     controller     = new RoccatKoneAimoController(dev, info->path);
        RGBController_RoccatKoneAimo * rgb_controller = new RGBController_RoccatKoneAimo(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectRoccatKeyboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RoccatVulcanAimoController *     controller     = new RoccatVulcanAimoController(dev, info->path);
        RGBController_RoccatVulcanAimo * rgb_controller = new RGBController_RoccatVulcanAimo(controller);
        rgb_controller->name = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Roccat Kone Aimo",       DetectRoccatMouseControllers,    ROCCAT_VID, ROCCAT_KONE_AIMO_PID,          0, 0x0B,    0);
REGISTER_HID_DETECTOR_IPU("Roccat Kone Aimo 16K",   DetectRoccatMouseControllers,    ROCCAT_VID, ROCCAT_KONE_AIMO_16K_PID,      0, 0x0B,    0);
REGISTER_HID_DETECTOR_IP ("Roccat Vulcan 120 Aimo", DetectRoccatKeyboardControllers, ROCCAT_VID, ROCCAT_VULCAN_120_AIMO_PID,    3,          1);
