/*---------------------------------------------------------------------*\
|  RGBController_StrimerLConnect.h                                      |
|                                                                       |
|  Driver for StrimerLConnect USB Controller                            |
|                                                                       |
|  Chris M (Dr_No)          03 Jul 2022                                 |
|                                                                       |
\*---------------------------------------------------------------------*/

#pragma once
#include <vector>
#include "LogManager.h"
#include "RGBController.h"
#include "StrimerLConnectController.h"

class RGBController_StrimerLConnect : public RGBController
{
public:
    RGBController_StrimerLConnect(StrimerLConnectController* controller_ptr);
    ~RGBController_StrimerLConnect();

    void                SetupZones();
    void                ResizeZone(int zone, int new_size);

    void                DeviceUpdateLEDs();
    void                UpdateZoneLEDs(int zone);
    void                UpdateSingleLED(int led);

    void                DeviceUpdateMode();

private:
    void                Init_Controller();
    int                 GetDeviceMode();
    int                 GetLED_Zone(int led_idx);

    mode                CreateMode(std::string name, int value, uint8_t colour_count, uint8_t colour_mode);
    bool                TimeToSend();

    StrimerLConnectController*      controller;
    std::chrono::time_point<std::chrono::steady_clock> last_commit_time;
};
