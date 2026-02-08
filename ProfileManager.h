/*---------------------------------------------------------*\
| ProfileManager.h                                          |
|                                                           |
|   OpenRGB profile manager                                 |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      09 Nov 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "filesystem.h"

/*---------------------------------------------------------*\
| Callback Types                                            |
\*---------------------------------------------------------*/
typedef void (*ProfileManagerCallback)(void *, unsigned int);

/*---------------------------------------------------------*\
| ProfileManager Update Reason Codes                        |
\*---------------------------------------------------------*/
enum
{
    PROFILEMANAGER_UPDATE_REASON_PROFILE_LIST_UPDATED,          /* Profile list updated             */
    PROFILEMANAGER_UPDATE_REASON_ACTIVE_PROFILE_CHANGED,        /* Active profile changed           */
    PROFILEMANAGER_UPDATE_REASON_PROFILE_ABOUT_TO_LOAD,         /* Profile about to load            */
};

class ProfileManagerInterface
{
public:
    virtual void                        ClearActiveProfile()                                            = 0;

    virtual void                        DeleteProfile(std::string profile_name)                         = 0;

    virtual std::string                 GetActiveProfile()                                              = 0;
    virtual std::vector<RGBController*> GetControllerListFromProfile(nlohmann::json profile_json)       = 0;
    virtual std::vector<RGBController*> GetControllerListFromSizes()                                    = 0;
    virtual std::vector<std::string>    GetProfileList()                                                = 0;
    virtual unsigned char *             GetProfileListDescription()                                     = 0;

    virtual bool                        LoadControllerFromListWithOptions
                                        (
                                        std::vector<RGBController*>&    temp_controllers,
                                        std::vector<bool>&              temp_controller_used,
                                        RGBController*                  load_controller,
                                        bool                            load_size,
                                        bool                            load_settings
                                        )                                                               = 0;

    virtual bool                        LoadProfile(std::string profile_name)                           = 0;

    virtual nlohmann::json              ReadProfileJSON(std::string profile_name)                       = 0;

    virtual bool                        SaveProfile(std::string profile_name)                           = 0;
    virtual bool                        SaveProfileFromJSON(nlohmann::json profile_json)                = 0;
    virtual bool                        SaveSizes()                                                     = 0;

    virtual void                        SetConfigurationDirectory(const filesystem::path& directory)    = 0;

    virtual void                        SetProfileListFromDescription(char * data_buf)                  = 0;

    virtual void                        UpdateProfileList()                                             = 0;

protected:
    virtual ~ProfileManagerInterface() {};
};

class ProfileManager: public ProfileManagerInterface
{
public:
    ProfileManager(const filesystem::path& config_dir);
    ~ProfileManager();

    void                        ClearActiveProfile();

    void                        DeleteProfile(std::string profile_name);

    std::string                 GetActiveProfile();
    std::vector<RGBController*> GetControllerListFromProfile(nlohmann::json profile_json);
    std::vector<RGBController*> GetControllerListFromSizes();
    std::vector<std::string>    GetProfileList();
    unsigned char *             GetProfileListDescription();

    bool                        LoadAutoProfileExit();
    bool                        LoadAutoProfileOpen();
    bool                        LoadAutoProfileResume();
    bool                        LoadAutoProfileSuspend();

    bool                        LoadControllerFromListWithOptions
                                    (
                                    std::vector<RGBController*>&    temp_controllers,
                                    std::vector<bool>&              temp_controller_used,
                                    RGBController*                  load_controller,
                                    bool                            load_size,
                                    bool                            load_settings
                                    );

    bool                        LoadProfile(std::string profile_name);

    void                        OnProfileAboutToLoad();
    void                        OnProfileLoaded(std::string profile_json_string);

    /*-----------------------------------------------------*\
    | Callback Registration Functions                       |
    \*-----------------------------------------------------*/
    void                        RegisterProfileManagerCallback(ProfileManagerCallback new_callback, void * new_callback_arg);
    void                        UnregisterProfileManagerCallback(ProfileManagerCallback callback, void * callback_arg);


    nlohmann::json              ReadProfileJSON(std::string profile_name);

    bool                        SaveProfile(std::string profile_name);
    bool                        SaveProfileFromJSON(nlohmann::json profile_json);
    bool                        SaveSizes();

    void                        SetActiveProfile(std::string profile_name);
    void                        SetConfigurationDirectory(const filesystem::path& directory);

    void                        SetProfileListFromDescription(char * data_buf);

    void                        SignalProfileManagerUpdate(unsigned int update_reason);

    void                        UpdateProfileList();

private:
    /*-----------------------------------------------------*\
    | List of available profiles                            |
    \*-----------------------------------------------------*/
    std::vector<std::string>    profile_list;

    /*-----------------------------------------------------*\
    | Active profile string                                 |
    \*-----------------------------------------------------*/
    std::string                 active_profile;

    /*-----------------------------------------------------*\
    | Profile paths                                         |
    \*-----------------------------------------------------*/
    filesystem::path            configuration_directory;
    filesystem::path            profile_directory;

    /*-----------------------------------------------------*\
    | ProfileManager Callbacks                              |
    \*-----------------------------------------------------*/
    std::vector<ProfileManagerCallback>         ProfileManagerCallbacks;
    std::vector<void *>                         ProfileManagerCallbackArgs;
    std::mutex                                  ProfileManagerCallbackMutex;

    /*-----------------------------------------------------*\
    | Private functions                                     |
    \*-----------------------------------------------------*/
    bool                        LoadAutoProfile(std::string setting_name);

    bool                        LoadProfileWithOptions
                                    (
                                    std::string     profile_name,
                                    bool            load_size,
                                    bool            load_settings
                                    );

    nlohmann::json              ReadProfileFileJSON(filesystem::path profile_filepath);
};
