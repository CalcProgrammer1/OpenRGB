/*---------------------------------------------------------*\
| ProfileManager.cpp                                        |
|                                                           |
|   OpenRGB profile manager                                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include <cstring>
#include "ProfileManager.h"
#include "ResourceManager.h"
#include "RGBController_Dummy.h"
#include "LogManager.h"
#include "NetworkProtocol.h"
#include "filesystem.h"
#include "StringUtils.h"

#define OPENRGB_PROFILE_HEADER  "OPENRGB_PROFILE"
#define OPENRGB_PROFILE_VERSION OPENRGB_SDK_PROTOCOL_VERSION

ProfileManager::ProfileManager(const filesystem::path& config_dir)
{
    configuration_directory = config_dir;
    UpdateProfileList();
}

ProfileManager::~ProfileManager()
{

}

bool ProfileManager::SaveProfile(std::string profile_name, bool sizes)
{
    profile_name = StringUtils::remove_null_terminating_chars(profile_name);

    /*---------------------------------------------------------*\
    | Get the list of controllers from the resource manager     |
    \*---------------------------------------------------------*/
    std::vector<RGBController *> controllers = ResourceManager::get()->GetRGBControllers();

    /*---------------------------------------------------------*\
    | If a name was entered, save the profile file              |
    \*---------------------------------------------------------*/
    if(profile_name != "")
    {
        /*---------------------------------------------------------*\
        | Extension .orp - OpenRgb Profile                          |
        \*---------------------------------------------------------*/
        std::string filename = profile_name;

        /*---------------------------------------------------------*\
        | Determine file extension                                  |
        \*---------------------------------------------------------*/
        if(sizes)
        {
            filename += ".ors";
        }
        else
        {
            filename += ".orp";
        }

        /*---------------------------------------------------------*\
        | Open an output file in binary mode                        |
        \*---------------------------------------------------------*/
        filesystem::path profile_path = configuration_directory / filesystem::u8path(filename);
        std::ofstream controller_file(profile_path, std::ios::out | std::ios::binary | std::ios::trunc);

        /*---------------------------------------------------------*\
        | Write header                                              |
        | 16 bytes - "OPENRGB_PROFILE"                              |
        | 4 bytes - Version, unsigned int                           |
        \*---------------------------------------------------------*/
        unsigned int profile_version = OPENRGB_PROFILE_VERSION;
        controller_file.write(OPENRGB_PROFILE_HEADER, 16);
        controller_file.write((char *)&profile_version, sizeof(unsigned int));

        /*---------------------------------------------------------*\
        | Write controller data for each controller                 |
        \*---------------------------------------------------------*/
        for(std::size_t controller_index = 0; controller_index < controllers.size(); controller_index++)
        {
            /*-----------------------------------------------------*\
            | Ignore remote and virtual controllers when saving     |
            | sizes                                                 |
            \*-----------------------------------------------------*/
            if(sizes && (controllers[controller_index]->flags & CONTROLLER_FLAG_REMOTE
                      || controllers[controller_index]->flags & CONTROLLER_FLAG_VIRTUAL))
            {
                break;
            }

            unsigned char *controller_data = controllers[controller_index]->GetDeviceDescription(profile_version);
            unsigned int controller_size;

            memcpy(&controller_size, controller_data, sizeof(controller_size));

            controller_file.write((const char *)controller_data, controller_size);

            delete[] controller_data;
        }

        /*---------------------------------------------------------*\
        | Close the file when done                                  |
        \*---------------------------------------------------------*/
        controller_file.close();

        /*---------------------------------------------------------*\
        | Update the profile list                                   |
        \*---------------------------------------------------------*/
        UpdateProfileList();

        return(true);
    }
    else
    {
        return(false);
    }
}

void ProfileManager::SetConfigurationDirectory(const filesystem::path& directory)
{
    configuration_directory = directory;
    UpdateProfileList();
}

bool ProfileManager::LoadProfile(std::string profile_name)
{
    profile_name = StringUtils::remove_null_terminating_chars(profile_name);
    return(LoadProfileWithOptions(profile_name, false, true));
}

