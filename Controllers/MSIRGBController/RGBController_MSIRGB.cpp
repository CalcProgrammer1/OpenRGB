/*---------------------------------------------------------*\
| RGBController_MSIRGB.cpp                                  |
|                                                           |
|   RGBController for MSI-RGB motherboard                   |
|                                                           |
|   Adam Honse (CalcProgrammer1)                14 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_MSIRGB.h"

/**------------------------------------------------------------------*\
    @name MSI RGB
    @category Motherboard
    @type SuperIO
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectMSIRGBControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_MSIRGB::RGBController_MSIRGB(MSIRGBController* controller_ptr)
{
    controller          = controller_ptr;

    name                = controller->GetDeviceName();
    vendor              = "MSI";
    type                = DEVICE_TYPE_MOTHERBOARD;
    description         = "MSI-RGB Device";
    location            = controller->GetDeviceLocation();

    mode Direct;
    Direct.name         = "Direct";
    Direct.value        = 0;
    Direct.flags        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_MSIRGB::~RGBController_MSIRGB()
{
    delete controller;
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

    controller->SetColor(red, grn, blu);
}

void RGBController_MSIRGB::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MSIRGB::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_MSIRGB::DeviceUpdateMode()
{

}
