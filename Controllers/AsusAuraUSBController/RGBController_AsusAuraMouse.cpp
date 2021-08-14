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

    for(std::vector<uint8_t>::iterator it = mm.begin(); it != mm.end(); it++)
    {
        switch(*it)
        {
            case AURA_MOUSE_MODE_STATIC:
                {
                    mode Direct;
                    Direct.name             = "Direct";
                    Direct.value            = AURA_MOUSE_MODE_STATIC;
                    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
                    Direct.color_mode       = MODE_COLORS_PER_LED;
                    modes.push_back(Direct);
                }
                break;

            case AURA_MOUSE_MODE_BREATHING:
                {
                    mode Breathing;
                    Breathing.name          = "Breathing";
                    Breathing.value         = AURA_MOUSE_MODE_BREATHING;
                    Breathing.flags         = MODE_FLAG_HAS_PER_LED_COLOR;
                    Breathing.color_mode    = MODE_COLORS_PER_LED;
                    modes.push_back(Breathing);
                }
                break;

            case AURA_MOUSE_MODE_SPECTRUM:
                {
                    mode ColorCycle;
                    ColorCycle.name         = "Spectrum Cycle";
                    ColorCycle.value        = AURA_MOUSE_MODE_SPECTRUM;
                    ColorCycle.flags        = 0;
                    ColorCycle.color_mode   = MODE_COLORS_NONE;
                    modes.push_back(ColorCycle);
                }
                break;

            case AURA_MOUSE_MODE_WAVE:
                {
                    mode Wave;
                    Wave.name               = "Wave";
                    Wave.value              = AURA_MOUSE_MODE_WAVE;
                    Wave.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
                    Wave.direction          = 0;
                    Wave.speed_min          = aura_mouse_devices[pid].speed_min;
                    Wave.speed_max          = aura_mouse_devices[pid].speed_max;
                    Wave.speed              = 128;
                    Wave.color_mode         = MODE_COLORS_NONE;
                    modes.push_back(Wave);
                }
                break;

            case AURA_MOUSE_MODE_REACTIVE:
                {
                    mode Reactive;
                    Reactive.name           = "Reactive";
                    Reactive.value          = AURA_MOUSE_MODE_REACTIVE;
                    Reactive.flags          = MODE_FLAG_HAS_PER_LED_COLOR;
                    Reactive.color_mode     = MODE_COLORS_PER_LED;
                    modes.push_back(Reactive);
                }
                break;

            case AURA_MOUSE_MODE_COMET:
                {
                    mode Comet;
                    Comet.name              = "Comet";
                    Comet.value             = AURA_MOUSE_MODE_COMET;
                    Comet.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
                    Comet.color_mode        = MODE_COLORS_MODE_SPECIFIC;
                    Comet.direction         = 0;
                    Comet.colors_min        = 1;
                    Comet.colors_max        = 1;
                    Comet.colors.resize(1);
                    modes.push_back(Comet);
                }
                break;

            case AURA_MOUSE_MODE_BATTERY:
                {
                    mode BatteryMode;
                    BatteryMode.name       = "Battery";
                    BatteryMode.value      = AURA_MOUSE_MODE_BATTERY;
                    BatteryMode.flags      = 0;
                    BatteryMode.color_mode = MODE_COLORS_NONE;
                    modes.push_back(BatteryMode);
                }
                break;
        }
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

    aura->SendUpdate(leds[led].value, active_mode, red, grn, blu, 0, false, 0);
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

        aura->SendUpdate(AURA_MOUSE_ZONE_ALL, active_mode, red, grn, blu, modes[active_mode].direction, modes[active_mode].color_mode == MODE_COLORS_RANDOM, modes[active_mode].speed);
        aura->SaveMode();
    }
}
