/*---------------------------------------------------------*\
| OpenRGBPluginsEntry.h                                     |
|                                                           |
|   User interface entry for OpenRGB plugin entry widget    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>

namespace Ui
{
    class OpenRGBPluginsEntry;
}

typedef void (*EnableClickCallback)(void *, void *);

struct OpenRGBPluginEntry;

class OpenRGBPluginsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBPluginsEntry(QWidget *parent = nullptr);
    ~OpenRGBPluginsEntry();
    void fillFrom(const OpenRGBPluginEntry* plugin);
    bool isSystem() const;
    bool isPluginEnabled() const;
    std::string getName() const;
    std::string getDescription() const;
    std::string getPath() const;


    void RegisterEnableClickCallback(EnableClickCallback new_callback, void * new_callback_arg);

private slots:
    void changeEvent(QEvent *event);
    void on_EnabledCheckBox_stateChanged(int checked);

private:
    EnableClickCallback EnableClickCallbackVal;
    void *              EnableClickCallbackArg;
    Ui::OpenRGBPluginsEntry * ui;
    bool                      is_system;
};
