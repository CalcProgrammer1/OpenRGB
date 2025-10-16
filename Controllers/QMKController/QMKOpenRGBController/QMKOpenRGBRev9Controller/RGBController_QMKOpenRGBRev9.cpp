/*---------------------------------------------------------*\
| RGBController_QMKOpenRGBRev9.cpp                          |
|                                                           |
|   RGBController for OpenRGB QMK Keyboard Protocol         |
|   Revision 9                                              |
|                                                           |
|   Kasper                                      10 Oct 2020 |
|   Jath03                                      28 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "hsv.h"
#include "LogManager.h"
#include "RGBController_QMKOpenRGBRev9.h"

/**------------------------------------------------------------------*\
    @name Quantum Mechanical Keyboard (QMK)
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors
    @comment Please see [the github page](https://github.com/qmk/qmk_firmware#supported-keyboards) for the up to date list of
        keyboards supported by the QMK controller.
\*-------------------------------------------------------------------*/

RGBController_QMKOpenRGBRev9::RGBController_QMKOpenRGBRev9(QMKOpenRGBRev9Controller* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = controller->GetDeviceVendor();
    description = "QMK OpenRGB Device (Protocol Version " + std::to_string(controller->GetProtocolVersion()) + ")";
    type        = DEVICE_TYPE_KEYBOARD;
    location    = controller->GetLocation();
    version     = controller->GetQMKVersion();

    unsigned int current_mode = 1;

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_OPENRGB_DIRECT))
    {
        InitializeMode("Direct", current_mode, MODE_FLAG_HAS_PER_LED_COLOR, MODE_COLORS_PER_LED);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_SOLID_COLOR))
    {
        InitializeMode("Static", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_ALPHA_MOD))
    {
        InitializeMode("Alpha Mod", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_GRADIENT_UP_DOWN))
    {
        InitializeMode("Gradient Up Down", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_GRADIENT_LEFT_RIGHT))
    {
        InitializeMode("Gradient Left Right", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_BREATHING))
    {
        InitializeMode("Breathing", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_BAND_SAT))
    {
        InitializeMode("Band Saturation", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_BAND_VAL))
    {
        InitializeMode("Band Value", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_BAND_PINWHEEL_SAT))
    {
        InitializeMode("Band Pinwheel Saturation", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_BAND_PINWHEEL_VAL))
    {
        InitializeMode("Band Pinwheel Value", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_BAND_SPIRAL_SAT))
    {
        InitializeMode("Band Spiral Saturation", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_BAND_SPIRAL_VAL))
    {
        InitializeMode("Band Spiral Value", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_CYCLE_ALL))
    {
        InitializeMode("Cycle All", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_CYCLE_LEFT_RIGHT))
    {
        InitializeMode("Cycle Left Right", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_CYCLE_UP_DOWN))
    {
        InitializeMode("Cycle Up Down", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_CYCLE_OUT_IN))
    {
        InitializeMode("Cycle Out In", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_CYCLE_OUT_IN_DUAL))
    {
        InitializeMode("Cycle Out In Dual", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_RAINBOW_MOVING_CHEVRON))
    {
        InitializeMode("Rainbow Moving Chevron", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_CYCLE_PINWHEEL))
    {
        InitializeMode("Cycle Pinwheel", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_CYCLE_SPIRAL))
    {
        InitializeMode("Cycle Spiral", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_DUAL_BEACON))
    {
        InitializeMode("Dual Beacon", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_RAINBOW_BEACON))
    {
        InitializeMode("Rainbow Beacon", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_RAINBOW_PINWHEELS))
    {
        InitializeMode("Rainbow Pinwheels", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_RAINDROPS))
    {
        InitializeMode("Raindrops", current_mode, 0, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_JELLYBEAN_RAINDROPS))
    {
        InitializeMode("Jellybean Raindrops", current_mode, 0, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_HUE_BREATHING))
    {
        InitializeMode("Hue Breathing", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_HUE_PENDULUM))
    {
        InitializeMode("Hue Pendulum", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_HUE_WAVE))
    {
        InitializeMode("Hue Wave", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_TYPING_HEATMAP))
    {
        InitializeMode("Typing Heatmap", current_mode, 0, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_DIGITAL_RAIN))
    {
        InitializeMode("Digital Rain", current_mode, 0, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_SOLID_REACTIVE_SIMPLE))
    {
        InitializeMode("Solid Reactive Simple", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_SOLID_REACTIVE))
    {
        InitializeMode("Solid Reactive", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_SOLID_REACTIVE_WIDE))
    {
        InitializeMode("Solid Reactive Wide", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_SOLID_REACTIVE_MULTIWIDE))
    {
        InitializeMode("Solid Reactive Multi Wide", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_SOLID_REACTIVE_CROSS))
    {
        InitializeMode("Solid Reactive Cross", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_SOLID_REACTIVE_MULTICROSS))
    {
        InitializeMode("Solid Reactive Multi Cross", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_SOLID_REACTIVE_NEXUS))
    {
        InitializeMode("Solid Reactive Nexus", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_SOLID_REACTIVE_MULTINEXUS))
    {
        InitializeMode("Solid Reactive Multi Nexus", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_SPLASH))
    {
        InitializeMode("Rainbow Reactive Splash", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_MULTISPLASH))
    {
        InitializeMode("Rainbow Reactive Multi Splash", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_SOLID_SPLASH))
    {
        InitializeMode("Solid Reactive Splash", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    if(controller->GetIsModeEnabled(QMK_OPENRGB_MODE_SOLID_MULTISPLASH))
    {
        InitializeMode("Solid Reactive Multi Splash", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED, MODE_COLORS_MODE_SPECIFIC);
    }

    active_mode = controller->GetMode() - 1;

    SetupZones();
}

RGBController_QMKOpenRGBRev9::~RGBController_QMKOpenRGBRev9()
{
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }
}

