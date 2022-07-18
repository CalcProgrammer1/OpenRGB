/*-----------------------------------------*\
|  RGBController_SteelSeriesApexTZone.cpp   |
|                                           |
|  Edbgon 06.10.21                          |
\*-----------------------------------------*/

#include "RGBController_SteelSeriesApexTZone.h"

/**------------------------------------------------------------------*\
    @name Steel Series Apex Tri Zone Keyboards
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectSteelSeriesApexTZone
    @comment
\*-------------------------------------------------------------------*/

RGBController_SteelSeriesApexTZone::RGBController_SteelSeriesApexTZone(SteelSeriesApexTZoneController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetDeviceName();
    vendor                  = "SteelSeries";
    type                    = DEVICE_TYPE_KEYBOARD;
    description             = "SteelSeries Apex 3 device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    mode direct;
    direct.name             = "Direct";
    direct.value            = 0;
    direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    direct.color_mode       = MODE_COLORS_PER_LED;
    direct.brightness_min   = STEELSERIES_TZ_BRIGHTNESS_MIN;
    direct.brightness_max   = STEELSERIES_TZ_BRIGHTNESS_MAX;
    direct.brightness       = STEELSERIES_TZ_BRIGHTNESS_MAX;
    modes.push_back(direct);

    SetupZones();
}

RGBController_SteelSeriesApexTZone::~RGBController_SteelSeriesApexTZone()
{
    delete controller;
}


void RGBController_SteelSeriesApexTZone::DeviceSaveMode()
{
    controller->Save();
}

void RGBController_SteelSeriesApexTZone::SetupZones()
{
    zone curr_zone;
    curr_zone.name          = "Keyboard Zone";
    curr_zone.type          = ZONE_TYPE_LINEAR;
    curr_zone.leds_min      = 10;
    curr_zone.leds_max      = 10;
    curr_zone.leds_count    = 10;
    curr_zone.matrix_map    = NULL;
    zones.push_back(curr_zone);

    for(unsigned int i = 1; i <= curr_zone.leds_count; i++)
    {
        led zone_led;
        zone_led.name = "LED " + std::to_string(i);
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
    controller->SetColor(colors, modes[active_mode].brightness);
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
