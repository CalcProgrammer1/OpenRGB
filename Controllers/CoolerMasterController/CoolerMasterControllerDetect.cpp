/*-----------------------------------------------------*\
| OpenRGB includes                                      |
\*-----------------------------------------------------*/
#include <hidapi/hidapi.h>
#include "Detector.h"
#include "LogManager.h"
#include "RGBController.h"

/*-----------------------------------------------------*\
| Coolermaster specific includes                        |
\*-----------------------------------------------------*/
#include "RGBController_CMMMController.h"
#include "RGBController_CMMM711Controller.h"
#include "RGBController_CMMP750Controller.h"
#include "RGBController_CMARGBController.h"
#include "RGBController_CMSmallARGBController.h"
#include "RGBController_CMARGBGen2A1Controller.h"
#include "RGBController_CMRGBController.h"
#include "RGBController_CMR6000Controller.h"
#include "RGBController_CMMKController.h"

/*-----------------------------------------------------*\
| Coolermaster USB vendor ID                            |
\*-----------------------------------------------------*/
#define COOLERMASTER_VID                            0x2516

/*-----------------------------------------------------*\
| Coolermaster Keyboards                                |
\*-----------------------------------------------------*/
#define COOLERMASTER_MASTERKEYS_PRO_L_PID           0x003B
#define COOLERMASTER_MASTERKEYS_PRO_L_WHITE_PID     0x0047
#define COOLERMASTER_MASTERKEYS_PRO_S_PID           0x003C
#define COOLERMASTER_MASTERKEYS_MK750_PID           0x0067
#define COOLERMASTER_MASTERKEYS_SK630_PID           0x0089
#define COOLERMASTER_MASTERKEYS_SK650_PID           0x008D

/*-----------------------------------------------------*\
| Coolermaster GPUs                                     |
|   PIDs defined in `CMR6000Controller.h`               |
\*-----------------------------------------------------*/

/*-----------------------------------------------------*\
| Coolermaster LEDstrip controllers                     |
\*-----------------------------------------------------*/
#define COOLERMASTER_ARGB_PID                       0x1011
#define COOLERMASTER_ARGB_GEN2_A1_PID               0x0173
#define COOLERMASTER_ARGB_GEN2_A1_V2_PID            0x01C9
#define COOLERMASTER_SMALL_ARGB_PID                 0x1000
#define COOLERMASTER_RGB_PID                        0x004F

/*-----------------------------------------------------*\
| Coolermaster Mice                                     |
\*-----------------------------------------------------*/
#define COOLERMASTER_MM530_PID                      0x0065
#define COOLERMASTER_MM531_PID                      0x0097
#define COOLERMASTER_MM711_PID                      0x0101
#define COOLERMASTER_MM720_PID                      0x0141
#define COOLERMASTER_MM730_PID                      0x0165

/*-----------------------------------------------------*\
| Coolermaster Mousemats                                |
\*-----------------------------------------------------*/
#define COOLERMASTER_MP750_XL_PID                   0x0109
#define COOLERMASTER_MP750_L_PID                    0x0107
#define COOLERMASTER_MP750_MEDIUM_PID               0x0105

/******************************************************************************************\
*                                                                                          *
*   DetectCoolerMasterControllers                                                          *
*                                                                                          *
*       Tests the USB address to see if any CoolerMaster controllers exists there.         *
*                                                                                          *
\******************************************************************************************/

