/*-------------------------------------------------------------------*\
|  RGBController_CMMKController.cpp                                   |
|                                                                     |
|  Driver for Coolermaster MasterKeys keyboards                       |
|                                                                     |
|  Lukas N (chmod222)          28th Jun 2020                          |
|  Tam D (too.manyhobbies)     25th Apr 2021                          |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RGBController_CMMKController.h"

#include <sstream>

using namespace std::chrono_literals;

#define CMMK_SPEED_MIN          CMMK_SPEED0
#define CMMK_SPEED_MID          CMMK_SPEED2
#define CMMK_SPEED_MAX          CMMK_SPEED4
#define CMMK_MODE_FIRMWARE      0xFF
#define CMMK_MODE_MANUAL        0x7F

/**------------------------------------------------------------------*\
    @name Coolermaster Masterkeys Keyboards
    @category Keyboard
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCoolerMasterKeyboards
    @comment
\*-------------------------------------------------------------------*/

RGBController_CMMKController::RGBController_CMMKController(CMMKController* controller_ptr)
{
    controller                  = controller_ptr;
    name                        = controller->GetDeviceName();
    vendor                      = controller->GetDeviceVendor();
    type                        = DEVICE_TYPE_KEYBOARD;
    description                 = "Cooler Master MasterKeys Device";
    version                     = controller->GetFirmwareVersion();
    serial                      = "";
    location                    = controller->GetLocation();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = CMMK_MODE_MANUAL;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = CMMK_EFFECT_FULLY_LIT;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = CMMK_EFFECT_BREATHE;
    Breathing.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed_min         = CMMK_SPEED_MIN;
    Breathing.speed_max         = CMMK_SPEED_MAX;
    Breathing.speed             = CMMK_SPEED_MID;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Cycle;
    Cycle.name                  = "Spectrum Cycle";
    Cycle.value                 = CMMK_EFFECT_CYCLE;
    Cycle.flags                 = MODE_FLAG_HAS_SPEED;
    Cycle.color_mode            = MODE_COLORS_NONE;
    Cycle.speed_min             = 2 * CMMK_SPEED_MIN; //Spectrum Cycle uses a unique speed range
    Cycle.speed_max             = 2 * CMMK_SPEED_MAX;
    Cycle.speed                 = 2 * CMMK_SPEED_MID;
    modes.push_back(Cycle);

    mode Reactive;
    Reactive.name               = "Reactive";
    Reactive.value              = CMMK_EFFECT_SINGLE;
    Reactive.flags              = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Reactive.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Reactive.speed_min          = CMMK_SPEED_MIN;
    Reactive.speed_max          = CMMK_SPEED_MAX;
    Reactive.speed              = CMMK_SPEED_MID;
    Reactive.colors_min         = 2;
    Reactive.colors_max         = 2;
    Reactive.colors.resize(2);
    modes.push_back(Reactive);

    mode Wave;
    Wave.name                   = "Rainbow Wave";
    Wave.value                  = CMMK_EFFECT_WAVE;
    Wave.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    Wave.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Wave.speed_min              = CMMK_SPEED_MIN;
    Wave.speed_max              = CMMK_SPEED_MAX;
    Wave.speed                  = CMMK_SPEED_MID;
    Wave.direction              = MODE_DIRECTION_LEFT;
    Wave.colors_min             = 1;
    Wave.colors_max             = 1;
    Wave.colors.resize(1);
    modes.push_back(Wave);

    mode Ripple;
    Ripple.name                 = "Ripple Effect";
    Ripple.value                = CMMK_EFFECT_RIPPLE;
    Ripple.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED;
    Ripple.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Ripple.speed_min            = CMMK_SPEED_MIN;
    Ripple.speed_max            = CMMK_SPEED_MAX;
    Ripple.speed                = CMMK_SPEED_MID;
    Ripple.colors_min           = 2;
    Ripple.colors_max           = 2;
    Ripple.colors.resize(2);
    modes.push_back(Ripple);

    mode Cross;
    Cross.name                  = "Cross";
    Cross.value                 = CMMK_EFFECT_CROSS;
    Cross.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Cross.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Cross.speed_min             = CMMK_SPEED_MIN;
    Cross.speed_max             = CMMK_SPEED_MAX;
    Cross.speed                 = CMMK_SPEED_MID;
    Cross.colors_min            = 2;
    Cross.colors_max            = 2;
    Cross.colors.resize(2);
    modes.push_back(Cross);

    mode Raindrops;
    Raindrops.name              = "Raindrops";
    Raindrops.value             = CMMK_EFFECT_RAINDROPS;
    Raindrops.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Raindrops.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Raindrops.speed_min         = CMMK_SPEED_MIN;
    Raindrops.speed_max         = CMMK_SPEED_MAX;
    Raindrops.speed             = CMMK_SPEED_MID;
    Raindrops.colors_min        = 2;
    Raindrops.colors_max        = 2;
    Raindrops.colors.resize(2);
    modes.push_back(Raindrops);

    mode Stars;
    Stars.name                  = "Starfield";
    Stars.value                 = CMMK_EFFECT_STARS;
    Stars.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Stars.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Stars.speed_min             = CMMK_SPEED_MIN;
    Stars.speed_max             = CMMK_SPEED_MAX;
    Stars.speed                 = CMMK_SPEED_MID;
    Stars.colors_min            = 2;
    Stars.colors_max = 2;
    Stars.colors.resize(2);
    modes.push_back(Stars);

    mode Snake;
    Snake.name                  = "Snake";
    Snake.value                 = CMMK_EFFECT_SNAKE;
    Snake.flags                 = MODE_FLAG_HAS_SPEED;
    Snake.color_mode            = MODE_COLORS_NONE;
    Snake.speed_min             = CMMK_SPEED_MIN;
    Snake.speed_max             = CMMK_SPEED_MAX;
    Snake.speed                 = CMMK_SPEED_MID;
    modes.push_back(Snake);

    mode FirmwareControl;
    FirmwareControl.name        = "Firmware Controlled";
    FirmwareControl.value       = 0xFF;
    FirmwareControl.flags       = 0;
    FirmwareControl.color_mode  = MODE_COLORS_NONE;
    modes.push_back(FirmwareControl);

    SetupZones();
}