void RGBController_QMKOpenRGBRev9::SetupZones()
{
    /*---------------------------------------------------------*\
    | Get the number of LEDs from the device                    |
    \*---------------------------------------------------------*/
    const unsigned int total_number_of_leds                     = controller->GetTotalNumberOfLEDs();
    const unsigned int total_number_of_leds_with_empty_space    = controller->GetTotalNumberOfLEDsWithEmptySpace();

    LOG_INFO("[%s] Keyboard has %u LEDs total", name.c_str(), total_number_of_leds);

    /*---------------------------------------------------------*\
    | Get information for each LED                              |
    \*---------------------------------------------------------*/
    for(unsigned int i = 0; i < std::max(total_number_of_leds, total_number_of_leds_with_empty_space); i++)
    {
        controller->GetLEDInfo(i);
    }

    /*---------------------------------------------------------*\
    | Get LED vectors from controller                           |
    \*---------------------------------------------------------*/
    std::vector<point_t>        led_points                      = controller->GetLEDPoints();
    std::vector<unsigned int>   led_flags                       = controller->GetLEDFlags();
    std::vector<std::string>    led_names                       = controller->GetLEDNames();

    /*---------------------------------------------------------*\
    | Count key LEDs and underglow LEDs                         |
    \*---------------------------------------------------------*/
    unsigned int number_of_key_leds;
    unsigned int number_of_underglow_leds;

    CountKeyTypes(led_flags, total_number_of_leds, number_of_key_leds, number_of_underglow_leds);

    /*---------------------------------------------------------*\
    | Add LED names for underglow zone                          |
    \*---------------------------------------------------------*/
    unsigned int    number_of_leds                              = number_of_key_leds + number_of_underglow_leds;
    bool            has_underglow                               = number_of_underglow_leds > 0;
    LOG_INFO("[%s] Keyboard has %u underglow LEDs", name.c_str(), number_of_underglow_leds);

    for(unsigned int i = 0; i < number_of_underglow_leds; i++)
    {
        led_names.push_back("Underglow: " + std::to_string(number_of_key_leds + i));
    }

    /*---------------------------------------------------------*\
    | Create sets for row and column position values            |
    \*---------------------------------------------------------*/
    std::set<int> rows, columns;
    for (unsigned int i = 0; i < number_of_leds; i++)
    {
        rows.insert(led_points[i].y);
        columns.insert(led_points[i].x);
    }

    /*---------------------------------------------------------*\
    | Calculate matrix map from QMK positions                   |
    \*---------------------------------------------------------*/
    unsigned int divisor = CalculateDivisor(led_points, rows, columns);
    LOG_DEBUG("[%s] Distance between standard keys calculated to be %u", name.c_str(), divisor);

    VectorMatrix matrix_map;
    VectorMatrix underglow_map;

    PlaceLEDsInMaps(rows, columns, divisor, led_points, led_flags, matrix_map, underglow_map);
    CleanMatrixMaps(matrix_map, underglow_map, (unsigned int)rows.size(), has_underglow);

    /*---------------------------------------------------------*\
    | These vectors are class members because if they go out of |
    | scope, the underlying array (used by each zones'          |
    | matrix_map) is unallocated.                               |
    \*---------------------------------------------------------*/
    flat_matrix_map     = FlattenMatrixMap(matrix_map);
    flat_underglow_map  = FlattenMatrixMap(underglow_map);

    /*---------------------------------------------------------*\
    | Create Keyboard zone                                      |
    \*---------------------------------------------------------*/
    zone keys_zone;
    keys_zone.name                          = "Keyboard";
    keys_zone.type                          = ZONE_TYPE_MATRIX;
    keys_zone.leds_min                      = number_of_key_leds;
    keys_zone.leds_max                      = keys_zone.leds_min;
    keys_zone.leds_count                    = keys_zone.leds_min;
    keys_zone.matrix_map                    = new matrix_map_type;
    keys_zone.matrix_map->width             = (unsigned int)matrix_map[0].size();
    keys_zone.matrix_map->height            = (unsigned int)matrix_map.size();
    keys_zone.matrix_map->map               = flat_matrix_map.data();
    zones.push_back(keys_zone);

    /*---------------------------------------------------------*\
    | Create Underglow zone if it exists                        |
    \*---------------------------------------------------------*/
    if(has_underglow)
    {
        zone underglow_zone;
        underglow_zone.name                 = "Underglow";
        underglow_zone.type                 = ZONE_TYPE_MATRIX;
        underglow_zone.leds_min             = number_of_underglow_leds;
        underglow_zone.leds_max             = underglow_zone.leds_min;
        underglow_zone.leds_count           = underglow_zone.leds_min;
        underglow_zone.matrix_map           = new matrix_map_type;
        underglow_zone.matrix_map->width    = (unsigned int)underglow_map[0].size();
        underglow_zone.matrix_map->height   = (unsigned int)underglow_map.size();
        underglow_zone.matrix_map->map      = flat_underglow_map.data();
        zones.push_back(underglow_zone);
    }

    /*---------------------------------------------------------*\
    | Create LEDs                                               |
    \*---------------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < number_of_leds; led_idx++)
    {
        led keyboard_led;

        if(led_idx < led_names.size())
        {
            keyboard_led.name               = led_names[led_idx];
        }
        keyboard_led.value                  = led_idx;

        leds.push_back(keyboard_led);
    }

    /*---------------------------------------------------------*\
    | Setup Colors                                              |
    \*---------------------------------------------------------*/
    SetupColors();

    /*---------------------------------------------------------*\
    | Initialize colors from device values                      |
    \*---------------------------------------------------------*/
    for(unsigned int i = 0; i < leds.size(); i++)
    {
        colors[i] = controller->GetLEDColors()[i];
    }
}

