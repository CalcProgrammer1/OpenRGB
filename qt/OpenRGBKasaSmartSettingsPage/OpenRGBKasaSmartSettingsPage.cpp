/*---------------------------------------------------------*\
| OpenRGBKasaSmartSettingsPage.cpp                          |
|                                                           |
|   User interface for OpenRGB Kasa Smart settings page     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBKasaSmartSettingsPage.h"
#include "ui_OpenRGBKasaSmartSettingsPage.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

using namespace Ui;

OpenRGBKasaSmartSettingsPage::OpenRGBKasaSmartSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBKasaSmartSettingsPageUi)
{
    ui->setupUi(this);

    json                KasaSmart_settings;

    /*-------------------------------------------------*\
    | Get KasaSmart settings                                 |
    \*-------------------------------------------------*/
    KasaSmart_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("KasaSmartDevices");

    /*-------------------------------------------------*\
    | If the Wiz settings contains devices, process     |
    \*-------------------------------------------------*/
    if(KasaSmart_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < KasaSmart_settings["devices"].size(); device_idx++)
        {
            OpenRGBKasaSmartSettingsEntry* entry = new OpenRGBKasaSmartSettingsEntry;

            entry->loadFromSettings(KasaSmart_settings["devices"][device_idx]);

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->KasaSmartDeviceList->addItem(item);
            ui->KasaSmartDeviceList->setItemWidget(item, entry);
            ui->KasaSmartDeviceList->show();
        }
    }
}

OpenRGBKasaSmartSettingsPage::~OpenRGBKasaSmartSettingsPage()
{
    delete ui;
}

void OpenRGBKasaSmartSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBKasaSmartSettingsPage::on_AddKasaSmartDeviceButton_clicked()
{
    OpenRGBKasaSmartSettingsEntry* entry = new OpenRGBKasaSmartSettingsEntry;

    entry->setName(QString("KasaSmart%1").arg(entries.size()));

    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->KasaSmartDeviceList->addItem(item);
    ui->KasaSmartDeviceList->setItemWidget(item, entry);
    ui->KasaSmartDeviceList->show();
}

void Ui::OpenRGBKasaSmartSettingsPage::on_RemoveKasaSmartDeviceButton_clicked()
{
    int cur_row = ui->KasaSmartDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->KasaSmartDeviceList->takeItem(cur_row);

    ui->KasaSmartDeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void Ui::OpenRGBKasaSmartSettingsPage::on_SaveKasaSmartConfigurationButton_clicked()
{
    json                KasaSmart_settings;

    /*-------------------------------------------------*\
    | Get KasaSmart settings                                 |
    \*-------------------------------------------------*/
    KasaSmart_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("KasaSmartDevices");

    KasaSmart_settings["devices"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        /*-------------------------------------------------*\
        | Required parameters                               |
        \*-------------------------------------------------*/
        KasaSmart_settings["devices"][device_idx] = entries[device_idx]->saveSettings();
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("KasaSmartDevices", KasaSmart_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
