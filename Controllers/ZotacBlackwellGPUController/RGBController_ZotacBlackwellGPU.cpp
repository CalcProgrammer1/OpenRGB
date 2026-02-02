/*---------------------------------------------------------*\
| RGBController_ZotacBlackwellGPU.cpp                       |
|                                                           |
|   RGBController for ZOTAC Blackwell (RTX 50 series) GPU   |
|                                                           |
|   Eder Sánchez                                27 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_ZotacBlackwellGPU.h"

/**------------------------------------------------------------------*\
    @name ZOTAC RTX 50 series GPU
    @category GPU
    @type I2C
    @save :robot:
    @direct :x:
    @effects :tools:
    @detectors DetectZotacBlackwellGPUControllersPCI
    @comment
        Supports ZOTAC Blackwell (RTX 50 series) GPUs with 3 zones:
        Logo, Side Bar, and Infinity Mirror.

        The controller uses individual SMBus byte writes (registers
        0x20-0x2F) with a 3ms delay between each transaction.

        To add new cards, add PCI ID entries in `pci_ids/pci_ids.h`
        and detection entries in
        `Controllers/ZotacBlackwellGPUController/ZotacBlackwellGPUControllerDetect.cpp`.
\*-------------------------------------------------------------------*/

RGBController_ZotacBlackwellGPU::RGBController_ZotacBlackwellGPU(ZotacBlackwellGPUController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetName();
    vendor                  = "ZOTAC";
    description             = "ZOTAC RTX 50 series RGB GPU Device (" + controller->GetVersion() + ")";
    location                = controller->GetDeviceLocation();
    type                    = DEVICE_TYPE_GPU;
    version                 = controller->GetVersion();

    /*---------------------------------------------------------*\
    | Static mode                                               |
    \*---------------------------------------------------------*/
    mode STATIC;
    STATIC.name             = "Static";
    STATIC.value            = ZOTAC_BLACKWELL_GPU_MODE_STATIC;
    STATIC.flags            = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    STATIC.brightness_min   = 0;
    STATIC.brightness_max   = 100;
    STATIC.brightness       = 100;
    STATIC.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(STATIC);

    /*---------------------------------------------------------*\
    | Breathe mode                                              |
    \*---------------------------------------------------------*/
    mode BREATHE;
    BREATHE.name            = "Breathe";
    BREATHE.value           = ZOTAC_BLACKWELL_GPU_MODE_BREATHE;
    BREATHE.flags           = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    BREATHE.brightness_min  = 0;
    BREATHE.brightness_max  = 100;
    BREATHE.brightness      = 100;
    BREATHE.speed_min       = 0;
    BREATHE.speed_max       = 100;
    BREATHE.speed           = 20;
    BREATHE.color_mode      = MODE_COLORS_PER_LED;
    modes.push_back(BREATHE);

    /*---------------------------------------------------------*\
    | Fade mode                                                 |
    \*---------------------------------------------------------*/
    mode FADE;
    FADE.name               = "Fade";
    FADE.value              = ZOTAC_BLACKWELL_GPU_MODE_FADE;
    FADE.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED;
    FADE.speed_min          = 0;
    FADE.speed_max          = 100;
    FADE.speed              = 20;
    FADE.color_mode         = MODE_COLORS_NONE;
    modes.push_back(FADE);

    /*---------------------------------------------------------*\
    | Wink mode                                                 |
    \*---------------------------------------------------------*/
    mode WINK;
    WINK.name               = "Wink";
    WINK.value              = ZOTAC_BLACKWELL_GPU_MODE_WINK;
    WINK.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    WINK.brightness_min     = 0;
    WINK.brightness_max     = 100;
    WINK.brightness         = 100;
    WINK.speed_min          = 0;
    WINK.speed_max          = 100;
    WINK.speed              = 20;
    WINK.color_mode         = MODE_COLORS_PER_LED;
    modes.push_back(WINK);

    /*---------------------------------------------------------*\
    | Glide mode                                                |
    \*---------------------------------------------------------*/
    mode GLIDE;
    GLIDE.name              = "Glide";
    GLIDE.value             = ZOTAC_BLACKWELL_GPU_MODE_GLIDE;
    GLIDE.flags             = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_PER_LED_COLOR;
    GLIDE.brightness_min    = 0;
    GLIDE.brightness_max    = 100;
    GLIDE.brightness        = 100;
    GLIDE.speed_min         = 0;
    GLIDE.speed_max         = 100;
    GLIDE.speed             = 20;
    GLIDE.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(GLIDE);

    /*---------------------------------------------------------*\
    | Prism mode (called "Rainbow" in older firmware)           |
    \*---------------------------------------------------------*/
    mode PRISM;
    PRISM.name              = "Prism";
    PRISM.value             = ZOTAC_BLACKWELL_GPU_MODE_PRISM;
    PRISM.flags             = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    PRISM.speed_min         = 0;
    PRISM.speed_max         = 100;
    PRISM.speed             = 20;
    PRISM.color_mode        = MODE_COLORS_NONE;
    modes.push_back(PRISM);

    /*---------------------------------------------------------*\
    | Bokeh mode                                                |
    \*---------------------------------------------------------*/
    mode BOKEH;
    BOKEH.name              = "Bokeh";
    BOKEH.value             = ZOTAC_BLACKWELL_GPU_MODE_BOKEH;
    BOKEH.flags             = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    BOKEH.brightness_min    = 0;
    BOKEH.brightness_max    = 100;
    BOKEH.brightness        = 100;
    BOKEH.speed_min         = 0;
    BOKEH.speed_max         = 100;
    BOKEH.speed             = 20;
    BOKEH.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(BOKEH);

    /*---------------------------------------------------------*\
    | Beacon mode                                               |
    \*---------------------------------------------------------*/
    mode BEACON;
    BEACON.name             = "Beacon";
    BEACON.value            = ZOTAC_BLACKWELL_GPU_MODE_BEACON;
    BEACON.flags            = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    BEACON.brightness_min   = 0;
    BEACON.brightness_max   = 100;
    BEACON.brightness       = 100;
    BEACON.speed_min        = 0;
    BEACON.speed_max        = 100;
    BEACON.speed            = 20;
    BEACON.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(BEACON);

    /*---------------------------------------------------------*\
    | Tandem mode                                               |
    \*---------------------------------------------------------*/
    mode TANDEM;
    TANDEM.name             = "Tandem";
    TANDEM.value            = ZOTAC_BLACKWELL_GPU_MODE_TANDEM;
    TANDEM.flags            = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    TANDEM.brightness_min   = 0;
    TANDEM.brightness_max   = 100;
    TANDEM.brightness       = 100;
    TANDEM.speed_min        = 0;
    TANDEM.speed_max        = 100;
    TANDEM.speed            = 20;
    TANDEM.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(TANDEM);

    /*---------------------------------------------------------*\
    | Tidal mode                                                |
    \*---------------------------------------------------------*/
    mode TIDAL;
    TIDAL.name              = "Tidal";
    TIDAL.value             = ZOTAC_BLACKWELL_GPU_MODE_TIDAL;
    TIDAL.flags             = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_PER_LED_COLOR;
    TIDAL.brightness_min    = 0;
    TIDAL.brightness_max    = 100;
    TIDAL.brightness        = 100;
    TIDAL.speed_min         = 0;
    TIDAL.speed_max         = 100;
    TIDAL.speed             = 20;
    TIDAL.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(TIDAL);

    /*---------------------------------------------------------*\
    | Astra mode                                                |
    \*---------------------------------------------------------*/
    mode ASTRA;
    ASTRA.name              = "Astra";
    ASTRA.value             = ZOTAC_BLACKWELL_GPU_MODE_ASTRA;
    ASTRA.flags             = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    ASTRA.brightness_min    = 0;
    ASTRA.brightness_max    = 100;
    ASTRA.brightness        = 100;
    ASTRA.speed_min         = 0;
    ASTRA.speed_max         = 100;
    ASTRA.speed             = 20;
    ASTRA.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(ASTRA);

    /*---------------------------------------------------------*\
    | Cosmic mode                                               |
    \*---------------------------------------------------------*/
    mode COSMIC;
    COSMIC.name             = "Cosmic";
    COSMIC.value            = ZOTAC_BLACKWELL_GPU_MODE_COSMIC;
    COSMIC.flags            = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    COSMIC.brightness_min   = 0;
    COSMIC.brightness_max   = 100;
    COSMIC.brightness       = 100;
    COSMIC.speed_min        = 0;
    COSMIC.speed_max        = 100;
    COSMIC.speed            = 20;
    COSMIC.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(COSMIC);

    /*---------------------------------------------------------*\
    | Volta mode                                                |
    \*---------------------------------------------------------*/
    mode VOLTA;
    VOLTA.name              = "Volta";
    VOLTA.value             = ZOTAC_BLACKWELL_GPU_MODE_VOLTA;
    VOLTA.flags             = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    VOLTA.brightness_min    = 0;
    VOLTA.brightness_max    = 100;
    VOLTA.brightness        = 100;
    VOLTA.speed_min         = 0;
    VOLTA.speed_max         = 100;
    VOLTA.speed             = 20;
    VOLTA.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(VOLTA);

    SetupZones();
}

