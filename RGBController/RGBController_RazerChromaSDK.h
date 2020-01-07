/*-----------------------------------------*\
|  RGBController_RazerChromaSDK.h           |
|                                           |
|  Generic RGB Interface for official Razer |
|  Chroma SDK                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/2/2019    |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include <Windows.h>

#include "RzChromaSDKDefines.h"
#include "RzChromaSDKTypes.h"
#include "RzErrors.h"

using namespace ChromaSDK::Keyboard;

typedef RZRESULT(*INIT)(void);
typedef RZRESULT(*UNINIT)(void);
typedef RZRESULT(*CREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATEMOUSEEFFECT)(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATEMOUSEPADEFFECT)(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATEHEADSETEFFECT)(ChromaSDK::Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATECHROMALINKEFFECT)(ChromaSDK::ChromaLink::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);

#define RAZER_MAX_ZONES 6
#define RAZER_NUM_DEVICES 3

enum
{
    RAZER_GENERIC_KEYBOARD,
    RAZER_GENERIC_MOUSE,
    RAZER_GENERIC_MOUSEPAD,
    RAZER_CHROMA_HDK
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

typedef struct
{
    std::string name;
    unsigned int type;
    unsigned int leds;
} razer_zone;

typedef struct
{
    std::string name;
    unsigned int razer_type;
    unsigned int type;
    const razer_zone* zones[RAZER_MAX_ZONES];
} razer_device;

/*------------------------------------------------------------ *\
|  Razer Generic Mouse                                          |
|                                                               |
|  Zone "Left"                                                  |
|       Linear                                                  |
|       7 LEDs                                                  |
|                                                               |
|  Zone "Right"                                                 |
|       Linear                                                  |
|       7 LEDs                                                  |
|                                                               |
|  Zone "Bottom"                                                |
|       Linear                                                  |
|       5 LEDs                                                  |
|                                                               |
|  Zone "Scroll Wheel"                                          |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Logo"                                                  |
|       Single                                                  |
|       1 LED                                                   |
|                                                               |
|  Zone "Numpad/Backlight"                                      |
|       Single                                                  |
|       1 LED                                                   |
\*-------------------------------------------------------------*/
static const razer_zone mouse_left_zone =
{
    "Left LED Strip",
    ZONE_TYPE_LINEAR,
    7
};

static const razer_zone mouse_right_zone =
{
    "Right LED Strip",
    ZONE_TYPE_LINEAR,
    7
};

static const razer_zone mouse_bottom_zone =
{
    "Bottom LED Strip",
    ZONE_TYPE_LINEAR,
    5
};

static const razer_zone mouse_scroll_wheel_zone =
{
    "Scroll Wheel",
    ZONE_TYPE_SINGLE,
    1
};

static const razer_zone mouse_logo_zone =
{
    "Logo",
    ZONE_TYPE_SINGLE,
    1
};

static const razer_zone mouse_numpad_backlight_zone =
{
    "Numpad/Backlight",
    ZONE_TYPE_SINGLE,
    1
};

static const razer_device mouse_device =
{
    "Razer Generic Mouse",
    RAZER_GENERIC_MOUSE,
    DEVICE_TYPE_MOUSE,
    {
        &mouse_left_zone,
        &mouse_right_zone,
        &mouse_bottom_zone,
        &mouse_scroll_wheel_zone,
        &mouse_logo_zone,
        &mouse_numpad_backlight_zone
    }
};

/*------------------------------------------------------------ *\
|  Razer Generic Mousepad (Firefly, Mug Holder)                 |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       15 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone mousepad_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    15
};

static const razer_device mousepad_device =
{
    "Razer Generic Mousepad",
    RAZER_GENERIC_MOUSEPAD,
    DEVICE_TYPE_MOUSE,
    {
        &mousepad_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    }
};

/*------------------------------------------------------------ *\
|  Razer Chroma HDK                                             |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       16 LEDs                                                 |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       16 LEDs                                                 |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       16 LEDs                                                 |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       16 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone chromahdk_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    16
};

static const razer_device chromahdk_device =
{
    "Razer Chroma HDK",
    RAZER_CHROMA_HDK,
    DEVICE_TYPE_LEDSTRIP,
    {
        &chromahdk_zone,
        &chromahdk_zone,
        &chromahdk_zone,
        &chromahdk_zone,
        NULL,
        NULL
    }
};

static const razer_device* device_list[RAZER_NUM_DEVICES] =
{
    &mouse_device,
    &mousepad_device,
    &chromahdk_device
};

class RGBController_RazerChromaSDK : public RGBController
{
public:
    RGBController_RazerChromaSDK(unsigned int device_type, HMODULE* hModule);
    int         GetMode();
    void        SetMode(int mode);
    void        SetCustomMode();
    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

private:
    const razer_device* device;

    static CREATEEFFECT CreateEffect;
    static CREATEKEYBOARDEFFECT CreateKeyboardEffect;
    static CREATEMOUSEEFFECT CreateMouseEffect;
    static CREATEMOUSEPADEFFECT CreateMousepadEffect;
    static CREATEHEADSETEFFECT CreateHeadsetEffect;
    static CREATECHROMALINKEFFECT CreateChromaLinkEffect;
};
