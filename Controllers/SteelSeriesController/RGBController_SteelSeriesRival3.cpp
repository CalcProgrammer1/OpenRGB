/*-----------------------------------------*\
|  RGBController_SteelSeriesRival.cpp       |
|                                           |
|  Generic RGB Interface SteelSeriesRival3  |
|  Class                                    |
|                                           |
|  B Horn (bahorn) 29/08/2021               |
\*-----------------------------------------*/

#include "RGBController_SteelSeriesRival3.h"

typedef struct
{
    const char* name;
    const int   value;
} steelseries_rival_led_info;

static const steelseries_rival_led_info rival_3_leds[]=
{
    {"Front",           0x01},
    {"Middle",          0x02},
    {"Rear",            0x03},
    {"Logo",            0x04}
};

RGBController_SteelSeriesRival3::RGBController_SteelSeriesRival3(SteelSeriesRival3Controller* rival_ptr)
{
    rival       = rival_ptr;
    
    name        = rival->GetDeviceName();
    vendor      = "SteelSeries";
    type        = DEVICE_TYPE_MOUSE; 
    description = "SteelSeries Rival 3 Device";
    location    = rival->GetDeviceLocation();
    serial      = rival->GetSerialString();
    version     = rival->GetFirmwareVersion();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = STEELSERIES_RIVAL_3_EFFECT_DIRECT;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    Direct.brightness_min   = 0x00;
    Direct.brightness_max   = STEELSERIES_RIVAL_3_BRIGHTNESS_MAX;
    Direct.brightness       = STEELSERIES_RIVAL_3_BRIGHTNESS_MAX;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = STEELSERIES_RIVAL_3_EFFECT_BREATHING_MIN;
    Breathing.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_MANUAL_SAVE;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    Breathing.speed_min     = 0;
    Breathing.speed_max     = 2;
    Breathing.speed         = 1;
    modes.push_back(Breathing);

    mode SpectrumCycle;
    SpectrumCycle.name      = "Spectrum Cycle";
    SpectrumCycle.value     = STEELSERIES_RIVAL_3_EFFECT_SPECTRUM_CYCLE;
    SpectrumCycle.flags     = MODE_FLAG_MANUAL_SAVE;
    modes.push_back(SpectrumCycle);

    mode RainbowBreathing;
    RainbowBreathing.name   = "Rainbow Breathing";
    RainbowBreathing.value  = STEELSERIES_RIVAL_3_EFFECT_RAINBOW_BREATHING;
    RainbowBreathing.flags  = MODE_FLAG_MANUAL_SAVE;
    modes.push_back(RainbowBreathing);

    /*------------------------------------------------------------------------*\
    | This is a pretty cool mode where it flashes random colors.               |
    |                                                                          |
    | However, the flashes are in the frequency range where it probably needs  |
    | a proper warning for it to be compiled in by default.                    |
    |                                                                          |
    | It is disabled in the vendor software, and is only known about as it is  |
    | documented in rivalcfg.                                                  |
    |                                                                          |   
    | If this does get re-enabled, worth noting it has an issue where this     |
    | mode is black if you come directly from one of the pulsating modes.      |
    \*------------------------------------------------------------------------*/
    /*
    mode Disco;
    Disco.name              = "Disco";
    Disco.value             = STEELSERIES_RIVAL_3_EFFECT_DISCO;
    Disco.flags             = MODE_FLAG_MANUAL_SAVE;
    modes.push_back(Disco);
    */

    SetupZones();
}

void RGBController_SteelSeriesRival3::DeviceSaveMode()
{
    rival->Save();
}

RGBController_SteelSeriesRival3::~RGBController_SteelSeriesRival3()
{
    delete rival;
}

void RGBController_SteelSeriesRival3::SetupZones()
{
    for(const steelseries_rival_led_info led_info: rival_3_leds)
    {
        zone zone;
        zone.name          = led_info.name;
        zone.type          = ZONE_TYPE_SINGLE;
        zone.leds_min      = 1;
        zone.leds_max      = 1;
        zone.leds_count    = 1;
        zone.matrix_map    = NULL;
        zones.push_back(zone);

        led mouse_led;
        mouse_led.name          = led_info.name;
        mouse_led.value         = led_info.value;
        leds.push_back(mouse_led);
    }
    SetupColors();
}

void RGBController_SteelSeriesRival3::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SteelSeriesRival3::DeviceUpdateLEDs()
{
    for(unsigned int i = 0; i < zones.size(); i++)
    {
        UpdateZoneLEDs(i);
    }
    DeviceUpdateMode();
}

void RGBController_SteelSeriesRival3::UpdateZoneLEDs(int zone)
{
    /*--------------------------------------------------*\
    | Subtracting one as the zone and led indexes differ |
    \*--------------------------------------------------*/
    UpdateSingleLED(zones[zone].leds[0].value - 1);
}   

void RGBController_SteelSeriesRival3::UpdateSingleLED(int led)
{
    unsigned char red = RGBGetRValue(colors[led]);
    unsigned char grn = RGBGetGValue(colors[led]);
    unsigned char blu = RGBGetBValue(colors[led]);
    rival->SetColor(leds[led].value, red, grn, blu, modes[active_mode].brightness);
}

void RGBController_SteelSeriesRival3::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_SteelSeriesRival3::DeviceUpdateMode()
{
    rival->SetLightEffectAll(modes[active_mode].value - modes[active_mode].speed);
}
