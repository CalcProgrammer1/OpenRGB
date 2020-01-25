/*-----------------------------------------*\
|  RGBController_AorusGPU.cpp               |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  Aorus GPU                                |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/17/2019   |
\*-----------------------------------------*/

#include "RGBController_AorusGPU.h"

#include <Windows.h>

static HMODULE handle;
static unsigned char data[] = { 0x8E, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00,
                                0x40, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00,
                                0x04, 0x00, 0x00, 0x00,
                                0x64, 0x00, 0x00, 0x00,
                                0x01, 0x00, 0x00, 0x00 };

void RGBController_AorusGPU::UpdateLEDs()
{
    RGBColor color = colors[0];
    data[9] = RGBGetRValue(color);
    data[10] = RGBGetGValue(color);
    data[11] = RGBGetBValue(color);

    GvWriteI2C(0x00000000, data, 0x00000000);
}

void RGBController_AorusGPU::UpdateZoneLEDs(int zone)
{
    UpdateLEDs();
}

void RGBController_AorusGPU::UpdateSingleLED(int led)
{
    UpdateLEDs();
}

RGBController_AorusGPU::RGBController_AorusGPU()
{
    name = "Aorus GPU";
    type = DEVICE_TYPE_GPU;

    handle = LoadLibrary("GvDisplay.dll");

    GvWriteI2C = (_GvWriteI2C)GetProcAddress(handle, "GvWriteI2C");
    GvFreeDispLib = (_GvFreeDispLib)GetProcAddress(handle, "GvFreeDispLib");
    GvInitDispLib = (_GvInitDispLib)GetProcAddress(handle, "GvInitDispLib");
    
    GvFreeDispLib();
    GvInitDispLib();

    colors.push_back(0x00000000);

    mode aorus_mode;
    aorus_mode.name = "Static";
    modes.push_back(aorus_mode);

    led aorus_led;
    aorus_led.name = "GPU LED";
    leds.push_back(aorus_led);

    zone aorus_zone;
    aorus_zone.name = "GPU LED";
    std::vector<int> aorus_zone_map;
    aorus_zone_map.push_back(0);
    aorus_zone.map.push_back(aorus_zone_map);
    zones.push_back(aorus_zone);
}

void RGBController_AorusGPU::SetCustomMode()
{

}

void RGBController_AorusGPU::UpdateMode()
{

}