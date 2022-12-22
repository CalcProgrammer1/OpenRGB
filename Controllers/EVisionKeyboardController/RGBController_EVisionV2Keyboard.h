/*-------------------------------------------------------------------*\
|  RGBController_EVisionV2KeyboardController.h                        |
|                                                                     |
|  Generic RGB Interface for EVision V2 RGB Keyboard                  |
|                                                                     |
|  Le Philousophe          25 Dec 2022                                |
\*-------------------------------------------------------------------*/

#pragma once
#include "LogManager.h"
#include "RGBController.h"
#include "EVisionV2KeyboardController.h"
#include <vector>

class RGBController_EVisionV2Keyboard : public RGBController
{
public:
    RGBController_EVisionV2Keyboard(EVisionV2KeyboardController* controller_ptr, EVisionV2KeyboardPart part);
    ~RGBController_EVisionV2Keyboard();

    void SetupZones() override;
    void ResizeZone(int zone, int new_size) override;
    void SetCustomMode() override;

    void DeviceUpdateLEDs() override;
    void UpdateZoneLEDs(int zone) override;
    void UpdateSingleLED(int led) override;

    void DeviceUpdateMode() override;
    void DeviceSaveMode() override;

private:
    void SetupKeyboardModes();
    void SetupLogoEdgeModes();
    void LoadConfig();

    void KeepaliveThread();

    EVisionV2KeyboardController*                       controller;
    EVisionV2KeyboardPart                              part;

    std::thread*                                       keepalive_thread;
    std::atomic<bool>                                  keepalive_thread_run;
    std::atomic<bool>                                  has_color_set;
    std::chrono::time_point<std::chrono::steady_clock> last_update_time;
};
