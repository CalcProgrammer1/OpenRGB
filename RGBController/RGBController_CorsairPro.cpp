/*-----------------------------------------*\
|  RGBController_CorsairPro.cpp             |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  Corsair Vengeance Pro RGB driver         |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/30/2019   |
\*-----------------------------------------*/

#include "RGBController_CorsairPro.h"

int RGBController_CorsairPro::GetMode()
{
    return(0);
}

void RGBController_CorsairPro::SetMode(int mode)
{
    switch (mode)
    {
    case 0:
        corsair->SetEffect(CORSAIR_PRO_MODE_COLOR_SHIFT);
        break;
    case 1:
        corsair->SetEffect(CORSAIR_PRO_MODE_COLOR_PULSE);
        break;
    case 2:
        corsair->SetEffect(CORSAIR_PRO_MODE_RAINBOW_WAVE);
        break;
    case 3:
        corsair->SetEffect(CORSAIR_PRO_MODE_COLOR_WAVE);
        break;
    case 4:
        corsair->SetEffect(CORSAIR_PRO_MODE_VISOR);
        break;
    case 5:
        corsair->SetEffect(CORSAIR_PRO_MODE_RAIN);
        break;
    case 6:
        corsair->SetEffect(CORSAIR_PRO_MODE_MARQUEE);
        break;
    case 7:
        corsair->SetEffect(CORSAIR_PRO_MODE_RAINBOW);
        break;
    case 8:
        corsair->SetEffect(CORSAIR_PRO_MODE_SEQUENTIAL);
        break;
    case 9:
        corsair->SetEffect(CORSAIR_PRO_MODE_STATIC);
        break;
    }
}

void RGBController_CorsairPro::SetCustomMode()
{
    corsair->SetCustom();
}

void RGBController_CorsairPro::SetAllLEDs(RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    corsair->SetAllColors(red, grn, blu);
    corsair->ApplyColors();
}

void RGBController_CorsairPro::SetAllZoneLEDs(int zone, RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    for (int x = 0; x < zones[zone].map.size(); x++)
    {
        for (int y = 0; y < zones[zone].map[x].size(); y++)
        {
            corsair->SetLEDColor(zones[zone].map[x][y], red, grn, blu);
        }
    }

    corsair->ApplyColors();
}

void RGBController_CorsairPro::SetLED(int led, RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    corsair->SetLEDColor(led, red, grn, blu);
    corsair->ApplyColors();
}

void RGBController_CorsairPro::UpdateLEDs()
{

}

RGBController_CorsairPro::RGBController_CorsairPro(CorsairProController* corsair_ptr)
{
    corsair = corsair_ptr;

    name = corsair->GetDeviceName();

    type = DEVICE_TYPE_DRAM;

    mode corsair_modes[CORSAIR_PRO_NUMBER_MODES];

    corsair_modes[0].name = "Color Shift";
    corsair_modes[1].name = "Color Pulse";
    corsair_modes[2].name = "Rainbow Wave";
    corsair_modes[3].name = "Color Wave";
    corsair_modes[4].name = "Visor";
    corsair_modes[5].name = "Rain";
    corsair_modes[6].name = "Marquee";
    corsair_modes[7].name = "Rainbow";
    corsair_modes[8].name = "Sequential";
    corsair_modes[9].name = "Static";

    for (int i = 0; i < CORSAIR_PRO_NUMBER_MODES; i++)
    {
        modes.push_back(corsair_modes[i]);
    }

    for (int i = 0; i < corsair->GetLEDCount(); i++)
    {
        led* new_led = new led();

        new_led->name = "Corsair Pro LED";

        leds.push_back(*new_led);
        colors.push_back(0x00000000);
    }

    zone new_zone;

    new_zone.name = "Corsair Pro Zone";
    new_zone.type = ZONE_TYPE_LINEAR;

    std::vector<int> zone_row;

    for (int i = 0; i < corsair->GetLEDCount(); i++)
    {
        zone_row.push_back(i);
    }

    new_zone.map.push_back(zone_row);

    zones.push_back(new_zone);
}
