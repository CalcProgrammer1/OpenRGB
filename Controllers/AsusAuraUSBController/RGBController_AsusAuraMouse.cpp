/*-----------------------------------------*\
|  RGBController_AsusAuraMouse.cpp          |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Adam Honse (CalcProgrammer1) 10/25/2020  |
\*-----------------------------------------*/

#include "RGBController_AsusAuraMouse.h"
#include <vector>

typedef struct
{
    std::vector<std::string>  zone_names;
    std::vector<std::string>  mode_names;
} mouse_type;

std::map<int,mouse_type> DEVICES =
{
    { 0x18DD, {{ "Logo", "Scroll Wheel"              }, { "Static", "Breathing", "Color Cycle",         "Reactive"                          }}}, // ROG Gladius II Core
    { 0x1845, {{ "Logo", "Scroll Wheel", "Underglow" }, { "Static", "Breathing", "Color Cycle", "Wave", "Reactive", "Comet"                 }}}, // ROG Gladius II
    { 0x1877, {{ "Logo", "Scroll Wheel", "Underglow" }, { "Static", "Breathing", "Color Cycle", "Wave", "Reactive", "Comet"                 }}}, // ROG Gladius II Origin
    { 0x18B1, {{ "Logo", "Scroll Wheel", "Underglow" }, { "Static", "Breathing", "Color Cycle", "Wave", "Reactive", "Comet"                 }}}, // ROG Gladius II COD
    { 0x18CD, {{         "Scroll Wheel", "Underglow" }, { "Static", "Breathing", "Color Cycle", "Wave", "Reactive", "Comet"                 }}}, // ROG Gladius II PNK LTD
    { 0x189E, {{ "Logo", "Scroll Wheel"              }, { "Static", "Breathing", "Color Cycle", "",     "Reactive", "",      "Battery Mode" }}}, // ROG Gladius II Wireless
    { 0x18A0, {{ "Logo", "Scroll Wheel"              }, { "Static", "Breathing", "Color Cycle", "",     "Reactive", "",      "Battery Mode" }}}, // ROG Gladius II Wireless
    { 0x18E5, {{ "Logo", "Scroll Wheel", "Underglow" }, { "Static", "Breathing", "Color Cycle", "Wave", "Reactive", "Comet", "Battery Mode" }}}, // ROG Chakram Wireless
    { 0x18E3, {{ "Logo", "Scroll Wheel", "Underglow" }, { "Static", "Breathing", "Color Cycle", "Wave", "Reactive", "Comet", "Battery Mode" }}}, // ROG Chakram Wired 1
    { 0x1958, {{ "Logo", "Scroll Wheel", "Underglow" }, { "Static", "Breathing", "Color Cycle", "Wave", "Reactive", "Comet", "Battery Mode" }}}, // ROG Chakram Wired 2
    { 0x1846, {{ "Logo", "Scroll Wheel", "Underglow" }, { "Static", "Breathing", "Color Cycle", "Wave", "Reactive", "Comet"                 }}}, // ROG Pugio
    { 0x1906, {{ "Logo", "Scroll Wheel", "Underglow" }, { "Static", "Breathing", "Color Cycle", "Wave", "Reactive", "Comet", "Battery Mode" }}}, // ROG Pugio II Wired
    { 0x1908, {{ "Logo", "Scroll Wheel", "Underglow" }, { "Static", "Breathing", "Color Cycle", "Wave", "Reactive", "Comet", "Battery Mode" }}}, // ROG Pugio II Wireless
    { 0x18E1, {{ "Logo", "Scroll Wheel", "Underglow" }, { "Static", "Breathing", "Color Cycle",         "Reactive"                          }}}, // ROG Strix Impact II
    { 0x1910, {{ "Logo"                              }, { "Static", "Breathing", "Color Cycle",         "Reactive"                          }}}, // TUF M3
    { 0x1898, {{ "Logo"                              }, { "Static", "Breathing", "Color Cycle",         "Reactive"                          }}}, // TUF M5

};

