/*---------------------------------------------------------*\
| RGBController_Govee.h                                     |
|                                                           |
|   RGBController for Govee wireless lighting devices       |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      01 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <condition_variable>
#include <mutex>
#include "RGBController.h"
#include "GoveeController.h"

class RGBController_Govee : public RGBController
{
public:
    RGBController_Govee(GoveeController* controller_ptr);
    ~RGBController_Govee();

    void        SetupZones();
    void        DeviceConfigureZone(int zone_idx);

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceUpdateDeviceSpecificConfiguration() override;

    void        KeepaliveThread();

private:
    void        InitDeviceSpecificConfiguration();
    void        ApplyDeviceSpecificConfiguration();
    void        UpdateStatic(bool force);
    void        UpdateLEDs();
    void        UpdatePower(unsigned int brightness);

    GoveeController*                                    controller;
    std::thread*                                        keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::mutex                                          keepalive_mutex;
    std::condition_variable                             keepalive_cv;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
    RGBColor                                            last_static_color;
    unsigned int                                        last_static_brightness;
    bool                                                razer_supported;
    bool                                                razer_status_available;
    bool                                                gradient_enabled;
    bool                                                control_initialized;
    unsigned int                                        razer_delay_ms;
    unsigned int                                        configured_led_count;
    unsigned int                                        configured_matrix_width;
    unsigned int                                        configured_gap_columns;
    unsigned int                                        configured_layout;
    bool                                                razer_active;
    bool                                                power_known;
    bool                                                power_on;
    bool                                                static_initialized;
    bool                                                direct_brightness_initialized;
    unsigned int                                        last_direct_brightness;
};
