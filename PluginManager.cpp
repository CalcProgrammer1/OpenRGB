/*---------------------------------------------------------*\
| PluginManager.cpp                                         |
|                                                           |
|   OpenRGB plugin manager                                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "PluginManager.h"
#include "LogManager.h"
#include "OpenRGBThemeManager.h"
#include "SettingsManager.h"
#include "filesystem.h"

PluginManager::PluginManager()
{
    /*---------------------------------------------------------*\
    | Initialize plugin manager class variables                 |
    \*---------------------------------------------------------*/
    AddPluginCallbackVal = nullptr;
    AddPluginCallbackArg = nullptr;
    RemovePluginCallbackVal = nullptr;
    RemovePluginCallbackArg = nullptr;

    /*-------------------------------------------------------------------------*\
    | Create OpenRGB plugins directory                                          |
    \*-------------------------------------------------------------------------*/
    filesystem::path plugins_dir = ResourceManager::get()->GetConfigurationDirectory()
                                   / plugins_path;

    filesystem::create_directories(plugins_dir);
}

void PluginManager::RegisterAddPluginCallback(AddPluginCallback new_callback, void *new_callback_arg)
{
    AddPluginCallbackVal = new_callback;
    AddPluginCallbackArg = new_callback_arg;
}

void PluginManager::RegisterRemovePluginCallback(RemovePluginCallback new_callback,
                                                 void *new_callback_arg)
{
    RemovePluginCallbackVal = new_callback;
    RemovePluginCallbackArg = new_callback_arg;
}

void PluginManager::ScanAndLoadPlugins()
{
    /*---------------------------------------------------------*\
    | Get the plugins directory                                 |
    |                                                           |
    | The plugins directory is a directory named "plugins" in   |
    | the configuration directory                               |
    \*---------------------------------------------------------*/
    filesystem::path plugins_dir = ResourceManager::get()->GetConfigurationDirectory()
                                   / plugins_path;
    ScanAndLoadPluginsFrom(plugins_dir);

#ifdef OPENRGB_EXTRA_PLUGIN_DIRECTORY
    /*-----------------------------------------------------------------*\
    | An additional plugin directory can be set during build time, e.g. |
    | by the Linux distro to load plugins installed via package manager |
    \*-----------------------------------------------------------------*/
    ScanAndLoadPluginsFrom(OPENRGB_EXTRA_PLUGIN_DIRECTORY);
#endif
}

void PluginManager::ScanAndLoadPluginsFrom(const filesystem::path &plugins_dir)
{
    LOG_TRACE("[PluginManager] Scanning plugin directory: %s",
              plugins_dir.generic_u8string().c_str());

    if (!filesystem::is_directory(plugins_dir)) {
        return;
    }

    /*---------------------------------------------------------*\
    | Get a list of all files in the plugins directory          |
    \*---------------------------------------------------------*/

    for (const filesystem::directory_entry &entry : filesystem::directory_iterator(plugins_dir)) {
        if (filesystem::is_directory(entry.path())) {
            continue;
        }

        filesystem::path plugin_path = entry.path();
        LOG_TRACE("[PluginManager] Found plugin file %s",
                  plugin_path.filename().generic_u8string().c_str());
        AddPlugin(plugin_path);
    }
}

