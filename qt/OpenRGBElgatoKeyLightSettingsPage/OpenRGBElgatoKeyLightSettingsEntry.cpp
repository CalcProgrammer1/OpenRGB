/*---------------------------------------------------------*\
| OpenRGBElgatoKeyLightSettingsEntry.cpp                    |
|                                                           |
|   User interface for OpenRGB Elgato Key Light entry       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBElgatoKeyLightSettingsEntry.h"
#include "ui_OpenRGBElgatoKeyLightSettingsEntry.h"

using namespace Ui;

OpenRGBElgatoKeyLightSettingsEntry::OpenRGBElgatoKeyLightSettingsEntry(QWidget *parent) :
    BaseManualDeviceEntry(parent),
    ui(new Ui::OpenRGBElgatoKeyLightSettingsEntryUi)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

OpenRGBElgatoKeyLightSettingsEntry::~OpenRGBElgatoKeyLightSettingsEntry()
{
    delete ui;
}

void OpenRGBElgatoKeyLightSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBElgatoKeyLightSettingsEntry::loadFromSettings(const json& data)
{
    if(data.contains("ip"))
    {
        ui->IPEdit->setText(QString::fromStdString(data["ip"]));
    }
}

json OpenRGBElgatoKeyLightSettingsEntry::saveSettings()
{
    json result;
    result["ip"] = ui->IPEdit->text().toStdString();
    return result;
}

const char* OpenRGBElgatoKeyLightSettingsEntry::settingsSection()
{
    return "ElgatoKeyLightDevices";
}
