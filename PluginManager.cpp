#include "LogManager.h"
#include "PluginManager.h"

PluginManager::PluginManager(bool dark_theme_val)
{
    dark_theme = dark_theme_val;
}

void PluginManager::RegisterAddPluginTabCallback(AddPluginTabCallback new_callback, void * new_callback_arg)
{
    AddPluginTabCallbacks.push_back(new_callback);
    AddPluginTabCallbackArgs.push_back(new_callback_arg);
}

void PluginManager::RegisterRemovePluginTabCallback(RemovePluginTabCallback new_callback, void * new_callback_arg)
{
    RemovePluginTabCallbacks.push_back(new_callback);
    RemovePluginTabCallbackArgs.push_back(new_callback_arg);
}

void PluginManager::ScanAndLoadPlugins()
{
    LOG_INFO("Loading plugins");

    /*---------------------------------------------------------*\
    | Get the plugins directory                                 |
    |                                                           |
    | The plugins directory is a directory named "plugins" in   |
    | the configuration directory                               |
    \*---------------------------------------------------------*/
    const QDir plugins_dir = QString().fromStdString(ResourceManager::get()->GetConfigurationDirectory()) + "plugins/";

    /*---------------------------------------------------------*\
    | Get a list of all files in the plugins directory          |
    \*---------------------------------------------------------*/
    std::vector<std::string> FileList;

    for(int i = 0; i < QDir(plugins_dir).entryList(QDir::Files).size(); i++)
    {
        FileList.push_back(QDir(plugins_dir).entryList(QDir::Files)[i].toStdString());
    }

    /*---------------------------------------------------------*\
    | Attempt to load each file in the plugins directory        |
    \*---------------------------------------------------------*/
    for(const std::string &plugin_name : FileList)
    {
        const std::string plugin_path = plugins_dir.absoluteFilePath(QString().fromStdString(plugin_name)).toStdString();

        AddPlugin(plugin_path);
    }
}

void PluginManager::AddPlugin(std::string path)
{
    OpenRGBPluginInterface* plugin = nullptr;

    unsigned int plugin_idx;

    for(plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++)
    {
        if(path == ActivePlugins[plugin_idx].path)
        {
            break;
        }
    }

    /*---------------------------------------------------------------------*\
    | If the path does not match an existing entry, create a new entry      |
    \*---------------------------------------------------------------------*/
    if(plugin_idx == ActivePlugins.size())
    {
        /*-----------------------------------------------------------------*\
        | Create a QPluginLoader and load the plugin                        |
        \*-----------------------------------------------------------------*/
        QPluginLoader*  loader      = new QPluginLoader(QString().fromStdString(path));
        QObject*        instance    = loader->instance();

        /*-----------------------------------------------------------------*\
        | Check that the plugin is valid, then check the API version        |
        \*-----------------------------------------------------------------*/
        if(instance)
        {
            plugin = qobject_cast<OpenRGBPluginInterface*>(instance);

            if(plugin)
            {
                if(plugin->GetPluginAPIVersion() == OPENRGB_PLUGIN_API_VERSION)
                {
                    /*-----------------------------------------------------*\
                    | Get the plugin information                            |
                    \*-----------------------------------------------------*/
                    OpenRGBPluginInfo info = plugin->GetPluginInfo();

                    /*-----------------------------------------------------*\
                    | Search the settings to see if it is enabled           |
                    \*-----------------------------------------------------*/
                    std::string     name        = "";
                    std::string     description = "";
                    bool            enabled     = true;
                    bool            found       = false;
                    unsigned int    plugin_ct   = 0;

                    /*-----------------------------------------------------*\
                    | Open plugin list and check if plugin is in the list   |
                    \*-----------------------------------------------------*/
                    json plugin_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Plugins");

                    if(plugin_settings.contains("plugins"))
                    {
                        plugin_ct = plugin_settings["plugins"].size();

                        for(unsigned int plugin_idx = 0; plugin_idx < plugin_settings["plugins"].size(); plugin_idx++)
                        {
                            if(plugin_settings["plugins"][plugin_idx].contains("name"))
                            {
                                name        = plugin_settings["plugins"][plugin_idx]["name"];
                            }

                            if(plugin_settings["plugins"][plugin_idx].contains("description"))
                            {
                                description = plugin_settings["plugins"][plugin_idx]["description"];
                            }

                            if(plugin_settings["plugins"][plugin_idx].contains("enabled"))
                            {
                                enabled     = plugin_settings["plugins"][plugin_idx]["enabled"];
                            }

                            if((info.Name == name)
                             &&(info.Description == description))
                            {
                                found = true;
                                break;
                            }
                        }
                    }

                    /*-----------------------------------------------------*\
                    | If the plugin was not in the list, add it to the list |
                    | and default it to enabled, then save the settings     |
                    \*-----------------------------------------------------*/
                    if(!found)
                    {
                        plugin_settings["plugins"][plugin_ct]["name"]           = info.Name;
                        plugin_settings["plugins"][plugin_ct]["description"]    = info.Description;
                        plugin_settings["plugins"][plugin_ct]["enabled"]        = enabled;

                        ResourceManager::get()->GetSettingsManager()->SetSettings("Plugins", plugin_settings);
                        ResourceManager::get()->GetSettingsManager()->SaveSettings();
                    }

                    LOG_VERBOSE("Loaded plugin %s", info.Name.c_str());

                    /*-----------------------------------------------------*\
                    | Add the plugin to the PluginManager active plugins    |
                    \*-----------------------------------------------------*/
                    OpenRGBPluginEntry entry;

                    entry.info    = info;
                    entry.plugin  = plugin;
                    entry.loader  = loader;
                    entry.loaded  = false;
                    entry.path    = path;
                    entry.enabled = enabled;
                    entry.widget  = nullptr;

                    loader->unload();

                    PluginManager::ActivePlugins.push_back(entry);

                    if(entry.enabled)
                    {
                        LoadPlugin(path);
                    }
                }
            }
        }
    }
}

