/*---------------------------------------------------------*\
| OpenRGBSerialSettingsPage.cpp                             |
|                                                           |
|   User interface for serial device configuration page     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBSerialSettingsPage.h"
#include "ui_OpenRGBSerialSettingsPage.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

using namespace Ui;

OpenRGBSerialSettingsPage::OpenRGBSerialSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBSerialSettingsPageUi)
{
    ui->setupUi(this);

    json                ledstrip_settings;

    /*-------------------------------------------------*\
    | Get LED Strip settings from settings manager      |
    \*-------------------------------------------------*/
    ledstrip_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("LEDStripDevices");

    /*-------------------------------------------------*\
    | If the LEDStrip settings contains devices, process|
    \*-------------------------------------------------*/
    if(ledstrip_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < ledstrip_settings["devices"].size(); device_idx++)
        {
            OpenRGBSerialSettingsEntry* entry = new OpenRGBSerialSettingsEntry;

            entry->loadFromSettings(ledstrip_settings["devices"][device_idx]);

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->SerialDeviceList->addItem(item);
            ui->SerialDeviceList->setItemWidget(item, entry);
            ui->SerialDeviceList->show();
        }
    }
}

OpenRGBSerialSettingsPage::~OpenRGBSerialSettingsPage()
{
    delete ui;
}

void OpenRGBSerialSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBSerialSettingsPage::on_AddSerialDeviceButton_clicked()
{
    OpenRGBSerialSettingsEntry* entry = new OpenRGBSerialSettingsEntry;
    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->SerialDeviceList->addItem(item);
    ui->SerialDeviceList->setItemWidget(item, entry);
    ui->SerialDeviceList->show();
}

void Ui::OpenRGBSerialSettingsPage::on_RemoveSerialDeviceButton_clicked()
{
    int cur_row = ui->SerialDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->SerialDeviceList->takeItem(cur_row);

    ui->SerialDeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void Ui::OpenRGBSerialSettingsPage::on_SaveSerialConfigurationButton_clicked()
{
    json                ledstrip_settings;

    /*-------------------------------------------------*\
    | Get E1.31 settings from settings manager          |
    \*-------------------------------------------------*/
    ledstrip_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("LEDStripDevices");

    ledstrip_settings["devices"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        ledstrip_settings["devices"][device_idx] = entries[device_idx]->saveSettings();
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("LEDStripDevices", ledstrip_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
