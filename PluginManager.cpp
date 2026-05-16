/*---------------------------------------------------------*\
| PluginManager.cpp                                         |
|                                                           |
|   OpenRGB plugin manager                                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "LogManager.h"
#include "filesystem.h"
#include "PluginManager.h"
#include "OpenRGBPluginAPI.h"
#include "SettingsManager.h"
#include "ResourceManager.h"

#ifdef _WIN32
#include <Windows.h>
#endif

/*---------------------------------------------------------*\
| PluginManager name for log entries                        |
\*---------------------------------------------------------*/
const char* PLUGINMANAGER = "PluginManager";

void PluginManagerProfileManagerCallback(void * this_ptr, unsigned int update_reason)
{
    PluginManager * this_obj = (PluginManager *)this_ptr;

    this_obj->ProfileManagerCallback(update_reason);
}

void PluginManagerResourceManagerCallback(void * this_ptr, unsigned int update_reason)
{
    PluginManager * this_obj = (PluginManager *)this_ptr;

    this_obj->ResourceManagerCallback(update_reason);
}

void PluginManagerSettingsManagerCallback(void * this_ptr, unsigned int update_reason)
{
    PluginManager * this_obj = (PluginManager *)this_ptr;

    this_obj->SettingsManagerCallback(update_reason);
}

PluginManager::PluginManager()
{
    /*-----------------------------------------------------*\
    | Initialize plugin manager class variables             |
    \*-----------------------------------------------------*/
    AddPluginCallbackVal    = nullptr;
    AddPluginCallbackArg    = nullptr;
    RemovePluginCallbackVal = nullptr;
    RemovePluginCallbackArg = nullptr;

    /*-----------------------------------------------------*\
    | Create OpenRGB plugins directory                      |
    \*-----------------------------------------------------*/
    filesystem::path plugins_dir = ResourceManager::get()->GetConfigurationDirectory() / plugins_path;

    filesystem::create_directories(plugins_dir);

    /*-----------------------------------------------------*\
    | Register callbacks                                    |
    \*-----------------------------------------------------*/
    ResourceManager::get()->GetProfileManager()->RegisterProfileManagerCallback(PluginManagerProfileManagerCallback, this);
    ResourceManager::get()->RegisterResourceManagerCallback(PluginManagerResourceManagerCallback, this);
    ResourceManager::get()->GetSettingsManager()->RegisterSettingsManagerCallback(PluginManagerSettingsManagerCallback, this);
}

PluginManager::~PluginManager()
{
    /*-----------------------------------------------------*\
    | Unegister callbacks                                   |
    \*-----------------------------------------------------*/
    ResourceManager::get()->GetProfileManager()->UnregisterProfileManagerCallback(PluginManagerProfileManagerCallback, this);
    ResourceManager::get()->UnregisterResourceManagerCallback(PluginManagerResourceManagerCallback, this);
    ResourceManager::get()->GetSettingsManager()->UnregisterSettingsManagerCallback(PluginManagerSettingsManagerCallback, this);
}

void PluginManager::RegisterAddPluginCallback(AddPluginCallback new_callback, void * new_callback_arg)
{
    AddPluginCallbackVal    = new_callback;
    AddPluginCallbackArg    = new_callback_arg;
}

void PluginManager::RegisterRemovePluginCallback(RemovePluginCallback new_callback, void * new_callback_arg)
{
    RemovePluginCallbackVal = new_callback;
    RemovePluginCallbackArg = new_callback_arg;
}

