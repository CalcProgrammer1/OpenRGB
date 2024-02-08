/*---------------------------------------------------------*\
| HPOmenLaptopController_Windows.h                          |
|                                                           |
|   Driver for HP Omen laptop                               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include <Windows.h>

struct IWbemLocator;
struct IWbemServices;
struct IWbemClassObject;
struct IWbemCallResult;

enum KeyboardType
{
    INVALID = 0,
    NORMAL,
    WITH_NUMPAD,
    WITHOUT_NUMPAD,
    RGB
};

enum KeyboardMode
{
    OFF = 0,
    DIRECT
};

class HPOmenLaptopController_Windows
{
private:
    /*-----------------------------------------------------*\
    | Controller private functions                          |
    \*-----------------------------------------------------*/
    int execute(int command, int commandType, int inputDataSize, BYTE* inputData, int* returnDataSize, BYTE** returnData);
    void cleanup(int fail_level);

    IWbemLocator* pLoc;
    IWbemServices* pSvc;
    IWbemClassObject* classObject;
    IWbemClassObject* methodParameters;
    IWbemClassObject* dataInClass;
    IWbemCallResult* callResult;

public:
    HPOmenLaptopController_Windows();
    ~HPOmenLaptopController_Windows();

    /*-----------------------------------------------------*\
    | Controller public functions                           |
    \*-----------------------------------------------------*/
    void setColors(std::vector<RGBColor>& colors);
    bool isLightingSupported();
    KeyboardType getKeyboardType();
    void changeMode(KeyboardMode mode);
};
