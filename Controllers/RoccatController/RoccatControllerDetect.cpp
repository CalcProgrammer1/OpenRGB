#include "Detector.h"
#include "RoccatBurstController.h"
#include "RoccatBurstProAirController.h"
#include "RoccatKoneAimoController.h"
#include "RoccatKoneProController.h"
#include "RoccatSenseAimoController.h"
#include "RoccatVulcanKeyboardController.h"
#include "RoccatKovaController.h"
#include "RoccatEloController.h"
#include "RGBController.h"
#include "RGBController_RoccatBurst.h"
#include "RGBController_RoccatBurstProAir.h"
#include "RGBController_RoccatHordeAimo.h"
#include "RGBController_RoccatKoneAimo.h"
#include "RGBController_RoccatKonePro.h"
#include "RGBController_RoccatSenseAimo.h"
#include "RGBController_RoccatVulcanKeyboard.h"
#include "RGBController_RoccatKova.h"
#include "RGBController_RoccatElo.h"
#include <hidapi/hidapi.h>
#include <unordered_set>

#define ROCCAT_VID                  0x1E7D

/*--------------------------------------------------------------------------------*\
|  KEYBOARDS                                                                       |
|  RoccatVulcanKeyboardController PIDs defined in RoccatVulcanKeyboardController.h |
\*--------------------------------------------------------------------------------*/
#define ROCCAT_HORDE_AIMO_PID       0x303E

/*-----------------------------------------------------------------*\
|  MICE                                                             |
\*-----------------------------------------------------------------*/
#define ROCCAT_BURST_CORE_PID       0x2DE6
#define ROCCAT_BURST_PRO_PID        0x2DE1
#define ROCCAT_BURST_PRO_AIR_PID    0x2CA6
#define ROCCAT_KONE_AIMO_PID        0x2E27
#define ROCCAT_KONE_AIMO_16K_PID    0x2E2C
#define ROCCAT_KONE_PRO_PID         0x2C88
#define ROCCAT_KOVA_PID             0x2CEE

/*-----------------------------------------------------------------*\
| MOUSEMATS                                                         |
\*-----------------------------------------------------------------*/
#define ROCCAT_SENSE_AIMO_MID_PID   0x343A
#define ROCCAT_SENSE_AIMO_XXL_PID   0x343B

/*-----------------------------------------------------------------*\
| HEADSETS                                                          |
\*-----------------------------------------------------------------*/
#define ROCCAT_ELO_PID              0x3A34

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

/*---------------------------------------------------------------------------------*\
| Tracks the paths used in DetectRoccatVulcanKeyboardControllers so multiple Roccat |
| devices can be detected without all controlling the same device.                  |
\*---------------------------------------------------------------------------------*/
static std::unordered_set<std::string> used_paths;

/*--------------------------------------------------------------------------------*\
| Removes all entries in used_paths so device discovery does not skip any of them. |
\*--------------------------------------------------------------------------------*/
void ResetRoccatVulcanKeyboardControllersPaths()
{
    used_paths.clear();
}

