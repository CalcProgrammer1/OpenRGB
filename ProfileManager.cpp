/*---------------------------------------------------------*\
| ProfileManager.cpp                                        |
|                                                           |
|   OpenRGB profile manager                                 |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      09 Nov 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include <cstring>
#include "filesystem.h"
#include "LogManager.h"
#include "NetworkClient.h"
#include "NetworkProtocol.h"
#include "NetworkServer.h"
#include "PluginManagerInterface.h"
#include "ProfileManager.h"
#include "ResourceManager.h"
#include "RGBController_Dummy.h"
#include "SettingsManager.h"
#include "StringUtils.h"

#define OPENRGB_PROFILE_HEADER  "OPENRGB_PROFILE"
#define OPENRGB_PROFILE_VERSION OPENRGB_SDK_PROTOCOL_VERSION

/*---------------------------------------------------------*\
| ProfileManager name for log entries                       |
\*---------------------------------------------------------*/
const char* PROFILEMANAGER = "ProfileManager";

ProfileManager::ProfileManager(const filesystem::path& config_dir)
{
    /*-----------------------------------------------------*\
    | Initialize configuration directory and update profile |
    | list                                                  |
    \*-----------------------------------------------------*/
    SetConfigurationDirectory(config_dir);
    UpdateProfileList();

    /*-----------------------------------------------------*\
    | Read in profile manager settings and initialize any   |
    | missing settings to defaults                          |
    \*-----------------------------------------------------*/
    SettingsManager*    settings_manager            = ResourceManager::get()->GetSettingsManager();
    json                profilemanager_settings     = settings_manager->GetSettings("ProfileManager");
    bool                new_settings_keys           = false;

    if(!profilemanager_settings.contains("open_profile"))
    {
        json profile;;
        profile["enabled"]                          = false;
        profile["name"]                             = "";
        profilemanager_settings["open_profile"]     = profile;
        new_settings_keys                           = true;
    }

    if(!profilemanager_settings.contains("exit_profile"))
    {
        json profile;
        profile["enabled"]                          = false;
        profile["name"]                             = "";
        profilemanager_settings["exit_profile"]     = profile;
        new_settings_keys                           = true;
    }

    if(!profilemanager_settings.contains("resume_profile"))
    {
        json profile;
        profile["enabled"]                          = false;
        profile["name"]                             = "";
        profilemanager_settings["resume_profile"]   = profile;
        new_settings_keys                           = true;
    }

    if(!profilemanager_settings.contains("suspend_profile"))
    {
        json profile;
        profile["enabled"]                          = false;
        profile["name"]                             = "";
        profilemanager_settings["suspend_profile"]  = profile;
        new_settings_keys                           = true;
    }

    /*-----------------------------------------------------*\
    | Save the settings if new default values have been     |
    | inserted                                              |
    \*-----------------------------------------------------*/
    if(new_settings_keys)
    {
        settings_manager->SetSettings("ProfileManager", profilemanager_settings);
        settings_manager->SaveSettings();
    }

    /*-----------------------------------------------------*\
    | Initialize manually configured controllers list       |
    \*-----------------------------------------------------*/
    manually_configured_rgb_controllers = GetControllerListFromSizes();
}

ProfileManager::~ProfileManager()
{

}

void ProfileManager::ClearActiveProfile()
{
    /*-------------------------------------------------*\
    | Clear stored active profile data                  |
    \*-------------------------------------------------*/
    std::vector active_rgb_controllers_copy = active_rgb_controllers;

    active_base_color_enabled               = false;
    active_base_color                       = 0;
    active_rgb_controllers.clear();

    for(unsigned int controller_idx = 0; controller_idx < active_rgb_controllers_copy.size(); controller_idx++)
    {
        delete active_rgb_controllers_copy[controller_idx];
    }

    if(ResourceManager::get()->IsLocalClient() && ResourceManager::get()->GetLocalClient()->GetSupportsProfileManagerAPI())
    {
        ResourceManager::get()->GetLocalClient()->ProfileManager_ClearActiveProfile();
    }

    SetActiveProfile("");
}