void PluginManager::AddPlugin(const filesystem::path &path)
{
    OpenRGBPluginInterface *plugin = nullptr;

    unsigned int plugin_idx;

    /*---------------------------------------------------------------------*\
    | Search active plugins to see if this path already exists              |
    \*---------------------------------------------------------------------*/
    for (plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++) {
        if (path == ActivePlugins[plugin_idx].path) {
            break;
        }
    }

    /*---------------------------------------------------------------------*\
    | If the path does not match an existing entry, create a new entry      |
    \*---------------------------------------------------------------------*/
    if (plugin_idx == ActivePlugins.size()) {
        /*-----------------------------------------------------------------*\
        | Create a QPluginLoader and load the plugin                        |
        \*-----------------------------------------------------------------*/
        std::string path_string = path.generic_u8string();
        QPluginLoader *loader = new QPluginLoader(QString::fromStdString(path_string));
        QObject *instance = loader->instance();

        if (!loader->isLoaded()) {
            LOG_WARNING("[PluginManager] Plugin %s cannot be loaded: %s",
                        path.c_str(),
                        loader->errorString().toStdString().c_str());
        }

        /*-----------------------------------------------------------------*\
        | Check that the plugin is valid, then check the API version        |
        \*-----------------------------------------------------------------*/
        if (instance) {
            plugin = qobject_cast<OpenRGBPluginInterface *>(instance);

            if (plugin) {
                if (plugin->GetPluginAPIVersion() == OPENRGB_PLUGIN_API_VERSION) {
                    LOG_TRACE("[PluginManager] Plugin %s has a compatible API version",
                              path.c_str());

                    /*-----------------------------------------------------*\
                    | Get the plugin information                            |
                    \*-----------------------------------------------------*/
                    OpenRGBPluginInfo info = plugin->GetPluginInfo();

                    /*-----------------------------------------------------*\
                    | Search the settings to see if it is enabled           |
                    \*-----------------------------------------------------*/
                    std::string name = "";
                    std::string description = "";
                    bool enabled = true;
                    bool found = false;
                    unsigned int plugin_ct = 0;

                    /*-----------------------------------------------------*\
                    | Open plugin list and check if plugin is in the list   |
                    \*-----------------------------------------------------*/
                    json plugin_settings = ResourceManager::get()->GetSettingsManager()->GetSettings(
                        "Plugins");

                    if (plugin_settings.contains("plugins")) {
                        plugin_ct = plugin_settings["plugins"].size();

                        for (unsigned int plugin_settings_idx = 0;
                             plugin_settings_idx < plugin_settings["plugins"].size();
                             plugin_settings_idx++) {
                            if (plugin_settings["plugins"][plugin_settings_idx].contains("name")) {
                                name = plugin_settings["plugins"][plugin_settings_idx]["name"];
                            }

                            if (plugin_settings["plugins"][plugin_settings_idx].contains(
                                    "description")) {
                                description = plugin_settings["plugins"][plugin_settings_idx]
                                                             ["description"];
                            }

                            if (plugin_settings["plugins"][plugin_settings_idx].contains(
                                    "enabled")) {
                                enabled
                                    = plugin_settings["plugins"][plugin_settings_idx]["enabled"];
                            }

                            if ((info.Name == name) && (info.Description == description)) {
                                found = true;
                                break;
                            }
                        }
                    }

                    /*-----------------------------------------------------*\
                    | If the plugin was not in the list, add it to the list |
                    | and default it to enabled, then save the settings     |
                    \*-----------------------------------------------------*/
                    if (!found) {
                        plugin_settings["plugins"][plugin_ct]["name"] = info.Name;
                        plugin_settings["plugins"][plugin_ct]["description"] = info.Description;
                        plugin_settings["plugins"][plugin_ct]["enabled"] = enabled;

                        ResourceManager::get()->GetSettingsManager()->SetSettings("Plugins",
                                                                                  plugin_settings);
                        ResourceManager::get()->GetSettingsManager()->SaveSettings();
                    }

                    LOG_VERBOSE("Loaded plugin %s", info.Name.c_str());

                    /*-----------------------------------------------------*\
                    | Add the plugin to the PluginManager active plugins    |
                    \*-----------------------------------------------------*/
                    OpenRGBPluginEntry entry;

                    entry.info = info;
                    entry.plugin = plugin;
                    entry.loader = loader;
                    entry.path = path_string;
                    entry.enabled = enabled;
                    entry.widget = nullptr;
                    entry.incompatible = false;
                    entry.api_version = plugin->GetPluginAPIVersion();

                    loader->unload();

                    ActivePlugins.push_back(entry);

                    if (entry.enabled) {
                        LoadPlugin(path);
                    }
                } else {
                    /*-----------------------------------------------------*\
                    | Fill in a plugin information object with text showing |
                    | the plugin is incompatible                            |
                    \*-----------------------------------------------------*/
                    OpenRGBPluginInfo info;

                    info.Name = "Incompatible Plugin";
                    info.Description
                        = "This plugin is not compatible with this version of OpenRGB.";

                    /*-----------------------------------------------------*\
                    | Add the plugin to the PluginManager active plugins    |
                    | but mark it as incompatible                           |
                    \*-----------------------------------------------------*/
                    OpenRGBPluginEntry entry;

                    entry.info = info;
                    entry.plugin = plugin;
                    entry.loader = loader;
                    entry.path = path_string;
                    entry.enabled = false;
                    entry.widget = nullptr;
                    entry.incompatible = true;
                    entry.api_version = plugin->GetPluginAPIVersion();

                    loader->unload();

                    PluginManager::ActivePlugins.push_back(entry);

                    bool unloaded = loader->unload();

                    LOG_WARNING("[PluginManager] Plugin %s has an incompatible API version",
                                path.c_str());

                    if (!unloaded) {
                        LOG_WARNING("[PluginManager] Plugin %s cannot be unloaded", path.c_str());
                    }
                }
            } else {
                LOG_WARNING("[PluginManager] Plugin %s cannot be casted to OpenRGBPluginInterface",
                            path.c_str());
            }
        } else {
            LOG_WARNING("[PluginManager] Plugin %s cannot be instantiated.", path.c_str());
        }
    }
}

