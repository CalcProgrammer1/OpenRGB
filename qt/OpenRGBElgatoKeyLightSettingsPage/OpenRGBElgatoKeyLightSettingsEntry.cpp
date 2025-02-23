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
    QDialog(parent),
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
