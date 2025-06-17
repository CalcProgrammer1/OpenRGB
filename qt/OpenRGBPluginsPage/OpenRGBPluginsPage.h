/*---------------------------------------------------------*\
| OpenRGBPluginsPage.h                                      |
|                                                           |
|   User interface entry for OpenRGB plugin settings        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "OpenRGBPluginsEntry.h"
#include "PluginManager.h"

namespace Ui
{
    class OpenRGBPluginsPage;
}

class OpenRGBPluginsPage : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBPluginsPage(PluginManager* plugin_manager_ptr, QWidget *parent = nullptr);
    ~OpenRGBPluginsPage();

    void on_EnableButton_clicked(OpenRGBPluginsEntry* entry);
    void RefreshList();

private slots:
    void changeEvent(QEvent *event);
    void on_InstallPluginButton_clicked();

    void on_RemovePluginButton_clicked();

    void on_PluginsList_itemSelectionChanged();

    void on_PluginsList_PluginsDropped(std::vector<std::string>);

private:
    Ui::OpenRGBPluginsPage*     ui;
    PluginManager*              plugin_manager;
    std::vector<OpenRGBPluginsEntry*> entries;

    bool InstallPlugin(std::string path);
};
