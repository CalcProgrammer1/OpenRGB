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
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include "SettingsManager.h"
#include "LogManager.h"

SettingsManager::SettingsManager()
{
    config_found = false;
}

SettingsManager::~SettingsManager()
{

}

json SettingsManager::GetSettings(std::string settings_key)
{
    /*-----------------------------------------------------*\
    | Check to see if the key exists in the settings store  |
    | and return the settings associated with the key if it |
    | exists.  We lock the mutex to protect the value from  |
    | changing while data is being read and copy before     |
    | unlocking.                                            |
    \*-----------------------------------------------------*/
    json result;

    mutex.lock();
    if(settings_data.contains(settings_key))
    {
        result = settings_data[settings_key];
    }

    mutex.unlock();

    return result;
}

void SettingsManager::SetSettings(std::string settings_key, json new_settings)
{
    mutex.lock();
    settings_data[settings_key] = new_settings;
    mutex.unlock();
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
            LOG_ERROR("[SettingsManager] Cannot write to file: %s", e.what());
        }

        settings_file.close();
    }
    mutex.unlock();
}