void DetectRoccatVulcanKeyboardControllers(hid_device_info* info, const std::string& name)
{
    /*-------------------------------------------------------------------------------------------------*\
    | Create a local copy of the HID enumerations for the Roccat Vulcan Keyboard VID/PID and iterate    |
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

    int dev_led_page;
    int dev_ctrl_page;

    switch (info->product_id)
    {
        case ROCCAT_VULCAN_PRO_PID:
        case ROCCAT_PYRO_PID:
        case ROCCAT_MAGMA_PID:
        case ROCCAT_MAGMA_MINI_PID:
            dev_led_page   = 0xFF00;
            dev_ctrl_page  = 0xFF01;
            break;
        default:
            dev_led_page   = 0x0001;
            dev_ctrl_page  = 0x000B;
            break;
    }

    while(info_temp)
    {
        /*----------------------------------------------------------------------------------------*\
        | Check for paths used on an already registered Roccat Vulcan Keyboard controller to avoid |
        | registering multiple controllers that refer to the same physical hardware.               |
        \*----------------------------------------------------------------------------------------*/
        if(info_temp->vendor_id             == info->vendor_id
        && info_temp->product_id            == info->product_id
        && used_paths.find(info_temp->path) == used_paths.end() )
        {
            if(info_temp->interface_number == 1 && info_temp->usage_page == dev_ctrl_page)
            {
                dev_ctrl      = hid_open_path(info_temp->path);
                dev_ctrl_path = info_temp->path;
            }
            else if(info_temp->interface_number == 3 && info_temp->usage_page == dev_led_page)
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
        RoccatVulcanKeyboardController *     controller     = new RoccatVulcanKeyboardController(dev_ctrl, dev_led, info->path, info->product_id);
        RGBController_RoccatVulcanKeyboard * rgb_controller = new RGBController_RoccatVulcanKeyboard(controller);
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
        RoccatBurstController *     controller          = new RoccatBurstController(dev, *info);
        RGBController_RoccatBurst * rgb_controller      = new RGBController_RoccatBurst(controller, ROCCAT_BURST_CORE_NUMBER_OF_LEDS);
        rgb_controller->name                            = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectRoccatBurstProControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RoccatBurstController *     controller          = new RoccatBurstController(dev, *info);
        RGBController_RoccatBurst * rgb_controller      = new RGBController_RoccatBurst(controller, ROCCAT_BURST_PRO_NUMBER_OF_LEDS);
        rgb_controller->name                            = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectRoccatBurstProAirControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RoccatBurstProAirController *    controller      = new RoccatBurstProAirController(dev, *info);
        RGBController_RoccatBurstProAir * rgb_controller = new RGBController_RoccatBurstProAir(controller);
        rgb_controller->name                             = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectRoccatKoneProControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RoccatKoneProController *     controller          = new RoccatKoneProController(dev, *info);
        RGBController_RoccatKonePro * rgb_controller      = new RGBController_RoccatKonePro(controller);
        rgb_controller->name                              = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectRoccatKovaControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RoccatKovaController *     controller           = new RoccatKovaController(dev, info->path);
        RGBController_RoccatKova * rgb_controller       = new RGBController_RoccatKova(controller);
        rgb_controller->name                            = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectRoccatEloControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RoccatEloController *      controller           = new RoccatEloController(dev, *info);
        RGBController_RoccatElo  * rgb_controller       = new RGBController_RoccatElo(controller);
        rgb_controller->name                            = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectRoccatSenseAimoControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        RoccatSenseAimoController *     controller      = new RoccatSenseAimoController(dev, info->path);
        RGBController_RoccatSenseAimo * rgb_controller  = new RGBController_RoccatSenseAimo(controller);
        rgb_controller->name                            = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_PRE_DETECTION_HOOK(ResetRoccatVulcanKeyboardControllersPaths);

/*-----------------------------------------------------------------*\
|  KEYBOARDS                                                        |
\*-----------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Roccat Horde Aimo",              DetectRoccatHordeAimoKeyboardControllers,   ROCCAT_VID, ROCCAT_HORDE_AIMO_PID,         1, 0x0B,    0 );

REGISTER_HID_DETECTOR_IP ("Roccat Magma",                   DetectRoccatVulcanKeyboardControllers,      ROCCAT_VID, ROCCAT_MAGMA_PID,              1,          0xFF01);
REGISTER_HID_DETECTOR_IP ("Roccat Magma Mini",              DetectRoccatVulcanKeyboardControllers,      ROCCAT_VID, ROCCAT_MAGMA_MINI_PID,         1,          0xFF01);
REGISTER_HID_DETECTOR_IP ("Roccat Pyro",                    DetectRoccatVulcanKeyboardControllers,      ROCCAT_VID, ROCCAT_PYRO_PID,               1,          0xFF01);
REGISTER_HID_DETECTOR_IP ("Roccat Vulcan 100 Aimo",         DetectRoccatVulcanKeyboardControllers,      ROCCAT_VID, ROCCAT_VULCAN_100_AIMO_PID,    1,          11);
REGISTER_HID_DETECTOR_IP ("Roccat Vulcan 120-Series Aimo",  DetectRoccatVulcanKeyboardControllers,      ROCCAT_VID, ROCCAT_VULCAN_120_AIMO_PID,    1,          11);
REGISTER_HID_DETECTOR_IP ("Roccat Vulcan TKL",              DetectRoccatVulcanKeyboardControllers,      ROCCAT_VID, ROCCAT_VULCAN_TKL_PID,         1,          11);
REGISTER_HID_DETECTOR_IP ("Roccat Vulcan Pro",              DetectRoccatVulcanKeyboardControllers,      ROCCAT_VID, ROCCAT_VULCAN_PRO_PID,         1,          0xFF01);

/*-----------------------------------------------------------------*\
|  MICE                                                             |
\*-----------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Roccat Burst Core",              DetectRoccatBurstCoreControllers,           ROCCAT_VID, ROCCAT_BURST_CORE_PID,         3, 0xFF01,  1 );
REGISTER_HID_DETECTOR_IPU("Roccat Burst Pro",               DetectRoccatBurstProControllers,            ROCCAT_VID, ROCCAT_BURST_PRO_PID,          3, 0xFF01,  1 );
REGISTER_HID_DETECTOR_IPU("Roccat Burst Pro Air",           DetectRoccatBurstProAirControllers,         ROCCAT_VID, ROCCAT_BURST_PRO_AIR_PID,      0, 0x01,    2 );

REGISTER_HID_DETECTOR_IPU("Roccat Kone Aimo",               DetectRoccatMouseControllers,               ROCCAT_VID, ROCCAT_KONE_AIMO_PID,          0, 0x0B,    0 );
REGISTER_HID_DETECTOR_IPU("Roccat Kone Aimo 16K",           DetectRoccatMouseControllers,               ROCCAT_VID, ROCCAT_KONE_AIMO_16K_PID,      0, 0x0B,    0 );

REGISTER_HID_DETECTOR_IPU("Roccat Kone Pro",                DetectRoccatKoneProControllers,             ROCCAT_VID, ROCCAT_KONE_PRO_PID,           3, 0xFF01,  1 );

REGISTER_HID_DETECTOR_IPU("Roccat Kova",                    DetectRoccatKovaControllers,                ROCCAT_VID, ROCCAT_KOVA_PID,               0, 0x0B,    0 );

/*-----------------------------------------------------------------*\
| MOUSEMATS                                                         |
\*-----------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Roccat Sense Aimo Mid",          DetectRoccatSenseAimoControllers,           ROCCAT_VID, ROCCAT_SENSE_AIMO_MID_PID,     0, 0xFF01,  1 );
REGISTER_HID_DETECTOR_IPU("Roccat Sense Aimo XXL",          DetectRoccatSenseAimoControllers,           ROCCAT_VID, ROCCAT_SENSE_AIMO_XXL_PID,     0, 0xFF01,  1 );

/*-----------------------------------------------------------------*\
| HEADSETS                                                          |
\*-----------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Roccat Elo 7.1",                 DetectRoccatEloControllers,                 ROCCAT_VID, ROCCAT_ELO_PID,                3, 0x0C,    1 );
