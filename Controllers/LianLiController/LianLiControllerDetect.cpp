/*---------------------------------------------------------*\
| LianLiControllerDetect.cpp                                |
|                                                           |
|   Detector for Lian Li devices                            |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string>
#include <vector>
#include <libusb.h>
#include "Detector.h"
#include "ResourceManager.h"

/*-----------------------------------------------------*\
| LianLi USB Controller specific includes               |
\*-----------------------------------------------------*/
#include "RGBController_LianLiUniHub.h"
#include "RGBController_LianLiStrimerLConnect.h"
#include "LianLiUniHubController.h"
#include "RGBController_LianLiUniHub.h"
#include "LianLiUniHubSLController.h"
#include "RGBController_LianLiUniHubSL.h"
#include "LianLiUniHubALController.h"
#include "RGBController_LianLiUniHubAL.h"
#include "LianLiUniHub_AL10Controller.h"
#include "RGBController_LianLiUniHub_AL10.h"
#include "LianLiUniHubSLV2Controller.h"
#include "RGBController_LianLiUniHubSLV2.h"
#include "LianLiUniHubSLInfinityController.h"
#include "RGBController_LianLiUniHubSLInfinity.h"
#include "LianLiGAIITrinityController.h"
#include "RGBController_LianLiGAIITrinity.h"
#include "LianLiUniversalScreenController.h"
#include "RGBController_LianLiUniversalScreen.h"

/*-----------------------------------------------------*\
| USB vendor IDs                                        |
\*-----------------------------------------------------*/
#define ENE_USB_VID                                 0x0CF2
#define NUVOTON_USB_VID                             0x0416

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define STRIMER_L_CONNECT_PID                       0xA200

/*-----------------------------------------------------*\
| Fan controller product IDs                            |
\*-----------------------------------------------------*/
#define UNI_HUB_PID                                 0x7750
#define UNI_HUB_SL_PID                              0xA100
#define UNI_HUB_AL_PID                              0xA101
#define UNI_HUB_SLINF_PID                           0xA102
#define UNI_HUB_SLV2_PID                            0xA103
#define UNI_HUB_ALV2_PID                            0xA104
#define UNI_HUB_SLV2_V05_PID                        0xA105
#define GAII_USB_PID                                0x7373
#define GAII_Perf_USB_PID                           0x7371

/*-----------------------------------------------------*\
| Screen product IDs                                    |
\*-----------------------------------------------------*/
#define UNIVERSAL_SCREEN_LED_PID                    0x8050

/*----------------------------------------------------------------------------*\
| The Uni Hub is controlled by sending control transfers to various wIndex     |
| addresses, allthough it announces some kind of hid interface. Hence it       |
| requires libusb as hidapi provides no wIndex customization.                  |
\*----------------------------------------------------------------------------*/

