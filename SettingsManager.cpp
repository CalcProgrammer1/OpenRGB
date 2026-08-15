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
#include "JsonUtils.h"
#include "LogManager.h"
#include "NetworkClient.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "StringUtils.h"

/*---------------------------------------------------------*\
| SettingsManager name for log entries                      |
\*---------------------------------------------------------*/
const char* SETTINGSMANAGER = "SettingsManager";

static const std::string ui_settings_keys[4] =
{
    "Plugins",
    "Client",
};

/*---------------------------------------------------------*\
| Schema Validation Static Helper                           |
\*---------------------------------------------------------*/
static bool SettingsValueMatchesType(const json& value, const std::string& schema_type)
{
    /*-----------------------------------------------------*\
    | Map schema type strings to nlohmann::json type checks |
    \------------------------------------------------------*/
    if(schema_type == "bool")
    {
        return value.is_boolean();
    }

    if(schema_type == "integer")
    {
        return value.is_number_integer();
    }

    if(schema_type == "number")
    {
        return value.is_number();
    }

    if(schema_type == "string")
    {
        return value.is_string();
    }

    if(schema_type == "array")
    {
        return value.is_array();
    }

    if(schema_type == "object")
    {
        return value.is_object();
    }

    /*-----------------------------------------------------*\
    | Custom OpenRGB type aliases                           |
    \------------------------------------------------------*/
    if(schema_type == "language")
    {
        return value.is_string();
    }

    if(schema_type == "profile")
    {
        return value.is_object();
    }

    /*-----------------------------------------------------*\
    | Unknown type string                                   |
    \------------------------------------------------------*/
    LOG_WARNING("[%s] Unknown schema type \"%s\"", SETTINGSMANAGER, schema_type.c_str());
    return false;
}