bool ProfileManager::LoadSizeFromProfile(std::string profile_name)
{
    profile_name = StringUtils::remove_null_terminating_chars(profile_name);
    return(LoadProfileWithOptions(profile_name, true, false));
}

std::vector<RGBController*> ProfileManager::LoadProfileToList
    (
    std::string     profile_name,
    bool            sizes
    )
{
    std::vector<RGBController*> temp_controllers;
    unsigned int                controller_size;
    unsigned int                controller_offset = 0;

    filesystem::path filename = configuration_directory / filesystem::u8path(profile_name);

    /*---------------------------------------------------------*\
    | Determine file extension                                  |
    \*---------------------------------------------------------*/
    if(sizes)
    {
        filename.concat(".ors");
    }
    else
    {
        if(filename.extension() != ".orp")
        {
            filename.concat(".orp");
        }
    }

    /*---------------------------------------------------------*\
    | Open input file in binary mode                            |
    \*---------------------------------------------------------*/
    std::ifstream controller_file(filename, std::ios::in | std::ios::binary);

    /*---------------------------------------------------------*\
    | Read and verify file header                               |
    \*---------------------------------------------------------*/
    char            profile_string[16]  = "";
    unsigned int    profile_version     = 0;

    controller_file.read(profile_string, 16);
    controller_file.read((char *)&profile_version, sizeof(unsigned int));

    /*---------------------------------------------------------*\
    | Profile version started at 1 and protocol version started |
    | at 0.  Version 1 profiles should use protocol 0, but 2 or |
    | greater should be synchronized                            |
    \*---------------------------------------------------------*/
    if(profile_version == 1)
    {
        profile_version = 0;
    }

    controller_offset += 16 + sizeof(unsigned int);
    controller_file.seekg(controller_offset);

    if(strcmp(profile_string, OPENRGB_PROFILE_HEADER) == 0)
    {
        if(profile_version <= OPENRGB_PROFILE_VERSION)
        {
            /*---------------------------------------------------------*\
            | Read controller data from file until EOF                  |
            \*---------------------------------------------------------*/
            while(!(controller_file.peek() == EOF))
            {
                controller_file.read((char *)&controller_size, sizeof(controller_size));

                unsigned char *controller_data = new unsigned char[controller_size];

                controller_file.seekg(controller_offset);

                controller_file.read((char *)controller_data, controller_size);

                RGBController_Dummy *temp_controller = new RGBController_Dummy();

                temp_controller->ReadDeviceDescription(controller_data, profile_version);

                temp_controllers.push_back(temp_controller);

                delete[] controller_data;

                controller_offset += controller_size;
                controller_file.seekg(controller_offset);
            }
        }
    }

    return(temp_controllers);
}

