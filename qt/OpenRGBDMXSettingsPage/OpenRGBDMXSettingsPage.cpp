/*---------------------------------------------------------*\
| OpenRGBDMXSettingsPage.cpp                                |
|                                                           |
|   User interface for OpenRGB DMX settings page            |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBDMXSettingsPage.h"
#include "ui_OpenRGBDMXSettingsPage.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

OpenRGBDMXSettingsPage::OpenRGBDMXSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBDMXSettingsPage)
{
    ui->setupUi(this);

    json                dmx_settings;

    /*-------------------------------------------------*\
    | Get DMX settings from settings manager            |
    \*-------------------------------------------------*/
    dmx_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("DMXDevices");

    /*-------------------------------------------------*\
    | If the DMX settings contains devices, process     |
    \*-------------------------------------------------*/
    if(dmx_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < dmx_settings["devices"].size(); device_idx++)
        {
            OpenRGBDMXSettingsEntry* entry = new OpenRGBDMXSettingsEntry;

            entry->loadFromSettings(dmx_settings["devices"][device_idx]);

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->DMXDeviceList->addItem(item);
            ui->DMXDeviceList->setItemWidget(item, entry);
            ui->DMXDeviceList->show();
        }
    }
}

OpenRGBDMXSettingsPage::~OpenRGBDMXSettingsPage()
{
    delete ui;
}

void OpenRGBDMXSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBDMXSettingsPage::on_AddDMXDeviceButton_clicked()
{
    OpenRGBDMXSettingsEntry* entry = new OpenRGBDMXSettingsEntry;
    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->DMXDeviceList->addItem(item);
    ui->DMXDeviceList->setItemWidget(item, entry);
    ui->DMXDeviceList->show();
}

void OpenRGBDMXSettingsPage::on_RemoveDMXDeviceButton_clicked()
{
    int cur_row = ui->DMXDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->DMXDeviceList->takeItem(cur_row);

    ui->DMXDeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void OpenRGBDMXSettingsPage::on_SaveDMXConfigurationButton_clicked()
{
    json                dmx_settings;

    /*-------------------------------------------------*\
    | Get DMX settings from settings manager            |
    \*-------------------------------------------------*/
    dmx_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("DMXDevices");

    dmx_settings["devices"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        dmx_settings["devices"][device_idx] = entries[device_idx]->saveSettings();
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("DMXDevices", dmx_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
