#include "RGBController.h"

RGBColor RGBController::GetLED(int led)
{
    if(led < colors.size())
    {
        return(colors[led]);
    }
    else
    {
        return(0x00000000);
    }
}

void RGBController::SetLED(int led, RGBColor color)
{
    if(led < colors.size())
    {
        colors[led] = color;

        UpdateSingleLED(led);
    }
}

void RGBController::SetAllLEDs(RGBColor color)
{
    for(int led = 0; led < colors.size(); led++)
    {
        colors[led] = color;
    }

    UpdateLEDs();
}

void RGBController::SetAllZoneLEDs(int zone, RGBColor color)
{
    for (std::size_t x = 0; x < zones[zone].map.size(); x++)
    {
        for (std::size_t y = 0; y < zones[zone].map[x].size(); y++)
        {
            colors[zones[zone].map[x][y]] = color;
        }
    }

    UpdateZoneLEDs(zone);
}