bool ProfileManager::CompareControllers(RGBController* controller_1, RGBController* controller_2)
{
    /*-----------------------------------------------------*\
    | Do not compare location string for HID devices, as    |
    | the location string may change between runs as        |
    | devices are connected and disconnected. Also do not   |
    | compare the I2C bus number, since it is not           |
    | persistent across reboots on Linux - strip the I2C    |
    | number and compare only address.                      |
    \*-----------------------------------------------------*/
    bool location_check;

    if(controller_1->GetLocation().find("HID: ") == 0)
    {
        location_check = true;
    }
    else if(controller_1->GetLocation().find("I2C: ") == 0)
    {
        std::size_t loc = controller_1->GetLocation().rfind(", ");
        if(loc == std::string::npos)
        {
            location_check = false;
        }
        else
        {
            std::string i2c_address = controller_1->GetLocation().substr(loc + 2);
            location_check = controller_2->GetLocation().find(i2c_address) != std::string::npos;
        }
    }
    else
    {
        location_check = controller_2->GetLocation() == controller_1->GetLocation();
    }

    /*-----------------------------------------------------*\
    | Compare top-level controller information              |
    \*-----------------------------------------------------*/
    if((controller_1->GetDeviceType()    != controller_2->GetDeviceType() )
    || (controller_1->GetName()          != controller_2->GetName()       )
    || (controller_1->GetDescription()   != controller_2->GetDescription())
    || (controller_1->GetVersion()       != controller_2->GetVersion()    )
    || (controller_1->GetSerial()        != controller_2->GetSerial()     )
    || (location_check                   != true                          ))
    {
        return(false);
    }

    /*-----------------------------------------------------*\
    | Compare modes                                         |
    \*-----------------------------------------------------*/
    if(controller_1->modes.size() != controller_2->modes.size())
    {
        return(false);
    }
    else
    {
        for(std::size_t mode_index = 0; mode_index < controller_1->modes.size(); mode_index++)
        {
            if((controller_1->GetModeName(mode_index)            != controller_2->GetModeName(mode_index)         )
            || (controller_1->GetModeValue(mode_index)           != controller_2->GetModeValue(mode_index)        )
            || (controller_1->GetModeFlags(mode_index)           != controller_2->GetModeFlags(mode_index)        )
            || (controller_1->GetModeSpeedMin(mode_index)        != controller_2->GetModeSpeedMin(mode_index)     )
            || (controller_1->GetModeSpeedMax(mode_index)        != controller_2->GetModeSpeedMax(mode_index)     )
            || (controller_1->GetModeBrightnessMin(mode_index)   != controller_2->GetModeBrightnessMin(mode_index))
            || (controller_1->GetModeBrightnessMax(mode_index)   != controller_2->GetModeBrightnessMax(mode_index))
            || (controller_1->GetModeColorsMin(mode_index)       != controller_2->GetModeColorsMin(mode_index)    )
            || (controller_1->GetModeColorsMax(mode_index)       != controller_2->GetModeColorsMax(mode_index)    ))
            {
                return(false);
            }
        }
    }
    /*-----------------------------------------------------*\
    | Compare zones                                         |
    \*-----------------------------------------------------*/
    if(controller_1->zones.size() != controller_2->zones.size())
    {
        return(false);
    }
    else
    {
        for(std::size_t zone_index = 0; zone_index < controller_1->zones.size(); zone_index++)
        {
            bool    check_zone_name = true;
            bool    check_zone_type = true;

            /*---------------------------------------------*\
            | Do not check zone name if manually configured |
            \*---------------------------------------------*/
            if((controller_1->GetZoneFlags(zone_index) & ZONE_FLAG_MANUALLY_CONFIGURED_NAME)
            || (controller_2->GetZoneFlags(zone_index) & ZONE_FLAG_MANUALLY_CONFIGURED_NAME))
            {
                check_zone_name = false;
            }

            /*---------------------------------------------*\
            | Do not check zone type if manually configured |
            \*---------------------------------------------*/
            if((controller_1->GetZoneFlags(zone_index) & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE)
            || (controller_2->GetZoneFlags(zone_index) & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE))
            {
                check_zone_type = false;
            }

            if((check_zone_name && (controller_1->GetZoneName(zone_index)      != controller_2->GetZoneName(zone_index)     ))
            || (check_zone_type && (controller_1->GetZoneType(zone_index)      != controller_2->GetZoneType(zone_index)     ))
            ||                     (controller_1->GetZoneLEDsMin(zone_index)   != controller_2->GetZoneLEDsMin(zone_index)  )
            ||                     (controller_1->GetZoneLEDsMax(zone_index)   != controller_2->GetZoneLEDsMax(zone_index)  )
            ||                     (controller_1->GetZoneModeCount(zone_index) != controller_2->GetZoneModeCount(zone_index)))
            {
                return(false);
            }

            if(controller_1->GetZoneModeCount(zone_index) != controller_2->GetZoneModeCount(zone_index))
            {
                return(false);
            }
            else
            {
                for(std::size_t mode_index = 0; mode_index < controller_1->GetZoneModeCount(zone_index); mode_index++)
                {
                    if((controller_1->GetZoneModeName(zone_index, mode_index)          != controller_2->GetZoneModeName(zone_index, mode_index)         )
                    || (controller_1->GetZoneModeValue(zone_index, mode_index)         != controller_2->GetZoneModeValue(zone_index, mode_index)        )
                    || (controller_1->GetZoneModeFlags(zone_index, mode_index)         != controller_2->GetZoneModeFlags(zone_index, mode_index)        )
                    || (controller_1->GetZoneModeSpeedMin(zone_index, mode_index)      != controller_2->GetZoneModeSpeedMin(zone_index, mode_index)     )
                    || (controller_1->GetZoneModeSpeedMax(zone_index, mode_index)      != controller_2->GetZoneModeSpeedMax(zone_index, mode_index)     )
                    || (controller_1->GetZoneModeBrightnessMin(zone_index, mode_index) != controller_2->GetZoneModeBrightnessMin(zone_index, mode_index))
                    || (controller_1->GetZoneModeBrightnessMax(zone_index, mode_index) != controller_2->GetZoneModeBrightnessMax(zone_index, mode_index))
                    || (controller_1->GetZoneModeColorsMin(zone_index, mode_index)     != controller_2->GetZoneModeColorsMin(zone_index, mode_index)    )
                    || (controller_1->GetZoneModeColorsMax(zone_index, mode_index)     != controller_2->GetZoneModeColorsMax(zone_index, mode_index)    ))
                    {
                        return(false);
                    }
                }
            }
        }
    }

    return(true);
}

void ProfileManager::DeleteProfile(std::string profile_name)
{
    if(ResourceManager::get()->IsLocalClient() && (ResourceManager::get()->GetLocalClient()->GetSupportsProfileManagerAPI()))
    {
        ResourceManager::get()->GetLocalClient()->ProfileManager_DeleteProfile(profile_name);
    }
    else
    {
        filesystem::path filename = profile_directory / profile_name;
        filename.concat(".json");

        filesystem::remove(filename);
    }

    UpdateProfileList();
}

std::string ProfileManager::GetActiveProfile()
{
    return(active_profile);
}

std::vector<RGBController*> ProfileManager::GetControllerListFromProfileJson(nlohmann::json profile_json)
{
    std::vector<RGBController*> profile_controllers;

    /*-----------------------------------------------------*\
    | Read list of controllers from profile                 |
    \*-----------------------------------------------------*/
    if(profile_json.contains("controllers"))
    {
        for(std::size_t controller_idx = 0; controller_idx < profile_json["controllers"].size(); controller_idx++)
        {
            RGBController_Dummy * profile_controller = new RGBController_Dummy();

            RGBController::SetDeviceDescriptionJSON(profile_json["controllers"][controller_idx], profile_controller);

            profile_controllers.push_back(profile_controller);
        }
    }

    return(profile_controllers);
}

std::vector<RGBController*> ProfileManager::GetControllerListFromProfileName(std::string profile_name)
{
    return(GetControllerListFromProfileJson(ReadProfileJSON(profile_name)));
}

std::vector<RGBController*> ProfileManager::GetControllerListFromSizes()
{
    /*-----------------------------------------------------*\
    | Read the sizes JSON from the file                     |
    \*-----------------------------------------------------*/
    filesystem::path    filename    = configuration_directory / "Sizes.json";
    nlohmann::json      sizes_json  = ReadProfileFileJSON(filename);

    return(GetControllerListFromProfileJson(sizes_json));
}

std::vector<std::string> ProfileManager::GetProfileList()
{
    return(profile_list);
}

