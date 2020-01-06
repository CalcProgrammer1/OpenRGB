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

    mode led_mode;
    led_mode.name = "Custom";
    modes.push_back(led_mode);

    led logo_led;
    logo_led.name = "Logo";
    leds.push_back(logo_led);

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

void RGBController_AMDWraithPrism::SetMode(int /*mode*/)
{

}

void RGBController_AMDWraithPrism::SetCustomMode()
{

}

void RGBController_AMDWraithPrism::SetAllLEDs(RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    wraith->SetFanColor(red, grn, blu);
    wraith->SetLogoColor(red, grn, blu);
    wraith->SetRingColor(red, grn, blu);
}

void RGBController_AMDWraithPrism::SetAllZoneLEDs(int zone, RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

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

void RGBController_AMDWraithPrism::SetLED(int led, RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    if(led == 0)
    {
        wraith->SetLogoColor(red, grn, blu);
    }
    else if(led == 1)
    {
        wraith->SetFanColor(red, grn, blu);
    }
    else if(led == 2)
    {
        wraith->SetRingColor(red, grn, blu);
    }
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