void PluginManager::ScanAndLoadPlugins()
{
    /*-----------------------------------------------------*\
    | Get the user plugins directory                        |
    |                                                       |
    | The user plugins directory is a directory named       |
    | "plugins" in the configuration directory              |
    \*-----------------------------------------------------*/
    filesystem::path plugins_dir = ResourceManager::get()->GetConfigurationDirectory() / plugins_path;
    ScanAndLoadPluginsFrom(plugins_dir, false);

#ifdef OPENRGB_SYSTEM_PLUGIN_DIRECTORY
    /*-----------------------------------------------------*\
    | Get the system plugins directory                      |
    |                                                       |
    | The system plugin directory can be set during build   |
    | time, e.g. by the package maintainer to load plugins  |
    | installed via package manager                         |
    \*-----------------------------------------------------*/
    ScanAndLoadPluginsFrom(OPENRGB_SYSTEM_PLUGIN_DIRECTORY, true);
#endif

#ifdef _WIN32
    /*-----------------------------------------------------*\
    | Get the exe folder plugins directory (Windows)        |
    |                                                       |
    | On Windows, system plugins are located in a folder    |
    | called "plugins" inside the folder where the          |
    | OpenRGB.exe file is installed.  Typically,            |
    | C:\Program Files\OpenRGB but other install paths are  |
    | allowed.                                              |
    \*-----------------------------------------------------*/
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);

    filesystem::path exe_dir(path);
    exe_dir = exe_dir.remove_filename() / plugins_path;

    ScanAndLoadPluginsFrom(exe_dir, true);
#endif
}

void PluginManager::ScanAndLoadPluginsFrom(const filesystem::path & plugins_dir, bool is_system)
{
    if(is_system)
    {
        LOG_TRACE("[%s] Scanning system plugin directory: %s", PLUGINMANAGER, plugins_dir.generic_u8string().c_str());
    }
    else
    {
        LOG_TRACE("[%s] Scanning user plugin directory: %s", PLUGINMANAGER, plugins_dir.generic_u8string().c_str());
    }

    if(!filesystem::is_directory(plugins_dir))
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Get a list of all files in the plugins directory      |
    \*-----------------------------------------------------*/
    for(const filesystem::directory_entry& entry: filesystem::directory_iterator(plugins_dir))
    {
        if(filesystem::is_directory(entry.path()))
        {
            continue;
        }

        filesystem::path plugin_path = entry.path();
        LOG_TRACE("[%s] Found plugin file %s", PLUGINMANAGER, plugin_path.filename().generic_u8string().c_str());
        AddPlugin(plugin_path, is_system);
    }
}