RGBController_CMMKController::~RGBController_CMMKController()
{
    delete controller;
}

void RGBController_CMMKController::SetupZones()
{
    /*---------------------------------------------------------*\
    | Create the keyboard zone usiung Keyboard Layout Manager   |
    \*---------------------------------------------------------*/
    zone new_zone;
    new_zone.name           = ZONE_EN_KEYBOARD;
    new_zone.type           = ZONE_TYPE_MATRIX;

    layoutManager = new KeyboardLayoutManager(controller->GetLayout(), controller->GetSize(), controller->GetLayoutValues());

    matrix_map_type * new_map   = new matrix_map_type;
    new_zone.matrix_map         = new_map;
    new_zone.matrix_map->height = layoutManager->GetRowCount();
    new_zone.matrix_map->width  = layoutManager->GetColumnCount();

    new_zone.matrix_map->map    = new unsigned int[new_map->height * new_map->width];
    new_zone.leds_count         = layoutManager->GetKeyCount();
    new_zone.leds_min           = new_zone.leds_count;
    new_zone.leds_max           = new_zone.leds_count;

    /*---------------------------------------------------------*\
    | Matrix map still uses declared zone rows and columns      |
    |   as the packet structure depends on the matrix map       |
    \*---------------------------------------------------------*/
    layoutManager->GetKeyMap(new_map->map, KEYBOARD_MAP_FILL_TYPE_COUNT, new_map->height, new_map->width);

    /*---------------------------------------------------------*\
    | Create LEDs for the Matrix zone                           |
    |   Place keys in the layout to populate the matrix         |
    \*---------------------------------------------------------*/
    for(size_t led_idx = 0; led_idx < new_zone.leds_count; led_idx++)
    {
        led new_led;

        new_led.name                = layoutManager->GetKeyNameAt(led_idx);
        new_led.value               = layoutManager->GetKeyValueAt(led_idx);
        leds.push_back(new_led);
    }

    zones.push_back(new_zone);

    SetupColors();
}

