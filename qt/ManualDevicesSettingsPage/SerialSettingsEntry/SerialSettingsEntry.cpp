/*---------------------------------------------------------*\
| SerialSettingsEntry.cpp                                   |
|                                                           |
|   User interface entry for serial device configuration    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "SerialSettingsEntry.h"
#include "ui_SerialSettingsEntry.h"

#include "serial_port.h"
#include <QStandardItemModel>

SerialSettingsEntry::SerialSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::SerialSettingsEntry)
{
    ui->setupUi(this);

    ui->ProtocolComboBox->addItem("Keyboard Visualizer");
    ui->ProtocolComboBox->addItem("Adalight");
    ui->ProtocolComboBox->addItem("TPM2");
    ui->ProtocolComboBox->addItem("Basic I2C");

    std::vector<std::string> serialPorts = serial_port::getSerialPorts();
    for(size_t i = 0; i < serialPorts.size(); ++i)
    {
        ui->PortComboBox->addItem(QString::fromStdString(serialPorts[i]));
    }
    if(serialPorts.empty())
    {
        /*---------------------------------------------------*\
        | When no ports were found, add an unselectable entry |
        | denoting this fact istead                           |
        \*---------------------------------------------------*/
        QStandardItemModel* comboBoxModel = qobject_cast<QStandardItemModel *>(ui->PortComboBox->model());
        if(comboBoxModel != nullptr)
        {
            ui->PortComboBox->addItem(tr("No serial ports found"));
            QStandardItem *item = comboBoxModel->item(0);
            item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        }
    }
    ui->PortComboBox->clearEditText();
}

SerialSettingsEntry::~SerialSettingsEntry()
{
    delete ui;
}

void SerialSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void SerialSettingsEntry::on_ProtocolComboBox_currentIndexChanged(int index)
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

void SerialSettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("name"))
    {
        ui->NameEdit->setText(QString::fromStdString(data["name"]));
    }

    if(data.contains("port"))
    {
        ui->PortComboBox->setCurrentText(QString::fromStdString(data["port"]));
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

json SerialSettingsEntry::saveSettings()
{
    json result;
    /*-------------------------------------------------*\
    | Required parameters                               |
    \*-------------------------------------------------*/
    result["name"]            = ui->NameEdit->text().toStdString();
    result["port"]            = ui->PortComboBox->currentText().toStdString();
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

bool SerialSettingsEntry::isDataValid()
{
    // stub
    return true;
}

static BaseManualDeviceEntry* SpawnSerialSettingsEntry(const json& data)
{
    SerialSettingsEntry* entry = new SerialSettingsEntry;
    entry->loadFromSettings(data);
    return entry;
}

static const char* SerialDeviceName = QT_TRANSLATE_NOOP("ManualDevice", "Serial Device");

REGISTER_MANUAL_DEVICE_TYPE(SerialDeviceName, "LEDStripDevices", SpawnSerialSettingsEntry);
