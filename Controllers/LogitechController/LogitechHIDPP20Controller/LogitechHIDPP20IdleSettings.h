/*---------------------------------------------------------*\
| LogitechHIDPP20IdleSettings.h                             |
|                                                           |
|   Host-side idle/dim/sleep configuration for Logitech     |
|   HID++ 2.0 devices. Two profiles (on_battery, plugged_in)|
|   selected at runtime based on the device's external-     |
|   power flag. `configured == false` means the JSON key    |
|   is absent entirely — the controller defers to firmware. |
|   Qt-free so the controller can consume it directly.      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

struct LogitechHIDPP20IdleProfile
{
    bool dim_when_idle    = false;
    int  dim_brightness   = 50;
    int  idle_timeout_s   = 60;
    bool allow_sleep      = false;
    int  sleep_timeout_s  = 300;
};

class LogitechHIDPP20IdleSettings
{
public:
    static LogitechHIDPP20IdleSettings* instance();

    void                               load();
    void                               save();

    bool                               isConfigured() const { return configured; }
    const LogitechHIDPP20IdleProfile&  onBattery()    const { return on_battery; }
    const LogitechHIDPP20IdleProfile&  pluggedIn()    const { return plugged_in; }

    void                               setOnBattery(const LogitechHIDPP20IdleProfile& p);
    void                               setPluggedIn(const LogitechHIDPP20IdleProfile& p);

private:
    LogitechHIDPP20IdleSettings() = default;

    bool                       configured = false;
    LogitechHIDPP20IdleProfile on_battery;
    LogitechHIDPP20IdleProfile plugged_in;
};
