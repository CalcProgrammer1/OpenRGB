/*-----------------------------------------*\
|  RGBController_ZalmanZSync.cpp            |
|                                           |
|  Generic RGB Interface for Zalman Z Sync  |
|  lighting devices                         |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/30/2021   |
\*-----------------------------------------*/

#include "RGBController_ZalmanZSync.h"

RGBController_ZalmanZSync::RGBController_ZalmanZSync(ZalmanZSyncController* controller_ptr)
{
    controller      = controller_ptr;
    type            = DEVICE_TYPE_LEDSTRIP;
    name            = "Zalman Z Sync Device";
    description     = "Zalman Z Sync Device";
    location        = controller->GetDeviceLocation();
    serial          = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_ZalmanZSync::~RGBController_ZalmanZSync()
{

}

void RGBController_ZalmanZSync::SetupZones()
{
    for(unsigned int zone_idx = 0; zone_idx < 8; zone_idx++)
    {
        zone new_zone;

        new_zone.name           = "Channel " + std::to_string(zone_idx + 1);
        new_zone.type           = ZONE_TYPE_LINEAR;
        new_zone.leds_min       = 24;
        new_zone.leds_max       = 24;
        new_zone.leds_count     = 24;
        new_zone.matrix_map     = NULL;

        zones.push_back(new_zone);

        for(unsigned int led_idx = 0; led_idx < 24; led_idx++)
        {
            led new_led;

            new_led.name        = "LED " + std::to_string(led_idx + 1);

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_ZalmanZSync::ResizeZone(int zone, int new_size)
{

}

void RGBController_ZalmanZSync::DeviceUpdateLEDs()
{
    for(unsigned int zone_idx = 0; zone_idx < 8; zone_idx++)
    {
        controller->SetLEDs(zone_idx, zones[zone_idx].colors);
    }
}

void RGBController_ZalmanZSync::UpdateZoneLEDs(int zone)
{
    controller->SetLEDs(zone, zones[zone].colors);
}

void RGBController_ZalmanZSync::UpdateSingleLED(int led)
{
    DeviceUpdateLEDs();
}

void RGBController_ZalmanZSync::SetCustomMode()
{

}

void RGBController_ZalmanZSync::DeviceUpdateMode()
{

}