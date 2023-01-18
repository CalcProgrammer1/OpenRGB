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
    QMenu*                      traymenu;
    std::string                 path;
    bool                        enabled;
} OpenRGBPluginEntry;

typedef void (*AddPluginCallback)(void *, OpenRGBPluginEntry* plugin);
typedef void (*RemovePluginCallback)(void *, OpenRGBPluginEntry* plugin);

class PluginManager
{
public:
    PluginManager();

    void RegisterAddPluginCallback(AddPluginCallback new_callback, void * new_callback_arg);
    void RegisterRemovePluginCallback(RemovePluginCallback new_callback, void * new_callback_arg);

    void ScanAndLoadPlugins();

    void AddPlugin(const filesystem::path& path);
    void RemovePlugin(const filesystem::path& path);

    void LoadPlugin(const filesystem::path& path);
    void UnloadPlugin(const filesystem::path& path);

    void UnloadPlugins();

    std::vector<OpenRGBPluginEntry> ActivePlugins;

private:
    void ScanAndLoadPluginsFrom(const filesystem::path & plugins_dir);

    AddPluginCallback       AddPluginCallbackVal;
    void *                  AddPluginCallbackArg;

    RemovePluginCallback    RemovePluginCallbackVal;
    void *                  RemovePluginCallbackArg;

    const char *            plugins_path = "plugins/";
};
