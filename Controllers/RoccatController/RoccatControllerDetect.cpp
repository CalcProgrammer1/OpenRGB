/******************************************************************************************\
*                                                                                          *
*   DetectRoccatControllers                                                                *
*                                                                                          *
*       Tests the USB address to see if a Roccat Kone Aimo controller exists there.        *
*                                                                                          *
\******************************************************************************************/

#include "Detector.h"
#include "RoccatBurstController.h"
#include "RoccatKoneAimoController.h"
#include "RoccatVulcanAimoController.h"
#include "RGBController.h"
#include "RGBController_RoccatBurst.h"
#include "RGBController_RoccatHordeAimo.h"
#include "RGBController_RoccatKoneAimo.h"
#include "RGBController_RoccatVulcanAimo.h"
#include <hidapi/hidapi.h>
#include <unordered_set>

#define ROCCAT_VID                  0x1E7D

#define ROCCAT_KONE_AIMO_PID        0x2E27
#define ROCCAT_KONE_AIMO_16K_PID    0x2E2C
#define ROCCAT_VULCAN_120_AIMO_PID  0x3098
#define ROCCAT_HORDE_AIMO_PID       0x303E
#define ROCCAT_BURST_CORE_PID       0x2DE6
#define ROCCAT_BURST_PRO_PID        0x2DE1

void DetectRoccatMouseControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RoccatKoneAimoController *     controller     = new RoccatKoneAimoController(dev, info->path);
        RGBController_RoccatKoneAimo * rgb_controller = new RGBController_RoccatKoneAimo(controller);
        rgb_controller->name                          = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

/*---------------------------------------------------------------------------*\
| Tracks the paths used in DetectRoccatKeyboardControllers so multiple Roccat |
| devices can be detected without all controlling the same device.            |
\*---------------------------------------------------------------------------*/
static std::unordered_set<std::string> used_paths;

/*--------------------------------------------------------------------------------*\
| Removes all entries in used_paths so device discovery does not skip any of them. |
\*--------------------------------------------------------------------------------*/
void ResetRoccatKeyboardControllersPaths()
{
    used_paths.clear();
}

void DetectRoccatKeyboardControllers(hid_device_info* info, const std::string& name)
{
    /*-------------------------------------------------------------------------------------------------*\
    | Create a local copy of the HID enumerations for the Roccat Keyboard VID/PID and iterate           |
    | through it.  This prevents detection from failing if interface 1 comes before interface 0 in the  |
    | main info list.                                                                                   |
    \*-------------------------------------------------------------------------------------------------*/
    hid_device* dev_ctrl       = nullptr;
    hid_device* dev_led        = nullptr;
    hid_device_info* info_full = hid_enumerate(info->vendor_id, info->product_id);
    hid_device_info* info_temp = info_full;
    /*--------------------------------------------------------------------------------------------*\
    | Keep track of paths so they can be added to used_paths only if both interfaces can be found. |
    \*--------------------------------------------------------------------------------------------*/
    std::string dev_ctrl_path;
    std::string dev_led_path;

    while(info_temp)
    {
        /*---------------------------------------------------------------------------------*\
        | Check for paths used on an already registered Roccat Keyboard controller to avoid |
        | registering multiple controllers that refer to the same physical hardware.        |
        \*---------------------------------------------------------------------------------*/
        if(info_temp->vendor_id             == info->vendor_id
        && info_temp->product_id            == info->product_id
        && used_paths.find(info_temp->path) == used_paths.end() )
        {
            if(info_temp->interface_number == 1 && info_temp->usage_page == 11)
            {
                dev_ctrl      = hid_open_path(info_temp->path);
                dev_ctrl_path = info_temp->path;
            }
            else if(info_temp->interface_number == 3 && info_temp->usage_page == 1)
            {
                dev_led      = hid_open_path(info_temp->path);
                dev_led_path = info_temp->path;
            }
        }
        if(dev_ctrl && dev_led)
        {
            break;
        }
        info_temp = info_temp->next;
    }

    hid_free_enumeration(info_full);

    if(dev_ctrl && dev_led)
    {
        RoccatVulcanAimoController *     controller     = new RoccatVulcanAimoController(dev_ctrl, dev_led, info->path);
        RGBController_RoccatVulcanAimo * rgb_controller = new RGBController_RoccatVulcanAimo(controller);
        rgb_controller->name                            = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
        used_paths.insert(dev_ctrl_path);
        used_paths.insert(dev_led_path);
    }
    else
    {
        // Not all of them could be opened, do some cleanup
        hid_close(dev_ctrl);
        hid_close(dev_led);
    }
}

void DetectRoccatHordeAimoKeyboardControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RoccatHordeAimoController *     controller      = new RoccatHordeAimoController(dev, *info);
        RGBController_RoccatHordeAimo * rgb_controller  = new RGBController_RoccatHordeAimo(controller);
        rgb_controller->name                            = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectRoccatBurstCoreControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RoccatBurstController *     controller      = new RoccatBurstController(dev, *info);
        RGBController_RoccatBurst * rgb_controller  = new RGBController_RoccatBurst(controller, ROCCAT_BURST_CORE_NUMBER_OF_LEDS);
        rgb_controller->name                            = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectRoccatBurstProControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RoccatBurstController *     controller      = new RoccatBurstController(dev, *info);
        RGBController_RoccatBurst * rgb_controller  = new RGBController_RoccatBurst(controller, ROCCAT_BURST_PRO_NUMBER_OF_LEDS);
        rgb_controller->name                            = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_IPU("Roccat Kone Aimo",               DetectRoccatMouseControllers,               ROCCAT_VID, ROCCAT_KONE_AIMO_PID,          0, 0x0B,    0 );
REGISTER_HID_DETECTOR_IPU("Roccat Kone Aimo 16K",           DetectRoccatMouseControllers,               ROCCAT_VID, ROCCAT_KONE_AIMO_16K_PID,      0, 0x0B,    0 );
REGISTER_HID_DETECTOR_IP ("Roccat Vulcan 120 Aimo",         DetectRoccatKeyboardControllers,            ROCCAT_VID, ROCCAT_VULCAN_120_AIMO_PID,    1,          11);
REGISTER_DYNAMIC_DETECTOR("Roccat Vulcan 120 Aimo Setup",   ResetRoccatKeyboardControllersPaths);
REGISTER_HID_DETECTOR_IPU("Roccat Horde Aimo",              DetectRoccatHordeAimoKeyboardControllers,   ROCCAT_VID, ROCCAT_HORDE_AIMO_PID,         1, 0x0B,    0);
REGISTER_HID_DETECTOR_IPU("Roccat Burst Core",              DetectRoccatBurstCoreControllers,           ROCCAT_VID, ROCCAT_BURST_CORE_PID,         3, 0xFF01,  1);
REGISTER_HID_DETECTOR_IPU("Roccat Burst Pro",               DetectRoccatBurstProControllers,            ROCCAT_VID, ROCCAT_BURST_PRO_PID,          3, 0xFF01,  1);