void RGBController_QMKOpenRGBRev9::DeviceUpdateLEDs()
{
    controller->DirectModeSetLEDs(colors, controller->GetTotalNumberOfLEDs());
}

void RGBController_QMKOpenRGBRev9::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_QMKOpenRGBRev9::DeviceUpdateSingleLED(int led)
{
    RGBColor      color = colors[led];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    controller->DirectModeSetSingleLED(led, red, grn, blu);
}

void RGBController_QMKOpenRGBRev9::DeviceUpdateMode()
{
    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        controller->SetMode({ 0, 255, 255 }, modes[active_mode].value, 127);
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_NONE)
    {
        controller->SetMode({ 0, 255, 255 }, modes[active_mode].value, modes[active_mode].speed);
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        RGBColor rgb_color = modes[active_mode].colors[0];
        hsv_t hsv_color;
        rgb2hsv(rgb_color, &hsv_color);

        if(modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
        {
            controller->SetMode(hsv_color, modes[active_mode].value, modes[active_mode].speed);
        }
        else
        {
            controller->SetMode(hsv_color, modes[active_mode].value, 127);
        }
    }
}

void RGBController_QMKOpenRGBRev9::InitializeMode
    (
    std::string name,
    unsigned int &current_mode,
    unsigned int flags,
    unsigned int color_mode
    )
{
    mode qmk_mode;
    qmk_mode.name       = name;
    qmk_mode.value      = current_mode++;
    qmk_mode.flags      = flags;
    qmk_mode.color_mode =  color_mode;

    if(flags & MODE_FLAG_HAS_SPEED)
    {
        qmk_mode.speed_min     = QMK_OPENRGB_SPEED_SLOWEST;
        qmk_mode.speed_max     = QMK_OPENRGB_SPEED_FASTEST;
        qmk_mode.speed         = QMK_OPENRGB_SPEED_NORMAL;
    }
    if(flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
    {
        qmk_mode.colors_min = 1;
        qmk_mode.colors_max = 1;
        qmk_mode.colors.resize(1);
        qmk_mode.colors[0] = controller->GetModeColor();
    }

    modes.push_back(qmk_mode);
}

unsigned int RGBController_QMKOpenRGBRev9::CalculateDivisor
    (
    std::vector<point_t> led_points,
    std::set<int> rows,
    std::set<int> /*columns*/
    )
{
    std::vector< std::vector<point_t> > row_points(rows.size());
    for(const point_t &pt : led_points)
    {
        for(const int &i : rows)
        {
            if(pt.y == i)
            {
                row_points[std::distance(rows.begin(), rows.find(i))].push_back(pt);
            }
        }
    }

    int last_pos;
    std::vector<int> distances;
    for(const std::vector<point_t> &row : row_points)
    {
        last_pos = 0;
        std::for_each(row.begin(), row.end(), [&distances, &last_pos](const point_t &pt)
        {
            distances.push_back(std::abs(pt.x - last_pos));
            last_pos = pt.x;
        });
    }
    std::map<int, int> counts;
    for(const int &i : distances)
    {
        counts[i]++;
    }

    unsigned int divisor = distances[0];
    for(const std::pair<const int, int> &i : counts)
    {
        if(counts[divisor] < i.second)
        {
            divisor = i.first;
        }
    }
    return divisor;
}

void RGBController_QMKOpenRGBRev9::CountKeyTypes
    (
        std::vector<unsigned int>   led_flags,
        unsigned int                total_led_count,
        unsigned int&               key_leds,
        unsigned int&               underglow_leds
    )
{
    underglow_leds  = 0;
    key_leds        = 0;

    for(unsigned int i = 0; i < total_led_count; i++)
    {
        if(led_flags[i] & 2)
        {
            underglow_leds++;
        }
        else if(led_flags[i] != 0)
        {
            key_leds++;
        }
    }
}

void RGBController_QMKOpenRGBRev9::PlaceLEDsInMaps
    (
        std::set<int>               unique_rows,
        std::set<int>               /*unique_cols*/,
        unsigned int                divisor,
        std::vector<point_t>        led_points,
        std::vector<unsigned int>   led_flags,
        VectorMatrix&               matrix_map_xl,
        VectorMatrix&               underglow_map_xl
     )
{
    matrix_map_xl                   = MakeEmptyMatrixMap(unique_rows.size(), (std::size_t)(std::round(255 / divisor) + 10));
    underglow_map_xl                = MakeEmptyMatrixMap(unique_rows.size(), (std::size_t)(std::round(255 / divisor) + 10));

    unsigned int x                  = 0;
    unsigned int y                  = 0;
    unsigned int underglow_counter  = 0;

    for(unsigned int i = 0; i < controller->GetTotalNumberOfLEDs(); i++)
    {
        if(led_points[i].x != 255 && led_points[i].y != 255)
        {
            bool underglow          = led_flags[i] & 2;

            x                       = (unsigned int)(std::round(led_points[i].x / divisor));
            y                       = (unsigned int)(std::distance(unique_rows.begin(), unique_rows.find(led_points[i].y)));

            if(!underglow)
            {
                while(matrix_map_xl[y][x] != NO_LED)
                {
                    x++;
                }
                matrix_map_xl[y][x] = i;
                LOG_DEBUG("[%s] Key Matrix LED %u, (%u, %u) being placed into (%u, %u)", name.c_str(), i, led_points[i].x, led_points[i].y, x, y);
            }
            else
            {
                while(underglow_map_xl[y][x] != NO_LED)
                {
                    x++;
                }
                underglow_map_xl[y][x] = underglow_counter;
                underglow_counter++;
                LOG_DEBUG("[%s] Underglow  LED %u, (%u, %u) being placed into (%u, %u)", name.c_str(), i, led_points[i].x, led_points[i].y, x, y);
            }
        }
    }
}

VectorMatrix RGBController_QMKOpenRGBRev9::MakeEmptyMatrixMap
    (
        std::size_t height,
        std::size_t width
    )
{
    std::vector<std::vector<unsigned int> > matrix_map(height);
    for(std::size_t i = 0; i < height; i++)
    {
        for(std::size_t j = 0; j < width; j++)
        {
            matrix_map[i].push_back(NO_LED);
        }
    }
    return matrix_map;
}

void RGBController_QMKOpenRGBRev9::CleanMatrixMaps
    (
        VectorMatrix&   matrix_map,
        VectorMatrix&   underglow_map,
        unsigned int    height,
        bool            has_underglow
    )
{
    bool empty_col              = true;
    bool empty_col_udg          = true;
    bool empty_row              = true;
    int width                   = 0;
    int width_udg               = 0;

    std::vector<int> empty_rows;

    bool can_break;
    bool can_break_udg;

    for(unsigned int i = 0; i < height; i++)
    {
        empty_row               = true;
        can_break               = false;
        can_break_udg           = false;

        for(int j = (int)matrix_map[i].size() - 1; j --> 0; )
        {
            if(matrix_map[i][j] != NO_LED && width < (j + 1) && !can_break)
            {
                width           = (j + 1);
                can_break       = true;
                empty_row       = false;
            }
            else if(matrix_map[i][j] != NO_LED)
            {
                empty_row       = false;
            }
            if(underglow_map[i][j] != NO_LED && width_udg < (j + 1) && !can_break_udg)
            {
                width_udg       = (j + 1);
                can_break_udg   = true;
            }
            if (can_break && can_break_udg) break;
        }

        if(matrix_map[i][0] != NO_LED)
        {
            empty_col           = false;
        }

        if(underglow_map[i][0] != NO_LED)
        {
            empty_col_udg       = false;
        }

        if(empty_row)
        {
            empty_rows.push_back(i);
        }
    }

    unsigned int new_height     = height - (unsigned int)empty_rows.size();
    width                       = empty_col ? width - 1 : width;
    width_udg                   = empty_col_udg && empty_col ? width_udg - 1 : width_udg;
    LOG_DEBUG("[%s] Key LED Matrix: %ux%u", name.c_str(), width, new_height);
    LOG_DEBUG("[%s] Underglow LED Matrix: %ux%u", name.c_str(), width_udg, new_height);

    for(unsigned int i = (unsigned int)empty_rows.size(); i --> 0; )
    {
        matrix_map.erase(matrix_map.begin() + empty_rows[i]);
    }

    for(unsigned int i = 0; i < new_height; i++)
    {
        if(empty_col)
        {
            matrix_map[i].erase(matrix_map[i].begin(), matrix_map[i].begin() + 1);
        }

        if(empty_col_udg && empty_col)
        {
            underglow_map[i].erase(underglow_map[i].begin(), underglow_map[i].begin() + 1);
        }

        matrix_map[i].erase(matrix_map[i].begin()+width, matrix_map[i].end());

        if(has_underglow)
        {
            underglow_map[i].erase(underglow_map[i].begin()+width_udg, underglow_map[i].end());
        }
    }

    if(has_underglow)
    {
        for(unsigned int i = new_height; i < height; i++)
        {
            underglow_map[i].erase(underglow_map[i].begin()+width_udg, underglow_map[i].end());
        }
    }
}

std::vector<unsigned int> RGBController_QMKOpenRGBRev9::FlattenMatrixMap
    (
        VectorMatrix matrix_map
    )
{
    std::vector<unsigned int> flat_map;

    for(const std::vector<unsigned int> &row : matrix_map)
    {
        for(const unsigned int &item : row)
        {
            flat_map.push_back(item);
        }
    }
    return flat_map;
}