void PluginManager::AddPlugin(const filesystem::path& path, bool is_system)
{
    /*-----------------------------------------------------*\
    | Prepare variables and open plugin settings            |
    \*-----------------------------------------------------*/
    OpenRGBPluginInterface* plugin          = nullptr;
    unsigned int            plugin_idx;
    json                    plugin_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Plugins");

    /*-----------------------------------------------------*\
    | Remove this plugin if it is on the remove list        |
    \*-----------------------------------------------------*/
    if(plugin_settings.contains("plugins_remove"))
    {
        for(std::size_t plugin_remove_idx = 0; plugin_remove_idx < plugin_settings["plugins_remove"].size(); plugin_remove_idx++)
        {
            LOG_WARNING("[%s] Checking remove %d, %s", PLUGINMANAGER, plugin_remove_idx, to_string(plugin_settings["plugins_remove"][plugin_remove_idx]).c_str());

            if(plugin_settings["plugins_remove"][plugin_remove_idx] == path.generic_u8string())
            {
                /*-----------------------------------------*\
                | Delete the plugin file                    |
                \*-----------------------------------------*/
                filesystem::remove(path);
            }

            /*---------------------------------------------*\
            | Erase the plugin from the remove list         |
            \*---------------------------------------------*/
            plugin_settings["plugins_remove"].erase(plugin_remove_idx);

            ResourceManager::get()->GetSettingsManager()->SetSettings("Plugins", plugin_settings);
            ResourceManager::get()->GetSettingsManager()->SaveSettings();
        }
    }

    /*-----------------------------------------------------*\
    | Search active plugins to see if this path already     |
    | exists                                                |
    \*-----------------------------------------------------*/
    for(plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++)
    {
        if(path == ActivePlugins[plugin_idx].path)
        {
            LOG_WARNING("[%s] Plugin file %s already in list, skipping.", PLUGINMANAGER, path.c_str());
            return;
        }
    }

    /*-----------------------------------------------------*\
    | Create a QPluginLoader and extract the metadata       |
    \*-----------------------------------------------------*/
    std::string         path_string         = path.generic_u8string();
    QPluginLoader*      loader              = new QPluginLoader(QString::fromStdString(path_string));
    QJsonObject         metadata            = loader->metaData();
    unsigned int        plugin_api_version  = 0;
    QString             plugin_commit;
    QString             plugin_id           = "incompatible";
    QString             plugin_name         = "Incompatible Plugin";
    QString             plugin_url;
    QString             plugin_version;

    if(metadata.contains("MetaData"))
    {
        metadata                            = metadata.value("MetaData").toObject();
    }
    else
    {
        LOG_WARNING("[%s] Plugin %s does not have a MetaData field, skipping.", PLUGINMANAGER, path.c_str());
        return;
    }

    if(metadata.contains("OpenRGBPluginAPIVersion"))
    {
        plugin_api_version                  = metadata.value("OpenRGBPluginAPIVersion").toInt();
    }
    else
    {
        LOG_WARNING("[%s] Plugin %s does not have an OpenRGBPluginAPIVersion field, skipping.", PLUGINMANAGER, path.c_str());
        return;
    }

    if(metadata.contains("Id"))
    {
        plugin_id                           = metadata.value("Id").toString();
    }
    else
    {
        LOG_WARNING("[%s] Plugin %s does not have an Id field, skipping.", PLUGINMANAGER, path.c_str());
        return;
    }

    if(metadata.contains("Commit"))
    {
        plugin_commit                       = metadata.value("Commit").toString();
    }

    if(metadata.contains("Name"))
    {
        plugin_name                         = metadata.value("Name").toString();
    }

    if(metadata.contains("Url"))
    {
        plugin_url                          = metadata.value("Url").toString();
    }

    if(metadata.contains("VersionStr"))
    {
        plugin_version                      = metadata.value("VersionStr").toString();
    }

    /*-----------------------------------------------------*\
    | Create a new plugin entry to add to the list          |
    \*-----------------------------------------------------*/
    OpenRGBPluginEntry  entry;

    entry.api                               = new OpenRGBPluginAPI();
    entry.api_version                       = plugin_api_version;
    entry.enabled                           = false;
    entry.info.Commit                       = plugin_commit.toStdString();
    entry.info.Description                  = "This plugin is not compatible with this version of OpenRGB.";
    entry.info.Name                         = plugin_name.toStdString();
    entry.info.URL                          = plugin_url.toStdString();
    entry.info.Version                      = plugin_version.toStdString();
    entry.id                                = plugin_id;
    entry.incompatible                      = (plugin_api_version == OPENRGB_PLUGIN_API_VERSION);
    entry.is_system                         = is_system;
    entry.loader                            = loader;
    entry.path                              = path_string;
    entry.plugin                            = plugin;
    entry.widget                            = nullptr;

    /*-----------------------------------------------------*\
    | Check to see if this plugin's ID already exists       |
    \*-----------------------------------------------------*/
    for(plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++)
    {
        if(plugin_id == ActivePlugins[plugin_idx].id)
        {
            entry.info.Description          = "This plugin has a duplicate ID as an already loaded plugin and cannot be loaded.";
            LOG_WARNING("[%s] Plugin ID %s already in list.", PLUGINMANAGER, plugin_id.toStdString().c_str());
            goto add_plugin_entry;
        }
    }

    /*-----------------------------------------------------*\
    | If metadata indicates a matching API version,         |
    | continue loading                                      |
    \*-----------------------------------------------------*/
    if(plugin_api_version == OPENRGB_PLUGIN_API_VERSION)
    {
        QObject*        instance            = loader->instance();

        if(!loader->isLoaded())
        {
            LOG_WARNING("[%s] Plugin %s cannot be loaded: %s", PLUGINMANAGER, path.c_str(), loader->errorString().toStdString().c_str());
            goto add_plugin_entry;
        }

        if(!instance)
        {
            LOG_WARNING("[%s] Plugin %s cannot be instantiated.", PLUGINMANAGER, path.c_str());
            goto add_plugin_entry;
        }

        /*-------------------------------------------------*\
        | Initialize the plugin pointer                     |
        \*-------------------------------------------------*/
        plugin                              = qobject_cast<OpenRGBPluginInterface*>(instance);

        if(!plugin)
        {
            LOG_WARNING("[%s] Plugin %s cannot be casted to OpenRGBPluginInterface", PLUGINMANAGER, path.c_str());
            goto add_plugin_entry;
        }

        if(plugin->GetPluginAPIVersion() != OPENRGB_PLUGIN_API_VERSION)
        {
            LOG_WARNING("[%s] Plugin %s has a compatible API version", PLUGINMANAGER, path.c_str());
            goto add_plugin_entry;
        }

        /*-------------------------------------------------*\
        | Get the plugin information                        |
        \*-------------------------------------------------*/
        OpenRGBPluginInfo info              = plugin->GetPluginInfo();

        /*-------------------------------------------------*\
        | Search the settings to see if it is enabled       |
        \*-------------------------------------------------*/
        std::string     name                = "";
        std::string     description         = "";
        bool            enabled             = true;
        bool            found               = false;
        unsigned int    plugin_ct           = 0;

        if(plugin_settings.contains("plugins"))
        {
            plugin_ct = (unsigned int)plugin_settings["plugins"].size();

            for(unsigned int plugin_settings_idx = 0; plugin_settings_idx < plugin_settings["plugins"].size(); plugin_settings_idx++)
            {
                if(plugin_settings["plugins"][plugin_settings_idx].contains("name"))
                {
                    name        = plugin_settings["plugins"][plugin_settings_idx]["name"];
                }

                if(plugin_settings["plugins"][plugin_settings_idx].contains("description"))
                {
                    description = plugin_settings["plugins"][plugin_settings_idx]["description"];
                }

                if(plugin_settings["plugins"][plugin_settings_idx].contains("enabled"))
                {
                    enabled     = plugin_settings["plugins"][plugin_settings_idx]["enabled"];
                }

                if((info.Name == name)
                &&(info.Description == description))
                {
                    found = true;
                    break;
                }
            }
        }

        /*-------------------------------------------------*\
        | If the plugin was not in the list, add it to the  |
        | list and default it to enabled, then save the     |
        | settings                                          |
        \*-------------------------------------------------*/
        if(!found)
        {
            plugin_settings["plugins"][plugin_ct]["name"]           = info.Name;
            plugin_settings["plugins"][plugin_ct]["description"]    = info.Description;
            plugin_settings["plugins"][plugin_ct]["enabled"]        = enabled;

            ResourceManager::get()->GetSettingsManager()->SetSettings("Plugins", plugin_settings);
            ResourceManager::get()->GetSettingsManager()->SaveSettings();
        }

        LOG_VERBOSE("[%s] Loaded plugin %s", PLUGINMANAGER, info.Name.c_str());

        /*-----------------------------------------------------*\
        | Add the plugin to the PluginManager active plugins    |
        \*-----------------------------------------------------*/
        entry.enabled                       = enabled;
        entry.info                          = info;
        entry.incompatible                  = false;
        entry.api_version                   = plugin->GetPluginAPIVersion();
    }

add_plugin_entry:
    loader->unload();

    ActivePlugins.push_back(entry);

    if(entry.enabled)
    {
        LoadPlugin(&ActivePlugins.back());
    }
}

