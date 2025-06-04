/*---------------------------------------------------------*\
| OpenRGBQMKORGBSettingsEntry.cpp                           |
|                                                           |
|   User interface entry for OpenRGB QMK configuration      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBQMKORGBSettingsEntry.h"
#include "ui_OpenRGBQMKORGBSettingsEntry.h"

using namespace Ui;

OpenRGBQMKORGBSettingsEntry::OpenRGBQMKORGBSettingsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBQMKORGBSettingsEntryUi)
{
    ui->setupUi(this);
}

OpenRGBQMKORGBSettingsEntry::~OpenRGBQMKORGBSettingsEntry()
{
    delete ui;
}

void OpenRGBQMKORGBSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBQMKORGBSettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("name"))
    {
        ui->NameEdit->setText(QString::fromStdString(data["name"]));
    }

    if(data.contains("usb_vid"))
    {
        ui->USBVIDEdit->setText(QString::fromStdString(data["usb_vid"]));
    }

    if(data.contains("usb_pid"))
    {
        ui->USBPIDEdit->setText(QString::fromStdString(data["usb_pid"]));
    }
}

json OpenRGBQMKORGBSettingsEntry::saveSettings()
{
    json result;
    /*-------------------------------------------------*\
    | Required parameters                               |
    \*-------------------------------------------------*/
    result["name"]     = ui->NameEdit->text().toStdString();
    result["usb_vid"]  = ui->USBVIDEdit->text().toStdString();
    result["usb_pid"]  = ui->USBPIDEdit->text().toStdString();

    return result;
}

const char* OpenRGBQMKORGBSettingsEntry::settingsSection()
{
    return "QMKOpenRGBDevices";
}
