#include "RGBController.h"

#pragma once

class ProfileManagerInterface
{
public:
    virtual bool SaveProfile
        (
        std::string profile_name,
        bool sizes = false
        )                                                                            = 0;
    virtual bool LoadProfile(std::string profile_name)                               = 0;
    virtual bool LoadSizeFromProfile(std::string profile_name)                       = 0;
    virtual void DeleteProfile(std::string profile_name)                             = 0;
    virtual unsigned char * GetProfileListDescription()                              = 0;

    std::vector<std::string> profile_list;

    virtual bool LoadDeviceFromListWithOptions
        (
        std::vector<RGBController*>&    temp_controllers,
        std::vector<bool>&              temp_controller_used,
        RGBController*                  load_controller,
        bool                            load_size,
        bool                            load_settings
        )                                                                            = 0;

    virtual std::vector<RGBController*> LoadProfileToList
        (
        std::string profile_name,
        bool sizes = false
        )                                                                            = 0;

    virtual void SetConfigurationDirectory(std::string directory)                    = 0;
protected:
    virtual ~ProfileManagerInterface() {};
};

class ProfileManager: public ProfileManagerInterface
{
public:
    ProfileManager(std::string config_dir);
    ~ProfileManager();

    bool SaveProfile
        (
        std::string     profile_name,
        bool            sizes = false
        );
    bool LoadProfile(std::string profile_name);
    bool LoadSizeFromProfile(std::string profile_name);
    void DeleteProfile(std::string profile_name);
    unsigned char * GetProfileListDescription();

    std::vector<std::string> profile_list;

    bool LoadDeviceFromListWithOptions
        (
        std::vector<RGBController*>&    temp_controllers,
        std::vector<bool>&              temp_controller_used,
        RGBController*                  load_controller,
        bool                            load_size,
        bool                            load_settings
        );

    std::vector<RGBController*> LoadProfileToList
        (
        std::string     profile_name,
        bool            sizes = false
        );

    void SetConfigurationDirectory(std::string directory);

private:
    std::string                         configuration_directory;

    void UpdateProfileList();
    bool LoadProfileWithOptions
            (
            std::string     profile_name,
            bool            load_size,
            bool            load_settings
            );
};