RGBController_ZotacBlackwellGPU::~RGBController_ZotacBlackwellGPU()
{
    Shutdown();

    delete controller;
}

void RGBController_ZotacBlackwellGPU::SetupZones()
{
    /*---------------------------------------------------------*\
    | Zone 0: Logo                                              |
    \*---------------------------------------------------------*/
    zone logo_zone;
    logo_zone.name          = "Logo";
    logo_zone.type          = ZONE_TYPE_SINGLE;
    logo_zone.leds_min      = 1;
    logo_zone.leds_max      = 1;
    logo_zone.leds_count    = 1;
    zones.push_back(logo_zone);

    led logo_led;
    logo_led.name           = "Logo LED";
    leds.push_back(logo_led);

    /*---------------------------------------------------------*\
    | Zone 1: Side Bar                                          |
    \*---------------------------------------------------------*/
    zone sidebar_zone;
    sidebar_zone.name       = "Side Bar";
    sidebar_zone.type       = ZONE_TYPE_SINGLE;
    sidebar_zone.leds_min   = 1;
    sidebar_zone.leds_max   = 1;
    sidebar_zone.leds_count = 1;
    zones.push_back(sidebar_zone);

    led sidebar_led;
    sidebar_led.name        = "Side Bar LED";
    leds.push_back(sidebar_led);

    /*---------------------------------------------------------*\
    | Zone 2: Infinity Mirror                                   |
    \*---------------------------------------------------------*/
    zone infinity_zone;
    infinity_zone.name       = "Infinity Mirror";
    infinity_zone.type       = ZONE_TYPE_SINGLE;
    infinity_zone.leds_min   = 1;
    infinity_zone.leds_max   = 1;
    infinity_zone.leds_count = 1;
    zones.push_back(infinity_zone);

    led infinity_led;
    infinity_led.name        = "Infinity Mirror LED";
    leds.push_back(infinity_led);

    SetupColors();
}

