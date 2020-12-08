#include "PluginManager.h"

void PluginManager::ScanAndLoadPlugins()
{
    std::string OpenRGBConfigDir = ResourceManager::get()->GetConfigurationDirectory();

    std::string PluginPath = OpenRGBConfigDir + "/Plugins";

    /*--------------------------------------------------------------------------------------*\
    | I used https://github.com/krf/cmake-qtqml-plugin-example to figure out how to do this  |
    | So BIG credit to krf                                                                   |
    \*--------------------------------------------------------------------------------------*/
    OpenRGBPluginInterface *OpenRGBPlugin = nullptr;

    const QDir pluginsDir = QString().fromStdString(ResourceManager::get()->GetConfigurationDirectory()) + "plugins/";

    std::vector<std::string> FileList;

    for(int i = 0; i < QDir(pluginsDir).entryList(QDir::Files).size(); i++)
    {
        /*--------------------------------------*\
        | Add all of the Plugin Files to a list  |
        \*--------------------------------------*/
        FileList.push_back(QDir(pluginsDir).entryList(QDir::Files)[i].toStdString());
    }

    for(const std::string &fileName : FileList)
    {
        const std::string filePath = pluginsDir.absoluteFilePath(QString().fromStdString(fileName)).toStdString();

        QPluginLoader loader(pluginsDir.absoluteFilePath(QString().fromStdString(fileName)));

        if (QObject *instance = loader.instance())
        {
            if ((OpenRGBPlugin = qobject_cast<OpenRGBPluginInterface*>(instance)))
            {
                PluginManager::ActivePlugins.push_back(OpenRGBPlugin);
            }
        }
        else
        {
            std::cout << loader.errorString().toStdString() << std::endl;
        }
    }
}
