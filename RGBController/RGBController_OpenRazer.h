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
        RAZER_NO_DEVICE,
        RAZER_BLACKWIDOW_CHROMA,
        RAZER_DEATHSTALKER_CHROMA,
        RAZER_ORNATA_CHROMA,
        RAZER_BLADE_STEALTH,
        RAZER_BLADE_PRO,
        RAZER_TARTARUS_CHROMA,
        RAZER_DEATHADDER_CHROMA,
        RAZER_DEATHADDER_ELITE,
        RAZER_NAGA_CHROMA,
        RAZER_DIAMONDBACK_CHROMA,
        RAZER_MAMBA_TOURNAMENT_EDITION_CHROMA,
        RAZER_FIREFLY_CHROMA,
        RAZER_GOLIATHUS_EXTENDED_CHROMA,
        RAZER_MUG_HOLDER,
        RAZER_CORE,
        RAZER_KRAKEN_V1,
        RAZER_KRAKEN_V2,
        RAZER_CHROMA_HDK,
        RAZER_NUM_DEVICES
    };

    enum
    {
        RAZER_MODE_CUSTOM,
        RAZER_MODE_OFF,
        RAZER_MODE_STATIC,
        RAZER_MODE_BREATHING,
        RAZER_MODE_SPECTRUM_CYCLE,
        RAZER_MODE_WAVE,
        RAZER_MODE_REACTIVE,
        RAZER_NUM_MODES
    };

    enum
    {
        RAZER_TYPE_MATRIX_FRAME,
        RAZER_TYPE_MATRIX_NOFRAME,
        RAZER_TYPE_NOMATRIX,
        RAZER_NUM_TYPES
    };

public:
    RGBController_OpenRazer(std::string dev_path);
    int GetMode();
    void SetMode(int mode);
    void SetCustomMode();
    void SetAllLEDs(RGBColor color);
    void SetAllZoneLEDs(int zone, RGBColor color);
    void SetLED(int led, RGBColor color);
    void UpdateLEDs();

    unsigned int device;

private:
    void SetupMatrixDevice(std::string dev_path, unsigned int rows, unsigned int cols);
    void SetupNonMatrixDevice(std::string dev_path);
    unsigned int GetTypeFromDeviceName(std::string dev_name);

    unsigned int type;
    unsigned int matrix_rows;
    unsigned int matrix_cols;

    //OpenRazer Sysfs Entries for Matrix Devices
    std::ofstream matrix_custom_frame;
    std::ofstream matrix_effect_custom;
    std::ofstream matrix_effect_breath;
    std::ofstream matrix_effect_none;
    std::ofstream matrix_effect_reactive;
    std::ofstream matrix_effect_spectrum;
    std::ofstream matrix_effect_static;
    std::ofstream matrix_effect_wave;

    //OpenRazer Sysfs Entries for Non-Matrix Devices
    std::ofstream logo_led_effect;
    std::ofstream logo_led_rgb;
    std::ofstream scroll_led_effect;
    std::ofstream scroll_led_rgb;
};