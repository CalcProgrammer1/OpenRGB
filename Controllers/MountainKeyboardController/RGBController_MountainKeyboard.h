/*-----------------------------------------*\
|  RGBController_MountainKeyboard.h         |
|                                           |
|  Generic RGB Interface for Mountain RGB   |
|  keyboard devices                         |
|                                           |
|  Wojciech Lazarski             01/2023    |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "MountainKeyboardController.h"

#define MOUNTAIN_KEYBOARD_BRIGHTNESS_MIN           0
#define MOUNTAIN_KEYBOARD_BRIGHTNESS_MAX         100

#define MOUNTAIN_KEYBOARD_SPEED_MIN                0
#define MOUNTAIN_KEYBOARD_SPEED_MAX                4
#define MOUNTAIN_KEYBOARD_SPEED_DEFAULT            3

enum
{
    MOUNTAIN_KEYBOARD_MODE_DIRECT       = MOUNTAIN_KEYBOARD_IDX_CUSTOM,
    MOUNTAIN_KEYBOARD_MODE_STATIC       = MOUNTAIN_KEYBOARD_IDX_STATIC,
    MOUNTAIN_KEYBOARD_MODE_COLOR_WAVE   = MOUNTAIN_KEYBOARD_IDX_COLOR_WAVE,
    MOUNTAIN_KEYBOARD_MODE_TORNADO      = MOUNTAIN_KEYBOARD_IDX_TORNADO,
    MOUNTAIN_KEYBOARD_MODE_BREATHING    = MOUNTAIN_KEYBOARD_IDX_BREATHING,
    MOUNTAIN_KEYBOARD_MODE_REACTIVE     = MOUNTAIN_KEYBOARD_IDX_REACTIVE,
    MOUNTAIN_KEYBOARD_MODE_MATRIX       = MOUNTAIN_KEYBOARD_IDX_MATRIX,
    MOUNTAIN_KEYBOARD_MODE_OFF          = MOUNTAIN_KEYBOARD_IDX_OFF
};

class RGBController_MountainKeyboard : public RGBController
{
public:
    RGBController_MountainKeyboard(MountainKeyboardController* controller_ptr);
    ~RGBController_MountainKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    MountainKeyboardController* controller;
    unsigned char ConvertDirection(unsigned int direction, bool rotation);
    void          DeviceUpdate(const mode& current_mode);
};