unsigned char * ProfileManager::GetProfileListDescription()
{
    unsigned int data_ptr = 0;
    unsigned int data_size = 0;

    /*-----------------------------------------------------*\
    | Calculate data size                                   |
    \*-----------------------------------------------------*/
     unsigned short num_profiles = (unsigned short)profile_list.size();

     data_size += sizeof(data_size);
     data_size += sizeof(num_profiles);

    for(unsigned int i = 0; i < num_profiles; i++)
    {
        data_size += sizeof (unsigned short);
        data_size += (unsigned int)strlen(profile_list[i].c_str()) + 1;
    }

    /*-----------------------------------------------------*\
    | Create data buffer                                    |
    \*-----------------------------------------------------*/
    unsigned char *data_buf = new unsigned char[data_size];

    /*-----------------------------------------------------*\
    | Copy in data size                                     |
    \*-----------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*-----------------------------------------------------*\
    | Copy in num_profiles                                  |
    \*-----------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &num_profiles, sizeof(num_profiles));
    data_ptr += sizeof(num_profiles);

    /*-----------------------------------------------------*\
    | Copy in profile names (size+data)                     |
    \*-----------------------------------------------------*/
    for(unsigned int i = 0; i < num_profiles; i++)
    {
        unsigned short name_len = (unsigned short)strlen(profile_list[i].c_str()) + 1;

        memcpy(&data_buf[data_ptr], &name_len, sizeof(name_len));
        data_ptr += sizeof(name_len);

        strcpy((char *)&data_buf[data_ptr], profile_list[i].c_str());
        data_ptr += name_len;
    }

    return(data_buf);
}

bool ProfileManager::LoadAutoProfileExit()
{
    return(LoadAutoProfile("exit_profile"));
}

bool ProfileManager::LoadAutoProfileOpen()
{
    return(LoadAutoProfile("open_profile"));
}

bool ProfileManager::LoadAutoProfileResume()
{
    return(LoadAutoProfile("resume_profile"));
}

bool ProfileManager::LoadAutoProfileSuspend()
{
    return(LoadAutoProfile("suspend_profile"));
}

bool ProfileManager::LoadControllerActiveProfile(RGBController* load_controller)
{
    return(LoadControllerFromListWithOptions(active_rgb_controllers, load_controller, false, true));
}

bool ProfileManager::LoadControllerConfiguration(RGBController* load_controller)
{
    return(LoadControllerFromListWithOptions(manually_configured_rgb_controllers, load_controller, true, false));
}

bool ProfileManager::LoadProfile(std::string profile_name)
{
    if(ResourceManager::get()->IsLocalClient() && (ResourceManager::get()->GetLocalClient()->GetSupportsProfileManagerAPI()))
    {
        ResourceManager::get()->GetLocalClient()->ProfileManager_LoadProfile(profile_name);

        return(true);
    }
    else
    {
        bool success = false;

        success = LoadProfileWithOptions(profile_name, false, true);

        return(success);
    }
}

void ProfileManager::OnProfileAboutToLoad()
{
    /*-------------------------------------------------*\
    | Signal to plugins that a profile is about to load |
    \*-------------------------------------------------*/
    PluginManagerInterface* plugin_manager = ResourceManager::get()->GetPluginManager();

    if(plugin_manager != NULL)
    {
        plugin_manager->OnProfileAboutToLoad();
    }

    SignalProfileManagerUpdate(PROFILEMANAGER_UPDATE_REASON_PROFILE_ABOUT_TO_LOAD);
}

void ProfileManager::OnProfileLoaded(std::string profile_json_string)
{
    nlohmann::json profile_json;

    profile_json = nlohmann::json::parse(profile_json_string);

    /*-------------------------------------------------*\
    | Get plugin profile data                           |
    \*-------------------------------------------------*/
    PluginManagerInterface* plugin_manager = ResourceManager::get()->GetPluginManager();

    if(plugin_manager != NULL && profile_json.contains("plugins"))
    {
        plugin_manager->OnProfileLoad(profile_json["plugins"]);
    }
}

void ProfileManager::RegisterProfileManagerCallback(ProfileManagerCallback new_callback, void * new_callback_arg)
{
    ProfileManagerCallbackMutex.lock();

    for(size_t idx = 0; idx < ProfileManagerCallbacks.size(); idx++)
    {
        if(ProfileManagerCallbacks[idx] == new_callback && ProfileManagerCallbackArgs[idx] == new_callback_arg)
        {
            ProfileManagerCallbackMutex.unlock();

            LOG_TRACE("[%s] Tried to register an already registered ProfileManager callback, skipping.  Total callbacks registered: %d", PROFILEMANAGER, ProfileManagerCallbacks.size());

            return;
        }
    }

    ProfileManagerCallbacks.push_back(new_callback);
    ProfileManagerCallbackArgs.push_back(new_callback_arg);

    ProfileManagerCallbackMutex.unlock();

    LOG_TRACE("[%s] Registered ProfileManager callback.  Total callbacks registered: %d", PROFILEMANAGER, ProfileManagerCallbacks.size());
}

void ProfileManager::UnregisterProfileManagerCallback(ProfileManagerCallback callback, void * callback_arg)
{
    ProfileManagerCallbackMutex.lock();

    for(size_t idx = 0; idx < ProfileManagerCallbacks.size(); idx++)
    {
        if(ProfileManagerCallbacks[idx] == callback && ProfileManagerCallbackArgs[idx] == callback_arg)
        {
            ProfileManagerCallbacks.erase(ProfileManagerCallbacks.begin() + idx);
            ProfileManagerCallbackArgs.erase(ProfileManagerCallbackArgs.begin() + idx);
        }
    }

    ProfileManagerCallbackMutex.unlock();

    LOG_TRACE("[%s] Unregistered ProfileManager callback.  Total callbacks registered: %d", PROFILEMANAGER, ProfileManagerCallbackArgs.size());
}

nlohmann::json ProfileManager::ReadProfileJSON(std::string profile_name)
{
    nlohmann::json profile_json;

    if(ResourceManager::get()->IsLocalClient() && (ResourceManager::get()->GetLocalClient()->GetSupportsProfileManagerAPI()))
    {
        profile_json = nlohmann::json::parse(ResourceManager::get()->GetLocalClient()->ProfileManager_DownloadProfile(profile_name));
    }
    else
    {
        /*-------------------------------------------------*\
        | File extension for v6+ profiles is .json          |
        \*-------------------------------------------------*/
        profile_name += ".json";

        /*-------------------------------------------------*\
        | Read the profile JSON from the file               |
        \*-------------------------------------------------*/
        filesystem::path profile_path = profile_directory / filesystem::u8path(profile_name);

        profile_json = ReadProfileFileJSON(profile_path);
    }

    return(profile_json);
}