void PluginManager::RemovePlugin(const filesystem::path& path)
{
    unsigned int plugin_idx;

    LOG_TRACE("[%s] Attempting to remove plugin %s", PLUGINMANAGER, path.c_str());

    /*-----------------------------------------------------*\
    | Search active plugins to see if this path already     |
    | exists                                                |
    \*-----------------------------------------------------*/
    for(plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++)
    {
        if(path == ActivePlugins[plugin_idx].path)
        {
            break;
        }
    }

    /*-----------------------------------------------------*\
    | If the plugin path does not exist in the active       |
    | plugins list, return                                  |
    \*-----------------------------------------------------*/
    if(plugin_idx == ActivePlugins.size())
    {
        LOG_TRACE("[%s] Plugin %s not active", PLUGINMANAGER, path.c_str());
        return;
    }

    /*-----------------------------------------------------*\
    | If the selected plugin is in the list and loaded,     |
    | unload it                                             |
    \*-----------------------------------------------------*/
    if(ActivePlugins[plugin_idx].loader->isLoaded())
    {
        LOG_TRACE("[%s] Plugin %s is active, unloading", PLUGINMANAGER, path.c_str());
        UnloadPlugin(&ActivePlugins[plugin_idx]);
    }

    /*-----------------------------------------------------*\
    | Remove the plugin from the active plugins list        |
    \*-----------------------------------------------------*/
    ActivePlugins.erase(ActivePlugins.begin() + plugin_idx);
}

