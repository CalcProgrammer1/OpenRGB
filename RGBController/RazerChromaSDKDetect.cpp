#include "RGBController.h"
#include "RGBController_RazerChromaSDK.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <string.h>

static HMODULE hModule;

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

#ifdef _WIN64
#define CHROMASDKDLL        ("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL        ("RzChromaSDK.dll")
#endif

/******************************************************************************************\
*                                                                                          *
*   DetectRazerChromaSDKControllers                                                        *
*                                                                                          *
*       Detect devices supported by the Razer Chroma SDK                                   *
*                                                                                          *                                                                                          *
\******************************************************************************************/

void DetectRazerChromaSDKControllers(std::vector<RGBController*>& rgb_controllers)
{
    RGBController_RazerChromaSDK* razer_rgb;

    // Dynamically loads the Chroma SDK library.
    hModule = LoadLibrary(CHROMASDKDLL);
    if (hModule)
    {
        INIT Init = (INIT)GetProcAddress(hModule, "Init");
        if (Init)
        {
            //Initialize the SDK
            Init();
        }
    }


    razer_rgb = new RGBController_RazerChromaSDK(RAZER_GENERIC_MOUSE, &hModule);

    rgb_controllers.push_back(razer_rgb);

    razer_rgb = new RGBController_RazerChromaSDK(RAZER_GENERIC_MOUSEPAD, &hModule);

    rgb_controllers.push_back(razer_rgb);

    razer_rgb = new RGBController_RazerChromaSDK(RAZER_CHROMA_HDK, &hModule);

    rgb_controllers.push_back(razer_rgb);

}   /* DetectRazerChromaSDKControllers() */