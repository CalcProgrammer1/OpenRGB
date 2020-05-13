/*-----------------------------------------*\
|  RGBController_SteelSeriesRival.cpp       |
|                                           |
|  Generic RGB Interface SteelSeriesRival   |
|  Class                                    |
|                                           |
|  B Horn (bahorn) 13/05/2020               |
\*-----------------------------------------*/

#include "RGBController_SteelSeriesRival.h"

RGBController_SteelSeriesRival::RGBController_SteelSeriesRival(SteelSeriesRivalController* rival_ptr)
{
    rival = rival_ptr;
    
    name        = "SteelSeries Rival 110";
    type        = DEVICE_TYPE_MOUSE; 
    description = "SteelSeries Rival 110";

    mode Static;
    Static.name       = "Static";
    Static.value      = STEELSERIES_RIVAL_STATIC;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Pulsate;
    Pulsate.name       = "Pulsate";
    Pulsate.value      = STEELSERIES_RIVAL_PULSATE;
    Pulsate.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Pulsate.color_mode = MODE_COLORS_PER_LED;
    Pulsate.speed_min  = STEELSERIES_RIVAL_EFFECT_PULSATE_MIN;
    Pulsate.speed_max  = STEELSERIES_RIVAL_EFFECT_PULSATE_MAX;
    Pulsate.speed      = STEELSERIES_RIVAL_EFFECT_PULSATE_MID;
    modes.push_back(Pulsate);

    SetupZones();
}

void RGBController_SteelSeriesRival::SetupZones()
{
    zone main_zone;
    main_zone.name          = "Main";
    main_zone.type          = ZONE_TYPE_SINGLE;
    main_zone.leds_min      = 1;
    main_zone.leds_max      = 1;
    main_zone.leds_count    = 1;
    main_zone.matrix_map    = NULL;
    zones.push_back(main_zone);

    led main_led;
    main_led.name = "Main LED";
    leds.push_back(main_led);

    SetupColors();
}

void RGBController_SteelSeriesRival::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SteelSeriesRival::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);
    rival->SetColor(red, grn, blu);
}

void RGBController_SteelSeriesRival::UpdateZoneLEDs(int zone)
{
    RGBColor      color = colors[zone];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);
    if(zone == 0)
    {
        rival->SetColor(red, grn, blu);
    }
}   

void RGBController_SteelSeriesRival::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_SteelSeriesRival::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_SteelSeriesRival::UpdateMode()
{
    switch (modes[active_mode].value)
    {
        case STEELSERIES_RIVAL_STATIC:
            rival->SetLightEffect(STEELSERIES_RIVAL_EFFECT_STATIC);
            break;
        case STEELSERIES_RIVAL_PULSATE:
            rival->SetLightEffect(modes[active_mode].speed);
            break;
    }

    DeviceUpdateLEDs();
}
