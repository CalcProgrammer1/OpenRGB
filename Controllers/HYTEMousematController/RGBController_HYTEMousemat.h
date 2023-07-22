/*-----------------------------------------*\
|  RGBController_HYTEMousemat.h             |
|                                           |
|  Generic RGB Interface for HYTE CNVS RGB  |
|  mousemat                                 |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/18/2023   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "HYTEMousematController.h"

enum
{
    HYTE_CNVS_MODE_DIRECT           = 0,    /* Direct (streaming) mode                  */
    HYTE_CNVS_MODE_RAINBOW          = 1,    /* Rainbow wave (firmware animation) mode   */
};

class RGBController_HYTEMousemat : public RGBController
{
public:
    RGBController_HYTEMousemat(HYTEMousematController* controller_ptr);
    ~RGBController_HYTEMousemat();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    HYTEMousematController*     controller;
};
