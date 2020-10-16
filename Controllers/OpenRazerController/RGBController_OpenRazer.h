/*-----------------------------------------*\
|  RGBController_OpenRazer.h                |
|                                           |
|  Generic RGB Interface for OpenRazer      |
|  kernel drivers for Chroma peripherals    |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/15/2019   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"

#include <fstream>

class RGBController_OpenRazer : public RGBController
{
public:
    enum
    {
        RAZER_MODE_CUSTOM,
        RAZER_MODE_OFF,
        RAZER_MODE_STATIC,
        RAZER_MODE_BREATHING,
        RAZER_MODE_SPECTRUM_CYCLE,
        RAZER_MODE_WAVE,
        RAZER_MODE_REACTIVE,
        RAZER_MODE_FLASHING,
        RAZER_NUM_MODES
    };

    enum
    {
        RAZER_TYPE_MATRIX_FRAME,
        RAZER_TYPE_MATRIX_NOFRAME,
        RAZER_TYPE_MATRIX_STATIC,
        RAZER_TYPE_NOMATRIX,
        RAZER_NUM_TYPES
    };

public:
    RGBController_OpenRazer(std::string dev_path);
    ~RGBController_OpenRazer();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

    int device_index;

private:
    void SetupMatrixDevice(unsigned int rows, unsigned int cols);
    void SetupNonMatrixDevice();

    unsigned int matrix_type;
    unsigned int matrix_rows;
    unsigned int matrix_cols;

    void OpenFunctions(std::string dev_path);

    std::ifstream device_type;
    std::ifstream device_serial;
    std::ifstream firmware_version;

    std::ofstream matrix_custom_frame;
    std::ofstream matrix_brightness;

    std::ofstream matrix_effect_custom;
    std::ofstream matrix_effect_none;
    std::ofstream matrix_effect_static;
    std::ofstream matrix_effect_breath;
    std::ofstream matrix_effect_spectrum;
    std::ofstream matrix_effect_reactive;
    std::ofstream matrix_effect_wave;

    std::ofstream logo_led_brightness;
    std::ofstream logo_matrix_effect_none;
    std::ofstream logo_matrix_effect_static;
    std::ofstream logo_matrix_effect_breath;
    std::ofstream logo_matrix_effect_spectrum;
    std::ofstream logo_matrix_effect_reactive;

    std::ofstream scroll_led_brightness;
    std::ofstream scroll_matrix_effect_none;
    std::ofstream scroll_matrix_effect_static;
    std::ofstream scroll_matrix_effect_breath;
    std::ofstream scroll_matrix_effect_spectrum;
    std::ofstream scroll_matrix_effect_reactive;

    std::ofstream left_led_brightness;
    std::ofstream left_matrix_effect_none;
    std::ofstream left_matrix_effect_static;
    std::ofstream left_matrix_effect_breath;
    std::ofstream left_matrix_effect_spectrum;
    std::ofstream left_matrix_effect_reactive;
    std::ofstream left_matrix_effect_wave;

    std::ofstream right_led_brightness;
    std::ofstream right_matrix_effect_none;
    std::ofstream right_matrix_effect_static;
    std::ofstream right_matrix_effect_breath;
    std::ofstream right_matrix_effect_spectrum;
    std::ofstream right_matrix_effect_reactive;
    std::ofstream right_matrix_effect_wave;

    std::ofstream backlight_led_effect;
    std::ofstream backlight_led_rgb;
    std::ofstream backlight_led_state;
    
    std::ofstream logo_led_effect;
    std::ofstream logo_led_rgb;
    std::ofstream logo_led_state;
    
    std::ofstream scroll_led_effect;
    std::ofstream scroll_led_rgb;
    std::ofstream scroll_led_state;
};
