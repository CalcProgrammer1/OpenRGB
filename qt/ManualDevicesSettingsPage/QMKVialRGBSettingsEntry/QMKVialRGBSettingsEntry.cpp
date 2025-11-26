/*---------------------------------------------------------*\
| QMKVialRGBSettingsEntry.cpp                               |
|                                                           |
|   User interface entry for VialRGB QMK configuration      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "QMKVialRGBSettingsEntry.h"
#include "ui_QMKVialRGBSettingsEntry.h"

QMKVialRGBSettingsEntry::QMKVialRGBSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::QMKVialRGBSettingsEntry)
{
    ui->setupUi(this);
}

QMKVialRGBSettingsEntry::~QMKVialRGBSettingsEntry()
{
    delete ui;
}

void QMKVialRGBSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void QMKVialRGBSettingsEntry::loadFromSettings(const json& data)
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

json QMKVialRGBSettingsEntry::saveSettings()
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

bool QMKVialRGBSettingsEntry::isDataValid()
{
    // stub
    return true;
}

static BaseManualDeviceEntry* SpawnQMKVialRGBSettingsEntry(const json& data)
{
    QMKVialRGBSettingsEntry* entry = new QMKVialRGBSettingsEntry;
    entry->loadFromSettings(data);
    return entry;
}

static const char* QMKDeviceName = QT_TRANSLATE_NOOP("ManualDevice", "QMK (VialRGB Protocol)");

REGISTER_MANUAL_DEVICE_TYPE(QMKDeviceName, "QMKVialRGBDevices", SpawnQMKVialRGBSettingsEntry);
