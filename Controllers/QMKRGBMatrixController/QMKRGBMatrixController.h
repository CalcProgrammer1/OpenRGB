/*-------------------------------------------------------------------*\
|  QMKRGBMatrixController.h                                           |
|                                                                     |
|  Driver for QMK keybaords using RGB Matrix                          |
|                                                                     |
|  Kasper         10th Octobber 2020                                  |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <string>
#include <array>
#include <hidapi/hidapi.h>
#include "RGBController.h"
#include "hsv.h"

#pragma once

enum CommandsId
{
    QMK_RGBMATRIX_SET_SINGLE_LED = 3,
    QMK_RGBMATRIX_SET_LEDS,

    QMK_RGBMATRIX_SET_MODE,
    QMK_RGBMATRIX_SET_MODE_AND_SPEED,
    QMK_RGBMATRIX_SET_COLOR_MODE_AND_SPEED,

    QMK_RGBMATRIX_GET_QMK_VERSION,
    QMK_RGBMATRIX_GET_DEVICE_NAME,
    QMK_RGBMATRIX_GET_ZONES_COUNT,
    QMK_RGBMATRIX_GET_ZONE_NAME,
    QMK_RGBMATRIX_GET_ZONE_TYPE,
    QMK_RGBMATRIX_GET_ZONE_SIZE,
    QMK_RGBMATRIX_GET_LED_NAME,
    QMK_RGBMATRIX_GET_LED_MATRIX_COLUMNS,
    QMK_RGBMATRIX_GET_LED_MATRIX_ROWS,
    QMK_RGBMATRIX_GET_LED_VALUE_IN_MATRIX,
    QMK_RGBMATRIX_GET_LED_COLOR,
    QMK_RGBMATRIX_GET_HSV
};

enum EEPROMCommands
{
    QMK_RGBMATRIX_EEPROM_SAVE,
    QMK_RGBMATRIX_EEPROM_DONT_SAVE
};

enum ModeCommands
{
    QMK_RGBMATRIX_MODE_HID_DIRECT = 1,
    QMK_RGBMATRIX_MODE_STATIC,
    QMK_RGBMATRIX_MODE_ALPHA_MOD,
    QMK_RGBMATRIX_MODE_STATIC_GRADIENT_LEFT_RIGHT,
    QMK_RGBMATRIX_MODE_STATIC_GRADIENT_UP_DOWN,
    QMK_RGBMATRIX_MODE_BREATHING,
    QMK_RGBMATRIX_MODE_BAND_SAT,
    QMK_RGBMATRIX_MODE_BAND_VAL,
    QMK_RGBMATRIX_MODE_BAND_PINWHEEL_SAT,
    QMK_RGBMATRIX_MODE_BAND_PINWHEEL_VAL,
    QMK_RGBMATRIX_MODE_BAND_SPIRAL_SAT,
    QMK_RGBMATRIX_MODE_BAND_SPIRAL_VAL,
    QMK_RGBMATRIX_MODE_CYCLE_ALL,
    QMK_RGBMATRIX_MODE_CYCLE_LEFT_RIGHT,
    QMK_RGBMATRIX_MODE_CYCLE_UP_DOWN,
    QMK_RGBMATRIX_MODE_CYCLE_OUT_IN,
    QMK_RGBMATRIX_MODE_CYCLE_OUT_IN_DUAL,
    QMK_RGBMATRIX_MODE_RAINBOW_MOVING_CHEVRON,
    QMK_RGBMATRIX_MODE_DUAL_BEACON,
    QMK_RGBMATRIX_MODE_CYCLE_PINWHEEL,
    QMK_RGBMATRIX_MODE_CYCLE_SPIRAL,
    QMK_RGBMATRIX_MODE_RAINBOW_BEACON,
    QMK_RGBMATRIX_MODE_RAINBOW_PINWHEELS,
    QMK_RGBMATRIX_MODE_RAINDROPS,
    QMK_RGBMATRIX_MODE_JELLYBEAN_RAINDROPS,
    QMK_RGBMATRIX_MODE_TYPING_HEATMAP,
    QMK_RGBMATRIX_MODE_DIGITAL_RAIN,
    QMK_RGBMATRIX_MODE_SOLID_REACTIVE,
    QMK_RGBMATRIX_MODE_SOLID_REACTIVE_SIMPLE,
    QMK_RGBMATRIX_MODE_SOLID_REACTIVE_WIDE,
    QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTIWIDE,
    QMK_RGBMATRIX_MODE_SOLID_REACTIVE_CROSS,
    QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTICROSS,
    QMK_RGBMATRIX_MODE_SOLID_REACTIVE_NEXUS,
    QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTINEXUS,
    QMK_RGBMATRIX_MODE_SPLASH,
    QMK_RGBMATRIX_MODE_MULTISPLASH,
    QMK_RGBMATRIX_MODE_SOLID_SPLASH,
    QMK_RGBMATRIX_MODE_SOLID_MULTISPLASH,
};

enum SpeedCommands
{
    QMK_RGBMATRIX_SPEED_SLOWEST          = 0x00,  /* Slowest speed                   */
    QMK_RGBMATRIX_SPEED_NORMAL           = 0x7F,  /* Normal speed                    */
    QMK_RGBMATRIX_SPEED_FASTEST          = 0xFF,  /* Fastest speed                   */
};

class QMKRGBMatrixController
{
public:
    QMKRGBMatrixController(hid_device* dev_handle, const char* path);
    ~QMKRGBMatrixController();

    void SetSingleLED(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void SetLEDs(RGBColor* colors, unsigned int num_colors);

    void SetMode(unsigned char mode);
    void SetModeAndSpeed(unsigned char mode, unsigned char speed);
    void SetColorModeAndSpeed(hsv_t hsv_color, unsigned char mode, unsigned char speed);

    std::string GetLocation();

    std::string GetVersion();
    std::string GetName();
    unsigned int GetZonesCount();
    std::string GetZoneName(int zone);
    unsigned int GetZoneType(int zone);
    unsigned int GetZoneSize(int zone);
    std::string GetLEDName(int led);
    unsigned int GetLEDMatirxRows();
    unsigned int GetLEDMatirxColumns();
    unsigned int GetLEDValueInMatrix(int column, int row);
    RGBColor GetLEDColor(unsigned int led);
    unsigned int GetHSV();
    
protected:
    hid_device* dev;
private:
    std::string location;
};
