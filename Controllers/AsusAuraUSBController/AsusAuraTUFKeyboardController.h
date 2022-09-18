/*-----------------------------------------*\
|  AsusAuraTUFKeyboardController.h          |
|                                           |
|  Definitions and types for ASUS Aura      |
|  USB RGB lighting controller              |
|                                           |
|  Mola19 03/03/2021                        |
\*-----------------------------------------*/

#include "RGBController.h"
#include "AsusAuraTUFKeyboardLayouts.h"

#include <string>
#include <vector>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    AURA_KEYBOARD_MODE_STATIC       =  0,
    AURA_KEYBOARD_MODE_BREATHING    =  1,
    AURA_KEYBOARD_MODE_COLOR_CYCLE  =  2,
    AURA_KEYBOARD_MODE_REACTIVE     =  3,
    AURA_KEYBOARD_MODE_WAVE         =  4,
    AURA_KEYBOARD_MODE_RIPPLE       =  5,
    AURA_KEYBOARD_MODE_STARRY_NIGHT =  6,
    AURA_KEYBOARD_MODE_QUICKSAND    =  7,
    AURA_KEYBOARD_MODE_CURRENT      =  8,
    AURA_KEYBOARD_MODE_RAIN_DROP    =  9,
    AURA_KEYBOARD_MODE_DIRECT       = 15,
};

enum
{
    AURA_ROG_FALCHION_WIRED_PID     = 0x193C,
    AURA_ROG_FALCHION_WIRELESS_PID  = 0x193E,
    AURA_TUF_K7_GAMING_PID          = 0x18AA,
    AURA_TUF_K3_GAMING_PID          = 0x194B,
    AURA_ROG_CLAYMORE_PID           = 0x184D,
};

struct led_color
{
    unsigned int    value;
    RGBColor        color;
};

class AuraTUFKeyboardController
{
public:
    AuraTUFKeyboardController(hid_device* dev_handle, const char* path, uint16_t pid, unsigned short rev_version);
    ~AuraTUFKeyboardController();

    std::string GetDeviceLocation();
    std::string GetSerialString();
    std::string GetVersion();
    int         GetLayout();
    int         GetNumpadLocation();
    void        SaveMode();
    void        AllowRemoteControl(unsigned char type);

    void UpdateSingleLed
        (
        int             led,
        unsigned char   red,
        unsigned char   green,
        unsigned char   blue
        );

    void UpdateLeds
        (
        std::vector<led_color>    colors
        );

    void UpdateDevice
        (
        unsigned char           mode,
        std::vector<RGBColor>   colors,
        unsigned char           dir,
        unsigned char           color_mode,
        unsigned char           speed,
        unsigned char           brightness
        );
    void UpdateQuicksandColors(std::vector<RGBColor> colors);
    void UpdateMode(unsigned char mode);
    void AwaitResponse(int ms);
    void ClearResponses();

    uint16_t                    device_pid;

private:
    hid_device*                 dev;
    std::string                 location;
    unsigned short              version;
};

