/*---------------------------------------------------------*\
| KlipschLuminaControllerDetect.cpp                         |
|                                                           |
|   Detector for Klipsch ProMedia Lumina speakers           |
|                                                           |
|   wlof                                        26 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DetectionManager.h"
#include "hidapi_wrapper.h"
#include "KlipschLuminaController.h"
#include "LogManager.h"
#include "RGBController_KlipschLumina.h"

/*---------------------------------------------------------*\
| Klipsch vendor and product IDs                            |
\*---------------------------------------------------------*/
#define KLIPSCH_VID                         0x18B5
#define KLIPSCH_PROMEDIA_LUMINA_PID         0x0070

/*---------------------------------------------------------*\
| Klipsch ProMedia Lumina interfaces                        |
\*---------------------------------------------------------*/
#define KLIPSCH_LUMINA_VENDOR_IFACE         4
#define KLIPSCH_LUMINA_LAMPARRAY_IFACE      5

DetectedControllers DetectKlipschLuminaControllers(hidapi_wrapper wrapper, hid_device_info* info, const std::string& /*name*/)
{
    DetectedControllers detected_controllers;
    hid_device*         dev;

    dev = wrapper.hid_open_path(info->path);

    if(dev)
    {
        /*-------------------------------------------------*\
        | Open the LampArray interface, not available on    |
        | Windows where the OS holds it                     |
        \*-------------------------------------------------*/
        hid_device*         lamparray_dev   = NULL;
        hid_device_info*    devices         = wrapper.hid_enumerate(KLIPSCH_VID, KLIPSCH_PROMEDIA_LUMINA_PID);

        for(hid_device_info* device = devices; device != NULL; device = device->next)
        {
            if(device->interface_number == KLIPSCH_LUMINA_LAMPARRAY_IFACE)
            {
                lamparray_dev = wrapper.hid_open_path(device->path);
                break;
            }
        }

        wrapper.hid_free_enumeration(devices);

        KlipschLuminaController* controller = new KlipschLuminaController(wrapper, dev, lamparray_dev, info->path);

        /*-------------------------------------------------*\
        | Only create the RGBController if the device       |
        | answered the state request.  Otherwise, delete    |
        | the controller.                                   |
        \*-------------------------------------------------*/
        if(controller->GetLightMode() != KLIPSCH_LUMINA_MODE_UNKNOWN)
        {
            RGBController_KlipschLumina* rgb_controller = new RGBController_KlipschLumina(controller);

            detected_controllers.push_back(rgb_controller);
        }
        else
        {
            LOG_WARNING("[Klipsch ProMedia Lumina] No state report, skipping");
            delete controller;
        }
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| Empty HID detector so the generic HID LampArray detector  |
| skips this device                                         |
\*---------------------------------------------------------*/
DetectedControllers DetectKlipschLuminaHIDControllers(hid_device_info* /*info*/, const std::string& /*name*/)
{
    DetectedControllers detected_controllers;

    return(detected_controllers);
}

REGISTER_HID_WRAPPED_DETECTOR_I("Klipsch ProMedia Lumina", DetectKlipschLuminaControllers, KLIPSCH_VID, KLIPSCH_PROMEDIA_LUMINA_PID, KLIPSCH_LUMINA_VENDOR_IFACE);
REGISTER_HID_DETECTOR("Klipsch ProMedia Lumina", DetectKlipschLuminaHIDControllers, KLIPSCH_VID, KLIPSCH_PROMEDIA_LUMINA_PID);
