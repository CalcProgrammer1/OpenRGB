/*-------------------------------------------------------------------*\
|  RGBController_QMKRGBMatrix.cpp                                     |
|                                                                     |
|  Driver for QMK keybaords using RGB Matrix                          |
|                                                                     |
|  Kasper24         11th November 2020                                |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "hsv.h"
#include "RGBController_QMKRGBMatrix.h"

RGBController_QMKRGBMatrix::RGBController_QMKRGBMatrix(QMKRGBMatrixController* qmk_rgb_matrix_ptr, unsigned int protocol_version)
{
    qmk_rgb_matrix = qmk_rgb_matrix_ptr;

    name = qmk_rgb_matrix_ptr->GetDeviceName();
    vendor = qmk_rgb_matrix_ptr->GetDeviceVendor();
    description = "QMK RGB Matrix Device";
    type = DEVICE_TYPE_KEYBOARD;
    location = qmk_rgb_matrix_ptr->GetLocation();
    version = std::to_string(protocol_version);

    unsigned int current_mode = 1;
    unsigned int hsv = 0;
    //unsigned int hsv = qmk_rgb_matrix_ptr->QMKModeGetColor();

    //if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_OPENRGB_DIRECT))
        InitializeMode("Direct", current_mode, MODE_FLAG_HAS_PER_LED_COLOR, MODE_COLORS_PER_LED, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_SOLID_COLOR))
//        InitializeMode("Static", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR, MODE_COLORS_MODE_SPECIFIC, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_ALPHA_MOD))
//        InitializeMode("Alpha Mod", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_GRADIENT_UP_DOWN))
//        InitializeMode("Gradient Up Down", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_GRADIENT_LEFT_RIGHT))
//        InitializeMode("Gradient Left Right", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_BREATHING))
//        InitializeMode("Breathing", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_BAND_SAT))
//        InitializeMode("Band Saturation", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_BAND_VAL))
//        InitializeMode("Band Value", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_BAND_PINWHEEL_SAT))
//        InitializeMode("Band Pinwheel Saturation", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);
    
//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_BAND_PINWHEEL_VAL))
//        InitializeMode("Band Pinwheel Value", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_BAND_SPIRAL_SAT))
//        InitializeMode("Band Spiral Saturation", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_BAND_SPIRAL_VAL))
//        InitializeMode("Band Spiral Value", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);
    
//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_CYCLE_ALL))
//        InitializeMode("Cycle All", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_CYCLE_LEFT_RIGHT))
//        InitializeMode("Cycle Left Right", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_CYCLE_UP_DOWN))
//        InitializeMode("Cycle Up Down", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_CYCLE_OUT_IN))
//        InitializeMode("Cycle Out In", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE, hsv);
    
//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_CYCLE_OUT_IN_DUAL))
//        InitializeMode("Cycle Out In Dual", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_RAINBOW_MOVING_CHEVRON))
//        InitializeMode("Rainbow Moving Chevron", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_CYCLE_PINWHEEL))
//        InitializeMode("Cycle Pinwheel", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_CYCLE_SPIRAL))
//        InitializeMode("Cycle Spiral", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_DUAL_BEACON))
//        InitializeMode("Dual Beacon", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_RAINBOW_BEACON))
//        InitializeMode("Rainbow Beacon", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_RAINBOW_PINWHEELS))
//        InitializeMode("Rainbow Pinwheels", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_RAINDROPS))
//        InitializeMode("Raindrops", current_mode, 0, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_JELLYBEAN_RAINDROPS))
//        InitializeMode("Jellybean Raindrops", current_mode, 0, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_TYPING_HEATMAP))
//        InitializeMode("Typing Heatmap", current_mode, 0, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_DIGITAL_RAIN))
//        InitializeMode("Digital Rain", current_mode, 0, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_SOLID_REACTIVE_SIMPLE))
//        InitializeMode("Solid Reactive Simple", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_SOLID_REACTIVE))
//        InitializeMode("Solid Reactive", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_SOLID_REACTIVE_WIDE))
//        InitializeMode("Solid Reactive Wide", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTIWIDE))
//        InitializeMode("Solid Reactive Multi Wide", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_SOLID_REACTIVE_CROSS))
//        InitializeMode("Solid Reactive Cross", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTICROSS))
//        InitializeMode("Solid Reactive Multi Cross", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_SOLID_REACTIVE_NEXUS))
//        InitializeMode("Solid Reactive Nexus", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTINEXUS))
//        InitializeMode("Solid Reactive Multi Nexus", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_SPLASH))
//        InitializeMode("Rainbow Reactive Splash", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_MULTISPLASH))
//        InitializeMode("Rainbow Reactive Multi Splash", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE, hsv);

//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_SOLID_SPLASH))
//        InitializeMode("Solid Reactive Splash", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);
    
//    if (qmk_rgb_matrix_ptr->GetEnabledModes(QMK_RGBMATRIX_MODE_SOLID_MULTISPLASH))
//        InitializeMode("Solid Reactive Multi Splash", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
//            MODE_COLORS_MODE_SPECIFIC, hsv);

    active_mode = qmk_rgb_matrix_ptr->GetActiveMode() - 1;

    SetupZones();
}

RGBController_QMKRGBMatrix::~RGBController_QMKRGBMatrix()
{
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }
}

void RGBController_QMKRGBMatrix::SetupZones()
{
    for(unsigned int i = 0; i < qmk_rgb_matrix->GetZonesCount(); i++)
    {
        zone keyboard_zone;
        keyboard_zone.name = qmk_rgb_matrix->GetZoneName(i);
        keyboard_zone.type = qmk_rgb_matrix->GetZoneType(i);
        keyboard_zone.leds_min = qmk_rgb_matrix->GetZoneSize(i);
        keyboard_zone.leds_max = keyboard_zone.leds_min;
        keyboard_zone.leds_count = keyboard_zone.leds_min;

        std::vector<std::string> led_names;
        if(keyboard_zone.type == ZONE_TYPE_MATRIX)
        {
            unsigned int led_matrix_columns = qmk_rgb_matrix->GetLEDMatirxColumns();
            unsigned int led_matrix_rows = qmk_rgb_matrix->GetLEDMatirxRows();

            keyboard_zone.matrix_map = new matrix_map_type;
            keyboard_zone.matrix_map->height = led_matrix_rows;
            keyboard_zone.matrix_map->width = led_matrix_columns;

            unsigned int* matrix_map = new unsigned int[led_matrix_rows * led_matrix_columns];
            for(unsigned int x = 0; x < led_matrix_rows; x++)
            {
                for(unsigned int y = 0; y < led_matrix_columns; y++)
                {
                    unsigned int led_value = qmk_rgb_matrix->GetLEDValueInMatrix(y, x);
                    matrix_map[led_matrix_columns * x + y] = led_value;

                    if(led_value != 255)
                    {
                        std::string led_name = qmk_rgb_matrix->GetLEDName(y, x);
                        led_names.push_back(led_name);
                    }
                }
            }

            keyboard_zone.matrix_map->map = matrix_map;
        }
        else 
        {
            keyboard_zone.matrix_map = NULL;
        }

        zones.push_back(keyboard_zone);

        for(unsigned int led_idx = 0; led_idx < keyboard_zone.leds_count; led_idx++)
        {
            led keyboard_led;

            if(leds.size() < zones[0].leds_count) 
            {
                if(led_idx < led_names.size()) keyboard_led.name = "Key: " + led_names[led_idx];
            }
            else 
            {
                keyboard_led.name = zones[i].name + ": " + std::to_string(led_idx);
            }

            leds.push_back(keyboard_led);
        }
    }

    SetupColors();

    for(unsigned int i = 0; i < leds.size(); i++)
    {
        colors[i] = qmk_rgb_matrix->DirectModeGetLEDColor(i);
    }
}

void RGBController_QMKRGBMatrix::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_QMKRGBMatrix::DeviceUpdateLEDs()
{
    qmk_rgb_matrix->DirectModeSetLEDs(colors, leds.size());
}

void RGBController_QMKRGBMatrix::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_QMKRGBMatrix::UpdateSingleLED(int led)
{
    RGBColor      color = colors[led];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);
    qmk_rgb_matrix->DirectModeSetSingleLED(led, red, grn, blu);
}

void RGBController_QMKRGBMatrix::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_QMKRGBMatrix::DeviceUpdateMode()
{
    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        DeviceUpdateLEDs();
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_NONE)
    {
        qmk_rgb_matrix->QMKModeSetModeAndSpeed(modes[active_mode].value, modes[active_mode].speed);
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        RGBColor color = modes[active_mode].colors[0];
        hsv_t hsv_color;
        rgb2hsv(color, &hsv_color);

        if(modes[active_mode].flags & MODE_FLAG_HAS_SPEED)    
            qmk_rgb_matrix->QMKModeSetColorModeAndSpeed(hsv_color, modes[active_mode].value, modes[active_mode].speed);
        else
            qmk_rgb_matrix->QMKModeSetColorModeAndSpeed(hsv_color, modes[active_mode].value, 127);
    }
}

void RGBController_QMKRGBMatrix::InitializeMode
    (
    std::string name, 
    unsigned int &current_mode, 
    unsigned int flags,
    unsigned int color_mode,
    unsigned int hsv
    )
{
    mode qmk_mode;
    qmk_mode.name       = name;
    qmk_mode.value      = current_mode++;
    qmk_mode.flags      = flags;
    qmk_mode.color_mode =  color_mode;

    if(flags & MODE_FLAG_HAS_SPEED)
    {    
        qmk_mode.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        qmk_mode.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        qmk_mode.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    }
    if(flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
    {    
        qmk_mode.colors_min = 1;
        qmk_mode.colors_max = 1;
        qmk_mode.colors.resize(1);
        qmk_mode.colors[0] = hsv;
    }

    modes.push_back(qmk_mode);   
}
