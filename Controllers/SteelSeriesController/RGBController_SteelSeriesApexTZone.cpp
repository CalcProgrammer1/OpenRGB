/*-----------------------------------------*\
|  RGBController_SteelSeriesApexTZone.cpp   |
|                                           |
|  Edbgon 06.10.21                          |
\*-----------------------------------------*/

#include "RGBController_SteelSeriesApexTZone.h"

RGBController_SteelSeriesApexTZone::RGBController_SteelSeriesApexTZone(SteelSeriesApexTZoneController* apex_tzone_ptr)
{
    ApexTZone     = apex_tzone_ptr;
    
    name        = ApexTZone->GetDeviceName();
    vendor      = "SteelSeries";
    type        = DEVICE_TYPE_KEYBOARD; 
    description = "SteelSeries Apex 3 device";
    location    = ApexTZone->GetDeviceLocation();
    serial      = ApexTZone->GetSerialString();

    mode direct;
    direct.name       = "Direct";
    direct.value      = 0;
    direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    direct.color_mode = MODE_COLORS_PER_LED;
    direct.brightness_min   = 0x00;
    direct.brightness_max   = 0x64;
    direct.brightness       = 0x64;    
    modes.push_back(direct);

    SetupZones();
}

RGBController_SteelSeriesApexTZone::~RGBController_SteelSeriesApexTZone()
{
    delete ApexTZone;
}


void RGBController_SteelSeriesApexTZone::DeviceSaveMode()
{
    ApexTZone->Save();
}

void RGBController_SteelSeriesApexTZone::SetupZones()
{
    for (int i = 1; i <= 10; i++)
    {
        zone curr_zone;
        curr_zone.name        = "Zone " + std::to_string(i);
        curr_zone.type        = ZONE_TYPE_LINEAR;
        curr_zone.leds_min    = 1;
        curr_zone.leds_max    = 1;
        curr_zone.leds_count  = 1;
        curr_zone.matrix_map  = NULL;
        zones.push_back(curr_zone);
        
        led zone_led;
        zone_led.name = "Zone " + std::to_string(i);
        leds.push_back(zone_led);
    }

    SetupColors();
}

void RGBController_SteelSeriesApexTZone::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SteelSeriesApexTZone::DeviceUpdateLEDs()
{
    ApexTZone->SetColor(colors, modes[active_mode].brightness);
}

void RGBController_SteelSeriesApexTZone::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}


void RGBController_SteelSeriesApexTZone::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesApexTZone::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_SteelSeriesApexTZone::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
