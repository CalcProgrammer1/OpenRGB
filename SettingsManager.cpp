/*---------------------------------------------------------*\
| SettingsManager.cpp                                       |
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

#include <fstream>
#include <iostream>
#include "LogManager.h"
#include "NetworkClient.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "StringUtils.h"

/*---------------------------------------------------------*\
| SettingsManager name for log entries                      |
\*---------------------------------------------------------*/
const char* SETTINGSMANAGER = "SettingsManager";

static const std::string ui_settings_keys[7] =
{
    "UserInterface",
    "AutoStart",
    "Theme",
    "Plugins",
    "Client",
    "LogManager",
};

SettingsManager::SettingsManager()
{
    config_found = false;
}

SettingsManager::~SettingsManager()
{

}

json SettingsManager::GetSettings(std::string settings_key)
{
    json result;
    bool ui_settings_key = false;

    for(std::size_t settings_key_idx = 0; settings_key_idx < 7; settings_key_idx++)
    {
        if(settings_key == ui_settings_keys[settings_key_idx])
        {
            ui_settings_key = true;
            break;
        }
    }

    if(!ui_settings_key && ResourceManager::get()->IsLocalClient() && (ResourceManager::get()->GetLocalClient()->GetSupportsSettingsManagerAPI()))
    {
        /*-------------------------------------------------*\
        | If this is a local client, request the settings   |
        | from the server                                   |
        \*-------------------------------------------------*/
        try
        {
            result = nlohmann::json::parse(ResourceManager::get()->GetLocalClient()->SettingsManager_GetSettings(settings_key));
        }
        catch(...)
        {
        }
    }
    else
    {
        /*-------------------------------------------------*\
        | Check to see if the key exists in the settings    |
        | store and return the settings associated with the |
        | key if it exists.  We lock the mutex to protect   |
        | the value from changing while data is being read  |
        | and copy before unlocking.                        |
        \*-------------------------------------------------*/
        mutex.lock();
        if(settings_data.contains(settings_key))
        {
            result = settings_data[settings_key];
        }
        mutex.unlock();
    }

    return result;
}

json SettingsManager::GetSettingsSchema(std::string settings_key)
{
    if(settings_key == "")
    {
        return(settings_schema);
    }
    else if(settings_schema.contains(settings_key) && settings_schema[settings_key].contains("properties"))
    {
        return(settings_schema[settings_key]["properties"]);
    }
    else
    {
        nlohmann::json empty;
        return(empty);
    }
}

void SettingsManager::RegisterSettingsSchema(std::string settings_key, std::string settings_title, json new_schema)
{
    settings_schema[settings_key]["title"] = settings_title;
    settings_schema[settings_key]["type"]  = "object";
    settings_schema[settings_key]["properties"].update(new_schema, true);

    SignalSettingsManagerUpdate(SETTINGSMANAGER_UPDATE_REASON_SETTINGS_SCHEMA_UPDATED);
}

void SettingsManager::ModifySettings(std::string settings_key, json new_settings)
{
    bool ui_settings_key = false;

    for(std::size_t settings_key_idx = 0; settings_key_idx < 7; settings_key_idx++)
    {
        if(settings_key == ui_settings_keys[settings_key_idx])
        {
            ui_settings_key = true;
            break;
        }
    }

    if(!ui_settings_key && ResourceManager::get()->IsLocalClient() && (ResourceManager::get()->GetLocalClient()->GetSupportsSettingsManagerAPI()))
    {
        /*-------------------------------------------------*\
        | If this is a local client, request the settings   |
        | from the server                                   |
        \*-------------------------------------------------*/
        nlohmann::json settings_json;

        settings_json[settings_key] = new_settings;

        ResourceManager::get()->GetLocalClient()->SettingsManager_SetSettings(settings_json.dump());
    }
    else
    {
        mutex.lock();
        settings_data[settings_key].update(new_settings, true);
        mutex.unlock();
    }

    SignalSettingsManagerUpdate(SETTINGSMANAGER_UPDATE_REASON_SETTINGS_UPDATED);
}

void SettingsManager::SetSettings(std::string settings_key, json new_settings)
{
    bool ui_settings_key = false;

    for(std::size_t settings_key_idx = 0; settings_key_idx < 7; settings_key_idx++)
    {
        if(settings_key == ui_settings_keys[settings_key_idx])
        {
            ui_settings_key = true;
            break;
        }
    }

    if(!ui_settings_key && ResourceManager::get()->IsLocalClient() && (ResourceManager::get()->GetLocalClient()->GetSupportsSettingsManagerAPI()))
    {
        /*-------------------------------------------------*\
        | If this is a local client, request the settings   |
        | from the server                                   |
        \*-------------------------------------------------*/
        nlohmann::json settings_json;

        settings_json[settings_key] = new_settings;

        ResourceManager::get()->GetLocalClient()->SettingsManager_SetSettings(settings_json.dump());
    }
    else
    {
        mutex.lock();
        settings_data[settings_key] = new_settings;
        mutex.unlock();
    }

    SignalSettingsManagerUpdate(SETTINGSMANAGER_UPDATE_REASON_SETTINGS_UPDATED);
}

