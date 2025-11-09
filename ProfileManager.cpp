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
#include "PluginManagerInterface.h"
#include "ProfileManager.h"
#include "ResourceManager.h"
#include "RGBController_Dummy.h"
#include "SettingsManager.h"
#include "StringUtils.h"

#define OPENRGB_PROFILE_HEADER  "OPENRGB_PROFILE"
#define OPENRGB_PROFILE_VERSION OPENRGB_SDK_PROTOCOL_VERSION

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
}

ProfileManager::~ProfileManager()
{

}

void ProfileManager::DeleteProfile(std::string profile_name)
{
    profile_name = StringUtils::remove_null_terminating_chars(profile_name);

    filesystem::path filename = profile_directory / profile_name;
    filename.concat(".json");

    filesystem::remove(filename);

    UpdateProfileList();
}

std::string ProfileManager::GetActiveProfile()
{
    return(active_profile);
}

std::vector<RGBController*> ProfileManager::GetControllerListFromProfile(nlohmann::json profile_json)
{
    std::vector<RGBController*> temp_controllers;

    /*-----------------------------------------------------*\
    | Read list of controllers from profile                 |
    \*-----------------------------------------------------*/
    if(profile_json.contains("controllers"))
    {
        for(std::size_t controller_idx = 0; controller_idx < profile_json["controllers"].size(); controller_idx++)
        {
            RGBController_Dummy * temp_controller = new RGBController_Dummy();

            temp_controller->SetDeviceDescriptionJSON(profile_json["controllers"][controller_idx]);

            temp_controllers.push_back(temp_controller);
        }
    }

    return(temp_controllers);
}

std::vector<RGBController*> ProfileManager::GetControllerListFromSizes()
{
    /*-----------------------------------------------------*\
    | Read the sizes JSON from the file                     |
    \*-----------------------------------------------------*/
    filesystem::path    filename    = configuration_directory / "Sizes.json";
    nlohmann::json      sizes_json  = ReadProfileFileJSON(filename);

    return(GetControllerListFromProfile(sizes_json));
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

bool ProfileManager::LoadProfile(std::string profile_name)
{
    return(LoadProfileWithOptions(profile_name, false, true));
}

nlohmann::json ProfileManager::ReadProfileJSON(std::string profile_name)
{
    /*-----------------------------------------------------*\
    | Clean up the profile name                             |
    \*-----------------------------------------------------*/
    profile_name = StringUtils::remove_null_terminating_chars(profile_name);

    /*-----------------------------------------------------*\
    | File extension for v6+ profiles is .json              |
    \*-----------------------------------------------------*/
    profile_name += ".json";

    /*-----------------------------------------------------*\
    | Read the profile JSON from the file                   |
    \*-----------------------------------------------------*/
    filesystem::path profile_path = profile_directory / filesystem::u8path(profile_name);

    nlohmann::json profile_json = ReadProfileFileJSON(profile_path);

    return(profile_json);
}

bool ProfileManager::SaveProfile(std::string profile_name)
{
    /*-----------------------------------------------------*\
    | Clean up the profile name                             |
    \*-----------------------------------------------------*/
    profile_name = StringUtils::remove_null_terminating_chars(profile_name);

    /*-----------------------------------------------------*\
    | If a name was entered, save the profile file          |
    \*-----------------------------------------------------*/
    if(profile_name != "")
    {
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
        | Write controller data for each controller         |
        \*-------------------------------------------------*/
        for(std::size_t controller_index = 0; controller_index < controllers.size(); controller_index++)
        {
            /*---------------------------------------------*\
            | Read the controller data for this controller  |
            | into the profile json                         |
            \*---------------------------------------------*/
            profile_json["controllers"][controller_index] = controllers[controller_index]->GetDeviceDescriptionJSON();
        }

        /*-------------------------------------------------*\
        | Get plugin profile data if the plugin manager is  |
        | available                                         |
        \*-------------------------------------------------*/
        PluginManagerInterface* plugin_manager = ResourceManager::get()->GetPluginManager();

        if(plugin_manager != NULL)
        {
            profile_json["plugins"] = plugin_manager->OnProfileSave();
        }

        /*-------------------------------------------------*\
        | Save the profile to file from the JSON            |
        \*-------------------------------------------------*/
        SaveProfileFromJSON(profile_json);

        /*-------------------------------------------------*\
        | Update the profile list                           |
        \*-------------------------------------------------*/
        UpdateProfileList();

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

        /*-------------------------------------------------*\
        | Read the controller data for this controller into |
        | the profile json                                  |
        \*-------------------------------------------------*/
        profile_json["controllers"][controller_index] = controllers[controller_index]->GetDeviceDescriptionJSON();
    }

    controller_file << std::setw(4) << profile_json << std::endl;

    /*-----------------------------------------------------*\
    | Close the file when done                              |
    \*-----------------------------------------------------*/
    controller_file.close();

    return(true);
}

void ProfileManager::SetConfigurationDirectory(const filesystem::path& directory)
{
    configuration_directory = directory;
    profile_directory       = configuration_directory / filesystem::u8path("profiles");

    filesystem::create_directories(profile_directory);

    UpdateProfileList();
}

void ProfileManager::SetProfileListFromDescription(char * data_buf)
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
}

