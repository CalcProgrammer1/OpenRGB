/*---------------------------------------------------------*\
| OpenRGBSettingsPage.cpp                                   |
|                                                           |
|   User interface for OpenRGB settings page                |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      15 Apr 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <QGroupBox>
#include <QLayoutItem>
#include <QSpacerItem>
#include <QVBoxLayout>
#include "OpenRGBDynamicSettingsWidget.h"
#include "OpenRGBSettingsPage.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "ui_OpenRGBSettingsPage.h"

static void OpenRGBSettingsManagerCallback(void * this_ptr, unsigned int update_reason)
{
    OpenRGBSettingsPage * this_obj = (OpenRGBSettingsPage *)this_ptr;

    switch(update_reason)
    {
        case SETTINGSMANAGER_UPDATE_REASON_SETTINGS_SCHEMA_UPDATED:
            this_obj->UpdateInterface();
            break;
    }
}

static void Callback(void* this_ptr, std::string key, nlohmann::json settings)
{
    ((OpenRGBSettingsPage*)this_ptr)->OnSettingChanged(key, settings);
}

OpenRGBSettingsPage::OpenRGBSettingsPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenRGBSettingsPage)
{
    ui->setupUi(this);

    /*-----------------------------------------------------*\
    | Register settings manager callbacks                   |
    \*-----------------------------------------------------*/
    ResourceManager::get()->GetSettingsManager()->RegisterSettingsManagerCallback(OpenRGBSettingsManagerCallback, this);

    UpdateInterface();
}

OpenRGBSettingsPage::~OpenRGBSettingsPage()
{
    /*-----------------------------------------------------*\
    | Unregister settings manager callbacks                 |
    \*-----------------------------------------------------*/
    ResourceManager::get()->GetSettingsManager()->UnregisterSettingsManagerCallback(OpenRGBSettingsManagerCallback, this);
}

void OpenRGBSettingsPage::OnSettingChanged(std::string key, nlohmann::json settings)
{
    ResourceManager::get()->GetSettingsManager()->ModifySettings(key, settings[key]);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}

void OpenRGBSettingsPage::UpdateInterface()
{
    /*-----------------------------------------------------*\
    | Delete existing items in the layout                   |
    \*-----------------------------------------------------*/
    QLayoutItem*    item;

    while((item = ui->ScrollAreaSettingsLayout->layout()->takeAt(0)) != NULL)
    {
        delete item->widget();
        delete item;
    }

    /*-----------------------------------------------------*\
    | Initialize settings schema                            |
    \*-----------------------------------------------------*/
    nlohmann::json settings_schema  = ResourceManager::get()->GetSettingsManager()->GetSettingsSchema("");

    /*-----------------------------------------------------*\
    | Loop through the schema and create an entry for each  |
    | setting                                               |
    \*-----------------------------------------------------*/
    for(nlohmann::json::iterator key_iterator = settings_schema.begin(); key_iterator != settings_schema.end(); key_iterator++)
    {
        nlohmann::json                  setting_value;
        setting_value[key_iterator.key()]               = ResourceManager::get()->GetSettingsManager()->GetSettings(key_iterator.key());
        OpenRGBDynamicSettingsWidget*   item_widget     = new OpenRGBDynamicSettingsWidget(key_iterator.key(), key_iterator.value(), setting_value);

        item_widget->SetCallback(Callback, this);
        ui->ScrollAreaSettingsLayout->addWidget(item_widget);
    }

    /*-----------------------------------------------------*\
    | Add a spacer at the end to prevent expanding          |
    \*-----------------------------------------------------*/
    QSpacerItem*    spacer          = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->ScrollAreaSettingsLayout->addItem(spacer);
}
