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

void PluginManager::ScanAndLoadPlugins()
{
    LOG_INFO("Loading plugins");

    const QDir plugins_dir = QString().fromStdString(ResourceManager::get()->GetConfigurationDirectory()) + "plugins/";

    std::vector<std::string> FileList;

    for(int i = 0; i < QDir(plugins_dir).entryList(QDir::Files).size(); i++)
    {
        /*--------------------------------------*\
        | Add all of the Plugin Files to a list  |
        \*--------------------------------------*/
        FileList.push_back(QDir(plugins_dir).entryList(QDir::Files)[i].toStdString());
    }

    for(const std::string &plugin_name : FileList)
    {
        const std::string plugin_path = plugins_dir.absoluteFilePath(QString().fromStdString(plugin_name)).toStdString();

        LoadPlugin(plugin_path);
    }
}

void PluginManager::LoadPlugin(std::string path)
{
    OpenRGBPluginInterface *OpenRGBPlugin = nullptr;

    LOG_VERBOSE("Attempting to load: %s", path.c_str());

    QPluginLoader loader(QString().fromStdString(path));

    if (QObject *instance = loader.instance())
    {
        if ((OpenRGBPlugin = qobject_cast<OpenRGBPluginInterface*>(instance)))
        {
            /*-----------------------------------------------------*\
            | Initialize the plugin                                 |
            \*-----------------------------------------------------*/
            OpenRGBPlugin->info = OpenRGBPlugin->Initialize(dark_theme, ResourceManager::get());

            /*-----------------------------------------------------*\
            | Search the settings to see if it is enabled           |
            \*-----------------------------------------------------*/
            std::string     name        = "";
            std::string     description = "";
            bool            enabled     = true;
            bool            found       = false;
            unsigned int    plugin_ct   = 0;

            /*-------------------------------------------------*\
            | Open device disable list and read in disabled     |
            | device strings                                    |
            \*-------------------------------------------------*/
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

                    if((OpenRGBPlugin->info.PluginName == name)
                     &&(OpenRGBPlugin->info.PluginDescription == description))
                    {
                        found = true;
                        break;
                    }
                }
            }

            if(!found)
            {
                plugin_settings["plugins"][plugin_ct]["name"]           = OpenRGBPlugin->info.PluginName;
                plugin_settings["plugins"][plugin_ct]["description"]    = OpenRGBPlugin->info.PluginDescription;
                plugin_settings["plugins"][plugin_ct]["enabled"]        = enabled;

                ResourceManager::get()->GetSettingsManager()->SetSettings("Plugins", plugin_settings);
                ResourceManager::get()->GetSettingsManager()->SaveSettings();
            }

            LOG_VERBOSE("Loaded plugin %s", OpenRGBPlugin->info.PluginName.c_str());

            OpenRGBPluginEntry entry;

            entry.plugin  = OpenRGBPlugin;
            entry.path    = path;
            entry.enabled = enabled;

            PluginManager::ActivePlugins.push_back(entry);

            /*-------------------------------------------------*\
            | Call the callbacks                                |
            \*-------------------------------------------------*/
            for(unsigned int callback_idx = 0; callback_idx < AddPluginTabCallbacks.size(); callback_idx++)
            {
                AddPluginTabCallbacks[callback_idx](AddPluginTabCallbackArgs[callback_idx], OpenRGBPlugin);
            }
        }
    }
    else
    {
        std::cout << loader.errorString().toStdString() << std::endl;
    }
}
