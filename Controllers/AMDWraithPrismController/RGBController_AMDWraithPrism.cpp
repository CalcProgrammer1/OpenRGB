/*-----------------------------------------*\
|  RGBController_AMDWraithPrism.cpp         |
|                                           |
|  Generic RGB Interface for AMD Wraith     |
|  Prism                                    |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/25/2019  |
\*-----------------------------------------*/

#include "RGBController_AMDWraithPrism.h"

/**------------------------------------------------------------------*\
    @name AMD Wraith Prism
    @category Cooler
    @type USB
    @save :o:
    @direct :white_check_mark:
    @effects :tools:
    @detectors DetectAMDWraithPrismControllers
    @comment The Wraith Prism comes with 2 cables but is only detectable
        and controlable when using the USB cable. `Morse Code` and `Mirage`
        modes have not been implemented. Saving to flash is supported by
        the device but not yet implemented.
\*-------------------------------------------------------------------*/

RGBController_AMDWraithPrism::RGBController_AMDWraithPrism(AMDWraithPrismController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "AMD Wraith Prism";
    vendor      = "Cooler Master";
    type        = DEVICE_TYPE_COOLER;
    description = "AMD Wraith Prism Device";
    version     = controller->GetFirmwareVersionString();
    location    = controller->GetLocationString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = 0;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.brightness_min       = 0;
    Direct.brightness_max       = 0;
    Direct.brightness           = 0;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    controller->SendEnableCommand();
    controller->SendApplyCommand();

    SetupZones();
}

RGBController_AMDWraithPrism::~RGBController_AMDWraithPrism()
{
    delete controller;
}

void RGBController_AMDWraithPrism::SetupZones()
{
    /*---------------------------------------------------------*\
    | LED maps                                                  |
    \*---------------------------------------------------------*/
    const unsigned int logo_leds[1] =   { 0x00 };
    const unsigned int fan_leds[1] =    { 0x01 };
    const unsigned int ring_leds[14] =  { 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
                                          0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    zone logo_zone;
    logo_zone.name          = "Logo";
    logo_zone.type          = ZONE_TYPE_SINGLE;
    logo_zone.leds_min      = 1;
    logo_zone.leds_max      = 1;
    logo_zone.leds_count    = 1;
    logo_zone.matrix_map    = NULL;
    zones.push_back(logo_zone);

    zone fan_zone;
    fan_zone.name           = "Fan";
    fan_zone.type           = ZONE_TYPE_SINGLE;
    fan_zone.leds_min       = 1;
    fan_zone.leds_max       = 1;
    fan_zone.leds_count     = 1;
    fan_zone.matrix_map     = NULL;
    zones.push_back(fan_zone);

    zone ring_zone;
    ring_zone.name          = "Ring";
    ring_zone.type          = ZONE_TYPE_LINEAR;
    ring_zone.leds_min      = 14;
    ring_zone.leds_max      = 14;
    ring_zone.leds_count    = 14;
    ring_zone.matrix_map    = NULL;
    zones.push_back(ring_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < 1; led_idx++)
    {
        led logo_led;
        logo_led.name = "Logo LED";
        logo_led.value = logo_leds[led_idx];
        leds.push_back(logo_led);
    }

    for(unsigned int led_idx = 0; led_idx < 1; led_idx++)
    {
        led fan_led;
        fan_led.name = "Fan LED";
        fan_led.value = fan_leds[led_idx];
        leds.push_back(fan_led);
    }

    for(unsigned int led_idx = 0; led_idx < 14; led_idx++)
    {
        led ring_led;
        ring_led.name = "Ring LED";
        ring_led.value = ring_leds[led_idx];
        leds.push_back(ring_led);
    }

    SetupColors();
}

void RGBController_AMDWraithPrism::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AMDWraithPrism::DeviceUpdateLEDs()
{
    unsigned char   led_ids[15];
    RGBColor        color_buf[15];
    unsigned int    leds_count = 0;

    for(unsigned int led_idx = 0; led_idx < colors.size(); led_idx++)
    {
        led_ids[leds_count]     = (unsigned char)leds[led_idx].value;
        color_buf[leds_count]   = colors[led_idx];

        leds_count++;

        if(leds_count >= 15)
        {
            controller->SendDirectPacket(15, led_ids, color_buf);
            leds_count = 0;
        }
    }

    if(leds_count > 0)
    {
        controller->SendDirectPacket(leds_count, led_ids, color_buf);
    }
}

void RGBController_AMDWraithPrism::UpdateZoneLEDs(int zone)
{

}

void RGBController_AMDWraithPrism::UpdateSingleLED(int led)
{

}

void RGBController_AMDWraithPrism::DeviceUpdateMode()
{

}