bool ProfileManager::SaveProfile(std::string profile_name)
{
    /*-----------------------------------------------------*\
    | If a name was entered, save the profile file          |
    \*-----------------------------------------------------*/
    if(profile_name != "")
    {
        /*-------------------------------------------------*\
        | Get the existing profile JSON data                |
        \*-------------------------------------------------*/
        nlohmann::json existing_profile_json = ReadProfileJSON(profile_name);

        /*-------------------------------------------------*\
        | Read the existing profile's base color settings   |
        \*-------------------------------------------------*/
        RGBColor    base_color          = 0;
        bool        base_color_enabled  = false;

        if(existing_profile_json.contains("base_color"))
        {
            base_color          = existing_profile_json["base_color"];
            base_color_enabled  = true;
        }

        /*-------------------------------------------------*\
        | Read the existing profile's controller states     |
        \*-------------------------------------------------*/
        std::vector<RGBController*> existing_controllers = GetControllerListFromProfileJson(existing_profile_json);

        /*-------------------------------------------------*\
        | If updating an existing profile, only save        |
        | controller states if the existing profile had one |
        | or more saved controller states already.          |
        | If creating a new profile, always save controller |
        | states.                                           |
        \*-------------------------------------------------*/
        bool save_controllers = (existing_profile_json.empty()) || (!existing_profile_json.empty() && !existing_controllers.empty());

        /*-------------------------------------------------*\
        | Get the list of controllers from the resource     |
        | manager                                           |
        \*-------------------------------------------------*/
        std::vector<RGBController *> controllers = ResourceManager::get()->GetRGBControllers();

        /*-------------------------------------------------*\
        | Start filling in profile json data                |
        \*-------------------------------------------------*/
        nlohmann::json profile_json;

        profile_json["profile_version"] = OPENRGB_PROFILE_VERSION;
        profile_json["profile_name"]    = profile_name;

        /*-------------------------------------------------*\
        | Write base color data if enabled                  |
        \*-------------------------------------------------*/
        if(base_color_enabled)
        {
            profile_json["base_color"] = base_color;
        }

        /*-------------------------------------------------*\
        | Write controller data for each controller if      |
        | enabled                                           |
        \*-------------------------------------------------*/
        if(save_controllers)
        {
            std::size_t new_profile_controller_index = 0;

            for(std::size_t controller_index = 0; controller_index < controllers.size(); controller_index++)
            {
                /*-----------------------------------------*\
                | Read the controller data for this         |
                | controller into the profile json          |
                \*-----------------------------------------*/
                profile_json["controllers"][new_profile_controller_index] = RGBController::GetDeviceDescriptionJSON(controllers[controller_index]);
                new_profile_controller_index++;
            }

            /*---------------------------------------------*\
            | Loop through the previously saved controllers |
            | and add any controllers that were previously  |
            | saved but not in the current controllers list |
            \*---------------------------------------------*/
            for(std::size_t existing_controller_index = 0; existing_controller_index < existing_controllers.size(); existing_controller_index++)
            {
                bool found = false;

                for(std::size_t controller_index = 0; controller_index < controllers.size(); controller_index++)
                {
                    if(ProfileManager::CompareControllers(existing_controllers[existing_controller_index], controllers[controller_index]))
                    {
                        found = true;
                        break;
                    }
                }

                if(!found)
                {
                /*-----------------------------------------*\
                | Read the controller data for this         |
                | controller into the profile json          |
                \*-----------------------------------------*/
                    profile_json["controllers"][new_profile_controller_index] = RGBController::GetDeviceDescriptionJSON(manually_configured_rgb_controllers[existing_controller_index]);
                    new_profile_controller_index++;
                }
            }
        }

        /*-------------------------------------------------*\
        | Get plugin profile data if the plugin manager is  |
        | available.  If updating existing profile, only    |
        | update the plugins saved in that profile.         |
        | Otherwise, save all plugins.                      |
        \*-------------------------------------------------*/
        PluginManagerInterface* plugin_manager = ResourceManager::get()->GetPluginManager();

        if(plugin_manager != NULL)
        {
            if(existing_profile_json.empty())
            {
                profile_json["plugins"] = plugin_manager->OnProfileSave();
            }
            else if(existing_profile_json.contains("plugins"))
            {
                std::vector<std::string> plugins_to_save;

                for(unsigned int plugin_idx = 0; plugin_idx < plugin_manager->GetPluginCount(); plugin_idx++)
                {
                    if(profile_json["plugins"].contains(plugin_manager->GetPluginName(plugin_idx)))
                    {
                        plugins_to_save.push_back(plugin_manager->GetPluginName(plugin_idx));
                    }
                }

                profile_json["plugins"] = plugin_manager->OnProfileSave(plugins_to_save);
            }
        }

        if(ResourceManager::get()->IsLocalClient() && (ResourceManager::get()->GetLocalClient()->GetSupportsProfileManagerAPI()))
        {
            /*---------------------------------------------*\
            | Upload the profile to the server              |
            \*---------------------------------------------*/
            ResourceManager::get()->GetLocalClient()->ProfileManager_UploadProfile(profile_json.dump());
        }
        else
        {
            /*---------------------------------------------*\
            | Save the profile to file from the JSON        |
            \*---------------------------------------------*/
            SaveProfileFromJSON(profile_json);
        }

        return(true);
    }
    else
    {
        return(false);
    }
}

bool ProfileManager::SaveProfileCustom(std::string profile_name, std::vector<RGBController*> controllers, RGBColor base_color, bool base_color_enabled, std::vector<std::string> enabled_plugins)
{
    /*-----------------------------------------------------*\
    | If a name was entered, save the profile file          |
    \*-----------------------------------------------------*/
    if(profile_name != "")
    {
        /*-------------------------------------------------*\
        | Start filling in profile json data                |
        \*-------------------------------------------------*/
        nlohmann::json profile_json;

        profile_json["profile_version"] = OPENRGB_PROFILE_VERSION;
        profile_json["profile_name"]    = profile_name;

        /*-------------------------------------------------*\
        | Write base color data if enabled                  |
        \*-------------------------------------------------*/
        if(base_color_enabled)
        {
            profile_json["base_color"] = base_color;
        }

        /*-------------------------------------------------*\
        | Write controller data for each controller         |
        \*-------------------------------------------------*/
        for(std::size_t controller_index = 0; controller_index < controllers.size(); controller_index++)
        {
            /*---------------------------------------------*\
            | Read the controller data for this controller  |
            | into the profile json                         |
            \*---------------------------------------------*/
            profile_json["controllers"][controller_index] = RGBController::GetDeviceDescriptionJSON(controllers[controller_index]);
        }

        /*-------------------------------------------------*\
        | Get plugin profile data if the plugin manager is  |
        | available                                         |
        \*-------------------------------------------------*/
        if(enabled_plugins.size() > 0)
        {
            PluginManagerInterface* plugin_manager = ResourceManager::get()->GetPluginManager();

            if(plugin_manager != NULL)
            {
                profile_json["plugins"] = plugin_manager->OnProfileSave(enabled_plugins);
            }
        }

        if(ResourceManager::get()->IsLocalClient() && (ResourceManager::get()->GetLocalClient()->GetSupportsProfileManagerAPI()))
        {
            /*---------------------------------------------*\
            | Upload the profile to the server              |
            \*---------------------------------------------*/
            ResourceManager::get()->GetLocalClient()->ProfileManager_UploadProfile(profile_json.dump());
        }
        else
        {
            /*---------------------------------------------*\
            | Save the profile to file from the JSON        |
            \*---------------------------------------------*/
            SaveProfileFromJSON(profile_json);
        }

        return(true);
    }
    else
    {
        return(false);
    }
}

