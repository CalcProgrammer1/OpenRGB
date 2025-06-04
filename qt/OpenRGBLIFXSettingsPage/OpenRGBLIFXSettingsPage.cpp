/*---------------------------------------------------------*\
| OpenRGBLIFXSettingsPage.cpp                               |
|                                                           |
|   User interface for OpenRGB LIFX settings page           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBLIFXSettingsPage.h"
#include "ui_OpenRGBLIFXSettingsPage.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

using namespace Ui;

OpenRGBLIFXSettingsPage::OpenRGBLIFXSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBLIFXSettingsPageUi)
{
    ui->setupUi(this);

    json                lifx_settings;

    /*-------------------------------------------------*\
    | Get LIFX settings                                 |
    \*-------------------------------------------------*/
    lifx_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("LIFXDevices");

    /*-------------------------------------------------*\
    | If the Wiz settings contains devices, process     |
    \*-------------------------------------------------*/
    if(lifx_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < lifx_settings["devices"].size(); device_idx++)
        {
            OpenRGBLIFXSettingsEntry* entry = new OpenRGBLIFXSettingsEntry;

            entry->loadFromSettings(lifx_settings["devices"][device_idx]);

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->LIFXDeviceList->addItem(item);
            ui->LIFXDeviceList->setItemWidget(item, entry);
            ui->LIFXDeviceList->show();
        }
    }
}

OpenRGBLIFXSettingsPage::~OpenRGBLIFXSettingsPage()
{
    delete ui;
}

void OpenRGBLIFXSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBLIFXSettingsPage::on_AddLIFXDeviceButton_clicked()
{
    OpenRGBLIFXSettingsEntry* entry = new OpenRGBLIFXSettingsEntry;

    entry->setName(QString("LIFX%1").arg(entries.size()));

    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->LIFXDeviceList->addItem(item);
    ui->LIFXDeviceList->setItemWidget(item, entry);
    ui->LIFXDeviceList->show();
}

void Ui::OpenRGBLIFXSettingsPage::on_RemoveLIFXDeviceButton_clicked()
{
    int cur_row = ui->LIFXDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->LIFXDeviceList->takeItem(cur_row);

    ui->LIFXDeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void Ui::OpenRGBLIFXSettingsPage::on_SaveLIFXConfigurationButton_clicked()
{
    json                lifx_settings;

    /*-------------------------------------------------*\
    | Get LIFX settings                                 |
    \*-------------------------------------------------*/
    lifx_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("LIFXDevices");

    lifx_settings["devices"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        lifx_settings["devices"][device_idx] = entries[device_idx]->saveSettings();
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("LIFXDevices", lifx_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
