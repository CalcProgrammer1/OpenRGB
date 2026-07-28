/*---------------------------------------------------------*\
| ValveSteamMachineController_Windows.cpp                   |
|                                                           |
|   Driver for Valve Steam Machine LEDs                     |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      28 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "LogManager.h"
#include "ValveSteamMachineController_Windows.h"

/*---------------------------------------------------------*\
| Name for log entries                                      |
\*---------------------------------------------------------*/
static const char* VALVESTEAMMACHINECONTROLLER = "ValveSteamMachineController";

static const std::vector<std::string> effects =
{
	"patrol",
	"breath",
	"factory",
	"normal",
	"off",
	"rainbow",
	"demo",
	"manual"
};

ValveSteamMachineController::ValveSteamMachineController(HANDLE dev_handle, std::string dev_name)
{
    name            = dev_name;
    pawnio_handle   = dev_handle;

    /*-----------------------------------------------------*\
    | Initialize driver                                     |
    \*-----------------------------------------------------*/
    ULONG64         in[1];
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_init", in, 0, out, 0, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_init failed", VALVESTEAMMACHINECONTROLLER);
    }
}

ValveSteamMachineController::~ValveSteamMachineController()
{

}

std::string ValveSteamMachineController::GetName()
{
    return(name);
}

size_t ValveSteamMachineController::GetLEDCount()
{
    return(17);
}

std::string ValveSteamMachineController::GetLocation()
{
    return("PawnIO leds-valve");
}

std::vector<std::string> ValveSteamMachineController::GetAvailableEffects()
{
    return(effects);
}

unsigned int ValveSteamMachineController::GetBrightness()
{
    /*-----------------------------------------------------*\
    | Get brightness                                        |
    \*-----------------------------------------------------*/
    ULONG64         in[1];
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_get_brightness_scale", in, 0, out, 1, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_get_brightness_scale failed", VALVESTEAMMACHINECONTROLLER);
        return(0);
    }

    if(out[0] <= 255)
    {
        return(out[0]);
    }
    else
    {
        LOG_ERROR("[%s] ioctl_get_brightness_scale delay out of range", VALVESTEAMMACHINECONTROLLER);
        return(0);
    }
}

unsigned int ValveSteamMachineController::GetDelay()
{
    /*-----------------------------------------------------*\
    | Get delay                                             |
    \*-----------------------------------------------------*/
    ULONG64         in[1];
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_get_delay", in, 0, out, 1, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_get_delay failed", VALVESTEAMMACHINECONTROLLER);
        return(0);
    }

    if(out[0] <= 20)
    {
        return(out[0]);
    }
    else
    {
        LOG_ERROR("[%s] ioctl_get_delay delay out of range", VALVESTEAMMACHINECONTROLLER);
        return(0);
    }
}

std::string ValveSteamMachineController::GetEffect()
{
    /*-----------------------------------------------------*\
    | Get effect                                            |
    \*-----------------------------------------------------*/
    ULONG64         in[1];
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_get_effect", in, 0, out, 1, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_get_effect failed", VALVESTEAMMACHINECONTROLLER);
        return("");
    }

    if(out[0] < effects.size())
    {
        return(effects[out[0]]);
    }
    else
    {
        LOG_ERROR("[%s] ioctl_get_effect effect out of range", VALVESTEAMMACHINECONTROLLER);
        return("");
    }
}

bool ValveSteamMachineController::GetEnabled()
{
    /*-----------------------------------------------------*\
    | Get enabled                                           |
    \*-----------------------------------------------------*/
    ULONG64         in[1];
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_get_enabled", in, 0, out, 1, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_get_enabled failed", VALVESTEAMMACHINECONTROLLER);
        return(false);
    }

    return((out[0] & 0xFF) != 0);
}

unsigned int ValveSteamMachineController::GetBreathOffset()
{
    /*-----------------------------------------------------*\
    | Get breath offset                                     |
    \*-----------------------------------------------------*/
    ULONG64         in[1];
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_get_breath_offset", in, 0, out, 1, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_get_breath_offset failed", VALVESTEAMMACHINECONTROLLER);
        return(0);
    }

    if(out[0] <= 255)
    {
        return(out[0]);
    }
    else
    {
        LOG_ERROR("[%s] ioctl_get_breath_offset out of range", VALVESTEAMMACHINECONTROLLER);
        return(0);
    }
}

unsigned int ValveSteamMachineController::GetBreathLevel()
{
    /*-----------------------------------------------------*\
    | Get breath level                                      |
    \*-----------------------------------------------------*/
    ULONG64         in[1];
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_get_breath_level", in, 0, out, 1, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_get_breath_level failed", VALVESTEAMMACHINECONTROLLER);
        return(0);
    }

    if(out[0] <= 255)
    {
        return(out[0]);
    }
    else
    {
        LOG_ERROR("[%s] ioctl_get_breath_level out of range", VALVESTEAMMACHINECONTROLLER);
        return(0);
    }
}

