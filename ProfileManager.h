#include "RGBController.h"

#pragma once

class ProfileManager
{
public:
    ProfileManager(std::vector<RGBController *>& control);
    ~ProfileManager();

    void UpdateProfileList();
    bool SaveProfile(std::string profile_name);
    bool LoadProfile(std::string profile_name);
    bool LoadSizeFromProfile(std::string profile_name);
    void DeleteProfile(std::string profile_name);

    std::string ppath;
    std::vector<std::string> profile_list;
    
protected:
    std::vector<RGBController *>& controllers;

private:
    bool LoadProfileWithOptions
            (
            std::string     profile_name,
            bool            load_size,
            bool            load_settings
            );
};
