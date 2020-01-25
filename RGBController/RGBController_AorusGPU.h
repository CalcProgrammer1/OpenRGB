/*-----------------------------------------*\
|  RGBController_AorusGPU.h                 |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  Aorus GPU                                |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/17/2019   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"

typedef unsigned int uint32;
typedef uint32(*_GvWriteI2C)(uint32, void*, uint32);
typedef void (*_GvFreeDispLib)();
typedef void (*_GvInitDispLib)();

class RGBController_AorusGPU : public RGBController
{
public:
    RGBController_AorusGPU();
    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();

private:
    _GvWriteI2C GvWriteI2C;
    _GvFreeDispLib GvFreeDispLib;
    _GvInitDispLib GvInitDispLib;
};
