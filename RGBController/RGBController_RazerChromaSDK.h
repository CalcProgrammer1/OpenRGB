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

#define RAZER_MAX_ZONES 4
#define RAZER_NUM_DEVICES 2

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
    unsigned int type;
    const razer_zone* zones[RAZER_MAX_ZONES];
} razer_device;

/*------------------------------------------------------------ *\
|  Razer Firefly                                                |
|                                                               |
|  Zone "LED Strip"                                             |
|       Linear                                                  |
|       15 LEDs                                                 |
\*-------------------------------------------------------------*/
static const razer_zone firefly_zone =
{
    "LED Strip",
    ZONE_TYPE_LINEAR,
    15
};

static const razer_device firefly_device =
{
    "Razer Firefly",
    RAZER_FIREFLY_CHROMA,
    {
        &firefly_zone,
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
    {
        &chromahdk_zone,
        &chromahdk_zone,
        &chromahdk_zone,
        &chromahdk_zone
    }
};

static const razer_device* device_list[RAZER_NUM_DEVICES] =
{
    &firefly_device,
    &chromahdk_device
};

class RGBController_RazerChromaSDK : public RGBController
{
public:
    RGBController_RazerChromaSDK(unsigned int device_type, HMODULE* hModule);
    int GetMode();
    void SetMode(int mode);
    void SetCustomMode();
    void SetAllLEDs(RGBColor color);
    void SetAllZoneLEDs(int zone, RGBColor color);
    void SetLED(int led, RGBColor color);
    void UpdateLEDs();

private:
    const razer_device* device;

    static CREATEEFFECT CreateEffect;
    static CREATEKEYBOARDEFFECT CreateKeyboardEffect;
    static CREATEMOUSEEFFECT CreateMouseEffect;
    static CREATEMOUSEPADEFFECT CreateMousepadEffect;
    static CREATEHEADSETEFFECT CreateHeadsetEffect;
    static CREATECHROMALINKEFFECT CreateChromaLinkEffect;
};