unsigned int ValveSteamMachineController::GetPatrolNum()
{
    /*-----------------------------------------------------*\
    | Get patrol num                                        |
    \*-----------------------------------------------------*/
    ULONG64         in[1];
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_get_patrol_num", in, 0, out, 1, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_get_patrol_num failed", VALVESTEAMMACHINECONTROLLER);
        return(0);
    }

    if(out[0] <= 255)
    {
        return(out[0]);
    }
    else
    {
        LOG_ERROR("[%s] ioctl_get_patrol_num out of range", VALVESTEAMMACHINECONTROLLER);
        return(0);
    }
}

unsigned int ValveSteamMachineController::GetColorShift()
{
    /*-----------------------------------------------------*\
    | Get color shift                                       |
    \*-----------------------------------------------------*/
    ULONG64         in[1];
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_get_color_shift", in, 0, out, 1, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_get_color_shift failed", VALVESTEAMMACHINECONTROLLER);
        return(0);
    }

    if(out[0] <= 255)
    {
        return(out[0]);
    }
    else
    {
        LOG_ERROR("[%s] ioctl_get_color_shift out of range", VALVESTEAMMACHINECONTROLLER);
        return(0);
    }
}

bool ValveSteamMachineController::GetPersistence()
{
    /*-----------------------------------------------------*\
    | Get persistence                                       |
    \*-----------------------------------------------------*/
    ULONG64         in[1];
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_get_persistence", in, 0, out, 1, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_get_persistence failed", VALVESTEAMMACHINECONTROLLER);
        return(false);
    }

    return((out[0] & 0xFF) != 0);
}

RGBColor ValveSteamMachineController::GetStartupColor()
{
    /*-----------------------------------------------------*\
    | Get startup color                                     |
    \*-----------------------------------------------------*/
    ULONG64         in[1];
    ULONG64         out[3];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_get_startup_color", in, 0, out, 3, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_get_startup_color failed", VALVESTEAMMACHINECONTROLLER);
        return(0);
    }

    return(ToRGBColor(out[0], out[1], out[2]));
}

unsigned int ValveSteamMachineController::GetBrightnessStartup()
{
    /*-----------------------------------------------------*\
    | Get brightness startup                                |
    \*-----------------------------------------------------*/
    ULONG64         in[1];
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_get_brightness_startup", in, 0, out, 1, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_get_brightness_startup failed", VALVESTEAMMACHINECONTROLLER);
        return(0);
    }

    if(out[0] <= 255)
    {
        return(out[0]);
    }
    else
    {
        LOG_ERROR("[%s] ioctl_get_brightness_startup out of range", VALVESTEAMMACHINECONTROLLER);
        return(0);
    }
}

void ValveSteamMachineController::SetLEDColor(unsigned int led_idx, RGBColor color)
{
    if(led_idx >= GetLEDCount())
    {
        LOG_ERROR("[%s] ioctl_set_led_color led index out of range", VALVESTEAMMACHINECONTROLLER);
        return;
    }

    /*-----------------------------------------------------*\
    | Reverse order of LEDs                                 |
    \*-----------------------------------------------------*/
    led_idx = (GetLEDCount() - 1) - led_idx;

    /*-----------------------------------------------------*\
    | Set LED color                                         |
    \*-----------------------------------------------------*/
    ULONG64         in[4] = {led_idx, RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color)};
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_set_led_color", in, 4, out, 0, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_set_led_color failed", VALVESTEAMMACHINECONTROLLER);
    }
}

void ValveSteamMachineController::SetEffect(std::string effect)
{
    /*-----------------------------------------------------*\
    | Determine effect index                                |
    \*-----------------------------------------------------*/
    std::size_t     effect_idx;

    for(effect_idx = 0; effect_idx < effects.size(); effect_idx++)
    {
        if(effect == effects[effect_idx])
        {
            break;
        }
    }

    if(effect_idx >= effects.size())
    {
        LOG_ERROR("[%s] ioctl_set_effect effect out of range", VALVESTEAMMACHINECONTROLLER);
        return;
    }

    /*-----------------------------------------------------*\
    | Set effect                                            |
    \*-----------------------------------------------------*/
    ULONG64         in[1]  = {effect_idx};
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_set_effect", in, 1, out, 0, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_set_effect failed", VALVESTEAMMACHINECONTROLLER);
    }
}

