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

using json = nlohmann::json;

class SettingsManager
{
public:
    SettingsManager();
    ~SettingsManager();

    json    GetSettings(std::string settings_key);
    void    SetSettings(std::string settings_key, json new_settings);

    void    LoadSettings(std::string filename);
    void    SaveSettings();

    
private:
    json        settings_data;
    json        settings_prototype;
    std::string settings_filename;
};