RGBController_AuraMouse::RGBController_AuraMouse(AuraMouseController* aura_ptr)
{
    aura            = aura_ptr;

    name            = "ASUS Aura Mouse";
    vendor          = "ASUS";
    type            = DEVICE_TYPE_MOUSE;
    description     = "ASUS Aura Mouse Device";
    location        = aura->GetDeviceLocation();
    serial          = aura->GetSerialString();

    uint16_t pid    = aura->device_pid;

    for(unsigned int i = 0; i < DEVICES[pid].mode_names.size(); i++)
    {
        if(DEVICES[pid].mode_names[i].size() == 0)
        {
            continue;
        }

        if(DEVICES[pid].mode_names[i] == "Static")
        {
            mode Direct;
            Direct.name       = "Direct";
            Direct.value      = i;
            Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
            Direct.color_mode = MODE_COLORS_PER_LED;
            modes.push_back(Direct);
        }
        else if(DEVICES[pid].mode_names[i] == "Breathing")
        {
            mode Breathing;
            Breathing.name       = "Breathing";
            Breathing.value      = i;
            Breathing.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
            Breathing.color_mode = MODE_COLORS_PER_LED;
            modes.push_back(Breathing);
        }
        else if(DEVICES[pid].mode_names[i] == "Color Cycle")
        {
            mode ColorCycle;
            ColorCycle.name       = "Color Cycle";
            ColorCycle.value      = i;
            ColorCycle.flags      = 0;
            ColorCycle.color_mode = MODE_COLORS_NONE;
            modes.push_back(ColorCycle);
        }
        else if(DEVICES[pid].mode_names[i] == "Wave")
        {
            mode Wave;
            Wave.name = "Wave";
            Wave.value = i;
            Wave.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
            Wave.direction = 0;
            Wave.speed_min  = 1;
            Wave.speed_max  = 255;
            Wave.speed = 128;
            Wave.color_mode = MODE_COLORS_NONE;
            modes.push_back(Wave);
        }
        else if(DEVICES[pid].mode_names[i] == "Reactive")
        {
            mode Reactive;
            Reactive.name       = "Reactive";
            Reactive.value      = i;
            Reactive.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
            Reactive.color_mode = MODE_COLORS_PER_LED;
            modes.push_back(Reactive);
        }
        else if(DEVICES[pid].mode_names[i] == "Comet")
        {
            mode Comet;
            Comet.name       = "Comet";
            Comet.value      = i;
            Comet.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
            Comet.color_mode = MODE_COLORS_MODE_SPECIFIC;
            Comet.direction  = 0;
            Comet.colors_min = 1;
            Comet.colors_max = 1;
            Comet.colors.resize(1);
            modes.push_back(Comet);
        }
        else if(DEVICES[pid].mode_names[i] == "Battery Mode")
        {
            mode BatteryMode;
            BatteryMode.name       = "Battery Mode";
            BatteryMode.value      = i;
            BatteryMode.flags      = 0;
            BatteryMode.color_mode = MODE_COLORS_NONE;
            modes.push_back(BatteryMode);
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
    uint16_t pid = aura->device_pid;

    for(unsigned long long i = 0; i < DEVICES[pid].zone_names.size(); i++)
	{
        zone mouse_zone;

        mouse_zone.name        = DEVICES[pid].zone_names[i];
        mouse_zone.type        = ZONE_TYPE_SINGLE;
        mouse_zone.leds_min    = 1;
        mouse_zone.leds_max    = 1;
        mouse_zone.leds_count  = 1;
        mouse_zone.matrix_map  = NULL;

        zones.push_back(mouse_zone);

        led mouse_led;

        mouse_led.name         = DEVICES[pid].zone_names[i];

        leds.push_back(mouse_led);
    }

    SetupColors();
}

void RGBController_AuraMouse::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_AuraMouse::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_AuraMouse::UpdateZoneLEDs(int zone)
{
    UpdateSingleLED(zone);
}

void RGBController_AuraMouse::UpdateSingleLED(int led)
{
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        if(leds[led].name == "Logo")
        {
            red = RGBGetRValue(colors[led]);
            grn = RGBGetGValue(colors[led]);
            blu = RGBGetBValue(colors[led]);

            aura->SendUpdate(AURA_MOUSE_ZONE_LOGO, modes[active_mode].value, red, grn, blu, 0, false, 0, false);
        }
        else if(leds[led].name == "Scroll Wheel")
        {
            red = RGBGetRValue(colors[led]);
            grn = RGBGetGValue(colors[led]);
            blu = RGBGetBValue(colors[led]);

            aura->SendUpdate(AURA_MOUSE_ZONE_SCROLL, modes[active_mode]. value, red, grn, blu, 0, false, 0, false);
        }
        else
        {
            red = RGBGetRValue(colors[led]);
            grn = RGBGetGValue(colors[led]);
            blu = RGBGetBValue(colors[led]);

            aura->SendUpdate(AURA_MOUSE_ZONE_UNDERGLOW, modes[active_mode].value, red, grn, blu, 0, false, 0, false);
        }
    }
    else
    {
        aura->SendUpdate(AURA_MOUSE_ZONE_ALL, modes[active_mode].value, red, grn, blu, 0, false, 0, false);
    }
}

void RGBController_AuraMouse::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_AuraMouse::DeviceUpdateMode()
{
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        bool save = false;
        red = RGBGetRValue(colors[0]);
        grn = RGBGetGValue(colors[0]);
        blu = RGBGetBValue(colors[0]);

        aura->SendUpdate(AURA_MOUSE_ZONE_LOGO, modes[active_mode].value, red, grn, blu, 0, false, 0, save);

        red = RGBGetRValue(colors[1]);
        grn = RGBGetGValue(colors[1]);
        blu = RGBGetBValue(colors[1]);

        aura->SendUpdate(AURA_MOUSE_ZONE_SCROLL, modes[active_mode].value, red, grn, blu, 0, false, 0, save);

        red = RGBGetRValue(colors[2]);
        grn = RGBGetGValue(colors[2]);
        blu = RGBGetBValue(colors[2]);

        aura->SendUpdate(AURA_MOUSE_ZONE_UNDERGLOW, modes[active_mode].value, red, grn, blu, 0, false, 0, save);
    }
    else
    {
        if(modes[active_mode].value == 5)
        {
            red = RGBGetRValue(modes[active_mode].colors[0]);
            grn = RGBGetGValue(modes[active_mode].colors[0]);
            blu = RGBGetBValue(modes[active_mode].colors[0]);
        }
        aura->SendUpdate(AURA_MOUSE_ZONE_ALL, modes[active_mode].value, red, grn, blu, modes[active_mode].direction, modes[active_mode].color_mode == MODE_COLORS_RANDOM, modes[active_mode].speed, true);
    }
}