void PluginManager::LoadPlugin(std::string path)
{
    unsigned int plugin_idx;

    for(plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++)
    {
        if(path == ActivePlugins[plugin_idx].path)
        {
            break;
        }
    }

    if(plugin_idx == ActivePlugins.size())
    {
        return;
    }

    if(!ActivePlugins[plugin_idx].loaded)
    {
        ActivePlugins[plugin_idx].loader->load();
        ActivePlugins[plugin_idx].loaded = true;

        QObject* instance                = ActivePlugins[plugin_idx].loader->instance();

        if(instance)
        {
            OpenRGBPluginInterface* plugin = qobject_cast<OpenRGBPluginInterface*>(instance);

            if(plugin)
            {
                if(plugin->GetPluginAPIVersion() == OPENRGB_PLUGIN_API_VERSION)
                {
                    ActivePlugins[plugin_idx].plugin = plugin;

                    plugin->Initialize(dark_theme, ResourceManager::get());

                    /*-------------------------------------------------*\
                    | Call the callbacks                                |
                    \*-------------------------------------------------*/
                    for(unsigned int callback_idx = 0; callback_idx < AddPluginTabCallbacks.size(); callback_idx++)
                    {
                        AddPluginTabCallbacks[callback_idx](AddPluginTabCallbackArgs[callback_idx], &ActivePlugins[plugin_idx]);
                    }
                }
            }
        }
    }
}

void PluginManager::UnloadPlugin(std::string path)
{
    unsigned int plugin_idx;

    for(plugin_idx = 0; plugin_idx < ActivePlugins.size(); plugin_idx++)
    {
        if(path == ActivePlugins[plugin_idx].path)
        {
            break;
        }
    }

    if(plugin_idx == ActivePlugins.size())
    {
        return;
    }

    if(ActivePlugins[plugin_idx].loaded)
    {
        /*-------------------------------------------------*\
        | Call plugin's Unload function before GUI removal  |
        \*-------------------------------------------------*/
        ActivePlugins[plugin_idx].plugin->Unload();

        /*-------------------------------------------------*\
        | Call the callbacks                                |
        \*-------------------------------------------------*/
        for(unsigned int callback_idx = 0; callback_idx < RemovePluginTabCallbacks.size(); callback_idx++)
        {
            RemovePluginTabCallbacks[callback_idx](RemovePluginTabCallbackArgs[callback_idx], &ActivePlugins[plugin_idx]);
        }

        ActivePlugins[plugin_idx].loader->unload();
        ActivePlugins[plugin_idx].loaded = false;
    }
}
