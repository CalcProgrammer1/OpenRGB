/*---------------------------------------------------------*\
| SettingsManager.h                                         |
|                                                           |
|   OpenRGB Settings Manager maintains a list of application|
|   settings in JSON format.  Other components may register |
|   settings with this class and store/load values.         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                04 Nov 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <nlohmann/json.hpp>
#include <mutex>
#include "filesystem.h"

/*---------------------------------------------------------*\
| Callback Types                                            |
\*---------------------------------------------------------*/
typedef void (*SettingsManagerCallback)(void *, unsigned int);

/*---------------------------------------------------------*\
| SettingsManager Update Reason Codes                       |
\*---------------------------------------------------------*/
enum
{
    SETTINGSMANAGER_UPDATE_REASON_SETTINGS_UPDATED,             /* Settings updated                 */
    SETTINGSMANAGER_UPDATE_REASON_SETTINGS_SCHEMA_UPDATED,      /* Settings schema updated          */
};

class SettingsManagerInterface
{
public:
    virtual nlohmann::json      GetSettings(std::string settings_key)                                   = 0;
    virtual void                ModifySettings(std::string settings_key, nlohmann::json new_settings)   = 0;
    virtual void                SetSettings(std::string settings_key, nlohmann::json new_settings)      = 0;
    virtual void                SetSettingsFromJsonString(std::string settings_json_str)                = 0;

    virtual void                LoadSettings(const filesystem::path& filename)                          = 0;
    virtual void                SaveSettings()                                                          = 0;

protected:
    virtual ~SettingsManagerInterface() {};
};

class SettingsManager: public SettingsManagerInterface
{
public:
    SettingsManager();
    ~SettingsManager();

    nlohmann::json              GetSettings(std::string settings_key);
    nlohmann::json              GetSettingsSchema(std::string settings_key);
    void                        RegisterSettingsSchema(std::string settings_key, std::string settings_title, nlohmann::json new_schema);
    void                        ModifySettings(std::string settings_key, nlohmann::json new_settings);
    void                        SetSettings(std::string settings_key, nlohmann::json new_settings);
    void                        SetSettingsFromJsonString(std::string settings_json_str);

    void                        LoadSettings(const filesystem::path& filename);
    void                        SaveSettings();

    /*-----------------------------------------------------*\
    | Callback Registration Functions                       |
    \*-----------------------------------------------------*/
    void                        RegisterSettingsManagerCallback(SettingsManagerCallback new_callback, void * new_callback_arg);
    void                        UnregisterSettingsManagerCallback(SettingsManagerCallback callback, void * callback_arg);

    void                        SignalSettingsManagerUpdate(unsigned int update_reason);

private:
    /*-----------------------------------------------------*\
    | JSON objects for settings data and schema             |
    \*-----------------------------------------------------*/
    nlohmann::json              settings_data;
    nlohmann::json              settings_schema;

    /*-----------------------------------------------------*\
    | Settings filename                                     |
    \*-----------------------------------------------------*/
    filesystem::path            settings_filename;

    /*-----------------------------------------------------*\
    | Settings mutex                                        |
    \*-----------------------------------------------------*/
    std::mutex                  mutex;

    /*-----------------------------------------------------*\
    | Configuration found flag                              |
    \*-----------------------------------------------------*/
    bool                        config_found;

    /*-----------------------------------------------------*\
    | SettingsManager Callbacks                             |
    \*-----------------------------------------------------*/
    std::vector<SettingsManagerCallback>        SettingsManagerCallbacks;
    std::vector<void *>                         SettingsManagerCallbackArgs;
    std::mutex                                  SettingsManagerCallbackMutex;
};
