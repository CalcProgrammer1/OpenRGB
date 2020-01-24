/*-----------------------------------------*\
|  RGBController_AMDWraithPrism.cpp         |
|                                           |
|  Generic RGB Interface for AMD Wraith     |
|  Prism                                    |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/25/2019  |
\*-----------------------------------------*/

#include "RGBController_AMDWraithPrism.h"

RGBController_AMDWraithPrism::RGBController_AMDWraithPrism(AMDWraithPrismController* wraith_ptr)
{
    wraith = wraith_ptr;

    name = "AMD Wraith Prism";
    type = DEVICE_TYPE_COOLER;
    version = wraith->GetFirmwareVersionString();

    mode Static;
    Static.name = "Static";
    Static.value = AMD_WRAITH_PRISM_EFFECT_CHANNEL_STATIC;
    Static.flags = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name      = "Breathing";
    Breathing.value     = AMD_WRAITH_PRISM_EFFECT_CHANNEL_BREATHING;
    Breathing.flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR | MODE_FLAG_RANDOM_COLOR;
    Breathing.speed_min = 0x3C;
    Breathing.speed_max = 0x26;
    Breathing.random    = false;
    Breathing.speed     = 0x31;
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name      = "Color Cycle";
    ColorCycle.value     = AMD_WRAITH_PRISM_EFFECT_CHANNEL_COLOR_CYCLE;
    ColorCycle.flags     = MODE_FLAG_HAS_SPEED;
    ColorCycle.speed_min = 0x96;
    ColorCycle.speed_max = 0x68;
    ColorCycle.speed     = 0x80;
    modes.push_back(ColorCycle);

    mode Rainbow;
    Rainbow.name      = "Rainbow";
    Rainbow.value     = AMD_WRAITH_PRISM_EFFECT_CHANNEL_RAINBOW;
    Rainbow.flags     = MODE_FLAG_HAS_SPEED;
    Rainbow.speed_min = 0x72;
    Rainbow.speed_max = 0x61;
    Rainbow.speed     = 0x64;
    modes.push_back(Rainbow);

    led logo_led;
    logo_led.name = "Logo";
    leds.push_back(logo_led);
    colors.push_back(0x00000000);

    zone logo_zone;
    logo_zone.name = "Logo";
    logo_zone.type = ZONE_TYPE_SINGLE;
    std::vector<int> logo_zone_map;
    logo_zone_map.push_back(0);
    logo_zone.map.push_back(logo_zone_map);
    zones.push_back(logo_zone);

    led fan_led;
    fan_led.name = "Fan";
    leds.push_back(fan_led);
    colors.push_back(0x00000000);

    zone fan_zone;
    fan_zone.name = "Fan";
    fan_zone.type = ZONE_TYPE_SINGLE;
    std::vector<int> fan_zone_map;
    fan_zone_map.push_back(1);
    fan_zone.map.push_back(fan_zone_map);
    zones.push_back(fan_zone);

    led ring_led;
    ring_led.name = "Ring";
    leds.push_back(ring_led);
    colors.push_back(0x00000000);

    zone ring_zone;
    ring_zone.name = "Ring";
    ring_zone.type = ZONE_TYPE_SINGLE;
    std::vector<int> ring_zone_map;
    ring_zone_map.push_back(2);
    ring_zone.map.push_back(ring_zone_map);
    zones.push_back(ring_zone);   
}

RGBController_AMDWraithPrism::~RGBController_AMDWraithPrism()
{

}

int RGBController_AMDWraithPrism::GetMode()
{
    return 0;
}

void RGBController_AMDWraithPrism::SetMode(int mode)
{
    wraith->SetRingMode(modes[mode].value, modes[mode].speed, modes[mode].direction, modes[mode].random);

    switch(modes[mode].value)
    {
        case AMD_WRAITH_PRISM_EFFECT_CHANNEL_COLOR_CYCLE:
        case AMD_WRAITH_PRISM_EFFECT_CHANNEL_RAINBOW:
            wraith->SetFanMode(AMD_WRAITH_PRISM_FAN_LOGO_MODE_COLOR_CYCLE, modes[mode].speed, modes[mode].random);
            wraith->SetLogoMode(AMD_WRAITH_PRISM_FAN_LOGO_MODE_COLOR_CYCLE, modes[mode].speed, modes[mode].random);
            break;
        
        case AMD_WRAITH_PRISM_EFFECT_CHANNEL_BREATHING:
            wraith->SetFanMode(AMD_WRAITH_PRISM_FAN_LOGO_MODE_BREATHING, modes[mode].speed, modes[mode].random);
            wraith->SetLogoMode(AMD_WRAITH_PRISM_FAN_LOGO_MODE_BREATHING, modes[mode].speed, modes[mode].random);
            break;

        default:
            wraith->SetFanMode(AMD_WRAITH_PRISM_FAN_LOGO_MODE_STATIC, modes[mode].speed, modes[mode].random);
            wraith->SetLogoMode(AMD_WRAITH_PRISM_FAN_LOGO_MODE_STATIC, modes[mode].speed, modes[mode].random);
            break;
    }

    UpdateLEDs();
}

void RGBController_AMDWraithPrism::SetCustomMode()
{

}

void RGBController_AMDWraithPrism::UpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);
    wraith->SetLogoColor(red, grn, blu);

    red = RGBGetRValue(colors[1]);
    grn = RGBGetGValue(colors[1]);
    blu = RGBGetBValue(colors[1]);
    wraith->SetFanColor(red, grn, blu);

    red = RGBGetRValue(colors[2]);
    grn = RGBGetGValue(colors[2]);
    blu = RGBGetBValue(colors[2]);
    wraith->SetRingColor(red, grn, blu);
}

void RGBController_AMDWraithPrism::UpdateZoneLEDs(int zone)
{
    RGBColor      color = colors[zone];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    if(zone == 0)
    {
        wraith->SetLogoColor(red, grn, blu);
    }
    else if(zone == 1)
    {
        wraith->SetFanColor(red, grn, blu);
    }
    else if(zone == 2)
    {
        wraith->SetRingColor(red, grn, blu);
    }
}

void RGBController_AMDWraithPrism::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}
