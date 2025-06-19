/*---------------------------------------------------------*\
| OpenRGBSerialSettingsEntry.cpp                            |
|                                                           |
|   User interface entry for serial device configuration    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBSerialSettingsEntry.h"
#include "ui_OpenRGBSerialSettingsEntry.h"

OpenRGBSerialSettingsEntry::OpenRGBSerialSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::OpenRGBSerialSettingsEntry)
{
    ui->setupUi(this);

    ui->ProtocolComboBox->addItem("Keyboard Visualizer");
    ui->ProtocolComboBox->addItem("Adalight");
    ui->ProtocolComboBox->addItem("TPM2");
    ui->ProtocolComboBox->addItem("Basic I2C");
}

OpenRGBSerialSettingsEntry::~OpenRGBSerialSettingsEntry()
{
    delete ui;
}

void OpenRGBSerialSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBSerialSettingsEntry::on_ProtocolComboBox_currentIndexChanged(int index)
{
    if(index == 3)
    {
        ui->BaudLabel->setText("Address:");
    }
    else
    {
        ui->BaudLabel->setText("Baud:");
    }
}

void OpenRGBSerialSettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("name"))
    {
        ui->NameEdit->setText(QString::fromStdString(data["name"]));
    }

    if(data.contains("port"))
    {
        ui->PortEdit->setText(QString::fromStdString(data["port"]));
    }

    if(data.contains("baud"))
    {
        ui->BaudEdit->setText(QString::number((int)data["baud"]));
    }

    if(data.contains("num_leds"))
    {
        ui->NumLEDsEdit->setText(QString::number((int)data["num_leds"]));
    }

    if(data.contains("protocol"))
    {
        std::string protocol_string = data["protocol"];

        if(protocol_string == "keyboard_visualizer")
        {
            ui->ProtocolComboBox->setCurrentIndex(0);
        }
        else if(protocol_string == "adalight")
        {
            ui->ProtocolComboBox->setCurrentIndex(1);
        }
        else if(protocol_string == "tpm2")
        {
            ui->ProtocolComboBox->setCurrentIndex(2);
        }
        else if(protocol_string == "basic_i2c")
        {
            ui->ProtocolComboBox->setCurrentIndex(3);
        }
    }
}

json OpenRGBSerialSettingsEntry::saveSettings()
{
    json result;
    /*-------------------------------------------------*\
    | Required parameters                               |
    \*-------------------------------------------------*/
    result["name"]            = ui->NameEdit->text().toStdString();
    result["port"]            = ui->PortEdit->text().toStdString();
    result["num_leds"]        = ui->NumLEDsEdit->text().toUInt();
    result["baud"]            = ui->BaudEdit->text().toUInt();

    switch(ui->ProtocolComboBox->currentIndex())
    {
    case 0:
        result["protocol"] = "keyboard_visualizer";
        break;
    case 1:
        result["protocol"] = "adalight";
        break;
    case 2:
        result["protocol"] = "tpm2";
        break;
    case 3:
        result["protocol"] = "basic_i2c";
        break;
    }

    return result;
}

const char* OpenRGBSerialSettingsEntry::settingsSection()
{
    return "LEDStripDevices";
}
