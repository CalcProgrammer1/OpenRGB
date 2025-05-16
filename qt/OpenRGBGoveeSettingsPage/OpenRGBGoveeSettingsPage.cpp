/*---------------------------------------------------------*\
| OpenRGBGoveeSettingsPage.cpp                              |
|                                                           |
|   User interface for OpenRGB Govee settings page          |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      15 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBGoveeSettingsPage.h"
#include "ui_OpenRGBGoveeSettingsPage.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

using namespace Ui;

OpenRGBGoveeSettingsPage::OpenRGBGoveeSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBGoveeSettingsPageUi)
{
    ui->setupUi(this);

    json                govee_settings;

    /*-------------------------------------------------*\
    | Get Govee settings                                |
    \*-------------------------------------------------*/
    govee_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("GoveeDevices");

    /*-------------------------------------------------*\
    | If the Govee settings contains devices, process   |
    \*-------------------------------------------------*/
    if(govee_settings.contains("devices"))
    {
        for(unsigned int device_idx = 0; device_idx < govee_settings["devices"].size(); device_idx++)
        {
            OpenRGBGoveeSettingsEntry* entry = new OpenRGBGoveeSettingsEntry;

            if(govee_settings["devices"][device_idx].contains("ip"))
            {
                entry->ui->IPEdit->setText(QString::fromStdString(govee_settings["devices"][device_idx]["ip"]));
            }

            entries.push_back(entry);

            QListWidgetItem* item = new QListWidgetItem;

            item->setSizeHint(entry->sizeHint());

            ui->GoveeDeviceList->addItem(item);
            ui->GoveeDeviceList->setItemWidget(item, entry);
            ui->GoveeDeviceList->show();
        }
    }
}

OpenRGBGoveeSettingsPage::~OpenRGBGoveeSettingsPage()
{
    delete ui;
}

void OpenRGBGoveeSettingsPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBGoveeSettingsPage::on_AddGoveeDeviceButton_clicked()
{
    OpenRGBGoveeSettingsEntry* entry = new OpenRGBGoveeSettingsEntry;
    entries.push_back(entry);

    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(entry->sizeHint());

    ui->GoveeDeviceList->addItem(item);
    ui->GoveeDeviceList->setItemWidget(item, entry);
    ui->GoveeDeviceList->show();
}

void Ui::OpenRGBGoveeSettingsPage::on_RemoveGoveeDeviceButton_clicked()
{
    int cur_row = ui->GoveeDeviceList->currentRow();

    if(cur_row < 0)
    {
        return;
    }

    QListWidgetItem* item = ui->GoveeDeviceList->takeItem(cur_row);

    ui->GoveeDeviceList->removeItemWidget(item);
    delete item;

    delete entries[cur_row];
    entries.erase(entries.begin() + cur_row);
}

void Ui::OpenRGBGoveeSettingsPage::on_SaveGoveeConfigurationButton_clicked()
{
    json                govee_settings;

    /*-------------------------------------------------*\
    | Get Govee settings                                |
    \*-------------------------------------------------*/
    govee_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("GoveeDevices");

    govee_settings["devices"].clear();

    for(unsigned int device_idx = 0; device_idx < entries.size(); device_idx++)
    {
        /*-------------------------------------------------*\
        | Required parameters                               |
        \*-------------------------------------------------*/
        govee_settings["devices"][device_idx]["ip"]             = entries[device_idx]->ui->IPEdit->text().toStdString();
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("GoveeDevices", govee_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
