/*---------------------------------------------------------*\
| OpenRGBPluginAPI.cpp                                      |
|                                                           |
|   Interface for OpenRGB plugins to call OpenRGB functions |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 Feb 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "OpenRGBPluginAPI.h"

OpenRGBPluginAPI::OpenRGBPluginAPI()
{
    log_manager         = ResourceManager::get()->GetLogManager();
    plugin_manager      = ResourceManager::get()->GetPluginManager();
    profile_manager     = ResourceManager::get()->GetProfileManager();
    resource_manager    = ResourceManager::get();
    settings_manager    = ResourceManager::get()->GetSettingsManager();
}

/*---------------------------------------------------------*\
| LogManager APIs                                           |
\*---------------------------------------------------------*/
void OpenRGBPluginAPI::append(const char* filename, int line, unsigned int level, const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);

    log_manager->append_va(filename, line, level, fmt, va);

    va_end(va);
}

/*---------------------------------------------------------*\
| PluginManager APIs                                        |
\*---------------------------------------------------------*/
static void CallRegisterRGBController(OpenRGBPluginAPI * this_ptr, RGBController * rgb_controller)
{
    this_ptr->RegisterRGBController(rgb_controller);
}

void OpenRGBPluginAPI::RegisterRGBControllerInThread(RGBController * rgb_controller)
{
    /*-----------------------------------------------------*\
    | To avoid deadlocks if this is called from a UI thread |
    | run the register operation in a background thread.    |
    \*-----------------------------------------------------*/
    std::thread register_thread(CallRegisterRGBController, this, rgb_controller);
    register_thread.detach();
}

void OpenRGBPluginAPI::RegisterRGBController(RGBController * rgb_controller)
{
    LOG_INFO("[PluginManager] Registering RGB controller %s", rgb_controller->GetName().c_str());

    /*-----------------------------------------------------*\
    | Mark this controller as locally owned                 |
    \*-----------------------------------------------------*/
    rgb_controller->flags &= ~CONTROLLER_FLAG_REMOTE;
    rgb_controller->flags |= CONTROLLER_FLAG_LOCAL;

    /*-----------------------------------------------------*\
    | Add the new controller to the list                    |
    \*-----------------------------------------------------*/
    rgb_controllers.push_back(rgb_controller);

    /*-----------------------------------------------------*\
    | Signal device list update in ResourceManager          |
    \*-----------------------------------------------------*/
    ResourceManager::get()->UpdateDeviceList();
}

static void CallUnregisterRGBController(OpenRGBPluginAPI * this_ptr, RGBController * rgb_controller)
{
    this_ptr->UnregisterRGBController(rgb_controller);
}

void OpenRGBPluginAPI::UnregisterRGBController(RGBController * rgb_controller)
{
    LOG_INFO("[PluginManager] Unregistering RGB controller %s", rgb_controller->GetName().c_str());

    /*-----------------------------------------------------*\
    | Clear callbacks from the controller before removal    |
    \*-----------------------------------------------------*/
    rgb_controller->ClearCallbacks();

    /*-----------------------------------------------------*\
    | Find the controller to remove and remove it from the  |
    | master list                                           |
    \*-----------------------------------------------------*/
    std::vector<RGBController*>::iterator rgb_it = std::find(rgb_controllers.begin(), rgb_controllers.end(), rgb_controller);

    if(rgb_it != rgb_controllers.end())
    {
        rgb_controllers.erase(rgb_it);
    }

    /*-----------------------------------------------------*\
    | Signal device list update in ResourceManager          |
    \*-----------------------------------------------------*/
    ResourceManager::get()->UpdateDeviceList();
}

void OpenRGBPluginAPI::UnregisterRGBControllerInThread(RGBController * rgb_controller)
{
    /*-----------------------------------------------------*\
    | To avoid deadlocks if this is called from a UI thread |
    | run the unregister operation in a background thread.  |
    \*-----------------------------------------------------*/
    std::thread unregister_thread(CallUnregisterRGBController, this, rgb_controller);
    unregister_thread.detach();
}

/*---------------------------------------------------------*\
| ProfileManager APIs                                       |
\*---------------------------------------------------------*/
void OpenRGBPluginAPI::ClearActiveProfile()
{
    profile_manager->ClearActiveProfile();
}

std::vector<std::string> OpenRGBPluginAPI::GetProfileList()
{
    return(profile_manager->GetProfileList());
}

bool OpenRGBPluginAPI::LoadProfile(std::string profile_name)
{
    return(profile_manager->LoadProfile(profile_name));
}

/*---------------------------------------------------------*\
| ResourceManager APIs                                      |
\*---------------------------------------------------------*/
filesystem::path OpenRGBPluginAPI::GetConfigurationDirectory()
{
    return(resource_manager->GetConfigurationDirectory());
}

bool OpenRGBPluginAPI::GetDetectionEnabled()
{
    return(resource_manager->GetDetectionEnabled());
}

unsigned int OpenRGBPluginAPI::GetDetectionPercent()
{
    return(resource_manager->GetDetectionPercent());
}

std::string OpenRGBPluginAPI::GetDetectionString()
{
    return(resource_manager->GetDetectionString());
}

void OpenRGBPluginAPI::WaitForDetection()
{
    resource_manager->WaitForDetection();
}

std::vector<RGBController*> & OpenRGBPluginAPI::GetRGBControllers()
{
    return(resource_manager->GetRGBControllers());
}

/*---------------------------------------------------------*\
| SettingsManager APIs                                      |
\*---------------------------------------------------------*/
nlohmann::json OpenRGBPluginAPI::GetSettings(std::string settings_key)
{
    return(settings_manager->GetSettings(settings_key));
}

void OpenRGBPluginAPI::SaveSettings()
{
    settings_manager->SaveSettings();
}

void OpenRGBPluginAPI::SetSettings(std::string settings_key, nlohmann::json new_settings)
{
    settings_manager->SetSettings(settings_key, new_settings);
}