bool ProfileManager::SaveProfileFromJSON(nlohmann::json profile_json)
{
    if(profile_json.contains("profile_name"))
    {
        std::string profile_filename = profile_json["profile_name"];

        profile_filename.append(".json");

        /*-------------------------------------------------*\
        | Open an output file in the profile directory      |
        \*-------------------------------------------------*/
        filesystem::path profile_path = profile_directory / profile_filename;
        std::ofstream profile_file(profile_path, std::ios::out );

        /*-------------------------------------------------*\
        | Write the JSON data to the file                   |
        \*-------------------------------------------------*/
        profile_file << std::setw(4) << profile_json << std::endl;

        /*-------------------------------------------------*\
        | Close the file when done                          |
        \*-------------------------------------------------*/
        profile_file.close();

        /*-------------------------------------------------*\
        | Update the profile list                           |
        \*-------------------------------------------------*/
        UpdateProfileList();

        SetActiveProfile(profile_json["profile_name"]);

        return(true);
    }
    else
    {
        return(false);
    }
}

bool ProfileManager::SaveSizes()
{
    /*-----------------------------------------------------*\
    | Get the list of controllers from the resource manager |
    \*-----------------------------------------------------*/
    std::vector<RGBController *> controllers = ResourceManager::get()->GetRGBControllers();

    /*-----------------------------------------------------*\
    | Open an output file in the profile directory          |
    \*-----------------------------------------------------*/
    filesystem::path profile_path = configuration_directory / "Sizes.json";
    std::ofstream controller_file(profile_path, std::ios::out );

    /*-----------------------------------------------------*\
    | Start filling in profile json data                    |
    \*-----------------------------------------------------*/
    nlohmann::json profile_json;

    profile_json["profile_version"] = OPENRGB_PROFILE_VERSION;
    profile_json["profile_name"]    = "Sizes";

    /*-----------------------------------------------------*\
    | Write controller data for each controller             |
    \*-----------------------------------------------------*/
    std::size_t new_saved_controller_index = 0;

    for(std::size_t controller_index = 0; controller_index < controllers.size(); controller_index++)
    {
        /*-------------------------------------------------*\
        | Ignore remote and virtual controllers when saving |
        | sizes                                             |
        \*-------------------------------------------------*/
        if(controllers[controller_index]->GetFlags() & CONTROLLER_FLAG_REMOTE
        || controllers[controller_index]->GetFlags() & CONTROLLER_FLAG_VIRTUAL)
        {
            break;
        }

        for(std::size_t zone_index = 0; zone_index < controllers[controller_index]->GetZoneCount(); zone_index++)
        {
            if(controllers[controller_index]->GetZoneFlags(zone_index) & ZONE_FLAGS_MANUALLY_CONFIGURED)
            {
            /*---------------------------------------------*\
            | Read the controller data for this controller  |
            | into the profile json if manually configured  |
            \*---------------------------------------------*/
            profile_json["controllers"][new_saved_controller_index] = RGBController::GetDeviceDescriptionJSON(controllers[controller_index]);
            new_saved_controller_index++;

            break;
            }
        }
    }

    /*-----------------------------------------------------*\
    | Loop through the previously saved sizes and add any   |
    | controllers that were previously saved but not in the |
    | current controllers list                              |
    \*-----------------------------------------------------*/
    for(std::size_t old_saved_controller_index = 0; old_saved_controller_index < manually_configured_rgb_controllers.size(); old_saved_controller_index++)
    {
        bool found = false;

        for(std::size_t controller_index = 0; controller_index < controllers.size(); controller_index++)
        {
            if(ProfileManager::CompareControllers(manually_configured_rgb_controllers[old_saved_controller_index], controllers[controller_index]))
            {
                found = true;
                break;
            }
        }

        if(!found)
        {
            /*---------------------------------------------*\
            | Read the controller data for this controller  |
            | into the profile json                         |
            \*---------------------------------------------*/
            profile_json["controllers"][new_saved_controller_index] = RGBController::GetDeviceDescriptionJSON(manually_configured_rgb_controllers[old_saved_controller_index]);
            new_saved_controller_index++;
        }
    }

    controller_file << std::setw(4) << profile_json << std::endl;

    /*-----------------------------------------------------*\
    | Close the file when done                              |
    \*-----------------------------------------------------*/
    controller_file.close();

    /*-----------------------------------------------------*\
    | Reinitialize manually configured controllers list     |
    \*-----------------------------------------------------*/
    manually_configured_rgb_controllers = GetControllerListFromSizes();

    return(true);
}

void ProfileManager::SetActiveProfile(std::string profile_name)
{
    active_profile = profile_name;

    NetworkServer* server = ResourceManager::get()->GetServer();

    if(server)
    {
        server->SendRequest_ProfileManager_ActiveProfileChanged(active_profile);
    }

    SignalProfileManagerUpdate(PROFILEMANAGER_UPDATE_REASON_ACTIVE_PROFILE_CHANGED);
}

void ProfileManager::SetConfigurationDirectory(const filesystem::path& directory)
{
    configuration_directory = directory;
    profile_directory       = configuration_directory / filesystem::u8path("profiles");

    filesystem::create_directories(profile_directory);

    /*-----------------------------------------------------*\
    | Reload profile list                                   |
    \*-----------------------------------------------------*/
    UpdateProfileList();

    /*-----------------------------------------------------*\
    | Reinitialize manually configured controllers list     |
    \*-----------------------------------------------------*/
    manually_configured_rgb_controllers = GetControllerListFromSizes();
}

