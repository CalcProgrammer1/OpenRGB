/*---------------------------------------------------------*\
| RGBController_Virtual.cpp                                 |
|                                                           |
|   Virtual RGBController that can be filled out by a       |
|   plugin, including function pointers for implementing    |
|   functionality plugin-side.                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                18 May 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_Virtual.h"
#include "RGBControllerInterface.h"

RGBController_Virtual::RGBController_Virtual(RGBController_Setup* setup)
{
    UpdateVirtualController(setup);
}

RGBController_Virtual::~RGBController_Virtual()
{

}

void RGBController_Virtual::SetupLEDs(RGBController_Setup* setup)
{
    leds.clear();

    unsigned int led_idx = 0;

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(std::size_t zone_led_idx = 0; zone_led_idx < zones[zone_idx].leds_count; zone_led_idx++)
        {
            if(led_idx < setup->leds.size())
            {
                leds.push_back(setup->leds[led_idx]);
            }
            else
            {
                led new_led;

                new_led.name    = "Zone " + std::to_string(zone_idx) + ", LED " + std::to_string(zone_led_idx);
                new_led.value   = 0;

                leds.push_back(new_led);
            }

            led_idx++;
        }
    }
}

void RGBController_Virtual::UpdateVirtualController(RGBController_Setup* setup)
{
    AccessMutex.lock();
    description                                     = setup->description;
    location                                        = setup->location;
    name                                            = setup->name;
    serial                                          = setup->serial;
    vendor                                          = setup->vendor;
    version                                         = setup->version;
    configuration                                   = setup->configuration;

    modes                                           = setup->modes;
    zones                                           = setup->zones;

    active_mode                                     = setup->active_mode;
    type                                            = setup->type;
    flags                                           = setup->flags & CONTROLLER_FLAG_VIRTUAL;

    DeviceConfigureZone_ptr                         = setup->DeviceConfigureZone;
    DeviceUpdateLEDs_ptr                            = setup->DeviceUpdateLEDs;
    DeviceUpdateZoneLEDs_ptr                        = setup->DeviceUpdateZoneLEDs;
    DeviceUpdateSingleLED_ptr                       = setup->DeviceUpdateSingleLED;
    DeviceUpdateMode_ptr                            = setup->DeviceUpdateMode;
    DeviceUpdateZoneMode_ptr                        = setup->DeviceUpdateZoneMode;
    DeviceSaveMode_ptr                              = setup->DeviceSaveMode;
    DeviceUpdateDeviceSpecificConfiguration_ptr     = setup->DeviceUpdateDeviceSpecificConfiguration;
    DeviceUpdateDeviceSpecificZoneConfiguration_ptr = setup->DeviceUpdateDeviceSpecificZoneConfiguration;

    object_ptr                                      = setup->object_ptr;

    SetupLEDs(setup);
    SetupColors();
    AccessMutex.unlock();

    /*-----------------------------------------------------*\
    | The device description was replaced in place, notify  |
    | subscribers so cached views of it are rebuilt.        |
    | Signal outside AccessMutex, callbacks take it shared. |
    \*-----------------------------------------------------*/
    SignalUpdate(RGBCONTROLLER_UPDATE_REASON_DEVICE_CHANGED);
}

void RGBController_Virtual::DeviceConfigureZone(int zone_idx)
{
    if(DeviceConfigureZone_ptr)
    {
        AccessMutex.unlock();
        DeviceConfigureZone_ptr(object_ptr, zone_idx);
        AccessMutex.lock();
    }
}

void RGBController_Virtual::DeviceUpdateLEDs()
{
    if(DeviceUpdateLEDs_ptr)
    {
        AccessMutex.unlock_shared();
        DeviceUpdateLEDs_ptr(object_ptr);
        AccessMutex.lock_shared();
    }
}

void RGBController_Virtual::DeviceUpdateZoneLEDs(int zone)
{
    if(DeviceUpdateZoneLEDs_ptr)
    {
        AccessMutex.unlock_shared();
        DeviceUpdateZoneLEDs_ptr(object_ptr, zone);
        AccessMutex.lock_shared();
    }
}

void RGBController_Virtual::DeviceUpdateSingleLED(int led)
{
    if(DeviceUpdateSingleLED_ptr)
    {
        AccessMutex.unlock_shared();
        DeviceUpdateSingleLED_ptr(object_ptr, led);
        AccessMutex.lock_shared();
    }
}

void RGBController_Virtual::DeviceUpdateMode()
{
    if(DeviceUpdateMode_ptr)
    {
        AccessMutex.unlock_shared();
        DeviceUpdateMode_ptr(object_ptr);
        AccessMutex.lock_shared();
    }
}

void RGBController_Virtual::DeviceUpdateZoneMode(int zone)
{
    if(DeviceUpdateZoneMode_ptr)
    {
        AccessMutex.unlock_shared();
        DeviceUpdateZoneMode_ptr(object_ptr, zone);
        AccessMutex.lock_shared();
    }
}

void RGBController_Virtual::DeviceSaveMode()
{
    if(DeviceSaveMode_ptr)
    {
        AccessMutex.unlock_shared();
        DeviceSaveMode_ptr(object_ptr);
        AccessMutex.lock_shared();
    }
}

void RGBController_Virtual::DeviceUpdateDeviceSpecificConfiguration()
{
    if(DeviceUpdateDeviceSpecificConfiguration_ptr)
    {
        AccessMutex.unlock();
        DeviceUpdateDeviceSpecificConfiguration_ptr(object_ptr);
        AccessMutex.lock();
    }
}

void RGBController_Virtual::DeviceUpdateDeviceSpecificZoneConfiguration(int zone)
{
    if(DeviceUpdateDeviceSpecificZoneConfiguration_ptr)
    {
        AccessMutex.unlock();
        DeviceUpdateDeviceSpecificZoneConfiguration_ptr(object_ptr, zone);
        AccessMutex.lock();
    }
}