void DetectLianLiUniHub()
{
    libusb_device** devices = nullptr;

    ssize_t ret;

    ret = libusb_init(NULL);
    if(ret < 0)
    {
        return;
    }

    ret = libusb_get_device_list(NULL, &devices);
    if(ret < 0)
    {
        return;
    }

    ssize_t deviceCount = ret;

    for(int i = 0; i < deviceCount; i++)
    {
        libusb_device* device = devices[i];
        libusb_device_descriptor descriptor;
        ret = libusb_get_device_descriptor(device, &descriptor);

        if(ret < 0)
        {
            continue;
        }

        if( descriptor.idVendor  == ENE_USB_VID
         && descriptor.idProduct == UNI_HUB_PID)
        {
            LianLiUniHubController*     controller     = new LianLiUniHubController(device, &descriptor);
            RGBController_LianLiUniHub* rgb_controller = new RGBController_LianLiUniHub(controller);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }

    if(devices != nullptr)
    {
        libusb_free_device_list(devices, 1);
    }
}

void DetectLianLiUniHub_AL10()
{
    libusb_device** devices = nullptr;

    ssize_t ret;

    ret = libusb_init(NULL);
    if(ret < 0)
    {
        return;
    }

    ret = libusb_get_device_list(NULL, &devices);
    if(ret < 0)
    {
        return;
    }

    ssize_t deviceCount = ret;

    for(int i = 0; i < deviceCount; i++)
    {
        libusb_device* device = devices[i];
        libusb_device_descriptor descriptor;
        ret = libusb_get_device_descriptor(device, &descriptor);

        if(ret < 0)
        {
            continue;
        }

        if( descriptor.idVendor  == ENE_USB_VID
         && descriptor.idProduct == UNI_HUB_AL_PID)
        {
            LianLiUniHub_AL10Controller*     controller     = new LianLiUniHub_AL10Controller(device, &descriptor);
            RGBController_LianLiUniHub_AL10* rgb_controller = new RGBController_LianLiUniHub_AL10(controller);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }

    if(devices != nullptr)
    {
        libusb_free_device_list(devices, 1);
    }
}   /* DetectLianLiUniHub_AL10() */

void DetectLianLiUniHubSL(hid_device_info* info, const std::string& name)
{
    hid_device* device = hid_open_path(info->path);
    if (!device)
    {
        return;
    }

    LianLiUniHubSLController* controller = new LianLiUniHubSLController(device, info->path);
    std::string version = controller->ReadVersion();

    if (version != "v1.8")
    {
        delete controller;
        return;
    }

    RGBController_LianLiUniHubSL* rgb_controller = new RGBController_LianLiUniHubSL(controller, name);
    ResourceManager::get()->RegisterRGBController(rgb_controller);
}   /* DetectLianLiUniHubSL() */

void DetectLianLiUniHubAL(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LianLiUniHubALController* controller = new LianLiUniHubALController(dev, info->path, info->product_id, name);

        std::string firmwareVersion = controller->GetFirmwareVersionString();

        if(firmwareVersion == "v1.7")
        {
            RGBController_LianLiUniHubAL* rgb_controller = new RGBController_LianLiUniHubAL(controller);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else if(firmwareVersion == "v1.0")
        {
            delete controller;
            REGISTER_DETECTOR("Lian Li Uni Hub - AL", DetectLianLiUniHub_AL10);
        }
        else
        {
            delete controller;
            return;
        }

    }
}   /* DetectLianLiUniHubAL() */

void DetectLianLiUniHubSLV2(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LianLiUniHubSLV2Controller* controller = new LianLiUniHubSLV2Controller(dev, info->path, name);

        RGBController_LianLiUniHubSLV2* rgb_controller = new RGBController_LianLiUniHubSLV2(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectLianLiUniHubSLV2() */

void DetectLianLiUniHubSLInfinity(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LianLiUniHubSLInfinityController* controller = new LianLiUniHubSLInfinityController(dev, info->path, name);

        RGBController_LianLiUniHubSLInfinity* rgb_controller = new RGBController_LianLiUniHubSLInfinity(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectLianLiUniHubSLInfinity() */

void DetectLianLiStrimerControllers(hid_device_info* info, const std::string& /*name*/)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LianLiStrimerLConnectController*     controller       = new LianLiStrimerLConnectController(dev, info->path);
        RGBController_LianLiStrimerLConnect* rgb_controller   = new RGBController_LianLiStrimerLConnect(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectLianLiGAIITrinity(hid_device_info* info, const std::string& /*name*/)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LianLiGAIITrinityController*     controller     = new LianLiGAIITrinityController(dev, info->path);
        RGBController_LianLiGAIITrinity* rgb_controller = new RGBController_LianLiGAIITrinity(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectLianLiUniversalScreen()
{
    libusb_init(NULL);

    #ifdef _WIN32
    libusb_set_option(NULL, LIBUSB_OPTION_USE_USBDK);
    #endif

    libusb_device_handle * dev = libusb_open_device_with_vid_pid(NULL, NUVOTON_USB_VID, UNIVERSAL_SCREEN_LED_PID);

    if(dev)
    {
        libusb_detach_kernel_driver(dev, 0);
        libusb_claim_interface(dev, 0);

        LianLiUniversalScreenController*     controller     = new LianLiUniversalScreenController(dev);
        RGBController_LianLiUniversalScreen* rgb_controller = new RGBController_LianLiUniversalScreen(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

/*---------------------------------------------------------------------------------------------------------*\
| Entries for dynamic UDEV rules for libusb devices                                                         |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("Lian Li Uni Hub", DetectLianLiUniHub, 0x0CF2, 0x7750 )                             |
| DUMMY_DEVICE_DETECTOR("Lian Li Universal Screen", DetectLianLiUniversalScreen, 0x0416, 0x8050 )           |
\*---------------------------------------------------------------------------------------------------------*/
REGISTER_DETECTOR("Lian Li Uni Hub",                            DetectLianLiUniHub);
REGISTER_DETECTOR("Lian Li Universal Screen",                   DetectLianLiUniversalScreen);

REGISTER_HID_DETECTOR_IPU("Lian Li Uni Hub - SL",               DetectLianLiUniHubSL,           ENE_USB_VID,        UNI_HUB_SL_PID,           0x01,   0xFF72, 0xA1);
REGISTER_HID_DETECTOR_IPU("Lian Li Uni Hub - AL",               DetectLianLiUniHubAL,           ENE_USB_VID,        UNI_HUB_AL_PID,           0x01,   0xFF72, 0xA1);
REGISTER_HID_DETECTOR_IPU("Lian Li Uni Hub - SL V2",            DetectLianLiUniHubSLV2,         ENE_USB_VID,        UNI_HUB_SLV2_PID,         0x01,   0xFF72, 0xA1);
REGISTER_HID_DETECTOR_IPU("Lian Li Uni Hub - AL V2",            DetectLianLiUniHubSLV2,         ENE_USB_VID,        UNI_HUB_ALV2_PID,         0x01,   0xFF72, 0xA1);
REGISTER_HID_DETECTOR_IPU("Lian Li Uni Hub - SL V2 v0.5",       DetectLianLiUniHubSLV2,         ENE_USB_VID,        UNI_HUB_SLV2_V05_PID,     0x01,   0xFF72, 0xA1);
REGISTER_HID_DETECTOR_IPU("Lian Li Uni Hub - SL Infinity",      DetectLianLiUniHubSLInfinity,   ENE_USB_VID,        UNI_HUB_SLINF_PID,        0x01,   0xFF72, 0xA1);
REGISTER_HID_DETECTOR_IPU("Lian Li Strimer L Connect",          DetectLianLiStrimerControllers, ENE_USB_VID,        STRIMER_L_CONNECT_PID,       1,   0xFF72, 0xA1);
REGISTER_HID_DETECTOR_I("Lian Li GA II Trinity",                DetectLianLiGAIITrinity,        NUVOTON_USB_VID,    GAII_USB_PID,                             0x02);
REGISTER_HID_DETECTOR_I("Lian Li GA II Trinity Performance",    DetectLianLiGAIITrinity,        NUVOTON_USB_VID,    GAII_Perf_USB_PID,                        0x02);
