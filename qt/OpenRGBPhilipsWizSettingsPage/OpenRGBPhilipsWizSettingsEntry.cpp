/*---------------------------------------------------------*\
| OpenRGBPhilipsWizSettingsEntry.cpp                        |
|                                                           |
|   User interface for OpenRGB Philips Wiz settings entry   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBPhilipsWizSettingsEntry.h"
#include "ui_OpenRGBPhilipsWizSettingsEntry.h"

using namespace Ui;

OpenRGBPhilipsWizSettingsEntry::OpenRGBPhilipsWizSettingsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBPhilipsWizSettingsEntryUi)
{
    ui->setupUi(this);

    ui->WhiteStrategyComboBox->addItem(tr("Average"));
    ui->WhiteStrategyComboBox->addItem(tr("Minimum"));
}

OpenRGBPhilipsWizSettingsEntry::~OpenRGBPhilipsWizSettingsEntry()
{
    delete ui;
}

void OpenRGBPhilipsWizSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}
