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
#include "LianLiUniHubController.h"
#include "RGBController_LianLiUniHub.h"
#include "ResourceManager.h"

#include <libusb-1.0/libusb.h>

#define UNI_HUB_VID 0x0CF2
#define UNI_HUB_PID 0x7750

/*----------------------------------------------------------------------------*\
| The Uni Hub is controlled by sending control transfers to various wIndex     |
| addresses, allthough it announces some kind of hid interface. Hence it       |
| requires libusb as hidapi provides no wIndex customization.                  |
\*----------------------------------------------------------------------------*/

void DetectLianLiUniHub(std::vector<RGBController*>&)
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

        if (ret < 0)
        {
            continue;
        }

        if( descriptor.idVendor  == UNI_HUB_VID
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

REGISTER_DETECTOR("Lian Li Uni Hub", DetectLianLiUniHub);
