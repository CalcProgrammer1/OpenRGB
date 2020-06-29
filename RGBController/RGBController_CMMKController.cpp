/*-------------------------------------------------------------------*\
|  RGBController_CMMKController.cpp                                   |
|                                                                     |
|  Driver for Coolermaster MasterKeys keyboards                       |
|                                                                     |
|  Lukas N (chmod222)          28th Jun 2020                          |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RGBController_CMMKController.h"

#include <sstream>
#include <cassert>

using namespace std::chrono_literals;

RGBController_CMMKController::RGBController_CMMKController(CMMKController* cmmk_ctrl)
{
    cmmk     = cmmk_ctrl;

    name        = cmmk->GetDeviceName();
    type        = DEVICE_TYPE_KEYBOARD;
    description = cmmk->GetDeviceName();
    version     = cmmk->GetFirmwareVersion();
    serial      = "";
    location    = cmmk->GetLocation();

    mode FirmwareControl;
    FirmwareControl.name = "Firmware Controlled";
    FirmwareControl.value = 0xff;
    FirmwareControl.flags = 0;
    modes.push_back(FirmwareControl);
    
    mode CustomKeys;
    CustomKeys.name = "Customized LEDs";
    CustomKeys.value = 0x7f;
    CustomKeys.flags = MODE_FLAG_HAS_PER_LED_COLOR;
    CustomKeys.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(CustomKeys);

    mode FullLit;
    FullLit.name       = "Single Color";
    FullLit.value      = CMMK_EFFECT_FULLY_LIT;
    FullLit.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    FullLit.color_mode = MODE_COLORS_MODE_SPECIFIC;
    FullLit.colors_min = 1;
    FullLit.colors_max = 1;
    FullLit.colors.resize(1);
    modes.push_back(FullLit);
    
    mode Breathe;
    Breathe.name       = "Breathe";
    Breathe.value      = CMMK_EFFECT_BREATHE;
    Breathe.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breathe.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathe.colors_min = 1;
    Breathe.colors_max = 1;
    Breathe.colors.resize(1);
    Breathe.speed_min = CMMK_SPEED0;
    Breathe.speed_max = CMMK_SPEED4;
    modes.push_back(Breathe);

    mode Cycle;
    Cycle.name       = "Cycle";
    Cycle.value      = CMMK_EFFECT_CYCLE;
    Cycle.flags      = MODE_FLAG_HAS_SPEED;
    Cycle.color_mode = MODE_COLORS_NONE;
    Cycle.speed_min = CMMK_SPEED0;
    Cycle.speed_max = CMMK_SPEED4;
    modes.push_back(Cycle);

    mode Single;
    Single.name       = "Single Key Fade-Out";
    Single.value      = CMMK_EFFECT_SINGLE;
    Single.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Single.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Single.colors_min = 2;
    Single.colors_max = 2;
    Single.colors.resize(2);
    Single.speed_min = CMMK_SPEED0;
    Single.speed_max = CMMK_SPEED4;
    modes.push_back(Single);

    mode Wave;
    Wave.name       = "Wave";
    Wave.value      = CMMK_EFFECT_WAVE;
    Wave.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    Wave.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Wave.colors_min = 1;
    Wave.colors_max = 1;
    Wave.colors.resize(1);
    Wave.speed_min = CMMK_SPEED0;
    Wave.speed_max = CMMK_SPEED4;
    modes.push_back(Wave);
    
    mode Ripple;
    Ripple.name       = "Ripple Effect";
    Ripple.value      = CMMK_EFFECT_RIPPLE;
    Ripple.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED;
    Ripple.colors_min = 2;
    Ripple.colors_max = 2;
    Ripple.colors.resize(2);
    Ripple.speed_min = CMMK_SPEED0;
    Ripple.speed_max = CMMK_SPEED4;
    modes.push_back(Ripple);
    
    mode Cross;
    Cross.name       = "Cross Effect";
    Cross.value      = CMMK_EFFECT_CROSS;
    Cross.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Cross.colors_min = 2;
    Cross.colors_max = 2;
    Cross.colors.resize(2);
    Cross.speed_min = CMMK_SPEED0;
    Cross.speed_max = CMMK_SPEED4;
    modes.push_back(Cross);

    mode Raindrops;
    Raindrops.name       = "Raindrops Effect";
    Raindrops.value      = CMMK_EFFECT_RAINDROPS;
    Raindrops.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Raindrops.colors_min = 2;
    Raindrops.colors_max = 2;
    Raindrops.colors.resize(2);
    Raindrops.speed_min = CMMK_SPEED0;
    Raindrops.speed_max = CMMK_SPEED4;
    modes.push_back(Raindrops);
    
    mode Stars;
    Stars.name       = "Starfield Effect";
    Stars.value      = CMMK_EFFECT_STARS;
    Stars.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Stars.colors_min = 2;
    Stars.colors_max = 2;
    Stars.colors.resize(2);
    Stars.speed_min = CMMK_SPEED0;
    Stars.speed_max = CMMK_SPEED4;
    modes.push_back(Stars);

    SetupZones();
}

RGBController_CMMKController::~RGBController_CMMKController()
{
    delete cmmk;
}

void RGBController_CMMKController::SetupMatrixMap()
{
    for (int y = 0; y < CMMK_ROWS_MAX; ++y) {
        for (int x = 0; x < CMMK_COLS_MAX; ++x) {
            matrix_map[y][x] = 0xFFFFFFFF;
        }
    }

    for (size_t i = 0; i < leds.size(); ++i) {
        led const& l = leds[i];

        int y = (l.value & 0xFF00) >> 8;
        int x = l.value & 0xFF;

        matrix_map[y][x] = i;
    }
}

void RGBController_CMMKController::SetupZones()
{
    for (int y = 0; y < CMMK_ROWS_MAX; ++y) {
        for (int x = 0; x < CMMK_COLS_MAX; ++x) {
            if (!cmmk->PositionValid(y, x)) {
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
    KeyboardZone.name          = "Keyboard";
    KeyboardZone.type          = ZONE_TYPE_MATRIX;
    KeyboardZone.leds_min      = leds.size();
    KeyboardZone.leds_max      = leds.size();
    KeyboardZone.leds_count    = leds.size();
    KeyboardZone.matrix_map    = new matrix_map_type;
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

namespace {
    struct rgb map_to_cmmk_rgb(RGBColor input) {
        return rgb {
            (uint8_t)RGBGetRValue(input),
            (uint8_t)RGBGetGValue(input),
            (uint8_t)RGBGetBValue(input)
        };
    }

    enum cmmk_wave_direction map_to_cmmk_dir(int input) {
        switch (input) {
            case MODE_DIRECTION_LEFT: return CMMK_RIGHT_TO_LEFT;
            case MODE_DIRECTION_RIGHT: return CMMK_LEFT_TO_RIGHT;
            case MODE_DIRECTION_UP: return CMMK_FRONT_TO_BACK;
            case MODE_DIRECTION_DOWN: return CMMK_BACK_TO_FRONT;
            default: assert(false && "unreachable");
        }
    }

    void copy_buffers(led* buf,  RGBColor* colbuf, size_t n, struct cmmk_color_matrix& mat, std::atomic<bool>& dirty) {
        dirty.store(false);

        for (size_t i = 0; i < n; ++i) {
            led const& selected_led = buf[i];

            int y = (selected_led.value & 0xFF00) >> 8;
            int x = selected_led.value & 0xFF;

            struct rgb col = map_to_cmmk_rgb(colbuf[i]);
            struct rgb ecol = mat.data[y][x];

            if (ecol.R != col.R || ecol.G != col.G || ecol.B != col.B) {
                dirty.store(true);

                mat.data[y][x] = col;
            }
        }
    }
}

void RGBController_CMMKController::DeviceUpdateLEDs()
{
    copy_buffers(leds.data(), colors.data(), leds.size(), current_matrix, dirty);

    if (force_update.load() || dirty.load()) {
        cmmk->SetAll(current_matrix);

        force_update.store(false);
    }
}

void RGBController_CMMKController::UpdateZoneLEDs(int _zone)
{
    zone const& z = zones[_zone];

    copy_buffers(z.leds, z.colors, z.leds_count, current_matrix, dirty);

    if (force_update.load() || dirty.load()) {
        cmmk->SetAll(current_matrix);

        force_update.store(false);
    }
}

void RGBController_CMMKController::UpdateSingleLED(int _led)
{
    led const& selected_led = leds[_led];

    int y = (selected_led.value & 0xFF00) >> 8;
    int x = selected_led.value & 0xFF;

    current_matrix.data[y][x] = map_to_cmmk_rgb(colors[_led]);

    cmmk->SetSingle(y, x, map_to_cmmk_rgb(colors[_led]));
    dirty.store(false);
}

void RGBController_CMMKController::SetCustomMode()
{
    force_update.store(true);

    active_mode = 1;
}

void RGBController_CMMKController::UpdateMode()
{
    force_update.store(true);

    switch(modes[active_mode].value)
    {
        case 0xff:
            cmmk->SetFirmwareControl();
            break;
        
        case 0x7f:
            cmmk->SetManualControl();
            break;

        case CMMK_EFFECT_FULLY_LIT:
            cmmk->SetMode(cmmk_effect_fully_lit {
                map_to_cmmk_rgb(modes[active_mode].colors[0])
            });

            break;

        case CMMK_EFFECT_BREATHE:
            cmmk->SetMode(cmmk_effect_breathe {
                (uint8_t)modes[active_mode].speed,
                map_to_cmmk_rgb(modes[active_mode].colors[0])
            });

            break;

        case CMMK_EFFECT_CYCLE:
            cmmk->SetMode(cmmk_effect_cycle {
                (uint8_t)modes[active_mode].speed
            });

            break;
        
        case CMMK_EFFECT_SINGLE:
            cmmk->SetMode(cmmk_effect_single {
                (uint8_t)modes[active_mode].speed,

                map_to_cmmk_rgb(modes[active_mode].colors[0]),
                map_to_cmmk_rgb(modes[active_mode].colors[1])
            });

            break;
        
        case CMMK_EFFECT_WAVE:
            cmmk->SetMode(cmmk_effect_wave {
                (uint8_t)modes[active_mode].speed,
                map_to_cmmk_dir(modes[active_mode].direction),
                map_to_cmmk_rgb(modes[active_mode].colors[0])
            });

            break;
        
        case CMMK_EFFECT_RIPPLE:
            cmmk->SetMode(cmmk_effect_ripple {
                (uint8_t)modes[active_mode].speed,
                modes[active_mode].color_mode == MODE_COLORS_RANDOM
                    ? CMMK_RIPPLE_RANDOM_COLOR
                    : CMMK_RIPPLE_GIVEN_COLOR,
                map_to_cmmk_rgb(modes[active_mode].colors[0]),
                map_to_cmmk_rgb(modes[active_mode].colors[1])
            });

            break;
        
        case CMMK_EFFECT_CROSS:
            cmmk->SetMode(cmmk_effect_cross {
                (uint8_t)modes[active_mode].speed,
                map_to_cmmk_rgb(modes[active_mode].colors[0]),
                map_to_cmmk_rgb(modes[active_mode].colors[1])
            });

            break;
        
        case CMMK_EFFECT_RAINDROPS:
            cmmk->SetMode(cmmk_effect_raindrops {
                (uint8_t)modes[active_mode].speed,
                CMMK_SPEED2,
                map_to_cmmk_rgb(modes[active_mode].colors[0]),
                map_to_cmmk_rgb(modes[active_mode].colors[1])
            });

            break;

        case CMMK_EFFECT_STARS:
            cmmk->SetMode(cmmk_effect_stars {
                (uint8_t)modes[active_mode].speed,
                CMMK_SPEED2,
                map_to_cmmk_rgb(modes[active_mode].colors[0]),
                map_to_cmmk_rgb(modes[active_mode].colors[1])
            });

            break;
    }
}
