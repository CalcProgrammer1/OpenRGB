/*---------------------------------------------------------*\
| LogitechHIDPP20IdleSettings.cpp                           |
|                                                           |
|   Host-side idle/dim/sleep settings storage helper for    |
|   Logitech HID++ 2.0 devices. Loads the configuration     |
|   block from SettingsManager JSON, caches it, and writes  |
|   changes back through SettingsManager::SetSettings().    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "LogitechHIDPP20IdleSettings.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

static const char* SETTINGS_KEY = "LogitechHIDPP20IdleSettings";

LogitechHIDPP20IdleSettings* LogitechHIDPP20IdleSettings::instance()
{
    static LogitechHIDPP20IdleSettings inst;
    return &inst;
}

/*---------------------------------------------------------*\
| Minimum idle / sleep values the controller will honor.    |
| Matches what Logitech firmware typically clamps to on     |
| HID++ 2.0 devices, and guarantees the skip-dim path's     |
| sleep_delay math always produces a positive window.       |
\*---------------------------------------------------------*/
static const int MIN_IDLE_S     = 15;
static const int MIN_SLEEP_S    = 45;
static const int MIN_DIM_WINDOW = 30;   // sleep must exceed idle by at least this

static void ClampProfile(LogitechHIDPP20IdleProfile& p)
{
    if(p.dim_brightness < 0)   p.dim_brightness = 0;
    if(p.dim_brightness > 100) p.dim_brightness = 100;

    if(p.idle_timeout_s < MIN_IDLE_S)
    {
        p.idle_timeout_s = MIN_IDLE_S;
    }

    if(p.sleep_timeout_s < MIN_SLEEP_S)
    {
        p.sleep_timeout_s = MIN_SLEEP_S;
    }

    if(p.sleep_timeout_s < p.idle_timeout_s + MIN_DIM_WINDOW)
    {
        p.sleep_timeout_s = p.idle_timeout_s + MIN_DIM_WINDOW;
    }
}

static LogitechHIDPP20IdleProfile ProfileFromJson(const json& j)
{
    LogitechHIDPP20IdleProfile p;

    if(j.contains("dim_when_idle"))   p.dim_when_idle   = j["dim_when_idle"];
    if(j.contains("dim_brightness"))  p.dim_brightness  = j["dim_brightness"];
    if(j.contains("idle_timeout_s"))  p.idle_timeout_s  = j["idle_timeout_s"];
    if(j.contains("allow_sleep"))     p.allow_sleep     = j["allow_sleep"];
    if(j.contains("sleep_timeout_s")) p.sleep_timeout_s = j["sleep_timeout_s"];

    ClampProfile(p);

    return p;
}

static json ProfileToJson(const LogitechHIDPP20IdleProfile& p)
{
    json j;

    j["dim_when_idle"]   = p.dim_when_idle;
    j["dim_brightness"]  = p.dim_brightness;
    j["idle_timeout_s"]  = p.idle_timeout_s;
    j["allow_sleep"]     = p.allow_sleep;
    j["sleep_timeout_s"] = p.sleep_timeout_s;

    return j;
}

void LogitechHIDPP20IdleSettings::load()
{
    json settings = ResourceManager::get()->GetSettingsManager()->GetSettings(SETTINGS_KEY);

    /*---------------------------------------------------------*\
    | Empty / missing key means the plugin is not in use.       |
    | Reset both profiles to defaults with configured=false so  |
    | the controller defers to firmware.                        |
    \*---------------------------------------------------------*/
    if(!settings.is_object() || settings.empty())
    {
        configured = false;
        on_battery = LogitechHIDPP20IdleProfile{};
        plugged_in = LogitechHIDPP20IdleProfile{};
        return;
    }

    configured = true;

    if(settings.contains("on_battery"))
    {
        on_battery = ProfileFromJson(settings["on_battery"]);
    }
    else
    {
        on_battery = LogitechHIDPP20IdleProfile{};
    }

    if(settings.contains("plugged_in"))
    {
        plugged_in = ProfileFromJson(settings["plugged_in"]);
    }
    else
    {
        plugged_in = LogitechHIDPP20IdleProfile{};
    }
}

void LogitechHIDPP20IdleSettings::save()
{
    json settings;

    settings["on_battery"] = ProfileToJson(on_battery);
    settings["plugged_in"] = ProfileToJson(plugged_in);

    SettingsManager* mgr = ResourceManager::get()->GetSettingsManager();
    mgr->SetSettings(SETTINGS_KEY, settings);
    mgr->SaveSettings();

    configured = true;
}

void LogitechHIDPP20IdleSettings::setOnBattery(const LogitechHIDPP20IdleProfile& p)
{
    on_battery = p;
    configured = true;
}

void LogitechHIDPP20IdleSettings::setPluggedIn(const LogitechHIDPP20IdleProfile& p)
{
    plugged_in = p;
    configured = true;
}
