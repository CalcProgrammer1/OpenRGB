/*-----------------------------------------*\
|  RGBController_AsusAuraMouse.cpp          |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Adam Honse (CalcProgrammer1) 10/25/2020  |
\*-----------------------------------------*/

#include "RGBController_AsusAuraMouse.h"

static std::string aura_mouse_zone_names[3]
{
    "Logo",
    "Scroll Wheel",
    "Underglow"
};

RGBController_AuraMouse::RGBController_AuraMouse(AuraMouseController* aura_ptr)
{
    aura                    = aura_ptr;

    name                    = "ASUS Aura Mouse";
    vendor                  = "ASUS";
    type                    = DEVICE_TYPE_MOUSE;
    description             = "ASUS Aura Mouse Device";
    location                = aura->GetDeviceLocation();
    serial                  = aura->GetSerialString();

    uint16_t pid            = aura->device_pid;
    std::vector<uint8_t> mm = aura_mouse_devices[pid].mouse_modes;

    int mode_value          = 0;

    for(std::vector<uint8_t>::iterator it = mm.begin(); it != mm.end(); it++)
    {
        switch(*it)
        {
            case AURA_MOUSE_MODE_STATIC:
                {
                    mode Direct;
                    Direct.name                 = "Direct";
                    Direct.value                = mode_value;
                    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
                    Direct.brightness_min       = AURA_MOUSE_BRIGHTNESS_MIN;
                    Direct.brightness_max       = AURA_MOUSE_BRIGHTNESS_MAX;
                    Direct.brightness           = AURA_MOUSE_BRIGHTNESS_DEFAULT;
                    Direct.color_mode           = MODE_COLORS_PER_LED;
                    modes.push_back(Direct);
                }
                break;

            case AURA_MOUSE_MODE_BREATHING:
                {
                    mode Breathing;
                    Breathing.name              = "Breathing";
                    Breathing.value             = mode_value;
                    Breathing.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
                    Breathing.brightness_min    = AURA_MOUSE_BRIGHTNESS_MIN;
                    Breathing.brightness_max    = AURA_MOUSE_BRIGHTNESS_MAX;
                    Breathing.brightness        = AURA_MOUSE_BRIGHTNESS_DEFAULT;
                    Breathing.color_mode        = MODE_COLORS_PER_LED;
                    modes.push_back(Breathing);
                }
                break;

            case AURA_MOUSE_MODE_SPECTRUM:
                {
                    mode ColorCycle;
                    ColorCycle.name             = "Spectrum Cycle";
                    ColorCycle.value            = mode_value;
                    ColorCycle.flags            = MODE_FLAG_HAS_BRIGHTNESS;
                    ColorCycle.brightness_min   = AURA_MOUSE_BRIGHTNESS_MIN;
                    ColorCycle.brightness_max   = AURA_MOUSE_BRIGHTNESS_MAX;
                    ColorCycle.brightness       = AURA_MOUSE_BRIGHTNESS_DEFAULT;
                    ColorCycle.color_mode       = MODE_COLORS_NONE;
                    modes.push_back(ColorCycle);
                }
                break;

            case AURA_MOUSE_MODE_WAVE:
                {
                    mode Wave;
                    Wave.name                   = "Wave";
                    Wave.value                  = mode_value;
                    Wave.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
                    Wave.direction              = 0;
                    Wave.speed_min              = aura_mouse_devices[pid].speed_min;
                    Wave.speed_max              = aura_mouse_devices[pid].speed_max;
                    Wave.speed                  = (aura_mouse_devices[pid].speed_min + aura_mouse_devices[pid].speed_max) / 2;
                    Wave.brightness_min         = AURA_MOUSE_BRIGHTNESS_MIN;
                    Wave.brightness_max         = AURA_MOUSE_BRIGHTNESS_MAX;
                    Wave.brightness             = AURA_MOUSE_BRIGHTNESS_DEFAULT;
                    Wave.color_mode             = MODE_COLORS_NONE;
                    modes.push_back(Wave);
                }
                break;

            case AURA_MOUSE_MODE_REACTIVE:
                {
                    mode Reactive;
                    Reactive.name               = "Reactive";
                    Reactive.value              = mode_value;
                    Reactive.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
                    Reactive.brightness_min     = AURA_MOUSE_BRIGHTNESS_MIN;
                    Reactive.brightness_max     = AURA_MOUSE_BRIGHTNESS_MAX;
                    Reactive.brightness         = AURA_MOUSE_BRIGHTNESS_DEFAULT;
                    Reactive.color_mode         = MODE_COLORS_PER_LED;
                    modes.push_back(Reactive);
                }
                break;

            case AURA_MOUSE_MODE_COMET:
                {
                    mode Comet;
                    Comet.name                  = "Comet";
                    Comet.value                 = mode_value;
                    Comet.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
                    Comet.brightness_min        = AURA_MOUSE_BRIGHTNESS_MIN;
                    Comet.brightness_max        = AURA_MOUSE_BRIGHTNESS_MAX;
                    Comet.brightness            = AURA_MOUSE_BRIGHTNESS_DEFAULT;
                    Comet.color_mode            = MODE_COLORS_MODE_SPECIFIC;
                    Comet.direction             = 0;
                    Comet.colors_min            = 1;
                    Comet.colors_max            = 1;
                    Comet.colors.resize(1);
                    modes.push_back(Comet);
                }
                break;

            case AURA_MOUSE_MODE_BATTERY:
                {
                    mode BatteryMode;
                    BatteryMode.name            = "Battery";
                    BatteryMode.value           = mode_value;
                    BatteryMode.flags           = 0;
                    BatteryMode.brightness_min  = AURA_MOUSE_BRIGHTNESS_MIN;
                    BatteryMode.brightness_max  = AURA_MOUSE_BRIGHTNESS_MAX;
                    BatteryMode.brightness      = AURA_MOUSE_BRIGHTNESS_DEFAULT;
                    BatteryMode.color_mode      = MODE_COLORS_NONE | MODE_FLAG_MANUAL_SAVE;
                    modes.push_back(BatteryMode);
                }
                break;
        }
        mode_value++;
    }

    SetupZones();
}

