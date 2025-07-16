/*---------------------------------------------------------*\
| DMXSettingsEntry.cpp                                      |
|                                                           |
|   User interface for OpenRGB DMX settings entry           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "DMXSettingsEntry.h"
#include "ui_DMXSettingsEntry.h"

#include "serial_port.h"
#include <QStandardItemModel>

DMXSettingsEntry::DMXSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::DMXSettingsEntry)
{
    ui->setupUi(this);

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

DMXSettingsEntry::~DMXSettingsEntry()
{
    delete ui;
}

void DMXSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void DMXSettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("name"))
    {
        ui->NameEdit->setText(QString::fromStdString(data["name"]));
    }

    if(data.contains("port"))
    {
        ui->PortComboBox->setCurrentText(QString::fromStdString(data["port"]));
    }

    if(data.contains("red_channel"))
    {
        ui->RedEdit->setText(QString::number((int)data["red_channel"]));
    }

    if(data.contains("green_channel"))
    {
        ui->GreenEdit->setText(QString::number((int)data["green_channel"]));
    }

    if(data.contains("blue_channel"))
    {
        ui->BlueEdit->setText(QString::number((int)data["blue_channel"]));
    }

    if(data.contains("brightness_channel"))
    {
        ui->BrightnessEdit->setText(QString::number((int)data["brightness_channel"]));
    }

    if(data.contains("keepalive_time"))
    {
        ui->KeepaliveTimeEdit->setText(QString::number((int)data["keepalive_time"]));
    }
}

json DMXSettingsEntry::saveSettings()
{
    json result;
    /*-------------------------------------------------*\
    | Required parameters                               |
    \*-------------------------------------------------*/
    result["name"]                     = ui->NameEdit->text().toStdString();
    result["port"]                     = ui->PortComboBox->currentText().toStdString();
    result["red_channel"]              = ui->RedEdit->text().toUInt();
    result["green_channel"]            = ui->GreenEdit->text().toUInt();
    result["blue_channel"]             = ui->BlueEdit->text().toUInt();

    /*-------------------------------------------------*\
    | Optional parameters                               |
    \*-------------------------------------------------*/
    if(ui->BrightnessEdit->text() != "")
    {
        result["brightness_channel"]   = ui->BrightnessEdit->text().toUInt();
    }

    if(ui->KeepaliveTimeEdit->text() != "")
    {
        result["keepalive_time"]       = ui->KeepaliveTimeEdit->text().toUInt();
    }

    return result;
}

bool DMXSettingsEntry::isDataValid()
{
    // stub
    return true;
}

static BaseManualDeviceEntry* SpawnDMXEntry(const json& data)
{
    DMXSettingsEntry* entry = new DMXSettingsEntry;
    entry->loadFromSettings(data);
    return entry;
}

REGISTER_MANUAL_DEVICE_TYPE("DMX", "DMXDevices", SpawnDMXEntry);
