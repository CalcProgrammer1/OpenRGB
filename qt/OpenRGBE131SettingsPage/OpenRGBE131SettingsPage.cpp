/*---------------------------------------------------------*\
| OpenRGBE131SettingsPage.cpp                               |
|                                                           |
|   User interface for OpenRGB E1.31 settings page          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBE131SettingsPage.h"
#include "ui_OpenRGBE131SettingsPage.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

using namespace Ui;

OpenRGBE131SettingsPage::OpenRGBE131SettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBE131SettingsPageUi)
{
    ui->setupUi(this);

    json                e131_settings;

    /*-------------------------------------------------*\
    | Get E1.31 settings from settings manager          |
    \*-------------------------------------------------*/
    e131_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("E131Devices");

    /*-------------------------------------------------*\
    | If the E1.31 settings contains devices, process   |
    \*-------------------------------------------------*/
    if(e131_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < e131_settings["devices"].size(); device_idx++)
        {
            OpenRGBE131SettingsEntry* entry = new OpenRGBE131SettingsEntry;

            entry->loadFromSettings(e131_settings["devices"][device_idx]);

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->E131DeviceList->addItem(item);
            ui->E131DeviceList->setItemWidget(item, entry);
            ui->E131DeviceList->show();
        }
    }
}

OpenRGBE131SettingsPage::~OpenRGBE131SettingsPage()
{
    delete ui;
}

void OpenRGBE131SettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBE131SettingsPage::on_AddE131DeviceButton_clicked()
{
    OpenRGBE131SettingsEntry* entry = new OpenRGBE131SettingsEntry;
    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->E131DeviceList->addItem(item);
    ui->E131DeviceList->setItemWidget(item, entry);
    ui->E131DeviceList->show();
}

void Ui::OpenRGBE131SettingsPage::on_RemoveE131DeviceButton_clicked()
{
    int cur_row = ui->E131DeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->E131DeviceList->takeItem(cur_row);

    ui->E131DeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void Ui::OpenRGBE131SettingsPage::on_SaveE131ConfigurationButton_clicked()
{
    json                e131_settings;

    /*-------------------------------------------------*\
    | Get E1.31 settings from settings manager          |
    \*-------------------------------------------------*/
    e131_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("E131Devices");

    e131_settings["devices"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        e131_settings["devices"][device_idx] = entries[device_idx]->saveSettings();
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("E131Devices", e131_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
