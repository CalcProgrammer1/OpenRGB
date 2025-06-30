/*---------------------------------------------------------*\
| QMKORGBSettingsEntry.cpp                                  |
|                                                           |
|   User interface entry for OpenRGB QMK configuration      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "QMKORGBSettingsEntry.h"
#include "ui_QMKORGBSettingsEntry.h"

QMKORGBSettingsEntry::QMKORGBSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::QMKORGBSettingsEntry)
{
    ui->setupUi(this);
}

QMKORGBSettingsEntry::~QMKORGBSettingsEntry()
{
    delete ui;
}

void QMKORGBSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void QMKORGBSettingsEntry::loadFromSettings(const json& data)
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

json QMKORGBSettingsEntry::saveSettings()
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

bool QMKORGBSettingsEntry::isDataValid()
{
    // stub
    return true;
}

static BaseManualDeviceEntry* SpawnQMKORGBSettingsEntry(const json& data)
{
    QMKORGBSettingsEntry* entry = new QMKORGBSettingsEntry;
    entry->loadFromSettings(data);
    return entry;
}

static const char* QMKDeviceName = QT_TRANSLATE_NOOP("ManualDevice", "QMK (built with ORGB support)");

REGISTER_MANUAL_DEVICE_TYPE(QMKDeviceName, "QMKOpenRGBDevices", SpawnQMKORGBSettingsEntry);
