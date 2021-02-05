/*-----------------------------------------*\
|  RGBController_PatriotViper.cpp           |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Patriot Viper RGB interface              |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/1/2020    |
\*-----------------------------------------*/

#include "RGBController_PatriotViper.h"

RGBController_PatriotViper::RGBController_PatriotViper(PatriotViperController* viper_ptr)
{
    viper = viper_ptr;

    name        = viper->GetDeviceName();
    vendor      = "Patriot";
    type        = DEVICE_TYPE_DRAM;
    description = "Patriot Viper Device";
    location    = viper->GetDeviceLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0xFFFF;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.speed_min  = 0;
    Direct.speed_max  = 0;
    Direct.color_mode = MODE_COLORS_PER_LED;
    Direct.speed      = 0;
    modes.push_back(Direct);

    mode Dark;
    Dark.name       = "Dark";
    Dark.value      = VIPER_MODE_DARK;
    Dark.flags      = 0;
    Dark.speed_min  = 0;
    Dark.speed_max  = 0;
    Dark.color_mode = MODE_COLORS_NONE;
    Dark.speed      = 0;
    modes.push_back(Dark);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = VIPER_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min  = VIPER_SPEED_BREATHING_MIN;
    Breathing.speed_max  = VIPER_SPEED_BREATHING_MAX;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    Breathing.speed      = VIPER_SPEED_BREATHING_DEFAULT;
    modes.push_back(Breathing);

    mode Viper;
    Viper.name       = "Viper";
    Viper.value      = VIPER_MODE_VIPER;
    Viper.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Viper.speed_min  = VIPER_SPEED_MIN;
    Viper.speed_max  = VIPER_SPEED_MAX;
    Viper.color_mode = MODE_COLORS_PER_LED;
    Viper.speed      = VIPER_SPEED_DEFAULT;
    modes.push_back(Viper);

    mode Heartbeat;
    Heartbeat.name       = "Heartbeat";
    Heartbeat.value      = VIPER_MODE_HEARTBEAT;
    Heartbeat.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Heartbeat.speed_min  = VIPER_SPEED_MIN;
    Heartbeat.speed_max  = VIPER_SPEED_MAX;
    Heartbeat.color_mode = MODE_COLORS_PER_LED;
    Heartbeat.speed      = VIPER_SPEED_DEFAULT;
    modes.push_back(Heartbeat);

    mode Marquee;
    Marquee.name       = "Marquee";
    Marquee.value      = VIPER_MODE_MARQUEE;
    Marquee.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Marquee.speed_min  = VIPER_SPEED_MIN;
    Marquee.speed_max  = VIPER_SPEED_MAX;
    Marquee.color_mode = MODE_COLORS_PER_LED;
    Marquee.speed      = VIPER_SPEED_DEFAULT;
    modes.push_back(Marquee);

    mode Raindrop;
    Raindrop.name       = "Raindrop";
    Raindrop.value      = VIPER_MODE_RAINDROP;
    Raindrop.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Raindrop.speed_min  = VIPER_SPEED_MIN;
    Raindrop.speed_max  = VIPER_SPEED_MAX;
    Raindrop.color_mode = MODE_COLORS_PER_LED;
    Raindrop.speed      = VIPER_SPEED_DEFAULT;
    modes.push_back(Raindrop);

    mode Aurora;
    Aurora.name       = "Aurora";
    Aurora.value      = VIPER_MODE_AURORA;
    Aurora.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Aurora.speed_min  = VIPER_SPEED_MIN;
    Aurora.speed_max  = VIPER_SPEED_MAX;
    Aurora.color_mode = MODE_COLORS_PER_LED;
    Aurora.speed      = VIPER_SPEED_DEFAULT;
    modes.push_back(Aurora);

    SetupZones();
}

RGBController_PatriotViper::~RGBController_PatriotViper()
{
    delete viper;
}

void RGBController_PatriotViper::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    zone* new_zone          = new zone;
    new_zone->name          = "Patriot Viper RGB";
    new_zone->type          = ZONE_TYPE_LINEAR;
    new_zone->leds_min      = 5;
    new_zone->leds_max      = 5;
    new_zone->leds_count    = 5;
    new_zone->matrix_map    = NULL;
    zones.push_back(*new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led* new_led = new led();

        new_led->name = "Patriot Viper RGB LED ";
        new_led->name.append(std::to_string(led_idx + 1));
        leds.push_back(*new_led);
    }

    SetupColors();
}

void RGBController_PatriotViper::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_PatriotViper::DeviceUpdateLEDs()
{
    if(viper->direct == true)
    {
        for(int led = 0; led < 5; led++)
        {
            RGBColor      color = colors[led];
            unsigned char red   = RGBGetRValue(color);
            unsigned char grn   = RGBGetGValue(color);
            unsigned char blu   = RGBGetBValue(color);
            viper->SetLEDColor(led, red, grn, blu);
        }
    }
    else
    {
        for(int led = 0; led < 5; led++)
        {
            RGBColor      color = colors[led];
            unsigned char red   = RGBGetRValue(color);
            unsigned char grn   = RGBGetGValue(color);
            unsigned char blu   = RGBGetBValue(color);
            viper->SetLEDEffectColor(led, red, grn, blu);
        }
    }
}

void RGBController_PatriotViper::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PatriotViper::UpdateSingleLED(int led)
{
    RGBColor      color = colors[led];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    if(viper->direct == true)
    {
        viper->SetLEDColor(led, red, grn, blu);
    }
    else
    {
        viper->SetLEDEffectColor(led, red, grn, blu);
    }
}

void RGBController_PatriotViper::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_PatriotViper::DeviceUpdateMode()
{
    if(modes[active_mode].value == 0xFFFF)
    {
        viper->SetDirect();
    }
    else
    {
        viper->SetMode(modes[active_mode].value, modes[active_mode].speed);
    }
}
