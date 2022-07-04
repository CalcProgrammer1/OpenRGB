/*-----------------------------------------*\
|  RGBController_ASRockPolychromeSMBus.cpp  |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  ASRock ASR LED and Polychrome RGB Driver |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/15/2019  |
\*-----------------------------------------*/

#include "RGBController_ASRockPolychromeV1SMBus.h"

static const char* polychrome_v1_zone_names[] =
{
    "RGB LED 1 Header",
    "RGB LED 2 Header",
    "PCH",
    "IO Cover",
    "Audio",
    "Addressable Header"
};

/**------------------------------------------------------------------*\
    @name ASRock Polychrome v1 SMBus
    @category Motherboard
    @type SMBus
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectASRockSMBusControllers
    @comment ASRock Polychrome v1 controllers will save with each update.
        Per ARGB LED support is not possible with these devices.
\*-------------------------------------------------------------------*/

RGBController_ASRockPolychromeV1SMBus::RGBController_ASRockPolychromeV1SMBus(ASRockPolychromeV1SMBusController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = "ASRock";
    version     = controller->GetFirmwareVersion();
    type        = DEVICE_TYPE_MOTHERBOARD;
    description = "ASRock Polychrome v1 Device";
    location    = controller->GetDeviceLocation();


    mode Off;
    Off.name                    = "Off";
    Off.value                   = POLYCHROME_V1_MODE_OFF;
    Off.flags                   = 0;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Static";
    Static.value                = POLYCHROME_V1_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = POLYCHROME_V1_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min         = POLYCHROME_V1_SPEED_MIN_BREATHING;
    Breathing.speed_max         = POLYCHROME_V1_SPEED_MAX_BREATHING;
    Breathing.speed             = POLYCHROME_V1_SPEED_DEFAULT_BREATHING;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Strobe;
    Strobe.name                 = "Strobe";
    Strobe.value                = POLYCHROME_V1_MODE_STROBE;
    Strobe.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Strobe.speed_min            = POLYCHROME_V1_SPEED_MIN_STROBE;
    Strobe.speed_max            = POLYCHROME_V1_SPEED_MAX_STROBE;
    Strobe.speed                = POLYCHROME_V1_SPEED_DEFAULT_STROBE;
    Strobe.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Strobe);

    mode SpectrumCycle;
    SpectrumCycle.name          = "Spectrum Cycle";
    SpectrumCycle.value         = POLYCHROME_V1_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags         = MODE_FLAG_HAS_SPEED;
    SpectrumCycle.speed_min     = POLYCHROME_V1_SPEED_MIN_CYCLE;
    SpectrumCycle.speed_max     = POLYCHROME_V1_SPEED_MAX_CYCLE;
    SpectrumCycle.speed         = POLYCHROME_V1_SPEED_DEFAULT_CYCLE;
    SpectrumCycle.color_mode    = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Random;
    Random.name                 = "Random";
    Random.value                = POLYCHROME_V1_MODE_RANDOM;
    Random.flags                = MODE_FLAG_HAS_SPEED;
    Random.speed_min            = POLYCHROME_V1_SPEED_MIN_RANDOM;
    Random.speed_max            = POLYCHROME_V1_SPEED_MAX_RANDOM;
    Random.speed                = POLYCHROME_V1_SPEED_DEFAULT_RANDOM;
    Random.color_mode           = MODE_COLORS_NONE;
    modes.push_back(Random);

    mode Music;
    Music.name                 = "Music";
    Music.value                = POLYCHROME_V1_MODE_MUSIC;
    Music.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Music.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Music);

    mode Wave;
    Wave.name                   = "Wave";
    Wave.value                  = POLYCHROME_V1_MODE_WAVE;
    Wave.flags                  = MODE_FLAG_HAS_SPEED;
    Wave.speed_min              = POLYCHROME_V1_SPEED_MIN_WAVE;
    Wave.speed_max              = POLYCHROME_V1_SPEED_MAX_WAVE;
    Wave.speed                  = POLYCHROME_V1_SPEED_DEFAULT_WAVE;
    Wave.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Wave);

    /*---------------------------------------------------------------------*\
    | Comment out until per zone modes are working. These are only for ARGB |
    \*---------------------------------------------------------------------*/
    /*
    mode Spring;
    Spring.name                 = "Spring";
    Spring.value                = POLYCHROME_V1_MODE_SPRING;
    Spring.flags                = MODE_FLAG_HAS_SPEED;
    Spring.speed_min            = POLYCHROME_V1_SPEED_MIN_ARGB;
    Spring.speed_max            = POLYCHROME_V1_SPEED_MAX_ARGB;
    Spring.speed                = POLYCHROME_V1_SPEED_DEFAULT_SPRING;
    Spring.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Spring);

    mode Stack;
    Stack.name                  = "Stack";
    Stack.value                 = POLYCHROME_V1_MODE_STACK;
    Stack.flags                 = MODE_FLAG_HAS_SPEED;
    Stack.speed_min             = POLYCHROME_V1_SPEED_MIN_ARGB;
    Stack.speed_max             = POLYCHROME_V1_SPEED_MAX_ARGB;
    Stack.speed                 = POLYCHROME_V1_SPEED_DEFAULT_STACK;
    Stack.color_mode            = MODE_COLORS_PER_LED;
    modes.push_back(Stack);

    mode Cram;
    Cram.name                   = "Cram";
    Cram.value                  = POLYCHROME_V1_MODE_CRAM;
    Cram.flags                  = MODE_FLAG_HAS_SPEED;
    Cram.speed_min              = POLYCHROME_V1_SPEED_MIN_ARGB;
    Cram.speed_max              = POLYCHROME_V1_SPEED_MAX_ARGB;
    Cram.speed                  = POLYCHROME_V1_SPEED_DEFAULT_CRAM;
    Cram.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(Cram);

    mode Scan;
    Scan.name                   = "Scan";
    Scan.value                  = POLYCHROME_V1_MODE_SCAN;
    Scan.flags                  = MODE_FLAG_HAS_SPEED;
    Scan.speed_min              = POLYCHROME_V1_SPEED_MIN_ARGB;
    Scan.speed_max              = POLYCHROME_V1_SPEED_MAX_ARGB;
    Scan.speed                  = POLYCHROME_V1_SPEED_DEFAULT_SCAN;
    Scan.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(Scan);

    mode Neon;
    Neon.name                   = "Neon";
    Neon.value                  = POLYCHROME_V1_MODE_NEON;
    Neon.flags                  = MODE_FLAG_HAS_SPEED;
    Neon.speed_min              = POLYCHROME_V1_SPEED_MIN_ARGB;
    Neon.speed_max              = POLYCHROME_V1_SPEED_MAX_ARGB;
    Neon.speed                  = POLYCHROME_V1_SPEED_DEFAULT_NEON;
    Neon.color_mode             = MODE_COLORS_PER_LED;
    modes.push_back(Neon);

    mode Water;
    Water.name                  = "Water";
    Water.value                 = POLYCHROME_V1_MODE_WATER;
    Water.flags                 = MODE_FLAG_HAS_SPEED;
    Water.speed_min             = POLYCHROME_V1_SPEED_MIN_ARGB;
    Water.speed_max             = POLYCHROME_V1_SPEED_MAX_ARGB;
    Water.speed                 = POLYCHROME_V1_SPEED_DEFAULT_WATER;
    Water.color_mode            = MODE_COLORS_PER_LED;
    modes.push_back(Water);

    mode Rainbow;
    Rainbow.name                = "Rainbow";
    Rainbow.value               = POLYCHROME_V1_MODE_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_SPEED;
    Rainbow.speed_min           = POLYCHROME_V1_SPEED_MIN_RAINBOW;
    Rainbow.speed_max           = POLYCHROME_V1_SPEED_MAX_RAINBOW;
    Rainbow.speed               = POLYCHROME_V1_SPEED_DEFAULT_RAINBOW;
    Rainbow.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Rainbow);
    */

    SetupZones();
}