void DetectCoolerMasterARGB(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        /*-------------------------------------------------*\
        | Create mutex to prevent the controllers sharing a |
        |   receiver from interfering with each other       |
        \*-------------------------------------------------*/
        std::shared_ptr<std::mutex>       cm_mutex = std::make_shared<std::mutex>();

        for(std::size_t i = 0; i < CM_ARGB_HEADER_DATA_SIZE; i++)
        {
            CMARGBController*               controller     = new CMARGBController(dev, info->path, i, cm_mutex);
            RGBController_CMARGBController* rgb_controller = new RGBController_CMARGBController(controller);
            // Constructor sets the name
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}

void DetectCoolerMasterARGBGen2A1(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CMARGBGen2A1controller*               controller     = new CMARGBGen2A1controller(dev, *info);
        RGBController_CMARGBGen2A1Controller* rgb_controller = new RGBController_CMARGBGen2A1Controller(controller);
        rgb_controller->name                                 = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectCoolerMasterGPU(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CMR6000Controller*               controller     = new CMR6000Controller(dev, info->path, info->product_id);
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
        CMMKController*               controller     = new CMMKController(dev, info);
        RGBController_CMMKController* rgb_controller = new RGBController_CMMKController(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectCoolerMasterMouse(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        switch(info->product_id)
        {
            case COOLERMASTER_MM530_PID:
            case COOLERMASTER_MM531_PID:
            case COOLERMASTER_MM720_PID:
            case COOLERMASTER_MM730_PID:
                {
                    CMMMController*               controller        = new CMMMController(dev, info->path, info->product_id);
                    RGBController_CMMMController* rgb_controller    = new RGBController_CMMMController(controller);
                    rgb_controller->name                            = name;
                    ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
                break;

            case COOLERMASTER_MM711_PID:
                {
                    CMMM711Controller*               controller     = new CMMM711Controller(dev, info->path);
                    RGBController_CMMM711Controller* rgb_controller = new RGBController_CMMM711Controller(controller);
                    // Constructor sets the name
                    ResourceManager::get()->RegisterRGBController(rgb_controller);
                }
                break;

            default:
                LOG_DEBUG("[%s] Controller not created as the product ID %04X is missing from detector switch", name.c_str(), info->product_id);
         }
    }
}

void DetectCoolerMasterMousemats(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CMMP750Controller*               controller     = new CMMP750Controller(dev, info->path);
        RGBController_CMMP750Controller* rgb_controller = new RGBController_CMMP750Controller(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectCoolerMasterRGB(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CMRGBController*               controller     = new CMRGBController(dev, info->path);
        RGBController_CMRGBController* rgb_controller = new RGBController_CMRGBController(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectCoolerMasterSmallARGB(hid_device_info* info, const std::string&)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CMSmallARGBController*               controller     = new CMSmallARGBController(dev, info->path, 0);
        RGBController_CMSmallARGBController* rgb_controller = new RGBController_CMSmallARGBController(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

/*-----------------------------------------------------*\
| Coolermaster Keyboards                                |
\*-----------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Cooler Master MasterKeys Pro L",         DetectCoolerMasterKeyboards,    COOLERMASTER_VID,   COOLERMASTER_MASTERKEYS_PRO_L_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MasterKeys Pro L White",   DetectCoolerMasterKeyboards,    COOLERMASTER_VID,   COOLERMASTER_MASTERKEYS_PRO_L_WHITE_PID,    1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MasterKeys Pro S",         DetectCoolerMasterKeyboards,    COOLERMASTER_VID,   COOLERMASTER_MASTERKEYS_PRO_S_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MK750",                    DetectCoolerMasterKeyboards,    COOLERMASTER_VID,   COOLERMASTER_MASTERKEYS_MK750_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master SK630",                    DetectCoolerMasterKeyboards,    COOLERMASTER_VID,   COOLERMASTER_MASTERKEYS_SK630_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master SK650",                    DetectCoolerMasterKeyboards,    COOLERMASTER_VID,   COOLERMASTER_MASTERKEYS_SK650_PID,          1,      0xFF00, 1);

/*-----------------------------------------------------*\
| Coolermaster LEDstrip controllers                     |
\*-----------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Cooler Master ARGB",                     DetectCoolerMasterARGB,         COOLERMASTER_VID,   COOLERMASTER_ARGB_PID,                      0,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master ARGB Gen 2 A1",            DetectCoolerMasterARGBGen2A1,   COOLERMASTER_VID,   COOLERMASTER_ARGB_GEN2_A1_PID,              1,      0xFF01, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master ARGB Gen 2 A1 V2",         DetectCoolerMasterARGBGen2A1,   COOLERMASTER_VID,   COOLERMASTER_ARGB_GEN2_A1_V2_PID,           1,      0xFF01, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master RGB",                      DetectCoolerMasterRGB,          COOLERMASTER_VID,   COOLERMASTER_RGB_PID,                       1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master Small ARGB",               DetectCoolerMasterSmallARGB,    COOLERMASTER_VID,   COOLERMASTER_SMALL_ARGB_PID,                0,      0xFF00, 1);

/*-----------------------------------------------------*\
| Coolermaster Mice                                     |
\*-----------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Cooler Master MM530",                    DetectCoolerMasterMouse,        COOLERMASTER_VID,   COOLERMASTER_MM530_PID,                     1,      0xFF00, 1);
//REGISTER_HID_DETECTOR_IPU("Cooler Master MM531",                  DetectCoolerMasterMouse,        COOLERMASTER_VID,   COOLERMASTER_MM531_PID,                     1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MM711",                    DetectCoolerMasterMouse,        COOLERMASTER_VID,   COOLERMASTER_MM711_PID,                     1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MM720",                    DetectCoolerMasterMouse,        COOLERMASTER_VID,   COOLERMASTER_MM720_PID,                     1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MM730",                    DetectCoolerMasterMouse,        COOLERMASTER_VID,   COOLERMASTER_MM730_PID,                     1,      0xFF00, 1);

/*-----------------------------------------------------*\
| Coolermaster Mousemats                                |
\*-----------------------------------------------------*/
REGISTER_HID_DETECTOR_PU ("Cooler Master MP750 XL",                 DetectCoolerMasterMousemats,    COOLERMASTER_VID,   COOLERMASTER_MP750_XL_PID,                          0xFF00, 1);
REGISTER_HID_DETECTOR_PU ("Cooler Master MP750 Large",              DetectCoolerMasterMousemats,    COOLERMASTER_VID,   COOLERMASTER_MP750_L_PID,                           0xFF00, 1);
REGISTER_HID_DETECTOR_PU ("Cooler Master MP750 Medium",             DetectCoolerMasterMousemats,    COOLERMASTER_VID,   COOLERMASTER_MP750_MEDIUM_PID,                      0xFF00, 1);

/*-----------------------------------------------------*\
| Coolermaster GPUs                                     |
|   PIDs defined in `CMR6000Controller.h`               |
\*-----------------------------------------------------*/
REGISTER_HID_DETECTOR_I  ("Cooler Master Radeon 6000 GPU",          DetectCoolerMasterGPU,          COOLERMASTER_VID,   COOLERMASTER_RADEON_6000_PID,               1                );
REGISTER_HID_DETECTOR_I  ("Cooler Master Radeon 6900 GPU",          DetectCoolerMasterGPU,          COOLERMASTER_VID,   COOLERMASTER_RADEON_6900_PID,               1                );
