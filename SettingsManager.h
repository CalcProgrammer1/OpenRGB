/*-----------------------------------------*\
|  SettingsManager.h                        |
|                                           |
|  OpenRGB Settings Manager maintains a list|
|  of application settings in JSON format.  |
|  Other components may register settings   |
|  with this class and store/load values.   |
|                                           |
|  Adam Honse (CalcProgrammer1) 11/4/2020   |
\*-----------------------------------------*/

#pragma once

#include "json.hpp"
#include "filesystem.h"

using json = nlohmann::json;

class SettingsManagerInterface
{
public:
    virtual json GetSettings(std::string settings_key)                       = 0;
    virtual void SetSettings(std::string settings_key, json new_settings)    = 0;

    virtual void LoadSettings(const filesystem::path& filename)              = 0;
    virtual void SaveSettings()                                              = 0;

protected:
    virtual ~SettingsManagerInterface() {};
};

class SettingsManager: public SettingsManagerInterface
{
public:
    SettingsManager();
    ~SettingsManager();

    json GetSettings(std::string settings_key) override;
    void SetSettings(std::string settings_key, json new_settings) override;

    void LoadSettings(const filesystem::path& filename) override;
    void SaveSettings() override;

private:
    json             settings_data;
    json             settings_prototype;
    filesystem::path settings_filename;
};
