/*---------------------------------------------------------*\
| CoolerMasterControllerDetect.cpp                          |
|                                                           |
|   Detector for Cooler Master devices                      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

/*-----------------------------------------------------*\
| OpenRGB includes                                      |
\*-----------------------------------------------------*/
#include <hidapi.h>
#include "Detector.h"
#include "LogManager.h"

/*-----------------------------------------------------*\
| Coolermaster specific includes                        |
\*-----------------------------------------------------*/
#include "RGBController_CMMMController.h"
#include "RGBController_CMMM711Controller.h"
#include "RGBController_CMMM712Controller.h"
#include "RGBController_CMMP750Controller.h"
#include "RGBController_CMARGBController.h"
#include "RGBController_CMSmallARGBController.h"
#include "RGBController_CMARGBGen2A1Controller.h"
#include "RGBController_CMRGBController.h"
#include "RGBController_CMR6000Controller.h"
#include "RGBController_CMMonitorController.h"
#include "RGBController_CMKeyboardController.h"

/*-----------------------------------------------------*\
| Coolermaster USB vendor ID                            |
\*-----------------------------------------------------*/
#define COOLERMASTER_VID                            0x2516

/*-----------------------------------------------------*\
| Coolermaster Keyboards                                |
|   PIDs defined in `CMMKControllerV2.h`                |
\*-----------------------------------------------------*/

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
#define COOLERMASTER_ARGB_GEN2_A1_MINI_PID          0x01CB
#define COOLERMASTER_SMALL_ARGB_PID                 0x1000
#define COOLERMASTER_RGB_PID                        0x004F

/*-----------------------------------------------------*\
| Coolermaster Mice                                     |
\*-----------------------------------------------------*/
#define COOLERMASTER_MM530_PID                      0x0065
#define COOLERMASTER_MM531_PID                      0x0097
#define COOLERMASTER_MM711_PID                      0x0101
#define COOLERMASTER_MM712_PID                      0x0169
#define COOLERMASTER_MM720_PID                      0x0141
#define COOLERMASTER_MM730_PID                      0x0165

/*-----------------------------------------------------*\
| Coolermaster Mousemats                                |
\*-----------------------------------------------------*/
#define COOLERMASTER_MP750_XL_PID                   0x0109
#define COOLERMASTER_MP750_L_PID                    0x0107
#define COOLERMASTER_MP750_MEDIUM_PID               0x0105

/*-----------------------------------------------------*\
| Coolermaster Monitors                                 |
\*-----------------------------------------------------*/
#define COOLERMASTER_GM27_FQS_PID                   0x01BB

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

        for(unsigned char i = 0; i < CM_ARGB_HEADER_DATA_SIZE; i++)
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

void DetectCoolerMasterV1Keyboards(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        switch(info->product_id)
        {
            case COOLERMASTER_KEYBOARD_PRO_L_PID:
            case COOLERMASTER_KEYBOARD_PRO_L_WHITE_PID:
            case COOLERMASTER_KEYBOARD_PRO_S_PID:
            {
                CMKeyboardV1Controller* controller                 = new CMKeyboardV1Controller(dev, info);
                controller->SetDeviceName(name);
                RGBController_CMKeyboardController* rgb_controller = new RGBController_CMKeyboardController(controller);
                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
            break;

            default:
                LOG_DEBUG("[%s] Controller not created as the product ID %04X is missing from detector switch", name.c_str(), info->product_id);
            break;
        }
    }
}

