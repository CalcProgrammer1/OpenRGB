#pragma once

#include "OpenRGBPluginInterface.h"
#include "ResourceManager.h"

#include <QPluginLoader>
#include <QLabel>
#include <QtPlugin>
#include <QDir>

#include <string>
#include <iostream>

typedef struct
{
    OpenRGBPluginInfo           info;
    OpenRGBPluginInterface*     plugin;
    std::string                 path;
    bool                        enabled;
} OpenRGBPluginEntry;

typedef void (*AddPluginTabCallback)(void *, OpenRGBPluginEntry plugin);

class PluginManager
{
public:
    PluginManager(bool dark_theme);

    void RegisterAddPluginTabCallback(AddPluginTabCallback new_callback, void * new_callback_arg);
    void ScanAndLoadPlugins();
    void LoadPlugin(std::string path);

    std::vector<OpenRGBPluginEntry> ActivePlugins;

private:
    bool dark_theme;

    std::vector<AddPluginTabCallback>   AddPluginTabCallbacks;
    std::vector<void *>                 AddPluginTabCallbackArgs;
};