void PluginManager::EnablePlugin(const filesystem::path& path)
{
    unsigned int plugin_idx;

    /*-----------------------------------------------------*\
    | Search active plugins to see if this path already     |
    | exists                                                |
    \*-----------------------------------------------------*/
    for(plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++)
    {
        if(path == ActivePlugins[plugin_idx].path)
        {
            break;
        }
    }

    /*-----------------------------------------------------*\
    | If the plugin path does not exist in the active       |
    | plugins list, return                                  |
    \*-----------------------------------------------------*/
    if(plugin_idx == ActivePlugins.size())
    {
        return;
    }

    ActivePlugins[plugin_idx].enabled = true;
    LoadPlugin(&ActivePlugins[plugin_idx]);
}

void PluginManager::LoadPlugin(OpenRGBPluginEntry* plugin_entry)
{
    /*-----------------------------------------------------*\
    | If the plugin is in the list but is incompatible,     |
    | return                                                |
    \*-----------------------------------------------------*/
    if(plugin_entry->incompatible)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | If the selected plugin is in the list but not loaded, |
    | load it                                               |
    \*-----------------------------------------------------*/
    if(!plugin_entry->loader->isLoaded())
    {
        plugin_entry->loader->load();

        QObject* instance                = plugin_entry->loader->instance();

        if(instance)
        {
            OpenRGBPluginInterface* plugin = qobject_cast<OpenRGBPluginInterface*>(instance);

            if(plugin)
            {
                if(plugin->GetPluginAPIVersion() == OPENRGB_PLUGIN_API_VERSION)
                {
                    plugin_entry->plugin = plugin;

                    plugin->Load(plugin_entry->api);

                    /*-------------------------------------*\
                    | Call the Add Plugin callback          |
                    \*-------------------------------------*/
                    if(AddPluginCallbackArg != nullptr)
                    {
                        AddPluginCallbackVal(AddPluginCallbackArg, plugin_entry);
                    }
                }
            }
        }
    }
}

void PluginManager::DisablePlugin(const filesystem::path& path)
{
    unsigned int plugin_idx;

    /*-----------------------------------------------------*\
    | Search active plugins to see if this path already     |
    | exists                                                |
    \*-----------------------------------------------------*/
    for(plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++)
    {
        if(path == ActivePlugins[plugin_idx].path)
        {
            break;
        }
    }

    /*-----------------------------------------------------*\
    | If the plugin path does not exist in the active       |
    | plugins list, return                                  |
    \*-----------------------------------------------------*/
    if(plugin_idx == ActivePlugins.size())
    {
        return;
    }

    ActivePlugins[plugin_idx].enabled = false;
    UnloadPlugin(&ActivePlugins[plugin_idx]);
}

void PluginManager::UnloadPlugin(OpenRGBPluginEntry* plugin_entry)
{
    /*-----------------------------------------------------*\
    | If the selected plugin is in the list and loaded,     |
    | unload it                                             |
    \*-----------------------------------------------------*/
    if(plugin_entry->loader->isLoaded())
    {
        /*-------------------------------------------------*\
        | Call plugin's Unload function before GUI removal  |
        \*-------------------------------------------------*/
        plugin_entry->plugin->Unload();

        /*-------------------------------------------------*\
        | Call the Remove Plugin callback                   |
        \*-------------------------------------------------*/
        if(RemovePluginCallbackVal != nullptr)
        {
            RemovePluginCallbackVal(RemovePluginCallbackArg, plugin_entry);
        }

        bool unloaded = plugin_entry->loader->unload();

        if(!unloaded)
        {
            LOG_WARNING("[%s] Plugin %s cannot be unloaded", PLUGINMANAGER, plugin_entry->path.c_str());
        }
        else
        {
            LOG_TRACE("[%s] Plugin %s successfully unloaded", PLUGINMANAGER, plugin_entry->path.c_str());
        }
    }
    else
    {
        LOG_TRACE("[%s] Plugin %s was already unloaded", PLUGINMANAGER, plugin_entry->path.c_str());
    }
}

