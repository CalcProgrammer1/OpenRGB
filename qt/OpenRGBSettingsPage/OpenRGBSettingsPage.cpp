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
    | Type to track ordering                                |
    \*-----------------------------------------------------*/
    typedef struct
    {
        int                                     order;
        std::string                             key;
        std::reference_wrapper<nlohmann::json>  value;
    } ordered_settings_t;

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
    std::vector<ordered_settings_t> setting_entries;
    nlohmann::json                  settings_schema     = ResourceManager::get()->GetSettingsManager()->GetSettingsSchema("");

    /*-----------------------------------------------------*\
    | Loop through the schema and build a vector containing |
    | the entries in desired order.  Entries with negative  |
    | order (default -1) are put at the end of the list.    |
    \*-----------------------------------------------------*/
    for(nlohmann::json::iterator key_iterator = settings_schema.begin(); key_iterator != settings_schema.end(); key_iterator++)
    {
        int                         order               = -1;

        if(key_iterator.value().contains("order"))
        {
            order                                       = key_iterator.value()["order"];
        }

        ordered_settings_t          setting             = {order, key_iterator.key(), key_iterator.value()};

        if((order < 0) || (setting_entries.size() == 0))
        {
            setting_entries.push_back(setting);
        }
        else
        {
            bool                    added               = false;

            for(std::size_t setting_idx = 0; setting_idx < setting_entries.size(); setting_idx++)
            {
                if((setting_entries[setting_idx].order > order) || (setting_entries[setting_idx].order < 0))
                {
                    setting_entries.insert(setting_entries.begin() + setting_idx, setting);
                    added = true;
                    break;
                }
            }

            if(!added)
            {
                setting_entries.push_back(setting);
            }
        }
    }

    /*-----------------------------------------------------*\
    | Create UI elements for each setting                   |
    \*-----------------------------------------------------*/
    for(std::size_t setting_idx = 0; setting_idx < setting_entries.size(); setting_idx++)
    {
        nlohmann::json                  setting_value;
        setting_value[setting_entries[setting_idx].key] = ResourceManager::get()->GetSettingsManager()->GetSettings(setting_entries[setting_idx].key);
        OpenRGBDynamicSettingsWidget*   item_widget     = new OpenRGBDynamicSettingsWidget(setting_entries[setting_idx].key, setting_entries[setting_idx].value, setting_value);

        item_widget->SetCallback(Callback, this);
        ui->ScrollAreaSettingsLayout->addWidget(item_widget);
    }

    /*-----------------------------------------------------*\
    | Add a spacer at the end to prevent expanding          |
    \*-----------------------------------------------------*/
    QSpacerItem*    spacer          = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->ScrollAreaSettingsLayout->addItem(spacer);
}
