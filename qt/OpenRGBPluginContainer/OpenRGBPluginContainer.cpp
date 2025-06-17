/*---------------------------------------------------------*\
| OpenRGBPluginContainer.h                                  |
|                                                           |
|   User interface entry for OpenRGB plugin container widget|
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBPluginContainer.h"
#include "ui_OpenRGBPluginContainer.h"

OpenRGBPluginContainer::OpenRGBPluginContainer(QWidget *plugin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBPluginContainer)
{
    ui->setupUi(this);

    plugin_widget = plugin;
    plugin_widget->setParent(this);

    ui->PluginContainerLayout->layout()->addWidget(plugin_widget);

    Hide();
}

OpenRGBPluginContainer::~OpenRGBPluginContainer()
{
    delete ui;
}

void OpenRGBPluginContainer::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBPluginContainer::Hide()
{
    plugin_widget->hide();
    ui->PluginContainerLayout->layout()->invalidate();
}

void OpenRGBPluginContainer::Show()
{
    plugin_widget->show();
    ui->PluginContainerLayout->layout()->invalidate();
}