RGBController_AuraMouse::~RGBController_AuraMouse()
{
    delete aura;
}

void RGBController_AuraMouse::SetupZones()
{

    uint16_t pid                = aura->device_pid;

    for(std::vector<uint8_t>::iterator zone_it = aura_mouse_devices[pid].mouse_zones.begin(); zone_it != aura_mouse_devices[pid].mouse_zones.end(); zone_it++)
    {
        zone mouse_zone;

        mouse_zone.name         = aura_mouse_zone_names[*zone_it];
        mouse_zone.type         = ZONE_TYPE_SINGLE;
        mouse_zone.leds_min     = 1;
        mouse_zone.leds_max     = 1;
        mouse_zone.leds_count   = 1;
        mouse_zone.matrix_map   = NULL;

        zones.push_back(mouse_zone);

        led mouse_led;

        mouse_led.name          = mouse_zone.name + " LED";
        mouse_led.value         = *zone_it;

        leds.push_back(mouse_led);
    }

    SetupColors();
}

void RGBController_AuraMouse::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_AuraMouse::DeviceUpdateLEDs()
{
    for(std::vector<led>::iterator led_it = leds.begin(); led_it != leds.end(); led_it++)
    {
        UpdateSingleLED(led_it->value);
    }
}

void RGBController_AuraMouse::UpdateZoneLEDs(int zone)
{
    UpdateSingleLED(zone);
}

void RGBController_AuraMouse::UpdateSingleLED(int led)
{
    uint8_t red = RGBGetRValue(colors[led]);
    uint8_t grn = RGBGetGValue(colors[led]);
    uint8_t blu = RGBGetBValue(colors[led]);

    aura->SendUpdate(leds[led].value, active_mode, red, grn, blu, 0, false, 0, modes[active_mode].brightness);
}

void RGBController_AuraMouse::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_AuraMouse::DeviceUpdateMode()
{
    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        DeviceUpdateLEDs();
    }
    else
    {
        unsigned char red = 0;
        unsigned char grn = 0;
        unsigned char blu = 0;

        if (modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
        {
            red = RGBGetRValue(modes[active_mode].colors[0]);
            grn = RGBGetGValue(modes[active_mode].colors[0]);
            blu = RGBGetBValue(modes[active_mode].colors[0]);
        }

        aura->SendUpdate(AURA_MOUSE_ZONE_ALL, active_mode, red, grn, blu, modes[active_mode].direction, modes[active_mode].color_mode == MODE_COLORS_RANDOM, modes[active_mode].speed, modes[active_mode].brightness);
    }
}

void RGBController_AuraMouse::DeviceSaveMode()
{
    DeviceUpdateMode();
    aura->SaveMode();
}
