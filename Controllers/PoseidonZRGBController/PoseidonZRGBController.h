/*-----------------------------------------*\
|  PoseidonZRGBController.h                 |
|                                           |
|  Definitions and types for Thermaltake    |
|  Poseidon Z RGB Keyboard lighting         |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/25/2019  |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include "hidapi.h"

#pragma once

#define POSEIDONZ_START       0x07
#define POSEIDONZ_PROFILE     0x01
#define POSEIDONZ_LED_CMD     0x0E
#define POSEIDONZ_RED_GRN_CH  0x01
#define POSEIDONZ_BLU_CH      0x02

class PoseidonZRGBController
{
public:
    PoseidonZRGBController(hid_device* dev_handle);
    ~PoseidonZRGBController();

    char* GetDeviceName();

    void SetLEDsDirect(std::vector<RGBColor> colors);
    void SetLEDs(std::vector<RGBColor> colors);
    
private:
    char                    device_name[32];
    hid_device*             dev;
};