void ProfileManager::UpdateProfileList()
{
    profile_list.clear();

    /*-----------------------------------------------------*\
    | Load profiles by looking for .json files in profile   |
    | directory                                             |
    \*-----------------------------------------------------*/
    for(const filesystem::directory_entry &entry : filesystem::directory_iterator(profile_directory))
    {
        std::string filename = entry.path().filename().string();

        if(filename.find(".json") != std::string::npos)
        {
            LOG_INFO("[ProfileManager] Found file: %s attempting to validate header", filename.c_str());

            /*---------------------------------------------*\
            | Open input file in binary mode                |
            \*---------------------------------------------*/
            filesystem::path file_path = profile_directory;
            file_path.append(filename);

            nlohmann::json profile_json = ReadProfileFileJSON(file_path);

            if(!profile_json.empty())
            {
                profile_list.push_back(filename.erase(filename.length() - 5));
            }
        }
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
    std::vector<RGBController*>&    temp_controllers,
    std::vector<bool>&              temp_controller_used,
    RGBController*                  load_controller,
    bool                            load_size,
    bool                            load_settings
    )
{
    for(std::size_t temp_index = 0; temp_index < temp_controllers.size(); temp_index++)
    {
        RGBController *temp_controller = temp_controllers[temp_index];

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
                location_check = temp_controller->GetLocation().find(i2c_address) != std::string::npos;
            }
        }
        else
        {
            location_check = temp_controller->GetLocation() == load_controller->GetLocation();
        }

        /*-------------------------------------------------*\
        | Test if saved controller data matches this        |
        | controller                                        |
        \*-------------------------------------------------*/
        if((temp_controller_used[temp_index]    == false                            )
         &&(temp_controller->GetDeviceType()    == load_controller->GetDeviceType() )
         &&(temp_controller->GetName()          == load_controller->GetName()       )
         &&(temp_controller->GetDescription()   == load_controller->GetDescription())
         &&(temp_controller->GetVersion()       == load_controller->GetVersion()    )
         &&(temp_controller->GetSerial()        == load_controller->GetSerial()     )
         &&(location_check                      == true                             ))
        {
            /*---------------------------------------------*\
            | Set used flag for this temp device            |
            \*---------------------------------------------*/
            temp_controller_used[temp_index] = true;

            /*---------------------------------------------*\
            | Update zone sizes if requested                |
            \*---------------------------------------------*/
            if(load_size)
            {
                if(temp_controller->zones.size() == load_controller->zones.size())
                {
                    for(std::size_t zone_idx = 0; zone_idx < temp_controller->zones.size(); zone_idx++)
                    {
                        if((temp_controller->GetZoneName(zone_idx)      == load_controller->GetZoneName(zone_idx)     )
                         &&(temp_controller->GetZoneType(zone_idx)      == load_controller->GetZoneType(zone_idx)     )
                         &&(temp_controller->GetZoneLEDsMin(zone_idx)   == load_controller->GetZoneLEDsMin(zone_idx)  )
                         &&(temp_controller->GetZoneLEDsMax(zone_idx)   == load_controller->GetZoneLEDsMax(zone_idx)  ))
                        {
                            if(temp_controller->GetZoneLEDsCount(zone_idx) != load_controller->GetZoneLEDsCount(zone_idx))
                            {
                                load_controller->ResizeZone((int)zone_idx, temp_controller->zones[zone_idx].leds_count);
                            }

                            if(temp_controller->zones[zone_idx].segments.size() != load_controller->zones[zone_idx].segments.size())
                            {
                                load_controller->zones[zone_idx].segments.clear();

                                for(std::size_t segment_idx = 0; segment_idx < temp_controller->zones[zone_idx].segments.size(); segment_idx++)
                                {
                                    load_controller->zones[zone_idx].segments.push_back(temp_controller->zones[zone_idx].segments[segment_idx]);
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
                if(temp_controller->modes.size() == load_controller->modes.size())
                {
                    for(std::size_t mode_index = 0; mode_index < temp_controller->modes.size(); mode_index++)
                    {
                        if((temp_controller->GetModeName(mode_index)            == load_controller->GetModeName(mode_index)         )
                         &&(temp_controller->GetModeValue(mode_index)           == load_controller->GetModeValue(mode_index)        )
                         &&(temp_controller->GetModeFlags(mode_index)           == load_controller->GetModeFlags(mode_index)        )
                         &&(temp_controller->GetModeSpeedMin(mode_index)        == load_controller->GetModeSpeedMin(mode_index)     )
                         &&(temp_controller->GetModeSpeedMax(mode_index)        == load_controller->GetModeSpeedMax(mode_index)     )
                         &&(temp_controller->GetModeBrightnessMin(mode_index)   == load_controller->GetModeBrightnessMin(mode_index))
                         &&(temp_controller->GetModeBrightnessMax(mode_index)   == load_controller->GetModeBrightnessMax(mode_index))
                         &&(temp_controller->GetModeColorsMin(mode_index)       == load_controller->GetModeColorsMin(mode_index)    )
                         &&(temp_controller->GetModeColorsMax(mode_index)       == load_controller->GetModeColorsMax(mode_index)    ))
                        {
                            load_controller->modes[mode_index].speed            = temp_controller->modes[mode_index].speed;
                            load_controller->modes[mode_index].brightness       = temp_controller->modes[mode_index].brightness;
                            load_controller->modes[mode_index].direction        = temp_controller->modes[mode_index].direction;
                            load_controller->modes[mode_index].color_mode       = temp_controller->modes[mode_index].color_mode;

                            load_controller->modes[mode_index].colors.resize(temp_controller->modes[mode_index].colors.size());

                            for(std::size_t mode_color_index = 0; mode_color_index < temp_controller->GetModeColorsCount(mode_index); mode_color_index++)
                            {
                                load_controller->modes[mode_index].colors[mode_color_index] = temp_controller->modes[mode_index].colors[mode_color_index];
                            }
                        }
                    }

                    load_controller->active_mode = temp_controller->active_mode;
                    load_controller->UpdateMode();
                }

                /*-----------------------------------------*\
                | If color list matches, load all colors    |
                \*-----------------------------------------*/
                if(temp_controller->colors.size() == load_controller->colors.size())
                {
                    for(std::size_t color_index = 0; color_index < temp_controller->colors.size(); color_index++)
                    {
                        load_controller->colors[color_index] = temp_controller->colors[color_index];
                    }

                    load_controller->UpdateLEDs();
                }

                /*-----------------------------------------*\
                | If zone mode list matches, load all zone  |
                | modes                                     |
                \*-----------------------------------------*/
                if(temp_controller->GetZoneCount() == load_controller->GetZoneCount())
                {
                    for(std::size_t zone_idx = 0; zone_idx < temp_controller->GetZoneCount(); zone_idx++)
                    {
                        if((temp_controller->GetZoneName(zone_idx)      == load_controller->GetZoneName(zone_idx)     )
                         &&(temp_controller->GetZoneType(zone_idx)      == load_controller->GetZoneType(zone_idx)     )
                         &&(temp_controller->GetZoneLEDsMin(zone_idx)   == load_controller->GetZoneLEDsMin(zone_idx)  )
                         &&(temp_controller->GetZoneLEDsMax(zone_idx)   == load_controller->GetZoneLEDsMax(zone_idx)  )
                         &&(temp_controller->GetZoneModeCount(zone_idx) == load_controller->GetZoneModeCount(zone_idx)))
                        {
                            for(std::size_t mode_index = 0; mode_index < temp_controller->GetZoneModeCount(zone_idx); mode_index++)
                            {
                                if((temp_controller->GetZoneModeName(zone_idx, mode_index)          == load_controller->GetZoneModeName(zone_idx, mode_index)         )
                                 &&(temp_controller->GetZoneModeValue(zone_idx, mode_index)         == load_controller->GetZoneModeValue(zone_idx, mode_index)        )
                                 &&(temp_controller->GetZoneModeFlags(zone_idx, mode_index)         == load_controller->GetZoneModeFlags(zone_idx, mode_index)        )
                                 &&(temp_controller->GetZoneModeSpeedMin(zone_idx, mode_index)      == load_controller->GetZoneModeSpeedMin(zone_idx, mode_index)     )
                                 &&(temp_controller->GetZoneModeSpeedMax(zone_idx, mode_index)      == load_controller->GetZoneModeSpeedMax(zone_idx, mode_index)     )
                                 &&(temp_controller->GetZoneModeBrightnessMin(zone_idx, mode_index) == load_controller->GetZoneModeBrightnessMin(zone_idx, mode_index))
                                 &&(temp_controller->GetZoneModeBrightnessMax(zone_idx, mode_index) == load_controller->GetZoneModeBrightnessMax(zone_idx, mode_index))
                                 &&(temp_controller->GetZoneModeColorsMin(zone_idx, mode_index)     == load_controller->GetZoneModeColorsMin(zone_idx, mode_index)    )
                                 &&(temp_controller->GetZoneModeColorsMax(zone_idx, mode_index)     == load_controller->GetZoneModeColorsMax(zone_idx, mode_index)    ))
                                {
                                    load_controller->zones[zone_idx].modes[mode_index].speed        = temp_controller->zones[zone_idx].modes[mode_index].speed;
                                    load_controller->zones[zone_idx].modes[mode_index].brightness   = temp_controller->zones[zone_idx].modes[mode_index].brightness;
                                    load_controller->zones[zone_idx].modes[mode_index].direction    = temp_controller->zones[zone_idx].modes[mode_index].direction;
                                    load_controller->zones[zone_idx].modes[mode_index].color_mode   = temp_controller->zones[zone_idx].modes[mode_index].color_mode;

                                    load_controller->zones[zone_idx].modes[mode_index].colors.resize(temp_controller->zones[zone_idx].modes[mode_index].colors.size());

                                    for(std::size_t mode_color_index = 0; mode_color_index < temp_controller->GetZoneModeColorsCount(zone_idx, mode_index); mode_color_index++)
                                    {
                                        load_controller->zones[zone_idx].modes[mode_index].colors[mode_color_index] = temp_controller->zones[zone_idx].modes[mode_index].colors[mode_color_index];
                                    }
                                }
                            }

                            load_controller->SetZoneActiveMode(zone_idx, temp_controller->GetZoneActiveMode(zone_idx));
                            load_controller->UpdateZoneMode(zone_idx);
                        }
                    }
                }
            }

            return(true);
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
    std::vector<RGBController*> temp_controllers;
    std::vector<bool>           temp_controller_used;
    bool                        ret_val = false;

    /*-------------------------------------------------*\
    | Get the list of controllers from the resource     |
    | manager                                           |
    \*-------------------------------------------------*/
    std::vector<RGBController *> controllers = ResourceManager::get()->GetRGBControllers();

    nlohmann::json profile_json = ReadProfileJSON(profile_name);

    /*-------------------------------------------------*\
    | Open input file in binary mode                    |
    \*-------------------------------------------------*/
    temp_controllers = GetControllerListFromProfile(profile_json);

    /*-------------------------------------------------*\
    | Signal to plugins that a profile is about to load |
    \*-------------------------------------------------*/
    PluginManagerInterface* plugin_manager = ResourceManager::get()->GetPluginManager();

    if(plugin_manager != NULL)
    {
        plugin_manager->OnProfileAboutToLoad();
    }

    /*-------------------------------------------------*\
    | Set up used flag vector                           |
    \*-------------------------------------------------*/
    temp_controller_used.resize(temp_controllers.size());

    for(unsigned int controller_idx = 0; controller_idx < temp_controller_used.size(); controller_idx++)
    {
        temp_controller_used[controller_idx] = false;
    }

    /*-------------------------------------------------*\
    | Loop through all controllers.  For each           |
    | controller, search all saved controllers until a  |
    | match is found                                    |
    \*-------------------------------------------------*/
    for(std::size_t controller_index = 0; controller_index < controllers.size(); controller_index++)
    {
        bool temp_ret_val = LoadControllerFromListWithOptions(temp_controllers, temp_controller_used, controllers[controller_index], load_size, load_settings);
        std::string current_name = controllers[controller_index]->GetName() + " @ " + controllers[controller_index]->GetLocation();
        LOG_INFO("[ProfileManager] Profile loading: %s for %s", ( temp_ret_val ? "Succeeded" : "FAILED!" ), current_name.c_str());
        ret_val |= temp_ret_val;
    }

    /*-------------------------------------------------*\
    | Delete all temporary controllers                  |
    \*-------------------------------------------------*/
    for(unsigned int controller_idx = 0; controller_idx < temp_controllers.size(); controller_idx++)
    {
        delete temp_controllers[controller_idx];
    }

    /*-------------------------------------------------*\
    | Get plugin profile data                           |
    \*-------------------------------------------------*/
    if(plugin_manager != NULL && profile_json.contains("plugins"))
    {
        plugin_manager->OnProfileLoad(profile_json["plugins"]);
    }

    return(ret_val);
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