void DetectCoolerMasterV2Keyboards(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        switch(info->product_id)
        {
            case COOLERMASTER_KEYBOARD_PRO_L_PID:
            case COOLERMASTER_KEYBOARD_PRO_L_WHITE_PID:
            case COOLERMASTER_KEYBOARD_PRO_S_PID:
            {
                CMKeyboardV1Controller* controller                 = new CMKeyboardV1Controller(dev, info);
                controller->SetDeviceName(name);
                RGBController_CMKeyboardController* rgb_controller = new RGBController_CMKeyboardController(controller);
                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
            break;

            case COOLERMASTER_KEYBOARD_SK622B_PID:
            case COOLERMASTER_KEYBOARD_SK622W_PID:
            case COOLERMASTER_KEYBOARD_SK630_PID:
            case COOLERMASTER_KEYBOARD_SK650_PID:
            case COOLERMASTER_KEYBOARD_SK652_PID:
            case COOLERMASTER_KEYBOARD_SK653_PID:
            case COOLERMASTER_KEYBOARD_CK530_PID:
            case COOLERMASTER_KEYBOARD_CK530_V2_PID:
            case COOLERMASTER_KEYBOARD_CK550_V2_PID:
            case COOLERMASTER_KEYBOARD_CK552_V2_PID:
            case COOLERMASTER_KEYBOARD_MK730_PID:
            case COOLERMASTER_KEYBOARD_MK750_PID:
            {
                CMKeyboardV2Controller* controller                 = new CMKeyboardV2Controller(dev, info);
                controller->SetDeviceName(name);
                RGBController_CMKeyboardController* rgb_controller = new RGBController_CMKeyboardController(controller);
                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
            break;

            default:
                LOG_DEBUG("[%s] Controller not created as the product ID %04X is missing from detector switch", name.c_str(), info->product_id);
            break;
        }
    }
}

void DetectCoolerMasterMouse(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CMMMController*               controller        = new CMMMController(dev, info->path, info->product_id);
        RGBController_CMMMController* rgb_controller    = new RGBController_CMMMController(controller);
        rgb_controller->name                            = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectCoolerMasterMouse711(hid_device_info* info, const std::string& /*name*/)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CMMM711Controller*               controller     = new CMMM711Controller(dev, info->path);
        RGBController_CMMM711Controller* rgb_controller = new RGBController_CMMM711Controller(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectCoolerMasterMouse712(hid_device_info* info, const std::string& /*name*/)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CMMM712Controller*               controller     = new CMMM712Controller(dev, info->path);
        RGBController_CMMM712Controller* rgb_controller = new RGBController_CMMM712Controller(controller);
        // Constructor sets the name
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectCoolerMasterMousemats(hid_device_info* info, const std::string& /*name*/)
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

void DetectCoolerMasterRGB(hid_device_info* info, const std::string& /*name*/)
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

void DetectCoolerMasterSmallARGB(hid_device_info* info, const std::string& /*name*/)
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

void DetectCoolerMasterMonitor(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CMMonitorController*               controller     = new CMMonitorController(dev, *info);
        RGBController_CMMonitorController* rgb_controller = new RGBController_CMMonitorController(controller);
        rgb_controller->name                                = name;
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

/*-----------------------------------------------------*\
| Coolermaster Keyboards                                |
|   PIDs defined in `CMKeyboardDevices.h`               |
\*-----------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Cooler Master MasterKeys Pro S",         DetectCoolerMasterV1Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_PRO_S_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MasterKeys Pro L",         DetectCoolerMasterV1Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_PRO_L_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MasterKeys Pro L White",   DetectCoolerMasterV1Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_PRO_L_WHITE_PID,    1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MK850",                    DetectCoolerMasterV2Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_MK850_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master SK620 White",              DetectCoolerMasterV2Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_SK620W_PID,         1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master SK620 Black",              DetectCoolerMasterV2Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_SK620B_PID,         1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master SK622 White",              DetectCoolerMasterV2Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_SK622W_PID,         1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master SK622 Black",              DetectCoolerMasterV2Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_SK622B_PID,         1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master SK630",                    DetectCoolerMasterV2Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_SK630_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master SK650",                    DetectCoolerMasterV2Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_SK650_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master SK652",                    DetectCoolerMasterV2Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_SK652_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master SK653",                    DetectCoolerMasterV2Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_SK653_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MK730",                    DetectCoolerMasterV2Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_MK730_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MK750",                    DetectCoolerMasterV2Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_MK750_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master CK530",                    DetectCoolerMasterV2Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_CK530_PID,          1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master CK530 V2",                 DetectCoolerMasterV2Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_CK530_V2_PID,       1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master CK550 V2",                 DetectCoolerMasterV2Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_CK550_V2_PID,       1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master CK550 V1 / CK552",         DetectCoolerMasterV2Keyboards,  COOLERMASTER_VID,   COOLERMASTER_KEYBOARD_CK552_V2_PID,       1,      0xFF00, 1);

/*-----------------------------------------------------*\
| Coolermaster LEDstrip controllers                     |
\*-----------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Cooler Master ARGB",                     DetectCoolerMasterARGB,         COOLERMASTER_VID,   COOLERMASTER_ARGB_PID,                      0,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master ARGB Gen 2 A1",            DetectCoolerMasterARGBGen2A1,   COOLERMASTER_VID,   COOLERMASTER_ARGB_GEN2_A1_PID,              1,      0xFF01, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master ARGB Gen 2 A1 V2",         DetectCoolerMasterARGBGen2A1,   COOLERMASTER_VID,   COOLERMASTER_ARGB_GEN2_A1_V2_PID,           1,      0xFF01, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master ARGB Gen 2 A1 Mini",       DetectCoolerMasterARGBGen2A1,   COOLERMASTER_VID,   COOLERMASTER_ARGB_GEN2_A1_MINI_PID,         1,      0xFF01, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master RGB",                      DetectCoolerMasterRGB,          COOLERMASTER_VID,   COOLERMASTER_RGB_PID,                       1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master Small ARGB",               DetectCoolerMasterSmallARGB,    COOLERMASTER_VID,   COOLERMASTER_SMALL_ARGB_PID,                0,      0xFF00, 1);

/*-----------------------------------------------------*\
| Coolermaster Mice                                     |
\*-----------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Cooler Master MM530",                    DetectCoolerMasterMouse,        COOLERMASTER_VID,   COOLERMASTER_MM530_PID,                     1,      0xFF00, 1);
//REGISTER_HID_DETECTOR_IPU("Cooler Master MM531",                  DetectCoolerMasterMouse,        COOLERMASTER_VID,   COOLERMASTER_MM531_PID,                     1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MM711",                    DetectCoolerMasterMouse711,     COOLERMASTER_VID,   COOLERMASTER_MM711_PID,                     1,      0xFF00, 1);
REGISTER_HID_DETECTOR_IPU("Cooler Master MM712",                    DetectCoolerMasterMouse712,     COOLERMASTER_VID,   COOLERMASTER_MM712_PID,                     3,      0xFF0A, 2);
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
REGISTER_HID_DETECTOR_I  ("Cooler Master Radeon RX 6000 GPU",        DetectCoolerMasterGPU,          COOLERMASTER_VID,   COOLERMASTER_RADEON_6000_PID,               1                );
REGISTER_HID_DETECTOR_I  ("Cooler Master Radeon RX 6900 GPU",        DetectCoolerMasterGPU,          COOLERMASTER_VID,   COOLERMASTER_RADEON_6900_PID,               1                );

/*-----------------------------------------------------*\
| Coolermaster Monitors                                 |
\*-----------------------------------------------------*/
REGISTER_HID_DETECTOR_IPU("Cooler Master GM27-FQS ARGB Monitor",    DetectCoolerMasterMonitor,      COOLERMASTER_VID,   COOLERMASTER_GM27_FQS_PID,                   0,      0xFF00, 1);
