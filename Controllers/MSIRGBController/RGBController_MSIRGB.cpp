/*-----------------------------------------*\
|  RGBController_MSIRGB.cpp                 |
|                                           |
|  Generic RGB Interface for MSI-RGB        |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/14/2020   |
\*-----------------------------------------*/

#include "RGBController_MSIRGB.h"

RGBController_MSIRGB::RGBController_MSIRGB(MSIRGBController* msi_ptr)
{
    msi = msi_ptr;

    name        = "MSI Motherboard";
    vendor      = "MSI";
    type        = DEVICE_TYPE_MOTHERBOARD;
    description = "MSI-RGB Device";
    
    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_MSIRGB::~RGBController_MSIRGB()
{
    delete msi;
}

void RGBController_MSIRGB::SetupZones()
{
    zone msi_zone;
    msi_zone.name           = "MSI Zone";
    msi_zone.type           = ZONE_TYPE_SINGLE;
    msi_zone.leds_min       = 1;
    msi_zone.leds_max       = 1;
    msi_zone.leds_count     = 1;
    msi_zone.matrix_map     = NULL;
    zones.push_back(msi_zone);

    led msi_led;
    msi_led.name = "MSI LED";
    leds.push_back(msi_led);

    SetupColors();
}

void RGBController_MSIRGB::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_MSIRGB::DeviceUpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    msi->SetColor(red, grn, blu);
}

void RGBController_MSIRGB::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MSIRGB::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MSIRGB::SetCustomMode()
{

}

void RGBController_MSIRGB::DeviceUpdateMode()
{

}
