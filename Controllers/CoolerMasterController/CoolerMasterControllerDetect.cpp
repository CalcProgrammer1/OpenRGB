#include "Detector.h"
#include "CMMP750Controller.h"
#include "CMARGBcontroller.h"
#include "RGBController.h"
#include "RGBController_CMMP750Controller.h"
#include "RGBController_CMARGBController.h"
#include <hidapi/hidapi.h>

#define COOLERMASTER_VID                0x2516

#define COOLERMASTER_MP750_XL_PID       0x0109
#define COOLERMASTER_MP750_MEDIUM_PID   0x0105
#define COOLERMASTER_ARGB_PID           0x1011

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

REGISTER_HID_DETECTOR_IPU("Cooler Master MP750 XL",     DetectCoolerMasterMousemats, COOLERMASTER_VID, COOLERMASTER_MP750_XL_PID,     0, 0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MP750 Medium", DetectCoolerMasterMousemats, COOLERMASTER_VID, COOLERMASTER_MP750_MEDIUM_PID, 0, 0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master ARGB",         DetectCoolerMasterARGB,      COOLERMASTER_VID, COOLERMASTER_ARGB_PID,         0, 0xFF00, 1);
