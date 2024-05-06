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

            if(ledstrip_settings["devices"][device_idx].contains("name"))
            {
                entry->ui->NameEdit->setText(QString::fromStdString(ledstrip_settings["devices"][device_idx]["name"]));
            }

            if(ledstrip_settings["devices"][device_idx].contains("port"))
            {
                entry->ui->PortEdit->setText(QString::fromStdString(ledstrip_settings["devices"][device_idx]["port"]));
            }

            if(ledstrip_settings["devices"][device_idx].contains("baud"))
            {
                entry->ui->BaudEdit->setText(QString::number((int)ledstrip_settings["devices"][device_idx]["baud"]));
            }

            if(ledstrip_settings["devices"][device_idx].contains("num_leds"))
            {
                entry->ui->NumLEDsEdit->setText(QString::number((int)ledstrip_settings["devices"][device_idx]["num_leds"]));
            }

            if(ledstrip_settings["devices"][device_idx].contains("protocol"))
            {
                std::string protocol_string = ledstrip_settings["devices"][device_idx]["protocol"];

                if(protocol_string == "keyboard_visualizer")
                {
                    entry->ui->ProtocolComboBox->setCurrentIndex(0);
                }
                else if(protocol_string == "adalight")
                {
                    entry->ui->ProtocolComboBox->setCurrentIndex(1);
                }
                else if(protocol_string == "tpm2")
                {
                    entry->ui->ProtocolComboBox->setCurrentIndex(2);
                }
                else if(protocol_string == "basic_i2c")
                {
                    entry->ui->ProtocolComboBox->setCurrentIndex(3);
                }
            }

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
        /*-------------------------------------------------*\
        | Required parameters                               |
        \*-------------------------------------------------*/
        ledstrip_settings["devices"][device_idx]["name"]            = entries[device_idx]->ui->NameEdit->text().toStdString();
        ledstrip_settings["devices"][device_idx]["port"]            = entries[device_idx]->ui->PortEdit->text().toStdString();
        ledstrip_settings["devices"][device_idx]["num_leds"]        = entries[device_idx]->ui->NumLEDsEdit->text().toUInt();
        ledstrip_settings["devices"][device_idx]["baud"]            = entries[device_idx]->ui->BaudEdit->text().toUInt();

        switch(entries[device_idx]->ui->ProtocolComboBox->currentIndex())
        {
            case 0:
                ledstrip_settings["devices"][device_idx]["protocol"] = "keyboard_visualizer";
                break;
            case 1:
                ledstrip_settings["devices"][device_idx]["protocol"] = "adalight";
                break;
            case 2:
                ledstrip_settings["devices"][device_idx]["protocol"] = "tpm2";
                break;
            case 3:
                ledstrip_settings["devices"][device_idx]["protocol"] = "basic_i2c";
                break;
        }
    }

    ResourceManager::get()->GetSettingsManager()->SetSettings("LEDStripDevices", ledstrip_settings);
    ResourceManager::get()->GetSettingsManager()->SaveSettings();
}