bool ProfileManager::LoadDeviceFromListWithOptions
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

        /*---------------------------------------------------------*\
        | Test if saved controller data matches this controller     |
        \*---------------------------------------------------------*/
        if((temp_controller_used[temp_index]    == false                            )
         &&(temp_controller->type               == load_controller->type            )
         &&(temp_controller->GetName()          == load_controller->GetName()       )
         &&(temp_controller->GetDescription()   == load_controller->GetDescription())
         &&(temp_controller->GetVersion()       == load_controller->GetVersion()    )
         &&(temp_controller->GetSerial()        == load_controller->GetSerial()     )
         &&(location_check                      == true                             ))
        {
            /*---------------------------------------------------------*\
            | Set used flag for this temp device                        |
            \*---------------------------------------------------------*/
            temp_controller_used[temp_index] = true;

            /*---------------------------------------------------------*\
            | Update zone sizes if requested                            |
            \*---------------------------------------------------------*/
            if(load_size)
            {
                if(temp_controller->zones.size() == load_controller->zones.size())
                {
                    for(std::size_t zone_idx = 0; zone_idx < temp_controller->zones.size(); zone_idx++)
                    {
                        if((temp_controller->zones[zone_idx].name       == load_controller->zones[zone_idx].name      )
                         &&(temp_controller->zones[zone_idx].type       == load_controller->zones[zone_idx].type      )
                         &&(temp_controller->zones[zone_idx].leds_min   == load_controller->zones[zone_idx].leds_min  )
                         &&(temp_controller->zones[zone_idx].leds_max   == load_controller->zones[zone_idx].leds_max  ))
                        {
                            if(temp_controller->zones[zone_idx].leds_count != load_controller->zones[zone_idx].leds_count)
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

            /*---------------------------------------------------------*\
            | Update settings if requested                              |
            \*---------------------------------------------------------*/
            if(load_settings)
            {
                /*---------------------------------------------------------*\
                | Update all modes                                          |
                \*---------------------------------------------------------*/
                if(temp_controller->modes.size() == load_controller->modes.size())
                {
                    for(std::size_t mode_index = 0; mode_index < temp_controller->modes.size(); mode_index++)
                    {
                        if((temp_controller->modes[mode_index].name             == load_controller->modes[mode_index].name          )
                         &&(temp_controller->modes[mode_index].value            == load_controller->modes[mode_index].value         )
                         &&(temp_controller->modes[mode_index].flags            == load_controller->modes[mode_index].flags         )
                         &&(temp_controller->modes[mode_index].speed_min        == load_controller->modes[mode_index].speed_min     )
                         &&(temp_controller->modes[mode_index].speed_max        == load_controller->modes[mode_index].speed_max     )
                       //&&(temp_controller->modes[mode_index].brightness_min   == load_controller->modes[mode_index].brightness_min)
                       //&&(temp_controller->modes[mode_index].brightness_max   == load_controller->modes[mode_index].brightness_max)
                         &&(temp_controller->modes[mode_index].colors_min       == load_controller->modes[mode_index].colors_min    )
                         &&(temp_controller->modes[mode_index].colors_max       == load_controller->modes[mode_index].colors_max   ))
                        {
                            load_controller->modes[mode_index].speed            = temp_controller->modes[mode_index].speed;
                            load_controller->modes[mode_index].brightness       = temp_controller->modes[mode_index].brightness;
                            load_controller->modes[mode_index].direction        = temp_controller->modes[mode_index].direction;
                            load_controller->modes[mode_index].color_mode       = temp_controller->modes[mode_index].color_mode;

                            load_controller->modes[mode_index].colors.resize(temp_controller->modes[mode_index].colors.size());

                            for(std::size_t mode_color_index = 0; mode_color_index < temp_controller->modes[mode_index].colors.size(); mode_color_index++)
                            {
                                load_controller->modes[mode_index].colors[mode_color_index] = temp_controller->modes[mode_index].colors[mode_color_index];
                            }
                        }

                    }

                    load_controller->active_mode = temp_controller->active_mode;
                }

                /*---------------------------------------------------------*\
                | Update all colors                                         |
                \*---------------------------------------------------------*/
                if(temp_controller->colors.size() == load_controller->colors.size())
                {
                    for(std::size_t color_index = 0; color_index < temp_controller->colors.size(); color_index++)
                    {
                        load_controller->colors[color_index] = temp_controller->colors[color_index];
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

    /*---------------------------------------------------------*\
    | Get the list of controllers from the resource manager     |
    \*---------------------------------------------------------*/
    std::vector<RGBController *> controllers = ResourceManager::get()->GetRGBControllers();

    /*---------------------------------------------------------*\
    | Open input file in binary mode                            |
    \*---------------------------------------------------------*/
    temp_controllers = LoadProfileToList(profile_name);

    /*---------------------------------------------------------*\
    | Set up used flag vector                                   |
    \*---------------------------------------------------------*/
    temp_controller_used.resize(temp_controllers.size());

    for(unsigned int controller_idx = 0; controller_idx < temp_controller_used.size(); controller_idx++)
    {
        temp_controller_used[controller_idx] = false;
    }

    /*---------------------------------------------------------*\
    | Loop through all controllers.  For each controller, search|
    | all saved controllers until a match is found              |
    \*---------------------------------------------------------*/
    for(std::size_t controller_index = 0; controller_index < controllers.size(); controller_index++)
    {
        bool temp_ret_val = LoadDeviceFromListWithOptions(temp_controllers, temp_controller_used, controllers[controller_index], load_size, load_settings);
        std::string current_name = controllers[controller_index]->GetName() + " @ " + controllers[controller_index]->GetLocation();
        LOG_INFO("[ProfileManager] Profile loading: %s for %s", ( temp_ret_val ? "Succeeded" : "FAILED!" ), current_name.c_str());
        ret_val |= temp_ret_val;
    }

    /*---------------------------------------------------------*\
    | Delete all temporary controllers                          |
    \*---------------------------------------------------------*/
    for(unsigned int controller_idx = 0; controller_idx < temp_controllers.size(); controller_idx++)
    {
        delete temp_controllers[controller_idx];
    }

    return(ret_val);
}

void ProfileManager::DeleteProfile(std::string profile_name)
{
    profile_name = StringUtils::remove_null_terminating_chars(profile_name);

    filesystem::path filename = configuration_directory / profile_name;
    filename.concat(".orp");

    filesystem::remove(filename);

    UpdateProfileList();
}

void ProfileManager::UpdateProfileList()
{
    profile_list.clear();

    /*---------------------------------------------------------*\
    | Load profiles by looking for .orp files in current dir    |
    \*---------------------------------------------------------*/
    for(const auto & entry : filesystem::directory_iterator(configuration_directory))
    {
        std::string filename = entry.path().filename().string();

        if(filename.find(".orp") != std::string::npos)
        {
            LOG_INFO("[ProfileManager] Found file: %s attempting to validate header", filename.c_str());

            /*---------------------------------------------------------*\
            | Open input file in binary mode                            |
            \*---------------------------------------------------------*/
            filesystem::path file_path = configuration_directory;
            file_path.append(filename);
            std::ifstream profile_file(file_path, std::ios::in | std::ios::binary);

            /*---------------------------------------------------------*\
            | Read and verify file header                               |
            \*---------------------------------------------------------*/
            char            profile_string[16];
            unsigned int    profile_version;

            profile_file.read(profile_string, 16);
            profile_file.read((char *)&profile_version, sizeof(unsigned int));

            if(strcmp(profile_string, OPENRGB_PROFILE_HEADER) == 0)
            {
                if(profile_version <= OPENRGB_PROFILE_VERSION)
                {
                    /*---------------------------------------------------------*\
                    | Add this profile to the list                              |
                    \*---------------------------------------------------------*/
                    filename.erase(filename.length() - 4);
                    profile_list.push_back(filename);

                    LOG_INFO("[ProfileManager] Valid v%i profile found for %s", profile_version, filename.c_str());
                }
                else
                {
                    LOG_WARNING("[ProfileManager] Profile %s isn't valid for current version (v%i, expected v%i at most)", filename.c_str(), profile_version, OPENRGB_PROFILE_VERSION);
                }
            }
            else
            {
                LOG_WARNING("[ProfileManager] Profile %s isn't valid: header is missing", filename.c_str());
            }

            profile_file.close();
        }
    }
}

unsigned char * ProfileManager::GetProfileListDescription()
{
    unsigned int data_ptr = 0;
    unsigned int data_size = 0;

    /*---------------------------------------------------------*\
    | Calculate data size                                       |
    \*---------------------------------------------------------*/
     unsigned short num_profiles = (unsigned short)profile_list.size();

     data_size += sizeof(data_size);
     data_size += sizeof(num_profiles);

    for(unsigned int i = 0; i < num_profiles; i++)
    {
        data_size += sizeof (unsigned short);
        data_size += (unsigned int)strlen(profile_list[i].c_str()) + 1;
    }

    /*---------------------------------------------------------*\
    | Create data buffer                                        |
    \*---------------------------------------------------------*/
    unsigned char *data_buf = new unsigned char[data_size];

    /*---------------------------------------------------------*\
    | Copy in data size                                         |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*---------------------------------------------------------*\
    | Copy in num_profiles                                      |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &num_profiles, sizeof(num_profiles));
    data_ptr += sizeof(num_profiles);

    /*---------------------------------------------------------*\
    | Copy in profile names (size+data)                         |
    \*---------------------------------------------------------*/
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
