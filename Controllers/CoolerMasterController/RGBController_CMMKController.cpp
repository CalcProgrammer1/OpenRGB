/*-------------------------------------------------------------------*\
|  RGBController_CMMKController.cpp                                   |
|                                                                     |
|  Driver for Coolermaster MasterKeys keyboards                       |
|                                                                     |
|  Lukas N (chmod222)          28th Jun 2020                          |
|  Tam D (too.manyhobbies)  25th Apr 2021                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RGBController_CMMKController.h"

#include <sstream>

using namespace std::chrono_literals;

#define CMMK_SPEED_MIN      CMMK_SPEED0
#define CMMK_SPEED_MID      CMMK_SPEED2 
#define CMMK_SPEED_MAX      CMMK_SPEED4
#define CMMK_MODE_FIRMWARE  0xFF
#define CMMK_MODE_MANUAL    0x7F

RGBController_CMMKController::RGBController_CMMKController(CMMKController* cmmk_ctrl)
{
    cmmk        = cmmk_ctrl;

    name        = cmmk->GetDeviceName();
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Cooler Master MasterKeys Device";
    version     = cmmk->GetFirmwareVersion();
    serial      = "";
    location    = cmmk->GetLocation();
    
    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = CMMK_MODE_MANUAL;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name       = "Static";
    Static.value      = CMMK_EFFECT_FULLY_LIT;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.colors.resize(1);
    modes.push_back(Static);
    
    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = CMMK_EFFECT_BREATHE;
    Breathing.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.speed_min  = 0x20;
    Breathing.speed_max  = 0x20;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min = 1;
    Breathing.colors_max = 1;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Cycle;
    Cycle.name       = "Spectrum Cycle";
    Cycle.value      = CMMK_EFFECT_CYCLE;
    Cycle.flags      = MODE_FLAG_HAS_SPEED;
    Cycle.speed_min  = CMMK_SPEED_MIN;
    Cycle.speed_max  = CMMK_SPEED_MAX;
    Cycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(Cycle);

    mode Reactive;
    Reactive.name       = "Reactive";
    Reactive.value      = CMMK_EFFECT_SINGLE;
    Reactive.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Reactive.speed_min  = CMMK_SPEED_MIN;
    Reactive.speed_max  = CMMK_SPEED_MAX;
    Reactive.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Reactive.colors_min = 2;
    Reactive.colors_max = 2;
    Reactive.colors.resize(2);
    modes.push_back(Reactive);

    mode Wave;
    Wave.name       = "Rainbow Wave";
    Wave.value      = CMMK_EFFECT_WAVE;
    Wave.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    Wave.speed_min  = CMMK_SPEED_MIN;
    Wave.speed_max  = CMMK_SPEED_MAX;
    Wave.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Wave.colors_min = 1;
    Wave.colors_max = 1;
    Wave.colors.resize(1);
    modes.push_back(Wave);
    
    mode Ripple;
    Ripple.name       = "Ripple Effect";
    Ripple.value      = CMMK_EFFECT_RIPPLE;
    Ripple.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED;
    Ripple.speed_min  = CMMK_SPEED_MIN;
    Ripple.speed_max  = CMMK_SPEED_MAX;
    Ripple.colors_min = 2;
    Ripple.colors_max = 2;
    Ripple.colors.resize(2);
    modes.push_back(Ripple);
    
    mode Cross;
    Cross.name       = "Cross";
    Cross.value      = CMMK_EFFECT_CROSS;
    Cross.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Cross.speed_min  = CMMK_SPEED_MIN;
    Cross.speed_max  = CMMK_SPEED_MAX;
    Cross.colors_min = 2;
    Cross.colors_max = 2;
    Cross.colors.resize(2);
    modes.push_back(Cross);

    mode Raindrops;
    Raindrops.name       = "Raindrops";
    Raindrops.value      = CMMK_EFFECT_RAINDROPS;
    Raindrops.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Raindrops.speed_min  = CMMK_SPEED_MIN;
    Raindrops.speed_max  = CMMK_SPEED_MAX;
    Raindrops.colors_min = 2;
    Raindrops.colors_max = 2;
    Raindrops.colors.resize(2);
    modes.push_back(Raindrops);
    
    mode Stars;
    Stars.name       = "Starfield";
    Stars.value      = CMMK_EFFECT_STARS;
    Stars.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Stars.speed_min  = CMMK_SPEED_MIN;
    Stars.speed_max  = CMMK_SPEED_MAX;
    Stars.colors_min = 2;
    Stars.colors_max = 2;
    Stars.colors.resize(2);
    modes.push_back(Stars);

    mode Snake;
    Snake.name      = "Snake";
    Snake.value     = CMMK_EFFECT_SNAKE;
    Snake.flags     = MODE_FLAG_HAS_SPEED;
    Snake.speed_min = CMMK_SPEED_MIN;
    Snake.speed_max = CMMK_SPEED_MAX;
    modes.push_back(Snake);

    mode FirmwareControl;
    FirmwareControl.name  = "Firmware Controlled";
    FirmwareControl.value = 0xFF;
    FirmwareControl.flags = 0;
    modes.push_back(FirmwareControl);

    SetupZones();
}

RGBController_CMMKController::~RGBController_CMMKController()
{
    delete cmmk;
}

void RGBController_CMMKController::SetupMatrixMap()
{
    for(int y = 0; y < CMMK_ROWS_MAX; y++)
    {
        for(int x = 0; x < CMMK_COLS_MAX; x++)
        {
            matrix_map[y][x] = 0xFFFFFFFF;
        }
    }

    for(size_t i = 0; i < leds.size(); i++)
    {
        led& l  = leds[i];

        int y   = (l.value & 0xFF00) >> 8;
        int x   = (l.value & 0xFF);

        matrix_map[y][x] = i;
    }
}

void RGBController_CMMKController::SetupZones()
{
    for(int y = 0; y < CMMK_ROWS_MAX; y++)
    {
        for(int x = 0; x < CMMK_COLS_MAX; x++)
        {
            if(!cmmk->PositionValid(y, x))
            {
                continue;
            }

            std::stringstream namestrm;

            led key;
            
            namestrm << "Key @ Row " << (y + 1) << ", Column" << (x + 1);

            key.name = namestrm.str();
            key.value = (y & 0xFF) << 8 | (x & 0xFF);

            leds.push_back(key);
        }
    }

    zone KeyboardZone;
    KeyboardZone.name               = "Keyboard";
    KeyboardZone.type               = ZONE_TYPE_MATRIX;
    KeyboardZone.leds_min           = leds.size();
    KeyboardZone.leds_max           = leds.size();
    KeyboardZone.leds_count         = leds.size();
    KeyboardZone.matrix_map         = new matrix_map_type;
    KeyboardZone.matrix_map->height = CMMK_ROWS_MAX;
    KeyboardZone.matrix_map->width  = CMMK_COLS_MAX;
    KeyboardZone.matrix_map->map    = (unsigned int *)&matrix_map;

    zones.push_back(KeyboardZone);

    SetupMatrixMap();
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

void copy_buffers(led* buf,  RGBColor* colbuf, size_t n, struct cmmk_color_matrix& mat, std::atomic<bool>& dirty)
{
    dirty.store(false);

    for(size_t i = 0; i < n; i++)
    {
        led const& selected_led = buf[i];

        int y = (selected_led.value & 0xFF00) >> 8;
        int x = selected_led.value & 0xFF;

        struct rgb col = map_to_cmmk_rgb(colbuf[i]);
        struct rgb ecol = mat.data[y][x];

        if(ecol.R != col.R || ecol.G != col.G || ecol.B != col.B)
        {
            dirty.store(true);

            mat.data[y][x] = col;
        }
    }
}

void RGBController_CMMKController::DeviceUpdateLEDs()
{
    copy_buffers(leds.data(), colors.data(), leds.size(), current_matrix, dirty);

    if(force_update.load() || dirty.load())
    {
        cmmk->SetAll(current_matrix);

        force_update.store(false);
    }
}

void RGBController_CMMKController::UpdateZoneLEDs(int zone_idx)
{
    zone& z = zones[zone_idx];

    copy_buffers(z.leds, z.colors, z.leds_count, current_matrix, dirty);

    if(force_update.load() || dirty.load())
    {
        cmmk->SetAll(current_matrix);

        force_update.store(false);
    }
}

void RGBController_CMMKController::UpdateSingleLED(int led_idx)
{
    led& selected_led = leds[led_idx];

    int y = (selected_led.value & 0xFF00) >> 8;
    int x = selected_led.value & 0xFF;

    current_matrix.data[y][x] = map_to_cmmk_rgb(colors[led_idx]);

    cmmk->SetSingle(y, x, map_to_cmmk_rgb(colors[led_idx]));
    dirty.store(false);
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
            cmmk->SetFirmwareControl();
            break;
        
        case CMMK_MODE_MANUAL:
            cmmk->SetManualControl();
            break;

        case CMMK_EFFECT_FULLY_LIT:
            {
                cmmk_effect_fully_lit fully_lit_effect;

                fully_lit_effect.color  = map_to_cmmk_rgb(modes[active_mode].colors[0]);

                cmmk->SetMode(fully_lit_effect);
            }
            break;

        case CMMK_EFFECT_BREATHE:
            {
                cmmk_effect_breathe breathe_effect;

                breathe_effect.speed    = (uint8_t)modes[active_mode].speed;
                breathe_effect.color    = map_to_cmmk_rgb(modes[active_mode].colors[0]);

                cmmk->SetMode(breathe_effect);
            }
            break;

        case CMMK_EFFECT_CYCLE:
            {
                cmmk_effect_cycle cycle_effect;

                cycle_effect.speed  = (uint8_t)modes[active_mode].speed;

                cmmk->SetMode(cycle_effect);
            }
            break;
        
        case CMMK_EFFECT_SINGLE:
            {
                cmmk_effect_single single_effect;

                single_effect.speed     = (uint8_t)modes[active_mode].speed;
                single_effect.active    = map_to_cmmk_rgb(modes[active_mode].colors[0]);
                single_effect.rest      = map_to_cmmk_rgb(modes[active_mode].colors[1]);

                cmmk->SetMode(single_effect);
            }
            break;
        
        case CMMK_EFFECT_WAVE:
            {
                cmmk_effect_wave wave_effect;

                wave_effect.speed       = (uint8_t)modes[active_mode].speed;
                wave_effect.direction   = map_to_cmmk_dir(modes[active_mode].direction);
                wave_effect.start       = map_to_cmmk_rgb(modes[active_mode].colors[0]);

                cmmk->SetMode(wave_effect);
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

                cmmk->SetMode(ripple_effect);
            }
            break;
        
        case CMMK_EFFECT_CROSS:
            {
                cmmk_effect_cross cross_effect;

                cross_effect.speed  = (uint8_t)modes[active_mode].speed;
                cross_effect.active = map_to_cmmk_rgb(modes[active_mode].colors[0]);
                cross_effect.rest   = map_to_cmmk_rgb(modes[active_mode].colors[1]);

                cmmk->SetMode(cross_effect);
            }
            break;
        
        case CMMK_EFFECT_RAINDROPS:
            {
                cmmk_effect_raindrops raindrops_effect;

                raindrops_effect.speed      = (uint8_t)modes[active_mode].speed;
                raindrops_effect.interval   = CMMK_SPEED_MID;
                raindrops_effect.active     = map_to_cmmk_rgb(modes[active_mode].colors[0]);
                raindrops_effect.rest       = map_to_cmmk_rgb(modes[active_mode].colors[1]);

                cmmk->SetMode(raindrops_effect);
            }
            break;

        case CMMK_EFFECT_STARS:
            {
                cmmk_effect_stars stars_effect;
                
                stars_effect.speed      = (uint8_t)modes[active_mode].speed;
                stars_effect.interval   = CMMK_SPEED_MID;
                stars_effect.active     = map_to_cmmk_rgb(modes[active_mode].colors[0]);
                stars_effect.rest       = map_to_cmmk_rgb(modes[active_mode].colors[1]);
    
                cmmk->SetMode(stars_effect);
            }
            break;

        case CMMK_EFFECT_SNAKE:
            {
                cmmk_effect_snake snake_effect;
                
                snake_effect.speed = (uint8_t)modes[active_mode].speed;

                cmmk->SetMode(snake_effect);
            }
            break;
    }
}
