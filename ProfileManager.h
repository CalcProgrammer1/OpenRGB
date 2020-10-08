#include "RGBController.h"

#pragma once

class ProfileManager
{
public:
    ProfileManager(std::vector<RGBController *>& control);
    ~ProfileManager();

    bool SaveProfile(std::string profile_name);
    bool LoadProfile(std::string profile_name);
    bool LoadSizeFromProfile(std::string profile_name);
    void DeleteProfile(std::string profile_name);

    std::vector<std::string> profile_list;
    
    std::vector<RGBController*> ProfileManager::LoadProfileToList
        (
        std::string     profile_name
        );

protected:
    std::vector<RGBController *>& controllers;

private:
    void UpdateProfileList();
    bool LoadProfileWithOptions
            (
            std::string     profile_name,
            bool            load_size,
            bool            load_settings
            );
};
