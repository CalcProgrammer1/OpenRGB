/*-----------------------------------------*\
|  RGBController_HyperX.cpp                 |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  HyperX Predator RGB interface            |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/29/2019   |
\*-----------------------------------------*/

#include "RGBController_HyperX.h"

int RGBController_HyperX::GetMode()
{
    return(0);
}

void RGBController_HyperX::SetMode(int mode)
{
    hyperx->SetMode(mode);
}

void RGBController_HyperX::SetCustomMode()
{
    hyperx->SetMode(HYPERX_MODE_STATIC);
}

void RGBController_HyperX::SetAllLEDs(RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    hyperx->SetAllColors(red, grn, blu);
}

void RGBController_HyperX::SetAllZoneLEDs(int zone, RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    for (int x = 0; x < zones[zone].map.size(); x++)
    {
        for (int y = 0; y < zones[zone].map[x].size(); y++)
        {
            hyperx->SetLEDColor(zones[zone].map[x][y], red, grn, blu);
        }
    }
}

void RGBController_HyperX::SetLED(int led, RGBColor color)
{
    unsigned char red = RGBGetRValue(color);
    unsigned char grn = RGBGetGValue(color);
    unsigned char blu = RGBGetBValue(color);

    hyperx->SetLEDColor(led, red, grn, blu);
}

void RGBController_HyperX::UpdateLEDs()
{
    
}

RGBController_HyperX::RGBController_HyperX(HyperXController* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name = hyperx->GetDeviceName();
    location = hyperx->GetDeviceLocation();

    type = DEVICE_TYPE_DRAM;

    mode hyperx_modes[HYPERX_NUMBER_MODES];

    hyperx_modes[0].name = "Static";
    hyperx_modes[1].name = "Rainbow";
    hyperx_modes[2].name = "Comet";
    hyperx_modes[3].name = "Heartbeat";
    hyperx_modes[4].name = "Spectrum Cycle";
    hyperx_modes[5].name = "Breathing";
    hyperx_modes[6].name = "Bounce";
    hyperx_modes[7].name = "Blink";
    
    for (int i = 0; i < HYPERX_NUMBER_MODES; i++)
    {
        modes.push_back(hyperx_modes[i]);
    }

    for (int i = 0; i < hyperx->GetLEDCount(); i++)
    {
        led* new_led = new led();

        new_led->name = "HyperX LED";

        leds.push_back(*new_led);
        colors.push_back(0x00000000);
    }

    zone new_zone;

    new_zone.name = "HyperX Zone";
    new_zone.type = ZONE_TYPE_SINGLE;

    std::vector<int> zone_row;

    for (int i = 0; i < hyperx->GetLEDCount(); i++)
    {
        zone_row.push_back(i);
    }

    new_zone.map.push_back(zone_row);

    zones.push_back(new_zone);
}
