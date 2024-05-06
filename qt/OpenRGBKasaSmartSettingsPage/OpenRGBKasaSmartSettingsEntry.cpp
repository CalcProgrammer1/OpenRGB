/*---------------------------------------------------------*\
| OpenRGBKasaSmartSettingsEntry.cpp                         |
|                                                           |
|   User interface for OpenRGB Kasa Smart settings entry    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBKasaSmartSettingsEntry.h"
#include "ui_OpenRGBKasaSmartSettingsEntry.h"

using namespace Ui;

OpenRGBKasaSmartSettingsEntry::OpenRGBKasaSmartSettingsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBKasaSmartSettingsEntryUi)
{
    ui->setupUi(this);
}

OpenRGBKasaSmartSettingsEntry::~OpenRGBKasaSmartSettingsEntry()
{
    delete ui;
}

void OpenRGBKasaSmartSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}
