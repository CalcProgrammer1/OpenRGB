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
    PluginManager(bool dark_theme);

    void RegisterAddPluginCallback(AddPluginCallback new_callback, void * new_callback_arg);
    void RegisterRemovePluginCallback(RemovePluginCallback new_callback, void * new_callback_arg);

    void ScanAndLoadPlugins();

    void AddPlugin(std::string path);
    void RemovePlugin(std::string path);

    void LoadPlugin(std::string path);
    void UnloadPlugin(std::string path);

    void UnloadPlugins();

    std::vector<OpenRGBPluginEntry> ActivePlugins;

private:
    bool dark_theme;

    AddPluginCallback       AddPluginCallbackVal;
    void *                  AddPluginCallbackArg;

    RemovePluginCallback    RemovePluginCallbackVal;
    void *                  RemovePluginCallbackArg;
};