void RGBController_ZotacBlackwellGPU::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_ZotacBlackwellGPU::DeviceUpdateZoneLEDs(int zone)
{
    unsigned int mode_val   = modes[active_mode].value;
    unsigned int brightness = modes[active_mode].brightness;
    unsigned int speed      = modes[active_mode].speed;
    unsigned int direction  = modes[active_mode].direction == MODE_DIRECTION_RIGHT
                            ? ZOTAC_BLACKWELL_GPU_DIR_RIGHT
                            : ZOTAC_BLACKWELL_GPU_DIR_LEFT;

    RGBColor color1;
    RGBColor color2 = ToRGBColor(0, 0, 0);

    switch(modes[active_mode].color_mode)
    {
        case MODE_COLORS_PER_LED:
            color1 = colors[zone];
            break;

        case MODE_COLORS_MODE_SPECIFIC:
            color1 = (modes[active_mode].colors.size() >= 1)
                   ? modes[active_mode].colors[0]
                   : ToRGBColor(0, 0, 0);
            color2 = (modes[active_mode].colors.size() >= 2)
                   ? modes[active_mode].colors[1]
                   : ToRGBColor(0, 0, 0);
            break;

        default:
            color1 = ToRGBColor(0, 0, 0);
            break;
    }

    controller->SetMode(zone, mode_val, color1, color2, brightness, speed, direction);
    controller->Commit();
}

void RGBController_ZotacBlackwellGPU::DeviceUpdateSingleLED(int led)
{
    DeviceUpdateZoneLEDs(led);
}

void RGBController_ZotacBlackwellGPU::DeviceUpdateMode()
{
    for(unsigned int zone_idx = 0; zone_idx < ZOTAC_BLACKWELL_GPU_NUM_ZONES; zone_idx++)
    {
        DeviceUpdateZoneLEDs(zone_idx);
    }
}
