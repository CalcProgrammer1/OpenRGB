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
    QPluginLoader*              loader;
    bool                        loaded;
    QWidget*                    widget;
    std::string                 path;
    bool                        enabled;
} OpenRGBPluginEntry;

typedef void (*AddPluginTabCallback)(void *, OpenRGBPluginEntry* plugin);
typedef void (*RemovePluginTabCallback)(void *, OpenRGBPluginEntry* plugin);

class PluginManager
{
public:
    PluginManager(bool dark_theme);

    void RegisterAddPluginTabCallback(AddPluginTabCallback new_callback, void * new_callback_arg);
    void RegisterRemovePluginTabCallback(RemovePluginTabCallback new_callback, void * new_callback_arg);

    void ScanAndLoadPlugins();

    void AddPlugin(std::string path);
    void RemovePlugin(std::string path);

    void LoadPlugin(std::string path);
    void UnloadPlugin(std::string path);

    std::vector<OpenRGBPluginEntry> ActivePlugins;

private:
    bool dark_theme;

    std::vector<AddPluginTabCallback>       AddPluginTabCallbacks;
    std::vector<void *>                     AddPluginTabCallbackArgs;

    std::vector<RemovePluginTabCallback>    RemovePluginTabCallbacks;
    std::vector<void *>                     RemovePluginTabCallbackArgs;
};