RGBController_ASRockPolychromeV1SMBus::~RGBController_ASRockPolychromeV1SMBus()
{
    delete controller;
}

void RGBController_ASRockPolychromeV1SMBus::SetupZones()
{
    /*---------------------------------------------------------*\
    | Polychrome motherboards should set up zones based on LED  |
    | configuration register read from device                   |
    \*---------------------------------------------------------*/
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(uint8_t zone_idx = 0; zone_idx < POLYCHROME_V1_ZONE_COUNT; zone_idx++)
    {
        if(controller->zone_led_count[zone_idx] > 0)
        {
            zone* new_zone = new zone();

            /*---------------------------------------------------------*\
            | Set zone name to channel name                             |
            \*---------------------------------------------------------*/

            new_zone->name          = polychrome_v1_zone_names[zone_idx];

            new_zone->leds_min      = 1;
            new_zone->leds_max      = 1;
            //new_zone->leds_count    = zone_led_count[zone_idx];
            new_zone->leds_count    = 1;

            new_zone->type          = ZONE_TYPE_SINGLE;

            new_zone->matrix_map    = NULL;

            /*---------------------------------------------------------*\
            | Push new zone to zones vector                             |
            \*---------------------------------------------------------*/
            zones.push_back(*new_zone);
        }
    }

    uint8_t led_count = 0;
    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(uint8_t zone_idx = 0; zone_idx < POLYCHROME_V1_ZONE_COUNT; zone_idx++)
    {
        if(controller->zone_led_count[zone_idx] > 0)
        {
            for(uint8_t led_idx = 0; led_idx < controller->zone_led_count[zone_idx]; led_idx++)
            {
                /*---------------------------------------------------------*\
                | Each zone only has one LED                                |
                \*---------------------------------------------------------*/
                led* new_led = new led();

                new_led->name           = polychrome_v1_zone_names[zone_idx];

                new_led->name.append(" " + std::to_string(led_idx + 1));
                new_led->value = 0;

                new_led->value              = zone_idx;

                /*---------------------------------------------------------*\
                | Push new LED to LEDs vector                               |
                \*---------------------------------------------------------*/
                leds.push_back(*new_led);

                led_count++;

                if(zone_idx == POLYCHROME_V1_ZONE_ADDRESSABLE)
                {
                    break;
                }
            }
        }
    }

    SetupColors();
}

void RGBController_ASRockPolychromeV1SMBus::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_ASRockPolychromeV1SMBus::DeviceUpdateLEDs()
{
    for (std::size_t led = 0; led < colors.size(); led++)
    {
        UpdateSingleLED(led);
    }
}

void RGBController_ASRockPolychromeV1SMBus::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ASRockPolychromeV1SMBus::UpdateSingleLED(int led)
{
    unsigned char red = RGBGetRValue(colors[led]);
    unsigned char grn = RGBGetGValue(colors[led]);
    unsigned char blu = RGBGetBValue(colors[led]);

    /*---------------------------------------------------------*\
    | If the LED value is non-zero, this LED overrides the LED  |
    | index                                                     |
    \*---------------------------------------------------------*/
    if(leds[led].value != 0)
    {
        led = leds[led].value;
    }

    controller->SetColorsAndSpeed(led, red, grn, blu);
}

void RGBController_ASRockPolychromeV1SMBus::SetCustomMode()
{
    active_mode = 1;
}

void RGBController_ASRockPolychromeV1SMBus::DeviceUpdateMode()
{
    for(uint8_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        controller->SetMode(led_idx, modes[active_mode].value, modes[active_mode].speed);
    }
    DeviceUpdateLEDs();
}
