/*-----------------------------------------*\
|  LianLiControllerDetect.cpp               |
|                                           |
|  Detector for Lian Li Uni Hub USB         |
|  lighting controller                      |
|                                           |
|  Luca Lovisa 2/20/2021                    |
\*-----------------------------------------*/

#include <string>
#include <vector>

#include "Detector.h"
#include "ResourceManager.h"

#ifdef __FreeBSD__
#include <libusb.h>
#else
#include <libusb-1.0/libusb.h>
#endif

/*-----------------------------------------------------*\
| LianLi USB Controller specific includes               |
\*-----------------------------------------------------*/
#include "RGBController_LianLiUniHub.h"
#include "RGBController_StrimerLConnect.h"
#include "LianLiUniHubController.h"
#include "RGBController_LianLiUniHub.h"
#include "LianLiUniHubALController.h"
#include "RGBController_LianLiUniHubAL.h"
#include "LianLiUniHub_AL10Controller.h"
#include "RGBController_LianLiUniHub_AL10.h"
#include "LianLiUniHubSLV2Controller.h"
#include "RGBController_LianLiUniHubSLV2.h"

/*-----------------------------------------------------*\
| ENE USB vendor ID                                     |
\*-----------------------------------------------------*/
#define ENE_USB_VID                                 0x0CF2

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
\*-----------------------------------------------------*/
#define STRIMER_L_CONNECT_PID                       0xA200

/*-----------------------------------------------------*\
| Fan controller product IDs                            |
\*-----------------------------------------------------*/
#define UNI_HUB_PID                                 0x7750
#define UNI_HUB_AL_PID                              0xA101
#define UNI_HUB_SLV2_PID                            0xA103

/*----------------------------------------------------------------------------*\
| The Uni Hub is controlled by sending control transfers to various wIndex     |
| addresses, allthough it announces some kind of hid interface. Hence it       |
| requires libusb as hidapi provides no wIndex customization.                  |
\*----------------------------------------------------------------------------*/

void DetectLianLiUniHub()
{
    libusb_device** devices = nullptr;

    int ret;

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

    int deviceCount = ret;

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

    int ret;

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

    int deviceCount = ret;

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

void DetectStrimerControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        StrimerLConnectController* controller           = new StrimerLConnectController(dev, info->path);
        RGBController_StrimerLConnect* rgb_controller   = new RGBController_StrimerLConnect(controller);
        rgb_controller->name                            = name;

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_DETECTOR("Lian Li Uni Hub", DetectLianLiUniHub);
REGISTER_HID_DETECTOR_IPU("Lian Li Uni Hub - AL", DetectLianLiUniHubAL,    ENE_USB_VID,  UNI_HUB_AL_PID,           0x01,  0xFF72, 0xA1);
REGISTER_HID_DETECTOR_IPU("Lian Li Uni Hub - SL V2", DetectLianLiUniHubSLV2,    ENE_USB_VID,  UNI_HUB_SLV2_PID,           0x01,  0xFF72, 0xA1);
/*---------------------------------------------------------------------------------------------------------*\
| Entries for dynamic UDEV rules                                                                            |
|                                                                                                           |
| DUMMY_DEVICE_DETECTOR("Lian Li Uni Hub", DetectLianLiUniHub, 0x0CF2, 0x7750 )                             |
\*---------------------------------------------------------------------------------------------------------*/

REGISTER_HID_DETECTOR_IPU("Strimer L Connect",   DetectStrimerControllers,  ENE_USB_VID,  STRIMER_L_CONNECT_PID,      1,  0xFF72, 0xA1);