void RGBController_CMMKController::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

struct rgb map_to_cmmk_rgb(RGBColor input)
{
    return rgb
    {
        (uint8_t)RGBGetRValue(input),
        (uint8_t)RGBGetGValue(input),
        (uint8_t)RGBGetBValue(input)
    };
}

enum cmmk_wave_direction map_to_cmmk_dir(int input)
{
    switch(input)
    {
        case MODE_DIRECTION_LEFT:
            return CMMK_RIGHT_TO_LEFT;

        case MODE_DIRECTION_RIGHT:
            return CMMK_LEFT_TO_RIGHT;

        case MODE_DIRECTION_UP:
            return CMMK_FRONT_TO_BACK;

        case MODE_DIRECTION_DOWN:
            return CMMK_BACK_TO_FRONT;

        default:
            return CMMK_RIGHT_TO_LEFT;
    }
}


void RGBController_CMMKController::copy_buffers(std::vector<RGBColor> &in_colors, struct cmmk_color_matrix& mat, std::atomic<bool>& dirty)
{
    dirty.store(false);

    keyboard_layout * transform = controller->GetTransform();

    if(0 == transform)
    {
        return;
    }

    for(int row = 0; row < controller->GetRowCount(); row++)
    {
        for(int col = 0; col < controller->GetColumnCount(); col++)
        {
            int key_idx = (*transform)[row][col];

            if(-1 == key_idx)
            {
                continue;
            }

            struct rgb color = map_to_cmmk_rgb(in_colors[key_idx]);

            dirty.store(true);
            mat.data[row][col] = color;
        }
    }
}

void RGBController_CMMKController::DeviceUpdateLEDs()
{
    copy_buffers(colors, current_matrix, dirty);

    if(force_update.load() || dirty.load())
    {
        controller->SetAll(current_matrix);

        force_update.store(false);
    }
}

void RGBController_CMMKController::UpdateZoneLEDs(int zone_idx)
{
    /*---------------------------------------------------------*\
    | This device only supports a single zone, as a result we  |
    | update all LEDs.                                         |
    \*---------------------------------------------------------*/
    DeviceUpdateLEDs();
}

void RGBController_CMMKController::UpdateSingleLED(int led_idx, RGBColor color)
{
    keyboard_layout * transform = controller->GetTransform();
    led selected_led = leds[led_idx];

    if(0 == transform)
    {
        return;
    }

    // this is really expensive
    for(int row = 0; row < controller->GetRowCount(); row++)
    {
        for(int col = 0; col < controller->GetColumnCount(); col++)
        {
            int val = (*transform)[row][col];

            if(val != (int)selected_led.value)
            {
                continue;
            }

            controller->SetSingle(row, col, map_to_cmmk_rgb(color));
            dirty.store(false);

            return;
        }
    }

}

void RGBController_CMMKController::UpdateSingleLED(int led_idx)
{
    UpdateSingleLED(led_idx, colors[led_idx]);
}

void RGBController_CMMKController::SetCustomMode()
{
    active_mode = 1;
}