void ValveSteamMachineController::SetBrightness(unsigned int brightness)
{
    if(brightness > 255)
    {
        LOG_ERROR("[%s] ioctl_set_brightness_scale brightness out of range", VALVESTEAMMACHINECONTROLLER);
        return;
    }

    /*-----------------------------------------------------*\
    | Set brightness                                        |
    \*-----------------------------------------------------*/
    ULONG64         in[1] = {brightness};
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_set_brightness_scale", in, 1, out, 0, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_set_brightness_scale failed", VALVESTEAMMACHINECONTROLLER);
    }
}

void ValveSteamMachineController::SetDelay(unsigned int delay)
{
    if(delay > 20)
    {
        LOG_ERROR("[%s] ioctl_set_delay delay out of range", VALVESTEAMMACHINECONTROLLER);
        return;
    }

    /*-----------------------------------------------------*\
    | Set delay                                             |
    \*-----------------------------------------------------*/
    ULONG64         in[1] = {delay};
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_set_delay", in, 1, out, 0, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_set_delay failed", VALVESTEAMMACHINECONTROLLER);
    }
}

void ValveSteamMachineController::SetEnabled(bool enabled)
{
    /*-----------------------------------------------------*\
    | Set enabled                                           |
    \*-----------------------------------------------------*/
    ULONG64         in[1] = {enabled ? 1 : 0};
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_set_enabled", in, 1, out, 0, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_set_enabled failed", VALVESTEAMMACHINECONTROLLER);
    }
}

void ValveSteamMachineController::SetBreathOffset(unsigned int breath_offset)
{
    if(breath_offset > 255)
    {
        LOG_ERROR("[%s] ioctl_set_breath_offset out of range", VALVESTEAMMACHINECONTROLLER);
        return;
    }

    /*-----------------------------------------------------*\
    | Set breath offset                                     |
    \*-----------------------------------------------------*/
    ULONG64         in[1] = {breath_offset};
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_set_breath_offset", in, 1, out, 0, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_set_breath_offset failed", VALVESTEAMMACHINECONTROLLER);
    }
}

void ValveSteamMachineController::SetBreathLevel(unsigned int breath_level)
{
    if(breath_level > 255)
    {
        LOG_ERROR("[%s] ioctl_set_breath_level out of range", VALVESTEAMMACHINECONTROLLER);
        return;
    }

    /*-----------------------------------------------------*\
    | Set breath level                                      |
    \*-----------------------------------------------------*/
    ULONG64         in[1] = {breath_level};
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_set_breath_level", in, 1, out, 0, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_set_breath_level failed", VALVESTEAMMACHINECONTROLLER);
    }
}

void ValveSteamMachineController::SetPatrolNum(unsigned int patrol_num)
{
    if(patrol_num > 255)
    {
        LOG_ERROR("[%s] ioctl_set_patrol_num out of range", VALVESTEAMMACHINECONTROLLER);
        return;
    }

    /*-----------------------------------------------------*\
    | Set patrol num                                        |
    \*-----------------------------------------------------*/
    ULONG64         in[1] = {patrol_num};
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_set_patrol_num", in, 1, out, 0, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_set_patrol_num failed", VALVESTEAMMACHINECONTROLLER);
    }
}

void ValveSteamMachineController::SetColorShift(unsigned int color_shift)
{
    if(color_shift > 255)
    {
        LOG_ERROR("[%s] ioctl_set_color_shift out of range", VALVESTEAMMACHINECONTROLLER);
        return;
    }

    /*-----------------------------------------------------*\
    | Set color shift                                       |
    \*-----------------------------------------------------*/
    ULONG64         in[1] = {color_shift};
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_set_color_shift", in, 1, out, 0, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_set_color_shift failed", VALVESTEAMMACHINECONTROLLER);
    }
}

void ValveSteamMachineController::SetStartupColor(RGBColor color)
{
    /*-----------------------------------------------------*\
    | Set startup color                                     |
    \*-----------------------------------------------------*/
    ULONG64         in[3] = {RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color)};
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_set_startup_color", in, 3, out, 0, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_set_startup_color failed", VALVESTEAMMACHINECONTROLLER);
    }
}

void ValveSteamMachineController::SetBrightnessStartup(unsigned int brightness_startup)
{
    if(brightness_startup > 255)
    {
        LOG_ERROR("[%s] ioctl_set_brightness_startup out of range", VALVESTEAMMACHINECONTROLLER);
        return;
    }

    /*-----------------------------------------------------*\
    | Set brightness startup                                |
    \*-----------------------------------------------------*/
    ULONG64         in[1] = {brightness_startup};
    ULONG64         out[1];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(pawnio_handle, "ioctl_set_brightness_startup", in, 1, out, 0, &return_size);

    if(status != S_OK)
    {
        LOG_ERROR("[%s] ioctl_set_brightness_startup failed", VALVESTEAMMACHINECONTROLLER);
    }
}
