/*---------------------------------------------------------*\
| PluginManager.h                                           |
|                                                           |
|   OpenRGB plugin manager                                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QDir>
#include <QLabel>
#include <QPluginLoader>
#include <QtPlugin>
#include "OpenRGBPluginInterface.h"
#include <iostream>
#include <string>

typedef struct
{
    OpenRGBPluginInfo info;
    OpenRGBPluginInterface *plugin;
    QPluginLoader *loader;
    QWidget *widget;
    QMenu *traymenu;
    std::string path;
    bool enabled;
    bool incompatible;
    int api_version;
} OpenRGBPluginEntry;

typedef void (*AddPluginCallback)(void *, OpenRGBPluginEntry *plugin);
typedef void (*RemovePluginCallback)(void *, OpenRGBPluginEntry *plugin);

class PluginManager
{
public:
    PluginManager();

    void RegisterAddPluginCallback(AddPluginCallback new_callback, void *new_callback_arg);
    void RegisterRemovePluginCallback(RemovePluginCallback new_callback, void *new_callback_arg);

    void ScanAndLoadPlugins();

    void AddPlugin(const filesystem::path &path);
    void RemovePlugin(const filesystem::path &path);

    void LoadPlugin(const filesystem::path &path);
    void UnloadPlugin(const filesystem::path &path);

    void UnloadPlugins();

    std::vector<OpenRGBPluginEntry> ActivePlugins;

private:
    void ScanAndLoadPluginsFrom(const filesystem::path &plugins_dir);

    AddPluginCallback AddPluginCallbackVal;
    void *AddPluginCallbackArg;

    RemovePluginCallback RemovePluginCallbackVal;
    void *RemovePluginCallbackArg;

    const char *plugins_path = "plugins/";
};
