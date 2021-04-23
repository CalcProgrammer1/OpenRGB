#include "Detector.h"
#include "CMMP750Controller.h"
#include "CMARGBcontroller.h"
#include "CMSmallARGBController.h"
#include "CMR6000Controller.h"
#include "CMMKController.h"
#include "RGBController.h"
#include "RGBController_CMMP750Controller.h"
#include "RGBController_CMARGBController.h"
#include "RGBController_CMSmallARGBController.h"
#include "RGBController_CMR6000Controller.h"
#include "RGBController_CMMKController.h"
#include <hidapi/hidapi.h>

#define COOLERMASTER_VID                        0x2516

#define COOLERMASTER_MP750_XL_PID               0x0109
#define COOLERMASTER_MP750_MEDIUM_PID           0x0105
#define COOLERMASTER_ARGB_PID                   0x1011
#define COOLERMASTER_SMALL_ARGB_PID             0x1000
#define COOLERMASTER_RADEON_6000_PID            0x014D
#define COOLERMASTER_MASTERKEYS_PRO_L_PID       CMMK_USB_MASTERKEYS_MK750
#define COOLERMASTER_MASTERKEYS_PRO_L_WHITE_PID CMMK_USB_MASTERKEYS_PRO_L_WHITE
#define COOLERMASTER_MASTERKEYS_PRO_S_PID       CMMK_USB_MASTERKEYS_PRO_S 
#define COOLERMASTER_MASTERKEYS_MK750_PID       CMMK_USB_MASTERKEYS_MK750
#define COOLERMASTER_MASTERKEYS_SK630_PID       CMMK_USB_MASTERKEYS_SK630
#define COOLERMASTER_MASTERKEYS_SK650_PID       CMMK_USB_MASTERKEYS_SK650

/******************************************************************************************\
*                                                                                          *
*   DetectCoolerMasterControllers                                                          *
*                                                                                          *
*       Tests the USB address to see if any CoolerMaster controllers exists there.         *
*                                                                                          *
\******************************************************************************************/

void DetectCoolerMasterMousemats(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        CMMP750Controller* controller                   = new CMMP750Controller(dev, info->path);
        RGBController_CMMP750Controller* rgb_controller = new RGBController_CMMP750Controller(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectCoolerMasterARGB(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        for(std::size_t i = 0; i < CM_ARGB_HEADER_DATA_SIZE; i++)
        {
            CMARGBController* controller = new CMARGBController(dev, info->path, i);
            RGBController_CMARGBController* rgb_controller = new RGBController_CMARGBController(controller);
            // Constructor sets the name
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}

void DetectCoolerMasterSmallARGB(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        CMSmallARGBController* controller = new CMSmallARGBController(dev, info->path, 0);
        RGBController_CMSmallARGBController* rgb_controller = new RGBController_CMSmallARGBController(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectCoolerMasterGPU(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        CMR6000Controller* controller                   = new CMR6000Controller(dev, info->path);
        RGBController_CMR6000Controller* rgb_controller = new RGBController_CMR6000Controller(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectCoolerMasterKeyboards(hid_device_info* info, const std::string&)
{

    hid_device* dev = hid_open_path(info->path);
    if(dev)
    {
        CMMKController* controller                   = new CMMKController(dev, info);
        RGBController_CMMKController* rgb_controller = new RGBController_CMMKController(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU ("Cooler Master MP750 XL",                 DetectCoolerMasterMousemats,    COOLERMASTER_VID,   COOLERMASTER_MP750_XL_PID,                  0xFF00, 1        );
REGISTER_HID_DETECTOR_PU ("Cooler Master MP750 Medium",             DetectCoolerMasterMousemats,    COOLERMASTER_VID,   COOLERMASTER_MP750_MEDIUM_PID,              0xFF00, 1        );
REGISTER_HID_DETECTOR_IPU("Cooler Master MasterKeys Pro L",         DetectCoolerMasterKeyboards,    COOLERMASTER_VID,   COOLERMASTER_MASTERKEYS_PRO_L_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MasterKeys Pro L White",   DetectCoolerMasterKeyboards,    COOLERMASTER_VID,   COOLERMASTER_MASTERKEYS_PRO_L_WHITE_PID,    1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MasterKeys Pro S",         DetectCoolerMasterKeyboards,    COOLERMASTER_VID,   COOLERMASTER_MASTERKEYS_PRO_S_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MK570",                    DetectCoolerMasterKeyboards,    COOLERMASTER_VID,   COOLERMASTER_MASTERKEYS_MK750_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master SK630",                    DetectCoolerMasterKeyboards,    COOLERMASTER_VID,   COOLERMASTER_MASTERKEYS_SK630_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master SK650",                    DetectCoolerMasterKeyboards,    COOLERMASTER_VID,   COOLERMASTER_MASTERKEYS_SK650_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master ARGB",                     DetectCoolerMasterARGB,         COOLERMASTER_VID,   COOLERMASTER_ARGB_PID,         	            0,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master Smalll ARGB",              DetectCoolerMasterSmallARGB,    COOLERMASTER_VID,   COOLERMASTER_SMALL_ARGB_PID,   	            0,      0xFF00, 1);
REGISTER_HID_DETECTOR_I  ("Cooler Master Radeon 6000 GPU",          DetectCoolerMasterGPU,          COOLERMASTER_VID,   COOLERMASTER_RADEON_6000_PID,               1                );
