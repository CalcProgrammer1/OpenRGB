/*-----------------------------------------*\
|  FanController_NZXTKraken.h               |
|                                           |
|  Generic Fan Interface for NZXT Kraken    |
|                                           |
|  swilde 5/2021                            |
\*-----------------------------------------*/

#pragma once
#include "FanController.h"
#include "NZXTKrakenController.h"

class FanController_NZXTKraken : public FanController
{
public:
    FanController_NZXTKraken(NZXTKrakenController* kraken_ptr);

    void        UpdateControl();
    void        UpdateReading();

private:
    NZXTKrakenController* kraken;
};