void PluginManager::LoadPlugins()
{
    for(OpenRGBPluginEntry& plugin_entry: ActivePlugins)
    {
        if(plugin_entry.enabled)
        {
            LoadPlugin(&plugin_entry);
        }
    }
}

void PluginManager::UnloadPlugins()
{
    for(OpenRGBPluginEntry& plugin_entry: ActivePlugins)
    {
        UnloadPlugin(&plugin_entry);
    }
}

void PluginManager::OnProfileAboutToLoad()
{
    /*-----------------------------------------------------*\
    | Loop through all plugins and signal profile about to  |
    | load                                                  |
    \*-----------------------------------------------------*/
    for(std::size_t plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++)
    {
        if(ActivePlugins[plugin_idx].enabled && ActivePlugins[plugin_idx].loader->isLoaded())
        {
            ActivePlugins[plugin_idx].plugin->OnProfileAboutToLoad();
        }
    }
}

void PluginManager::OnProfileLoad(nlohmann::json profile_data)
{
    /*-----------------------------------------------------*\
    | Loop through all plugins call their OnProfileLoad if  |
    | the profile data contains an entry for that plugin    |
    \*-----------------------------------------------------*/
    for(std::size_t plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++)
    {
        if(ActivePlugins[plugin_idx].enabled && ActivePlugins[plugin_idx].loader->isLoaded())
        {
            if(profile_data.contains(ActivePlugins[plugin_idx].plugin->GetPluginInfo().Name))
            {
                ActivePlugins[plugin_idx].plugin->OnProfileLoad(profile_data[ActivePlugins[plugin_idx].plugin->GetPluginInfo().Name]);
            }
        }
    }
}

nlohmann::json PluginManager::OnProfileSave()
{
    nlohmann::json plugin_json;

    /*-----------------------------------------------------*\
    | Loop through all plugins and gather their profile     |
    | data                                                  |
    \*-----------------------------------------------------*/
    for(std::size_t plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++)
    {
        if(ActivePlugins[plugin_idx].enabled && ActivePlugins[plugin_idx].loader->isLoaded())
        {
            plugin_json[ActivePlugins[plugin_idx].plugin->GetPluginInfo().Name] = ActivePlugins[plugin_idx].plugin->OnProfileSave();
        }
    }

    return(plugin_json);
}

nlohmann::json PluginManager::OnProfileSave(std::vector<std::string> enabled_plugins)
{
    nlohmann::json plugin_json;

    /*-----------------------------------------------------*\
    | Loop through all plugins and gather their profile     |
    | data if the plugin name is in the enabled list        |
    \*-----------------------------------------------------*/
    for(std::size_t plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++)
    {
        if(ActivePlugins[plugin_idx].enabled && ActivePlugins[plugin_idx].loader->isLoaded())
        {
            for(std::size_t enabled_plugin_idx = 0; enabled_plugin_idx < enabled_plugins.size(); enabled_plugin_idx++)
            {
                if(enabled_plugins[enabled_plugin_idx] == ActivePlugins[plugin_idx].info.Name)
                {
                    plugin_json[ActivePlugins[plugin_idx].plugin->GetPluginInfo().Name] = ActivePlugins[plugin_idx].plugin->OnProfileSave();
                    break;
                }
            }
        }
    }

    return(plugin_json);
}

