/*---------------------------------------------------------*\
| OpenRGBYeelightSettingsPage.cpp                           |
|                                                           |
|   User interface for configuring Yeelight settings        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBYeelightSettingsPage.h"
#include "ui_OpenRGBYeelightSettingsPage.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

OpenRGBYeelightSettingsPage::OpenRGBYeelightSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBYeelightSettingsPage)
{
    ui->setupUi(this);

    json                yeelight_settings;

    /*-------------------------------------------------*\
    | Get Philips Wiz settings                          |
    \*-------------------------------------------------*/
    yeelight_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("YeelightDevices");

    /*-------------------------------------------------*\
    | If the Wiz settings contains devices, process     |
    \*-------------------------------------------------*/
    if(yeelight_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < yeelight_settings["devices"].size(); device_idx++)
        {
            OpenRGBYeelightSettingsEntry* entry = new OpenRGBYeelightSettingsEntry;

            entry->loadFromSettings(yeelight_settings["devices"][device_idx]);

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->YeelightDeviceList->addItem(item);
            ui->YeelightDeviceList->setItemWidget(item, entry);
            ui->YeelightDeviceList->show();
        }
    }
}

OpenRGBYeelightSettingsPage::~OpenRGBYeelightSettingsPage()
{
    delete ui;
}

void OpenRGBYeelightSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBYeelightSettingsPage::on_AddYeelightDeviceButton_clicked()
{
    OpenRGBYeelightSettingsEntry* entry = new OpenRGBYeelightSettingsEntry;
    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->YeelightDeviceList->addItem(item);
    ui->YeelightDeviceList->setItemWidget(item, entry);
    ui->YeelightDeviceList->show();
}

void OpenRGBYeelightSettingsPage::on_RemoveYeelightDeviceButton_clicked()
{
    int cur_row = ui->YeelightDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->YeelightDeviceList->takeItem(cur_row);

    ui->YeelightDeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void OpenRGBYeelightSettingsPage::on_SaveYeelightConfigurationButton_clicked()
{
    json                yeelight_settings;

    /*-------------------------------------------------*\
    | Get Philips Wiz settings                          |
    \*-------------------------------------------------*/
    yeelight_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("YeelightDevices");

    yeelight_settings["devices"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        yeelight_settings["devices"][device_idx] = entries[device_idx]->saveSettings();
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("YeelightDevices", yeelight_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
