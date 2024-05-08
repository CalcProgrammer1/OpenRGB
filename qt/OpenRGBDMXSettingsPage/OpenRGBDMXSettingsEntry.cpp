/*---------------------------------------------------------*\
| OpenRGBDMXSettingsEntry.cpp                               |
|                                                           |
|   User interface for OpenRGB DMX settings entry           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBDMXSettingsEntry.h"
#include "ui_OpenRGBDMXSettingsEntry.h"

using namespace Ui;

OpenRGBDMXSettingsEntry::OpenRGBDMXSettingsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBDMXSettingsEntryUi)
{
    ui->setupUi(this);
}

OpenRGBDMXSettingsEntry::~OpenRGBDMXSettingsEntry()
{
    delete ui;
}

void OpenRGBDMXSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}