void ProfileManager::SetProfileListFromDescription(unsigned int /*data_size*/, char * data_buf)
{
    unsigned int   data_ptr     = sizeof(unsigned int);
    unsigned short num_profiles = 0;

    /*-----------------------------------------------------*\
    | Clear the profile list                                |
    \*-----------------------------------------------------*/
    profile_list.clear();

    /*-----------------------------------------------------*\
    | Copy in num_profiles                                  |
    \*-----------------------------------------------------*/
    memcpy(&num_profiles, &data_buf[data_ptr], sizeof(num_profiles));
    data_ptr += sizeof(num_profiles);

    /*-----------------------------------------------------*\
    | Copy in profile names (size+data)                     |
    \*-----------------------------------------------------*/
    for(unsigned int i = 0; i < num_profiles; i++)
    {
        unsigned short name_len = 0;

        memcpy(&name_len, &data_buf[data_ptr], sizeof(name_len));
        data_ptr += sizeof(name_len);

        profile_list.push_back((char *)&data_buf[data_ptr]);
        data_ptr += name_len;
    }

    SignalProfileManagerUpdate(PROFILEMANAGER_UPDATE_REASON_PROFILE_LIST_UPDATED);
}

void ProfileManager::SignalProfileManagerUpdate(unsigned int update_reason)
{
    NetworkServer* server = ResourceManager::get()->GetServer();

    if(server)
    {
        server->SignalProfileManagerUpdate(update_reason);
    }

    ProfileManagerCallbackMutex.lock();

    for(std::size_t callback_idx = 0; callback_idx < ProfileManagerCallbacks.size(); callback_idx++)
    {
        ProfileManagerCallbacks[callback_idx](ProfileManagerCallbackArgs[callback_idx], update_reason);
    }

    ProfileManagerCallbackMutex.unlock();

    LOG_TRACE("[%s] ProfileManager update signalled: %d", PROFILEMANAGER, update_reason);
}

void ProfileManager::UpdateProfileList()
{
    if(ResourceManager::get()->IsLocalClient() && (ResourceManager::get()->GetLocalClient()->GetSupportsProfileManagerAPI()))
    {
        ResourceManager::get()->GetLocalClient()->ProfileManager_GetProfileList();
        ResourceManager::get()->GetLocalClient()->ProfileManager_GetActiveProfile();
    }
    else
    {
        profile_list.clear();

        /*-------------------------------------------------*\
        | Load profiles by looking for .json files in       |
        | profile directory                                 |
        \*-------------------------------------------------*/
        for(const filesystem::directory_entry &entry : filesystem::directory_iterator(profile_directory))
        {
            std::string filename = entry.path().filename().string();

            if(filename.find(".json") != std::string::npos)
            {
                LOG_INFO("[ProfileManager] Found file: %s attempting to validate header", filename.c_str());

                /*-----------------------------------------*\
                | Open input file in binary mode            |
                \*-----------------------------------------*/
                filesystem::path file_path = profile_directory;
                file_path.append(filename);

                nlohmann::json profile_json = ReadProfileFileJSON(file_path);

                if(!profile_json.empty())
                {
                    profile_list.push_back(filename.erase(filename.length() - 5));
                }
            }
        }

        /*---------------------------------------------------------*\
        | Sort the profiles list                                    |
        \*---------------------------------------------------------*/
        std::sort(profile_list.begin(), profile_list.end());

        SignalProfileManagerUpdate(PROFILEMANAGER_UPDATE_REASON_PROFILE_LIST_UPDATED);
    }
}

/*---------------------------------------------------------*\
| Private functions                                         |
\*---------------------------------------------------------*/
bool ProfileManager::LoadAutoProfile(std::string setting_name)
{
    /*-----------------------------------------------------*\
    | Read in profile manager settings and check for the    |
    | given setting name                                    |
    \*-----------------------------------------------------*/
    json        profilemanager_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("ProfileManager");
    std::string profile_name;

    if(profilemanager_settings.contains(setting_name))
    {
        if(profilemanager_settings[setting_name].contains("name") && profilemanager_settings[setting_name].contains("enabled") && profilemanager_settings[setting_name]["enabled"] == true)
        {
            profile_name                = profilemanager_settings[setting_name]["name"];
        }
    }

    /*-----------------------------------------------------*\
    | Load the profile if it is valid                       |
    \*-----------------------------------------------------*/
    if(!profile_name.empty())
    {
        return(LoadProfile(profile_name));
    }
    else
    {
        return(false);
    }
}

