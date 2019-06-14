/*-----------------------------------------*\
|  RGBController_Aura.h                     |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  Asus Aura driver                         |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/13/2019   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AuraController.h"

class RGBController_Aura : public RGBController
{
public:
    RGBController_Aura(AuraController* aura_ptr);
    int GetMode();
    void SetMode(int mode);
    void SetAllLEDs(unsigned char red, unsigned char green, unsigned char blue);

private:
    AuraController* aura;
};