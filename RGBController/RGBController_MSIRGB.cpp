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

    mode Direct;
    Direct.name  = "Direct";
    Direct.value = 0;
    Direct.flags = MODE_FLAG_HAS_COLOR | MODE_FLAG_PER_LED_COLOR;
    modes.push_back(Direct);

    led msi_led;
    msi_led.name = "MSI LED";
    leds.push_back(msi_led);

    zone msi_zone;
    msi_zone.name = "MSI Zone";
    msi_zone.type = ZONE_TYPE_SINGLE;
    std::vector<int> msi_zone_map;
    msi_zone_map.push_back(0);
    msi_zone.map.push_back(msi_zone_map);
    zones.push_back(msi_zone);

    colors.push_back(0x00000000);
}

RGBController_MSIRGB::~RGBController_MSIRGB()
{

}

void RGBController_MSIRGB::UpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    msi->SetColor(red, grn, blu);
}

void RGBController_MSIRGB::UpdateZoneLEDs(int zone)
{
    UpdateLEDs();
}

void RGBController_MSIRGB::UpdateSingleLED(int led)
{
    UpdateLEDs();
}

void RGBController_MSIRGB::SetCustomMode()
{

}

void RGBController_MSIRGB::UpdateMode()
{

}