bool ProfileManager::LoadControllerFromListWithOptions
    (
    std::vector<RGBController*>&    profile_controllers,
    RGBController*                  load_controller,
    bool                            load_size,
    bool                            load_settings
    )
{
    for(std::size_t temp_index = 0; temp_index < profile_controllers.size(); temp_index++)
    {
        RGBController *profile_controller = profile_controllers[temp_index];

        /*---------------------------------------------------------*\
        | Do not compare location string for HID devices, as the    |
        | location string may change between runs as devices are    |
        | connected and disconnected. Also do not compare the I2C   |
        | bus number, since it is not persistent across reboots     |
        | on Linux - strip the I2C number and compare only address. |
        \*---------------------------------------------------------*/
        bool location_check;

        if(load_controller->GetLocation().find("HID: ") == 0)
        {
            location_check = true;
        }
        else if(load_controller->GetLocation().find("I2C: ") == 0)
        {
            std::size_t loc = load_controller->GetLocation().rfind(", ");
            if(loc == std::string::npos)
            {
                location_check = false;
            }
            else
            {
                std::string i2c_address = load_controller->GetLocation().substr(loc + 2);
                location_check = profile_controller->GetLocation().find(i2c_address) != std::string::npos;
            }
        }
        else
        {
            location_check = profile_controller->GetLocation() == load_controller->GetLocation();
        }

        /*-------------------------------------------------*\
        | Test if saved controller data matches this        |
        | controller                                        |
        \*-------------------------------------------------*/
        if((profile_controller->GetDeviceType()    == load_controller->GetDeviceType() )
         &&(profile_controller->GetName()          == load_controller->GetName()       )
         &&(profile_controller->GetDescription()   == load_controller->GetDescription())
         &&(profile_controller->GetVersion()       == load_controller->GetVersion()    )
         &&(profile_controller->GetSerial()        == load_controller->GetSerial()     )
         &&(location_check                         == true                             ))
        {
            /*---------------------------------------------*\
            | Update zone sizes if requested                |
            \*---------------------------------------------*/
            if(load_size)
            {
                if(profile_controller->zones.size() == load_controller->zones.size())
                {
                    for(std::size_t zone_idx = 0; zone_idx < profile_controller->zones.size(); zone_idx++)
                    {
                        bool    check_zone_name = true;
                        bool    check_zone_type = true;

                        /*---------------------------------*\
                        | Do not check zone name if         |
                        | manually configured               |
                        \*---------------------------------*/
                        if((profile_controller->GetZoneFlags(zone_idx) & ZONE_FLAG_MANUALLY_CONFIGURED_NAME)
                        || (load_controller->GetZoneFlags(zone_idx) & ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME))
                        {
                            check_zone_name = false;
                        }

                        /*---------------------------------*\
                        | Do not check zone type if         |
                        | manually configured               |
                        \*---------------------------------*/
                        if((profile_controller->GetZoneFlags(zone_idx) & ZONE_FLAG_MANUALLY_CONFIGURED_TYPE)
                        || (load_controller->GetZoneFlags(zone_idx) & ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE))
                        {
                            check_zone_type = false;
                        }

                        if((!check_zone_name || (profile_controller->GetZoneName(zone_idx)      == load_controller->GetZoneName(zone_idx)     ))
                        && (!check_zone_type || (profile_controller->GetZoneType(zone_idx)      == load_controller->GetZoneType(zone_idx)     ))
                        &&                      (profile_controller->GetZoneLEDsMin(zone_idx)   == load_controller->GetZoneLEDsMin(zone_idx)  )
                        &&                      (profile_controller->GetZoneLEDsMax(zone_idx)   == load_controller->GetZoneLEDsMax(zone_idx)  ))
                        {
                            load_controller->ConfigureZone(zone_idx, profile_controller->zones[zone_idx]);

                            if(profile_controller->GetZoneFlags(zone_idx) & ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS)
                            {
                                load_controller->zones[zone_idx].segments.clear();

                                for(std::size_t segment_idx = 0; segment_idx < profile_controller->zones[zone_idx].segments.size(); segment_idx++)
                                {
                                    load_controller->zones[zone_idx].segments.push_back(profile_controller->zones[zone_idx].segments[segment_idx]);
                                }
                            }
                        }
                    }
                }
            }

            /*---------------------------------------------*\
            | Update settings if requested                  |
            \*---------------------------------------------*/
            if(load_settings)
            {
                /*-----------------------------------------*\
                | If mode list matches, load all modes      |
                \*-----------------------------------------*/
                if(profile_controller->modes.size() == load_controller->modes.size())
                {
                    for(std::size_t mode_index = 0; mode_index < profile_controller->modes.size(); mode_index++)
                    {
                        if((profile_controller->GetModeName(mode_index)            == load_controller->GetModeName(mode_index)         )
                         &&(profile_controller->GetModeValue(mode_index)           == load_controller->GetModeValue(mode_index)        )
                         &&(profile_controller->GetModeFlags(mode_index)           == load_controller->GetModeFlags(mode_index)        )
                         &&(profile_controller->GetModeSpeedMin(mode_index)        == load_controller->GetModeSpeedMin(mode_index)     )
                         &&(profile_controller->GetModeSpeedMax(mode_index)        == load_controller->GetModeSpeedMax(mode_index)     )
                         &&(profile_controller->GetModeBrightnessMin(mode_index)   == load_controller->GetModeBrightnessMin(mode_index))
                         &&(profile_controller->GetModeBrightnessMax(mode_index)   == load_controller->GetModeBrightnessMax(mode_index))
                         &&(profile_controller->GetModeColorsMin(mode_index)       == load_controller->GetModeColorsMin(mode_index)    )
                         &&(profile_controller->GetModeColorsMax(mode_index)       == load_controller->GetModeColorsMax(mode_index)    ))
                        {
                            load_controller->modes[mode_index].speed            = profile_controller->modes[mode_index].speed;
                            load_controller->modes[mode_index].brightness       = profile_controller->modes[mode_index].brightness;
                            load_controller->modes[mode_index].direction        = profile_controller->modes[mode_index].direction;
                            load_controller->modes[mode_index].color_mode       = profile_controller->modes[mode_index].color_mode;

                            load_controller->modes[mode_index].colors.resize(profile_controller->modes[mode_index].colors.size());

                            for(std::size_t mode_color_index = 0; mode_color_index < profile_controller->GetModeColorsCount(mode_index); mode_color_index++)
                            {
                                load_controller->modes[mode_index].colors[mode_color_index] = profile_controller->modes[mode_index].colors[mode_color_index];
                            }
                        }
                    }

                    load_controller->active_mode = profile_controller->active_mode;
                    load_controller->UpdateMode();
                }

                /*-----------------------------------------*\
                | If color list matches, load all colors    |
                \*-----------------------------------------*/
                if(profile_controller->colors.size() == load_controller->colors.size())
                {
                    for(std::size_t color_index = 0; color_index < profile_controller->colors.size(); color_index++)
                    {
                        load_controller->colors[color_index] = profile_controller->colors[color_index];
                    }

                    load_controller->UpdateLEDs();
                }

                /*-----------------------------------------*\
                | If zone mode list matches, load all zone  |
                | modes                                     |
                \*-----------------------------------------*/
                if(profile_controller->GetZoneCount() == load_controller->GetZoneCount())
                {
                    for(std::size_t zone_idx = 0; zone_idx < profile_controller->GetZoneCount(); zone_idx++)
                    {
                        if((profile_controller->GetZoneName(zone_idx)      == load_controller->GetZoneName(zone_idx)     )
                         &&(profile_controller->GetZoneType(zone_idx)      == load_controller->GetZoneType(zone_idx)     )
                         &&(profile_controller->GetZoneLEDsMin(zone_idx)   == load_controller->GetZoneLEDsMin(zone_idx)  )
                         &&(profile_controller->GetZoneLEDsMax(zone_idx)   == load_controller->GetZoneLEDsMax(zone_idx)  )
                         &&(profile_controller->GetZoneModeCount(zone_idx) == load_controller->GetZoneModeCount(zone_idx)))
                        {
                            for(std::size_t mode_index = 0; mode_index < profile_controller->GetZoneModeCount(zone_idx); mode_index++)
                            {
                                if((profile_controller->GetZoneModeName(zone_idx, mode_index)          == load_controller->GetZoneModeName(zone_idx, mode_index)         )
                                 &&(profile_controller->GetZoneModeValue(zone_idx, mode_index)         == load_controller->GetZoneModeValue(zone_idx, mode_index)        )
                                 &&(profile_controller->GetZoneModeFlags(zone_idx, mode_index)         == load_controller->GetZoneModeFlags(zone_idx, mode_index)        )
                                 &&(profile_controller->GetZoneModeSpeedMin(zone_idx, mode_index)      == load_controller->GetZoneModeSpeedMin(zone_idx, mode_index)     )
                                 &&(profile_controller->GetZoneModeSpeedMax(zone_idx, mode_index)      == load_controller->GetZoneModeSpeedMax(zone_idx, mode_index)     )
                                 &&(profile_controller->GetZoneModeBrightnessMin(zone_idx, mode_index) == load_controller->GetZoneModeBrightnessMin(zone_idx, mode_index))
                                 &&(profile_controller->GetZoneModeBrightnessMax(zone_idx, mode_index) == load_controller->GetZoneModeBrightnessMax(zone_idx, mode_index))
                                 &&(profile_controller->GetZoneModeColorsMin(zone_idx, mode_index)     == load_controller->GetZoneModeColorsMin(zone_idx, mode_index)    )
                                 &&(profile_controller->GetZoneModeColorsMax(zone_idx, mode_index)     == load_controller->GetZoneModeColorsMax(zone_idx, mode_index)    ))
                                {
                                    load_controller->zones[zone_idx].modes[mode_index].speed        = profile_controller->zones[zone_idx].modes[mode_index].speed;
                                    load_controller->zones[zone_idx].modes[mode_index].brightness   = profile_controller->zones[zone_idx].modes[mode_index].brightness;
                                    load_controller->zones[zone_idx].modes[mode_index].direction    = profile_controller->zones[zone_idx].modes[mode_index].direction;
                                    load_controller->zones[zone_idx].modes[mode_index].color_mode   = profile_controller->zones[zone_idx].modes[mode_index].color_mode;

                                    load_controller->zones[zone_idx].modes[mode_index].colors.resize(profile_controller->zones[zone_idx].modes[mode_index].colors.size());

                                    for(std::size_t mode_color_index = 0; mode_color_index < profile_controller->GetZoneModeColorsCount(zone_idx, mode_index); mode_color_index++)
                                    {
                                        load_controller->zones[zone_idx].modes[mode_index].colors[mode_color_index] = profile_controller->zones[zone_idx].modes[mode_index].colors[mode_color_index];
                                    }
                                }
                            }

                            load_controller->SetZoneActiveMode(zone_idx, profile_controller->GetZoneActiveMode(zone_idx));
                            load_controller->UpdateZoneMode(zone_idx);
                        }
                    }
                }
            }

            return(true);
        }
    }

    /*-----------------------------------------------------*\
    | If no saved controller state in the profile matched   |
    | this controller, apply the base color if it is        |
    | enabled                                               |
    \*-----------------------------------------------------*/
    if(load_settings && active_base_color_enabled)
    {
        load_controller->SetCustomMode();

        if(load_controller->GetModeColorMode(load_controller->GetActiveMode()) == MODE_COLORS_PER_LED)
        {
            load_controller->SetAllColors(active_base_color);
            load_controller->UpdateLEDs();
        }
        else if(load_controller->GetModeColorMode(load_controller->GetActiveMode()) == MODE_COLORS_MODE_SPECIFIC)
        {
            load_controller->SetModeColor(load_controller->GetActiveMode(), 0, active_base_color);
            load_controller->UpdateMode();
        }
    }

    return(false);
}