void SettingsManager::SetSettingsFromJsonString(std::string settings_json_str)
{
    /*-----------------------------------------------------*\
    | Parse the JSON string                                 |
    \*-----------------------------------------------------*/
    nlohmann::json settings_json;

    try
    {
        settings_json = nlohmann::json::parse(settings_json_str);
    }
    catch(...)
    {
    }

    /*-----------------------------------------------------*\
    | Get key/value pairs from JSON, call SetSettings for   |
    | each key.  This use of `auto` is acceptable due to    |
    | how the JSON library implements iterators, the type   |
    | would change based on the library version.            |
    \*-----------------------------------------------------*/
    for(auto& element : settings_json.items())
    {
        SetSettings(element.key(), element.value());
    }
}

void SettingsManager::LoadSettings(const filesystem::path& filename)
{
    /*-----------------------------------------------------*\
    | Clear any stored settings before loading              |
    \*-----------------------------------------------------*/
    mutex.lock();

    settings_data.clear();

    /*-----------------------------------------------------*\
    | Store settings filename, so we can save to it later   |
    \*-----------------------------------------------------*/
    settings_filename = filename;

    /*-----------------------------------------------------*\
    | Open input file in binary mode                        |
    \*-----------------------------------------------------*/
    config_found = filesystem::exists(filename);
    if(config_found)
    {
        std::ifstream settings_file(settings_filename, std::ios::in | std::ios::binary);

        /*-------------------------------------------------*\
        | Read settings into JSON store                     |
        \*-------------------------------------------------*/
        if(settings_file)
        {
            try
            {
                settings_file >> settings_data;
            }
            catch(const std::exception& e)
            {
                /*-----------------------------------------*\
                | If an exception was caught, that means    |
                | the JSON parsing failed.  Clear out any   |
                | data in the store as it is corrupt.  We   |
                | could attempt a reload for backup         |
                | location                                  |
                \*-----------------------------------------*/
                LOG_ERROR("[SettingsManager] JSON parsing failed: %s", e.what());

                settings_data.clear();
            }
        }

        settings_file.close();
    }

    mutex.unlock();
}

void SettingsManager::SaveSettings()
{
    if(ResourceManager::get()->IsLocalClient() && (ResourceManager::get()->GetLocalClient()->GetSupportsSettingsManagerAPI()))
    {
        /*-------------------------------------------------*\
        | If this is a local client, save the settings on   |
        | the server                                        |
        \*-------------------------------------------------*/
        ResourceManager::get()->GetLocalClient()->SettingsManager_SaveSettings();
    }

    mutex.lock();
    std::ofstream settings_file(settings_filename, std::ios::out | std::ios::binary);

    if(settings_file)
    {
        try
        {
            settings_file << settings_data.dump(4);
        }
        catch(const std::exception& e)
        {
            LOG_ERROR("[%s] Cannot write to file: %s", SETTINGSMANAGER, e.what());
        }

        settings_file.close();
    }
    mutex.unlock();
}

/*---------------------------------------------------------*\
| Callback Registration Functions                           |
\*---------------------------------------------------------*/
void SettingsManager::RegisterSettingsManagerCallback(SettingsManagerCallback new_callback, void * new_callback_arg)
{
    SettingsManagerCallbackMutex.lock();

    for(size_t idx = 0; idx < SettingsManagerCallbacks.size(); idx++)
    {
        if(SettingsManagerCallbackArgs[idx] == new_callback && SettingsManagerCallbackArgs[idx] == new_callback_arg)
        {
            SettingsManagerCallbackMutex.unlock();

            LOG_TRACE("[%s] Tried to register an already registered SettingsManager callback, skipping.  Total callbacks registered: %d", SETTINGSMANAGER, SettingsManagerCallbacks.size());

            return;
        }
    }

    SettingsManagerCallbacks.push_back(new_callback);
    SettingsManagerCallbackArgs.push_back(new_callback_arg);

    SettingsManagerCallbackMutex.unlock();

    LOG_TRACE("[%s] Registered SettingsManager callback.  Total callbacks registered: %d", SETTINGSMANAGER, SettingsManagerCallbacks.size());
}

void SettingsManager::UnregisterSettingsManagerCallback(SettingsManagerCallback callback, void * callback_arg)
{
    SettingsManagerCallbackMutex.lock();

    for(size_t idx = 0; idx < SettingsManagerCallbacks.size(); idx++)
    {
        if(SettingsManagerCallbacks[idx] == callback && SettingsManagerCallbackArgs[idx] == callback_arg)
        {
            SettingsManagerCallbacks.erase(SettingsManagerCallbacks.begin() + idx);
            SettingsManagerCallbackArgs.erase(SettingsManagerCallbackArgs.begin() + idx);
        }
    }

    SettingsManagerCallbackMutex.unlock();

    LOG_TRACE("[%s] Unregistered SettingsManager callback.  Total callbacks registered: %d", SETTINGSMANAGER, SettingsManagerCallbackArgs.size());
}

void SettingsManager::SignalSettingsManagerUpdate(unsigned int update_reason)
{
//    NetworkServer* server = ResourceManager::get()->GetServer();
//
//    if(server)
//    {
//        server->SignalProfileManagerUpdate(update_reason);
//    }

    SettingsManagerCallbackMutex.lock();

    for(std::size_t callback_idx = 0; callback_idx < SettingsManagerCallbacks.size(); callback_idx++)
    {
        SettingsManagerCallbacks[callback_idx](SettingsManagerCallbackArgs[callback_idx], update_reason);
    }

    SettingsManagerCallbackMutex.unlock();

    LOG_TRACE("[%s] SettingsManager update signalled: %d", SETTINGSMANAGER, update_reason);
}
