/*---------------------------------------------------------*\
| PluginManager.h                                           |
|                                                           |
|   OpenRGB plugin manager                                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <iostream>
#include <QPluginLoader>
#include <QLabel>
#include <QtPlugin>
#include <QDir>
#include "OpenRGBPluginInterface.h"

struct OpenRGBPluginEntry
{
    OpenRGBPluginInfo           info;
    OpenRGBPluginInterface*     plugin;
    QPluginLoader*              loader;
    QWidget*                    widget;
    QMenu*                      traymenu;
    std::string                 path;
    bool                        enabled;
    bool                        incompatible;
    bool                        is_system;
    int                         api_version;
};

typedef void (*AddPluginCallback)(void *, OpenRGBPluginEntry* plugin);
typedef void (*RemovePluginCallback)(void *, OpenRGBPluginEntry* plugin);

class PluginManager
{
public:
    PluginManager();

    void RegisterAddPluginCallback(AddPluginCallback new_callback, void * new_callback_arg);
    void RegisterRemovePluginCallback(RemovePluginCallback new_callback, void * new_callback_arg);

    void ScanAndLoadPlugins();

    void AddPlugin(const filesystem::path& path, bool is_system);
    void RemovePlugin(const filesystem::path& path);

    void EnablePlugin(const filesystem::path& path);
    void DisablePlugin(const filesystem::path& path);

    void LoadPlugins();
    void UnloadPlugins();

    std::vector<OpenRGBPluginEntry> ActivePlugins;

private:
    void LoadPlugin(OpenRGBPluginEntry* plugin_entry);
    void UnloadPlugin(OpenRGBPluginEntry* plugin_entry);

    void ScanAndLoadPluginsFrom(const filesystem::path & plugins_dir, bool is_system);

    AddPluginCallback       AddPluginCallbackVal;
    void *                  AddPluginCallbackArg;

    RemovePluginCallback    RemovePluginCallbackVal;
    void *                  RemovePluginCallbackArg;

    const char *            plugins_path = "plugins/";
};
