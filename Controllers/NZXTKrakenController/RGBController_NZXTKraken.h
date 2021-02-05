/*-----------------------------------------*\
|  RGBController_NZXTKraken.h               |
|                                           |
|  Generic RGB Interface for NZXT Kraken    |
|                                           |
|  Martin Hartl (inlart)        04/04/2020  |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "NZXTKrakenController.h"

#include <set>

class RGBController_NZXTKraken : public RGBController
{
public:
    RGBController_NZXTKraken(NZXTKrakenController* nzxtkraken_ptr);
    ~RGBController_NZXTKraken();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    std::vector<std::vector<RGBColor>> GetColors
        (
        int zone,
        const mode& channel_mode
        );

    void UpdateChannel
        (
        NZXTKrakenChannel_t channel,
        int zone,
        const mode& channel_mode
        );

    NZXTKrakenController*       nzxtkraken;
    std::set<unsigned int>      logo_modes;
    int                         default_mode;
};
