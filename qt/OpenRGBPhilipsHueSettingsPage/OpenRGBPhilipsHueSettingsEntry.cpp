/*---------------------------------------------------------*\
| OpenRGBPhilipsHueSettingsEntry.cpp                        |
|                                                           |
|   User interface for OpenRGB Philips Hue settings entry   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBPhilipsHueSettingsEntry.h"
#include "ui_OpenRGBPhilipsHueSettingsEntry.h"

using namespace Ui;

OpenRGBPhilipsHueSettingsEntry::OpenRGBPhilipsHueSettingsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBPhilipsHueSettingsEntryUi)
{
    ui->setupUi(this);
}

OpenRGBPhilipsHueSettingsEntry::~OpenRGBPhilipsHueSettingsEntry()
{
    delete ui;
}

void OpenRGBPhilipsHueSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBPhilipsHueSettingsEntry::on_UnpairButton_clicked()
{
    ui->UsernameValue->setText("");
    ui->ClientKeyValue->setText("");
}

void OpenRGBPhilipsHueSettingsEntry::loadFromSettings(const json& data)
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

json OpenRGBPhilipsHueSettingsEntry::saveSettings()
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

const char* OpenRGBPhilipsHueSettingsEntry::settingsSection()
{
    return "PhilipsHueDevices";
}
