/*---------------------------------------------------------*\
| OpenRGBPluginsEntry.cpp                                   |
|                                                           |
|   User interface entry for OpenRGB plugin entry widget    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBPluginsEntry.h"
#include "ui_OpenRGBPluginsEntry.h"
#include "PluginManager.h"

OpenRGBPluginsEntry::OpenRGBPluginsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBPluginsEntry)
{
    ui->setupUi(this);

    EnableClickCallbackVal  = nullptr;
    EnableClickCallbackArg  = nullptr;
}

OpenRGBPluginsEntry::~OpenRGBPluginsEntry()
{
    delete ui;
}

void OpenRGBPluginsEntry::fillFrom(const OpenRGBPluginEntry *plugin)
{
    /*---------------------------------------------------------*\
    | Fill in plugin information fields                         |
    \*---------------------------------------------------------*/
    ui->NameValue->setText(QString::fromStdString(plugin->info.Name));
    ui->DescriptionValue->setText(QString::fromStdString(plugin->info.Description));
    ui->VersionValue->setText(QString::fromStdString(plugin->info.Version));
    ui->CommitValue->setText(QString::fromStdString(plugin->info.Commit));
    ui->URLValue->setText(QString::fromStdString(plugin->info.URL));
    ui->APIVersionValue->setText(QString::number(plugin->api_version));

    /*---------------------------------------------------------*\
    | If the plugin is incompatible, highlight the API version  |
    | in red and disable the enable checkbox                    |
    \*---------------------------------------------------------*/
    if(plugin->incompatible)
    {
        ui->APIVersionValue->setStyleSheet("QLabel { color : red; }");
        ui->EnabledCheckBox->setEnabled(false);
    }

    /*---------------------------------------------------------*\
    | Fill in plugin icon                                       |
    \*---------------------------------------------------------*/
    QPixmap pixmap(QPixmap::fromImage(plugin->info.Icon));

    ui->IconView->setPixmap(pixmap);
    ui->IconView->setScaledContents(true);

    /*---------------------------------------------------------*\
    | Fill in plugin path                                       |
    \*---------------------------------------------------------*/
    ui->PathValue->setText(QString::fromStdString(plugin->path));

    /*---------------------------------------------------------*\
    | Fill in plugin enabled status                             |
    \*---------------------------------------------------------*/
    ui->EnabledCheckBox->setChecked((plugin->enabled));

    is_system = plugin->is_system;
}

bool OpenRGBPluginsEntry::isSystem() const
{
    return is_system;
}

bool OpenRGBPluginsEntry::isPluginEnabled() const
{
    return ui->EnabledCheckBox->isChecked();
}

std::string OpenRGBPluginsEntry::getName() const
{
    return ui->NameValue->text().toStdString();
}

std::string OpenRGBPluginsEntry::getDescription() const
{
    return ui->DescriptionValue->text().toStdString();
}

std::string OpenRGBPluginsEntry::getPath() const
{
    return ui->PathValue->text().toStdString();
}

void OpenRGBPluginsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBPluginsEntry::RegisterEnableClickCallback(EnableClickCallback new_callback, void * new_callback_arg)
{
    EnableClickCallbackVal  = new_callback;
    EnableClickCallbackArg  = new_callback_arg;
}

void OpenRGBPluginsEntry::on_EnabledCheckBox_stateChanged(int /*checked*/)
{
    /*-------------------------------------------------*\
    | Call the callbacks                                |
    \*-------------------------------------------------*/
    if(EnableClickCallbackVal != nullptr)
    {
        EnableClickCallbackVal(EnableClickCallbackArg, this);
    }
}
