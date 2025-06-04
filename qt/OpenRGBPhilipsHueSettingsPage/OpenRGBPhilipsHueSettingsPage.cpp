/*---------------------------------------------------------*\
| OpenRGBPhilipsHueSettingsPage.cpp                         |
|                                                           |
|   User interface for OpenRGB Philips Hue settings page    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBPhilipsHueSettingsPage.h"
#include "ui_OpenRGBPhilipsHueSettingsPage.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

using namespace Ui;

OpenRGBPhilipsHueSettingsPage::OpenRGBPhilipsHueSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBPhilipsHueSettingsPageUi)
{
    ui->setupUi(this);

    json                hue_settings;

    /*-------------------------------------------------*\
    | Get Philips Hue settings                          |
    \*-------------------------------------------------*/
    hue_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("PhilipsHueDevices");

    /*-------------------------------------------------*\
    | If the Hue settings contains bridges, process     |
    \*-------------------------------------------------*/
    if(hue_settings.contains("bridges"))
    {
        for(unsigned int device_idx = 0; device_idx < hue_settings["bridges"].size(); device_idx++)
        {
            OpenRGBPhilipsHueSettingsEntry* entry = new OpenRGBPhilipsHueSettingsEntry;

            entry->loadFromSettings(hue_settings["bridges"][device_idx]);

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->PhilipsHueDeviceList->addItem(item);
            ui->PhilipsHueDeviceList->setItemWidget(item, entry);
            ui->PhilipsHueDeviceList->show();
        }
    }
}

OpenRGBPhilipsHueSettingsPage::~OpenRGBPhilipsHueSettingsPage()
{
    delete ui;
}

void OpenRGBPhilipsHueSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBPhilipsHueSettingsPage::on_AddPhilipsHueDeviceButton_clicked()
{
    OpenRGBPhilipsHueSettingsEntry* entry = new OpenRGBPhilipsHueSettingsEntry;
    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->PhilipsHueDeviceList->addItem(item);
    ui->PhilipsHueDeviceList->setItemWidget(item, entry);
    ui->PhilipsHueDeviceList->show();
}

void Ui::OpenRGBPhilipsHueSettingsPage::on_RemovePhilipsHueDeviceButton_clicked()
{
    int cur_row = ui->PhilipsHueDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->PhilipsHueDeviceList->takeItem(cur_row);

    ui->PhilipsHueDeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void Ui::OpenRGBPhilipsHueSettingsPage::on_SavePhilipsHueConfigurationButton_clicked()
{
    json                hue_settings;

    /*-------------------------------------------------*\
    | Get Philips Hue settings                          |
    \*-------------------------------------------------*/
    hue_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("PhilipsHueDevices");

    hue_settings["bridges"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        hue_settings["bridges"][device_idx] = entries[device_idx]->saveSettings();
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("PhilipsHueDevices", hue_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
