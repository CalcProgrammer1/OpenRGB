/*---------------------------------------------------------*\
| OpenRGBElgatoLightStripSettingsPage.cpp                   |
|                                                           |
|   User interface for OpenRGB Elgato Light Strips page     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBElgatoLightStripSettingsPage.h"
#include "ui_OpenRGBElgatoLightStripSettingsPage.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

using namespace Ui;

OpenRGBElgatoLightStripSettingsPage::OpenRGBElgatoLightStripSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBElgatoLightStripSettingsPageUi)
{
    ui->setupUi(this);

    json elgato_lightstrip_settings;

    elgato_lightstrip_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("ElgatoLightStripDevices");

    /*---------------------------------------------------------------*\
    | If the Elgato Light Strip settings contains devices, process     |
    \*---------------------------------------------------------------*/
    if(elgato_lightstrip_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < elgato_lightstrip_settings["devices"].size(); device_idx++)
        {
            OpenRGBElgatoLightStripSettingsEntry* entry = new OpenRGBElgatoLightStripSettingsEntry;

            entry->loadFromSettings(elgato_lightstrip_settings["devices"][device_idx]);

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->ElgatoLightStripDeviceList->addItem(item);
            ui->ElgatoLightStripDeviceList->setItemWidget(item, entry);
            ui->ElgatoLightStripDeviceList->show();
        }
    }
}

OpenRGBElgatoLightStripSettingsPage::~OpenRGBElgatoLightStripSettingsPage()
{
    delete ui;
}

void OpenRGBElgatoLightStripSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBElgatoLightStripSettingsPage::on_AddElgatoLightStripDeviceButton_clicked()
{
    OpenRGBElgatoLightStripSettingsEntry* entry = new OpenRGBElgatoLightStripSettingsEntry;
    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->ElgatoLightStripDeviceList->addItem(item);
    ui->ElgatoLightStripDeviceList->setItemWidget(item, entry);
    ui->ElgatoLightStripDeviceList->show();
}

void Ui::OpenRGBElgatoLightStripSettingsPage::on_RemoveElgatoLightStripDeviceButton_clicked()
{
    int cur_row = ui->ElgatoLightStripDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->ElgatoLightStripDeviceList->takeItem(cur_row);

    ui->ElgatoLightStripDeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void Ui::OpenRGBElgatoLightStripSettingsPage::on_SaveElgatoLightStripConfigurationButton_clicked()
{
    json elgato_lightstrip_settings;

    elgato_lightstrip_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("ElgatoLightStripDevices");

    elgato_lightstrip_settings["devices"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        elgato_lightstrip_settings["devices"][device_idx] = entries[device_idx]->saveSettings();
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("ElgatoLightStripDevices", elgato_lightstrip_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