bool ProfileManager::LoadProfileWithOptions
    (
    std::string     profile_name,
    bool            load_size,
    bool            load_settings
    )
{
    /*-------------------------------------------------*\
    | Clear stored active profile data                  |
    \*-------------------------------------------------*/
    std::vector active_rgb_controllers_copy = active_rgb_controllers;

    active_base_color_enabled               = false;
    active_base_color                       = 0;
    active_rgb_controllers.clear();

    for(unsigned int controller_idx = 0; controller_idx < active_rgb_controllers_copy.size(); controller_idx++)
    {
        delete active_rgb_controllers_copy[controller_idx];
    }

    /*-------------------------------------------------*\
    | Get JSON data for given profile name              |
    \*-------------------------------------------------*/
    nlohmann::json profile_json = ReadProfileJSON(profile_name);

    /*-------------------------------------------------*\
    | Load the controller state data for this profile   |
    | into the active profile data                      |
    \*-------------------------------------------------*/
    active_rgb_controllers = GetControllerListFromProfileJson(profile_json);

    /*-------------------------------------------------*\
    | Load the base color data for this profile into    |
    | the active profile data                           |
    \*-------------------------------------------------*/
    if(profile_json.contains("base_color"))
    {
        active_base_color           = profile_json["base_color"];
        active_base_color_enabled   = true;
    }
    else
    {
        active_base_color           = 0;
        active_base_color_enabled   = false;
    }

    /*-------------------------------------------------*\
    | Signal that a profile is about to load            |
    \*-------------------------------------------------*/
    PluginManagerInterface* plugin_manager = ResourceManager::get()->GetPluginManager();

    if(plugin_manager != NULL)
    {
        plugin_manager->OnProfileAboutToLoad();
    }

    NetworkServer* server = ResourceManager::get()->GetServer();

    if(server)
    {
        server->ProfileManager_ProfileAboutToLoad();
    }

    /*-------------------------------------------------*\
    | Get the list of controllers from the resource     |
    | manager                                           |
    \*-------------------------------------------------*/
    std::vector<RGBController *> controllers = ResourceManager::get()->GetRGBControllers();

    /*-------------------------------------------------*\
    | Loop through all controllers.  For each           |
    | controller, search all saved controllers until a  |
    | match is found                                    |
    \*-------------------------------------------------*/
    for(std::size_t controller_index = 0; controller_index < controllers.size(); controller_index++)
    {
        LoadControllerFromListWithOptions(active_rgb_controllers, controllers[controller_index], load_size, load_settings);
    }

    /*-------------------------------------------------*\
    | Get plugin profile data                           |
    \*-------------------------------------------------*/
    if(plugin_manager != NULL && profile_json.contains("plugins"))
    {
        plugin_manager->OnProfileLoad(profile_json["plugins"]);
    }

    /*-------------------------------------------------*\
    | Notify local client                               |
    \*-------------------------------------------------*/
    if(server)
    {
        server->SendRequest_ProfileManager_ProfileLoaded(profile_json.dump());
    }

    /*-------------------------------------------------*\
    | Update active profile                             |
    \*-------------------------------------------------*/
    SetActiveProfile(profile_name);

    if(server)
    {
        server->SendRequest_ProfileManager_ActiveProfileChanged(active_profile);
    }

    return(true);
}

nlohmann::json ProfileManager::ReadProfileFileJSON(filesystem::path profile_filepath)
{
    std::ifstream   profile_file(profile_filepath, std::ios::in);
    nlohmann::json  profile_json;

    /*-------------------------------------------------*\
    | Read settings into JSON store                     |
    \*-------------------------------------------------*/
    if(profile_file)
    {
        try
        {
            profile_file >> profile_json;
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
            LOG_ERROR("[ProfileManager] JSON parsing failed: %s", e.what());

            profile_json.clear();
        }
    }

    profile_file.close();

    return(profile_json);
}
