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
#include "ui_OpenRGBPluginsEntry.h"

namespace Ui
{
    class OpenRGBPluginsEntry;
}

typedef void (*EnableClickCallback)(void *, void *);

class Ui::OpenRGBPluginsEntry : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBPluginsEntry(QWidget *parent = nullptr);
    ~OpenRGBPluginsEntry();

    Ui::OpenRGBPluginsEntryUi * ui;
    bool                        is_system;

    void RegisterEnableClickCallback(EnableClickCallback new_callback, void * new_callback_arg);

private slots:
    void changeEvent(QEvent *event);
    void on_EnabledCheckBox_stateChanged(int checked);

private:
    EnableClickCallback EnableClickCallbackVal;
    void *              EnableClickCallbackArg;
};
