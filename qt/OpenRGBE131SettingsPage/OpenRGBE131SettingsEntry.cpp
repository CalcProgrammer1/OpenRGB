/*---------------------------------------------------------*\
| OpenRGBE131SettingsEntry.cpp                              |
|                                                           |
|   User interface for OpenRGB E1.31 settings entry         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBE131SettingsEntry.h"
#include "ui_OpenRGBE131SettingsEntry.h"

using namespace Ui;

OpenRGBE131SettingsEntry::OpenRGBE131SettingsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBE131SettingsEntryUi)
{
    ui->setupUi(this);

    ui->TypeComboBox->addItem(tr("Single"));
    ui->TypeComboBox->addItem(tr("Linear"));
    ui->TypeComboBox->addItem(tr("Matrix"));

    ui->MatrixOrderComboBox->addItem(tr("Horizontal Top Left"));
    ui->MatrixOrderComboBox->addItem(tr("Horizontal Top Right"));
    ui->MatrixOrderComboBox->addItem(tr("Horizontal Bottom Left"));
    ui->MatrixOrderComboBox->addItem(tr("Horizontal Bottom Right"));
    ui->MatrixOrderComboBox->addItem(tr("Vertical Top Left"));
    ui->MatrixOrderComboBox->addItem(tr("Vertical Top Right"));
    ui->MatrixOrderComboBox->addItem(tr("Vertical Bottom Left"));
    ui->MatrixOrderComboBox->addItem(tr("Vertical Bottom Right"));

    ui->RGBOrderComboBox->addItem("RGB");
    ui->RGBOrderComboBox->addItem("RBG");
    ui->RGBOrderComboBox->addItem("GRB");
    ui->RGBOrderComboBox->addItem("GBR");
    ui->RGBOrderComboBox->addItem("BRG");
    ui->RGBOrderComboBox->addItem("BGR");

    HideMatrixSettings();
}

OpenRGBE131SettingsEntry::~OpenRGBE131SettingsEntry()
{
    delete ui;
}

void OpenRGBE131SettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBE131SettingsEntry::HideMatrixSettings()
{
    ui->MatrixWidthLabel->setDisabled(true);
    ui->MatrixWidthEdit->setDisabled(true);

    ui->MatrixHeightLabel->setDisabled(true);
    ui->MatrixHeightEdit->setDisabled(true);

    ui->MatrixOrderLabel->setDisabled(true);
    ui->MatrixOrderComboBox->setDisabled(true);
}

void Ui::OpenRGBE131SettingsEntry::ShowMatrixSettings()
{
    ui->MatrixWidthLabel->setDisabled(false);
    ui->MatrixWidthEdit->setDisabled(false);

    ui->MatrixHeightLabel->setDisabled(false);
    ui->MatrixHeightEdit->setDisabled(false);

    ui->MatrixOrderLabel->setDisabled(false);
    ui->MatrixOrderComboBox->setDisabled(false);
}

void Ui::OpenRGBE131SettingsEntry::on_TypeComboBox_currentIndexChanged(int index)
{
    if(index == 2)
    {
        ShowMatrixSettings();
    }
    else
    {
        HideMatrixSettings();
    }
}