/*---------------------------------------------------------*\
| Check if setting schema has local_only parameter          |
\*---------------------------------------------------------*/
static bool IsLocalOnlySetting(const nlohmann::json& settings_schema, const std::string& settings_key)
{
    /*-----------------------------------------------------*\
    | Check if the schema exists for this settings_key      |
    \------------------------------------------------------*/
    if(!settings_schema.contains(settings_key))
    {
        return false;
    }

    /*-----------------------------------------------------*\
    | Check if the schema has local_only set to true        |
    \------------------------------------------------------*/
    if(settings_schema[settings_key].contains("local_only"))
    {
        return settings_schema[settings_key]["local_only"].get<bool>();
    }

    return false;
}

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
    bool local_setting = IsLocalOnlySetting(settings_schema, settings_key);

    for(std::size_t settings_key_idx = 0; settings_key_idx < 4; settings_key_idx++)
    {
        if(settings_key == ui_settings_keys[settings_key_idx])
        {
            local_setting = true;
            break;
        }
    }

    if(!local_setting && ResourceManager::get()->IsLocalClient() && (ResourceManager::get()->GetLocalClient()->GetSupportsSettingsManagerAPI()))
    {
        /*-------------------------------------------------*\
        | If this is a local client, request the settings   |
        | from the server                                   |
        \*-------------------------------------------------*/
        JsonUtils::JsonParse(ResourceManager::get()->GetLocalClient()->SettingsManager_GetSettings(settings_key), result);
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

void SettingsManager::RegisterSettingsSchema(std::string settings_key, std::string settings_title, json& new_schema)
{
    RegisterSettingsSchemaComplete(settings_key, settings_title, new_schema, -1, false);
}

void SettingsManager::RegisterSettingsSchemaComplete(std::string settings_key, std::string settings_title, json& new_schema, int order, bool local_only)
{
    settings_schema[settings_key]["title"]      = settings_title;
    settings_schema[settings_key]["type"]       = "object";
    settings_schema[settings_key]["properties"].update(new_schema, true);
    settings_schema[settings_key]["local_only"] = local_only;

    if(order >= 0)
    {
        settings_schema[settings_key]["order"] = order;
    }

    SignalSettingsManagerUpdate(SETTINGSMANAGER_UPDATE_REASON_SETTINGS_SCHEMA_UPDATED);
}

void SettingsManager::RegisterSettingsSchemaLocalOnly(std::string settings_key, std::string settings_title, json& new_schema)
{
    RegisterSettingsSchemaComplete(settings_key, settings_title, new_schema, -1, true);
}

void SettingsManager::RegisterSettingsSchemaOrder(std::string settings_key, std::string settings_title, json& new_schema, int order)
{
    RegisterSettingsSchemaComplete(settings_key, settings_title, new_schema, order, false);
}

void SettingsManager::ModifySettings(std::string settings_key, json new_settings, bool from_server)
{
    bool local_setting = IsLocalOnlySetting(settings_schema, settings_key);

    for(std::size_t settings_key_idx = 0; settings_key_idx < 4; settings_key_idx++)
    {
        if(settings_key == ui_settings_keys[settings_key_idx])
        {
            local_setting = true;
            break;
        }
    }

    if(!local_setting && ResourceManager::get()->IsLocalClient() && (ResourceManager::get()->GetLocalClient()->GetSupportsSettingsManagerAPI()))
    {
        /*-------------------------------------------------*\
        | If this is a local client, request the settings   |
        | from the server                                   |
        \*-------------------------------------------------*/
        nlohmann::json settings_json;

        settings_json[settings_key] = new_settings;

        ResourceManager::get()->GetLocalClient()->SettingsManager_ModifySettings(settings_json.dump());
    }
    else if(!from_server)
    {
        mutex.lock();
        json filtered_settings = FilterSettingsAgainstSchema(settings_key, new_settings);
        settings_data[settings_key].update(filtered_settings, true);
        mutex.unlock();
    }

    SignalSettingsManagerUpdate(SETTINGSMANAGER_UPDATE_REASON_SETTINGS_UPDATED);
}

void SettingsManager::ModifySettingsFromJsonString(std::string settings_json_str, bool from_server)
{
    /*-----------------------------------------------------*\
    | Parse the JSON string                                 |
    \*-----------------------------------------------------*/
    nlohmann::json settings_json;
    JsonUtils::JsonParse(settings_json_str, settings_json);

    /*-----------------------------------------------------*\
    | Get key/value pairs from JSON, call SetSettings for   |
    | each key.  This use of `auto` is acceptable due to    |
    | how the JSON library implements iterators, the type   |
    | would change based on the library version.            |
    \*-----------------------------------------------------*/
    for(auto& element : settings_json.items())
    {
        ModifySettings(element.key(), element.value());
    }
}

void SettingsManager::SetSettings(std::string settings_key, json new_settings, bool from_server)
{
    bool local_setting = IsLocalOnlySetting(settings_schema, settings_key);

    for(std::size_t settings_key_idx = 0; settings_key_idx < 4; settings_key_idx++)
    {
        if(settings_key == ui_settings_keys[settings_key_idx])
        {
            local_setting = true;
            break;
        }
    }

    if(!local_setting && ResourceManager::get()->IsLocalClient() && (ResourceManager::get()->GetLocalClient()->GetSupportsSettingsManagerAPI()))
    {
        /*-------------------------------------------------*\
        | If this is a local client, request the settings   |
        | from the server                                   |
        \*-------------------------------------------------*/
        nlohmann::json settings_json;

        settings_json[settings_key] = new_settings;

        ResourceManager::get()->GetLocalClient()->SettingsManager_SetSettings(settings_json.dump());
    }
    else if(!from_server)
    {
        mutex.lock();
        json filtered_settings = FilterSettingsAgainstSchema(settings_key, new_settings);
        settings_data[settings_key] = filtered_settings;
        mutex.unlock();
    }

    SignalSettingsManagerUpdate(SETTINGSMANAGER_UPDATE_REASON_SETTINGS_UPDATED);
}

void SettingsManager::SetSettingsFromJsonString(std::string settings_json_str, bool from_server)
{
    /*-----------------------------------------------------*\
    | Parse the JSON string                                 |
    \*-----------------------------------------------------*/
    nlohmann::json settings_json;
    JsonUtils::JsonParse(settings_json_str, settings_json);

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

/*---------------------------------------------------------*\
| Schema Validation                                         |
\*---------------------------------------------------------*/
json SettingsManager::FilterSettingsAgainstSchema(std::string settings_key, json new_settings)
{
    json filtered;

    /*------------------------------------------------------*
    | If new_settings is not an object, there is nothing    |
    | to filter. Return an empty object.                    |
    \------------------------------------------------------*/
    if(!new_settings.is_object())
    {
        LOG_WARNING("[%s] Settings for key \"%s\" is not a JSON object, nothing will be stored", SETTINGSMANAGER, settings_key.c_str());
        return filtered;
    }

    /*-----------------------------------------------------*\
    | Check if a schema is registered for this settings_key |
    | If not, return without filtering.                     |
    \------------------------------------------------------*/
    if(!settings_schema.contains(settings_key))
    {
        return new_settings;
    }

    /*-----------------------------------------------------*\
    | Check that this schema has properties.                |
    | If not, return an empty object.                       |
    \------------------------------------------------------*/
    if(!settings_schema[settings_key].contains("properties"))
    {
        LOG_WARNING("[%s] Schema for settings key \"%s\" missing the properties key, nothing will be stored", SETTINGSMANAGER, settings_key.c_str());
        return filtered;
    }

    json& schema_properties = settings_schema[settings_key]["properties"];

    /*-----------------------------------------------------*\
    | Iterate through each entry in new_settings and check  |
    | it against the schema properties.   This use of       |
    | `auto` is acceptable due to how the JSON library      |
    | implements iterators, the type would change based on  |
    | the library version.                                  |
    \------------------------------------------------------*/
    for(auto& element : new_settings.items())
    {
        std::string key = element.key();
        json& value     = element.value();

        /*-------------------------------------------------*\
        | Check if the key exists in the schema properties  |
        \*-------------------------------------------------*/
        if(!schema_properties.contains(key))
        {
            LOG_WARNING("[%s] Settings key \"%s\" not found in schema for \"%s\", skipping", SETTINGSMANAGER, key.c_str(), settings_key.c_str());
            continue;
        }

        /*-------------------------------------------------*\
        | Check if the value type matches the schema's      |
        | declared type                                     |
        \*-------------------------------------------------*/
        if(schema_properties[key].contains("type"))
        {
            std::string schema_type = schema_properties[key]["type"];

            if(!SettingsValueMatchesType(value, schema_type))
            {
                LOG_WARNING("[%s] Settings key \"%s\" has incorrect type (expected %s), skipping", SETTINGSMANAGER, key.c_str(), schema_type.c_str());
                continue;
            }
        }

        filtered[key] = value;
    }

    return filtered;
}