void RGBController_CMMKController::DeviceUpdateMode()
{
    force_update.store(true);

    switch(modes[active_mode].value)
    {
        case CMMK_MODE_FIRMWARE:
            controller->SetFirmwareControl();
            break;

        case CMMK_MODE_MANUAL:
            controller->SetManualControl();
            break;

        case CMMK_EFFECT_FULLY_LIT:
            {
                cmmk_effect_fully_lit fully_lit_effect;

                fully_lit_effect.color  = map_to_cmmk_rgb(modes[active_mode].colors[0]);

                controller->SetMode(fully_lit_effect);
            }
            break;

        case CMMK_EFFECT_BREATHE:
            {
                cmmk_effect_breathe breathe_effect;

                breathe_effect.speed    = (uint8_t)modes[active_mode].speed;
                breathe_effect.color    = map_to_cmmk_rgb(modes[active_mode].colors[0]);

                controller->SetMode(breathe_effect);
            }
            break;

        case CMMK_EFFECT_CYCLE:
            {
                cmmk_effect_cycle cycle_effect;

                cycle_effect.speed  = (uint8_t)modes[active_mode].speed;

                controller->SetMode(cycle_effect);
            }
            break;

        case CMMK_EFFECT_SINGLE:
            {
                cmmk_effect_single single_effect;

                single_effect.speed     = (uint8_t)modes[active_mode].speed;
                single_effect.active    = map_to_cmmk_rgb(modes[active_mode].colors[0]);
                single_effect.rest      = map_to_cmmk_rgb(modes[active_mode].colors[1]);

                controller->SetMode(single_effect);
            }
            break;

        case CMMK_EFFECT_WAVE:
            {
                cmmk_effect_wave wave_effect;

                wave_effect.speed       = (uint8_t)modes[active_mode].speed;
                wave_effect.direction   = map_to_cmmk_dir(modes[active_mode].direction);
                wave_effect.start       = map_to_cmmk_rgb(modes[active_mode].colors[0]);

                controller->SetMode(wave_effect);
            }
            break;

        case CMMK_EFFECT_RIPPLE:
            {
                cmmk_effect_ripple ripple_effect;

                ripple_effect.speed             = (uint8_t)modes[active_mode].speed;
                ripple_effect.active            = map_to_cmmk_rgb(modes[active_mode].colors[0]);
                ripple_effect.rest              = map_to_cmmk_rgb(modes[active_mode].colors[1]);

                if(modes[active_mode].color_mode == MODE_COLORS_RANDOM)
                {
                    ripple_effect.ripple_type   = CMMK_RIPPLE_RANDOM_COLOR;
                }
                else
                {
                    ripple_effect.ripple_type   = CMMK_RIPPLE_GIVEN_COLOR;
                }

                controller->SetMode(ripple_effect);
            }
            break;

        case CMMK_EFFECT_CROSS:
            {
                cmmk_effect_cross cross_effect;

                cross_effect.speed  = (uint8_t)modes[active_mode].speed;
                cross_effect.active = map_to_cmmk_rgb(modes[active_mode].colors[0]);
                cross_effect.rest   = map_to_cmmk_rgb(modes[active_mode].colors[1]);

                controller->SetMode(cross_effect);
            }
            break;

        case CMMK_EFFECT_RAINDROPS:
            {
                cmmk_effect_raindrops raindrops_effect;

                raindrops_effect.speed      = (uint8_t)modes[active_mode].speed;
                raindrops_effect.interval   = CMMK_SPEED_MID;
                raindrops_effect.active     = map_to_cmmk_rgb(modes[active_mode].colors[0]);
                raindrops_effect.rest       = map_to_cmmk_rgb(modes[active_mode].colors[1]);

                controller->SetMode(raindrops_effect);
            }
            break;

        case CMMK_EFFECT_STARS:
            {
                cmmk_effect_stars stars_effect;

                stars_effect.speed      = (uint8_t)modes[active_mode].speed;
                stars_effect.interval   = CMMK_SPEED_MID;
                stars_effect.active     = map_to_cmmk_rgb(modes[active_mode].colors[0]);
                stars_effect.rest       = map_to_cmmk_rgb(modes[active_mode].colors[1]);

                controller->SetMode(stars_effect);
            }
            break;

        case CMMK_EFFECT_SNAKE:
            {
                cmmk_effect_snake snake_effect;

                snake_effect.speed = (uint8_t)modes[active_mode].speed;

                controller->SetMode(snake_effect);
            }
            break;
    }
}
