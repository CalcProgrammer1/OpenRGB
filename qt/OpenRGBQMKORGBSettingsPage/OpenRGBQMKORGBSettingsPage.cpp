/*---------------------------------------------------------*\
| OpenRGBQMKORGBSettingsPage.cpp                            |
|                                                           |
|   User interface for OpenRGB QMK device configuration page|
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBQMKORGBSettingsPage.h"
#include "ui_OpenRGBQMKORGBSettingsPage.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

using namespace Ui;

OpenRGBQMKORGBSettingsPage::OpenRGBQMKORGBSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBQMKORGBSettingsPageUi)
{
    ui->setupUi(this);

    json                qmk_settings;

    /*-------------------------------------------------*\
    | Get QMKOpenRGB settings                           |
    \*-------------------------------------------------*/
    qmk_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("QMKOpenRGBDevices");

    /*-------------------------------------------------*\
    | If the LEDStrip settings contains devices, process|
    \*-------------------------------------------------*/
    if(qmk_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < qmk_settings["devices"].size(); device_idx++)
        {
            OpenRGBQMKORGBSettingsEntry* entry = new OpenRGBQMKORGBSettingsEntry;

            entry->loadFromSettings(qmk_settings["devices"][device_idx]);

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->QMKORGBDeviceList->addItem(item);
            ui->QMKORGBDeviceList->setItemWidget(item, entry);
            ui->QMKORGBDeviceList->show();
        }
    }
}

OpenRGBQMKORGBSettingsPage::~OpenRGBQMKORGBSettingsPage()
{
    delete ui;
}

void OpenRGBQMKORGBSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBQMKORGBSettingsPage::on_AddQMKORGBDeviceButton_clicked()
{
    OpenRGBQMKORGBSettingsEntry* entry = new OpenRGBQMKORGBSettingsEntry;
    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->QMKORGBDeviceList->addItem(item);
    ui->QMKORGBDeviceList->setItemWidget(item, entry);
    ui->QMKORGBDeviceList->show();
}

void Ui::OpenRGBQMKORGBSettingsPage::on_RemoveQMKORGBDeviceButton_clicked()
{
    int cur_row = ui->QMKORGBDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->QMKORGBDeviceList->takeItem(cur_row);

    ui->QMKORGBDeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void Ui::OpenRGBQMKORGBSettingsPage::on_SaveQMKORGBConfigurationButton_clicked()
{
    json                qmk_settings;

    /*-------------------------------------------------*\
    | Get QMKOpenRGB settings                           |
    \*-------------------------------------------------*/
    qmk_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("QMKOpenRGBDevices");

    qmk_settings["devices"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        /*-------------------------------------------------*\
        | Required parameters                               |
        \*-------------------------------------------------*/
        qmk_settings["devices"][device_idx] = entries[device_idx]->saveSettings();
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("QMKOpenRGBDevices", qmk_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