void PluginManager::RemovePlugin(const filesystem::path &path)
{
    unsigned int plugin_idx;

    LOG_TRACE("[PluginManager] Attempting to remove plugin %s", path.c_str());

    /*---------------------------------------------------------------------*\
    | Search active plugins to see if this path already exists              |
    \*---------------------------------------------------------------------*/
    for (plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++) {
        if (path == ActivePlugins[plugin_idx].path) {
            break;
        }
    }

    /*---------------------------------------------------------------------*\
    | If the plugin path does not exist in the active plugins list, return  |
    \*---------------------------------------------------------------------*/
    if (plugin_idx == ActivePlugins.size()) {
        LOG_TRACE("[PluginManager] Plugin %s not active", path.c_str());
        return;
    }

    /*---------------------------------------------------------------------*\
    | If the selected plugin is in the list and loaded, unload it           |
    \*---------------------------------------------------------------------*/
    if (ActivePlugins[plugin_idx].loader->isLoaded()) {
        LOG_TRACE("[PluginManager] Plugin %s is active, unloading", path.c_str());
        UnloadPlugin(path);
    }

    /*---------------------------------------------------------------------*\
    | Remove the plugin from the active plugins list                        |
    \*---------------------------------------------------------------------*/
    ActivePlugins.erase(ActivePlugins.begin() + plugin_idx);
}

void PluginManager::LoadPlugin(const filesystem::path &path)
{
    unsigned int plugin_idx;

    /*---------------------------------------------------------------------*\
    | Search active plugins to see if this path already exists              |
    \*---------------------------------------------------------------------*/
    for (plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++) {
        if (path == ActivePlugins[plugin_idx].path) {
            break;
        }
    }

    /*---------------------------------------------------------------------*\
    | If the plugin path does not exist in the active plugins list, return  |
    \*---------------------------------------------------------------------*/
    if (plugin_idx == ActivePlugins.size()) {
        return;
    }

    /*---------------------------------------------------------------------*\
    | If the plugin is in the list but is incompatible, return              |
    \*---------------------------------------------------------------------*/
    if (ActivePlugins[plugin_idx].incompatible) {
        return;
    }

    /*---------------------------------------------------------------------*\
    | If the selected plugin is in the list but not loaded, load it         |
    \*---------------------------------------------------------------------*/
    if (!ActivePlugins[plugin_idx].loader->isLoaded()) {
        ActivePlugins[plugin_idx].loader->load();

        QObject *instance = ActivePlugins[plugin_idx].loader->instance();

        bool dark_theme = OpenRGBThemeManager::IsDarkTheme();

        if (instance) {
            OpenRGBPluginInterface *plugin = qobject_cast<OpenRGBPluginInterface *>(instance);

            if (plugin) {
                if (plugin->GetPluginAPIVersion() == OPENRGB_PLUGIN_API_VERSION) {
                    ActivePlugins[plugin_idx].plugin = plugin;

                    plugin->Load(dark_theme, ResourceManager::get());

                    /*-------------------------------------------------*\
                    | Call the Add Plugin callback                      |
                    \*-------------------------------------------------*/
                    if (AddPluginCallbackArg != nullptr) {
                        AddPluginCallbackVal(AddPluginCallbackArg, &ActivePlugins[plugin_idx]);
                    }
                }
            }
        }
    }
}

void PluginManager::UnloadPlugin(const filesystem::path &path)
{
    unsigned int plugin_idx;

    /*---------------------------------------------------------------------*\
    | Search active plugins to see if this path already exists              |
    \*---------------------------------------------------------------------*/
    for (plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++) {
        if (path == ActivePlugins[plugin_idx].path) {
            break;
        }
    }

    /*---------------------------------------------------------------------*\
    | If the plugin path does not exist in the active plugins list, return  |
    \*---------------------------------------------------------------------*/
    if (plugin_idx == ActivePlugins.size()) {
        return;
    }

    /*---------------------------------------------------------------------*\
    | If the selected plugin is in the list and loaded, unload it           |
    \*---------------------------------------------------------------------*/
    if (ActivePlugins[plugin_idx].loader->isLoaded()) {
        /*-------------------------------------------------*\
        | Call plugin's Unload function before GUI removal  |
        \*-------------------------------------------------*/
        ActivePlugins[plugin_idx].plugin->Unload();

        /*-------------------------------------------------*\
        | Call the Remove Plugin callback                   |
        \*-------------------------------------------------*/
        if (RemovePluginCallbackVal != nullptr) {
            RemovePluginCallbackVal(RemovePluginCallbackArg, &ActivePlugins[plugin_idx]);
        }

        bool unloaded = ActivePlugins[plugin_idx].loader->unload();

        if (!unloaded) {
            LOG_WARNING("[PluginManager] Plugin %s cannot be unloaded", path.c_str());
        } else {
            LOG_TRACE("[PluginManager] Plugin %s successfully unloaded", path.c_str());
        }
    } else {
        LOG_TRACE("[PluginManager] Plugin %s was already unloaded", path.c_str());
    }
}

void PluginManager::UnloadPlugins()
{
    for (const OpenRGBPluginEntry &plugin_entry : ActivePlugins) {
        if (plugin_entry.loader->isLoaded()) {
            plugin_entry.plugin->Unload();
        }
    }
}
