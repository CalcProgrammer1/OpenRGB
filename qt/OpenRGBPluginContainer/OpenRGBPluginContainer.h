/*---------------------------------------------------------*\
| OpenRGBPluginContainer.h                                  |
|                                                           |
|   User interface entry for OpenRGB plugin container widget|
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "ui_OpenRGBPluginContainer.h"

namespace Ui
{
    class OpenRGBPluginContainer;
}

class Ui::OpenRGBPluginContainer : public QWidget
{
    Q_OBJECT

public:
    explicit OpenRGBPluginContainer(QWidget *plugin, QWidget *parent = nullptr);
    ~OpenRGBPluginContainer();
    void Hide();
    void Show();

    QWidget* plugin_widget;

private:
    Ui::OpenRGBPluginContainerUi *ui;

private slots:
    void changeEvent(QEvent *event);
};
