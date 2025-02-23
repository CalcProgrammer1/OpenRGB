/*---------------------------------------------------------*\
| OpenRGBElgatoLightStripSettingsEntry.cpp                  |
|                                                           |
|   User interface for OpenRGB Elgato Light Strips entry    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBElgatoLightStripSettingsEntry.h"
#include "ui_OpenRGBElgatoLightStripSettingsEntry.h"

using namespace Ui;

OpenRGBElgatoLightStripSettingsEntry::OpenRGBElgatoLightStripSettingsEntry(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenRGBElgatoLightStripSettingsEntryUi)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

OpenRGBElgatoLightStripSettingsEntry::~OpenRGBElgatoLightStripSettingsEntry()
{
    delete ui;
}

void OpenRGBElgatoLightStripSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}
