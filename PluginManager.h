/*---------------------------------------------------------*\
| PluginManager.h                                           |
|                                                           |
|   OpenRGB plugin manager                                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <iostream>
#include <QPluginLoader>
#include <QLabel>
#include <QtPlugin>
#include <QDir>
#include "OpenRGBPluginInterface.h"
#include "PluginManagerInterface.h"

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

class PluginManager : public PluginManagerInterface
{
public:
    PluginManager();

    unsigned int    GetPluginCount();
    std::string     GetPluginDescription(unsigned int plugin_idx);
    std::string     GetPluginName(unsigned int plugin_idx);
    unsigned int    GetPluginProtocolVersion(unsigned int plugin_idx);
    std::string     GetPluginVersion(unsigned int plugin_idx);

    void RegisterAddPluginCallback(AddPluginCallback new_callback, void * new_callback_arg);
    void RegisterRemovePluginCallback(RemovePluginCallback new_callback, void * new_callback_arg);

    void ScanAndLoadPlugins();

    void AddPlugin(const filesystem::path& path, bool is_system);
    void RemovePlugin(const filesystem::path& path);

    void EnablePlugin(const filesystem::path& path);
    void DisablePlugin(const filesystem::path& path);

    void LoadPlugins();
    void UnloadPlugins();

    void OnProfileAboutToLoad();
    void OnProfileLoad(nlohmann::json profile_data);
    nlohmann::json OnProfileSave();
    unsigned char * OnSDKCommand(unsigned int plugin_idx, unsigned int pkt_id, unsigned char * pkt_data, unsigned int * pkt_size);

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