unsigned char * PluginManager::OnSDKCommand(unsigned int plugin_idx, unsigned int pkt_id, unsigned char * pkt_data, unsigned int * pkt_size)
{
    unsigned char * out_data = NULL;

    if(plugin_idx < ActivePlugins.size())
    {
        if(ActivePlugins[plugin_idx].enabled && ActivePlugins[plugin_idx].loader->isLoaded())
        {
            out_data = ActivePlugins[plugin_idx].plugin->OnSDKCommand(pkt_id, pkt_data, pkt_size);
        }
    }

    return(out_data);
}

/*---------------------------------------------------------*\
| Callback functions                                        |
\*---------------------------------------------------------*/
void PluginManager::ProfileManagerCallback(unsigned int update_reason)
{
    for(std::size_t plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++)
    {
        if(ActivePlugins[plugin_idx].enabled && ActivePlugins[plugin_idx].loader->isLoaded())
        {
            ActivePlugins[plugin_idx].plugin->ProfileManagerUpdated(update_reason);
        }
    }
}

void PluginManager::ResourceManagerCallback(unsigned int update_reason)
{
    for(std::size_t plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++)
    {
        if(ActivePlugins[plugin_idx].enabled && ActivePlugins[plugin_idx].loader->isLoaded())
        {
            ActivePlugins[plugin_idx].plugin->ResourceManagerUpdated(update_reason);
        }
    }
}

void PluginManager::SettingsManagerCallback(unsigned int update_reason)
{
    for(std::size_t plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++)
    {
        if(ActivePlugins[plugin_idx].enabled && ActivePlugins[plugin_idx].loader->isLoaded())
        {
            ActivePlugins[plugin_idx].plugin->SettingsManagerUpdated(update_reason);
        }
    }
}

/*---------------------------------------------------------*\
| Plugin Information                                        |
\*---------------------------------------------------------*/
unsigned int PluginManager::GetPluginCount()
{
    return(ActivePlugins.size());
}

std::string PluginManager::GetPluginDescription(unsigned int plugin_idx)
{
    if(plugin_idx < ActivePlugins.size())
    {
        if(ActivePlugins[plugin_idx].enabled && ActivePlugins[plugin_idx].loader->isLoaded())
        {
            return(ActivePlugins[plugin_idx].plugin->GetPluginInfo().Description);
        }
    }

    return("");
}

std::string PluginManager::GetPluginName(unsigned int plugin_idx)
{
    if(plugin_idx < ActivePlugins.size())
    {
        if(ActivePlugins[plugin_idx].enabled && ActivePlugins[plugin_idx].loader->isLoaded())
        {
            return(ActivePlugins[plugin_idx].plugin->GetPluginInfo().Name);
        }
    }

    return("");
}

unsigned int PluginManager::GetPluginProtocolVersion(unsigned int plugin_idx)
{
    if(plugin_idx < ActivePlugins.size())
    {
        if(ActivePlugins[plugin_idx].enabled && ActivePlugins[plugin_idx].loader->isLoaded())
        {
            return(ActivePlugins[plugin_idx].plugin->GetPluginInfo().ProtocolVersion);
        }
    }

    return(0);
}

std::string PluginManager::GetPluginVersion(unsigned int plugin_idx)
{
    if(plugin_idx < ActivePlugins.size())
    {
        if(ActivePlugins[plugin_idx].enabled && ActivePlugins[plugin_idx].loader->isLoaded())
        {
            return(ActivePlugins[plugin_idx].plugin->GetPluginInfo().Version);
        }
    }

    return("");
}

/*---------------------------------------------------------*\
| Plugin-Created RGBControllers                             |
\*---------------------------------------------------------*/
std::vector<RGBController *> PluginManager::GetRGBControllers()
{
    std::vector<RGBController *> plugin_controllers;

    for(std::size_t plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++)
    {
        if(ActivePlugins[plugin_idx].enabled && ActivePlugins[plugin_idx].loader->isLoaded())
        {
            plugin_controllers.insert(plugin_controllers.begin(), ((OpenRGBPluginAPI*)ActivePlugins[plugin_idx].api)->rgb_controllers.begin(), ((OpenRGBPluginAPI*)ActivePlugins[plugin_idx].api)->rgb_controllers.end());
        }
    }

    return(plugin_controllers);
}
