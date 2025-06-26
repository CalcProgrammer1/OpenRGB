/*---------------------------------------------------------*\
| PhilipsHueSettingsEntry.cpp                               |
|                                                           |
|   User interface for OpenRGB Philips Hue settings entry   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "PhilipsHueSettingsEntry.h"
#include "ui_PhilipsHueSettingsEntry.h"

PhilipsHueSettingsEntry::PhilipsHueSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::PhilipsHueSettingsEntry)
{
    ui->setupUi(this);
}

PhilipsHueSettingsEntry::~PhilipsHueSettingsEntry()
{
    delete ui;
}

void PhilipsHueSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void PhilipsHueSettingsEntry::on_UnpairButton_clicked()
{
    ui->UsernameValue->setText("");
    ui->ClientKeyValue->setText("");
}

void PhilipsHueSettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("ip"))
    {
        ui->IPEdit->setText(QString::fromStdString(data["ip"]));
    }

    if(data.contains("mac"))
    {
        ui->MACEdit->setText(QString::fromStdString(data["mac"]));
    }

    if(data.contains("entertainment"))
    {
        ui->EntertainmentCheckBox->setChecked(data["entertainment"]);
    }

    if(data.contains("autoconnect"))
    {
        ui->AutoConnectCheckBox->setChecked(data["autoconnect"]);
    }

    if(data.contains("username"))
    {
        ui->UsernameValue->setText(QString::fromStdString(data["username"]));
    }

    if(data.contains("clientkey"))
    {
        ui->ClientKeyValue->setText(QString::fromStdString(data["clientkey"]));
    }
}

json PhilipsHueSettingsEntry::saveSettings()
{
    json result;
    /*-------------------------------------------------*\
    | Required parameters                               |
    \*-------------------------------------------------*/
    result["ip"]               = ui->IPEdit->text().toStdString();
    result["mac"]              = ui->MACEdit->text().toStdString();
    result["entertainment"]    = ui->EntertainmentCheckBox->isChecked();
    result["autoconnect"]      = ui->AutoConnectCheckBox->isChecked();

    if(ui->UsernameValue->text() != "")
    {
        result["username"]     = ui->UsernameValue->text().toStdString();
    }

    if(ui->ClientKeyValue->text() != "")
    {
        result["clientkey"]    = ui->ClientKeyValue->text().toStdString();
    }

    return result;
}

bool PhilipsHueSettingsEntry::isDataValid()
{
    // stub
    return true;
}

static BaseManualDeviceEntry* SpawnPhilipsHueSettingsEntry(const json& data)
{
    PhilipsHueSettingsEntry* entry = new PhilipsHueSettingsEntry;
    entry->loadFromSettings(data);
    return entry;
}

REGISTER_MANUAL_DEVICE_TYPE("Philips Hue", "PhilipsHueDevices", SpawnPhilipsHueSettingsEntry);
