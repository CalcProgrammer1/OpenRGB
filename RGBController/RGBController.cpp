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

int RGBController::GetMode()
{
    return(active_mode);
}

void RGBController::SetMode(int mode)
{
    active_mode = mode;

    UpdateMode();
}

std::string device_type_to_str(device_type type)
{
    switch(type)
    {
    case DEVICE_TYPE_MOTHERBOARD:
        return "Motherboard";
    case DEVICE_TYPE_DRAM:
        return "DRAM";
    case DEVICE_TYPE_GPU:
        return "GPU";
    case DEVICE_TYPE_COOLER:
        return "Cooler";
    case DEVICE_TYPE_LEDSTRIP:
        return "LED Strip";
    case DEVICE_TYPE_KEYBOARD:
        return "Keyboard";
    case DEVICE_TYPE_MOUSE:
        return "Mouse";
    case DEVICE_TYPE_MOUSEMAT:
        return "Mousemat";
    case DEVICE_TYPE_HEADSET:
        return "Headset";
    default:
        return "Unknown";
    }
}