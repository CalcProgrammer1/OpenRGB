/*---------------------------------------------------------*\
| OpenRGBSerialSettingsEntry.cpp                            |
|                                                           |
|   User interface entry for serial device configuration    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBSerialSettingsEntry.h"
#include "ui_OpenRGBSerialSettingsEntry.h"

using namespace Ui;

OpenRGBSerialSettingsEntry::OpenRGBSerialSettingsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBSerialSettingsEntryUi)
{
    ui->setupUi(this);

    ui->ProtocolComboBox->addItem("Keyboard Visualizer");
    ui->ProtocolComboBox->addItem("Adalight");
    ui->ProtocolComboBox->addItem("TPM2");
    ui->ProtocolComboBox->addItem("Basic I2C");
}

OpenRGBSerialSettingsEntry::~OpenRGBSerialSettingsEntry()
{
    delete ui;
}

void OpenRGBSerialSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBSerialSettingsEntry::on_ProtocolComboBox_currentIndexChanged(int index)
{
    if(index == 3)
    {
        ui->BaudLabel->setText("Address:");
    }
    else
    {
        ui->BaudLabel->setText("Baud:");
    }
}
