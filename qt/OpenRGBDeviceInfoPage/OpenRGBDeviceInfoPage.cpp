/*---------------------------------------------------------*\
| OpenRGBDeviceInfoPage.cpp                                 |
|                                                           |
|   User interface for OpenRGB device information page      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBDeviceInfoPage.h"

using namespace Ui;

OpenRGBDeviceInfoPage::OpenRGBDeviceInfoPage(RGBController *dev, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBDeviceInfoPageUi)
{
    controller = dev;

    ui->setupUi(this);

    ui->TypeValue->setText(device_type_to_str(dev->type).c_str());

    ui->NameValue->setText(QString::fromStdString(dev->GetName()));
    ui->VendorValue->setText(QString::fromStdString(dev->GetVendor()));
    ui->DescriptionValue->setText(QString::fromStdString(dev->GetDescription()));
    ui->VersionValue->setText(QString::fromStdString(dev->GetVersion()));
    ui->LocationValue->setText(QString::fromStdString(dev->GetLocation()));
    ui->SerialValue->setText(QString::fromStdString(dev->GetSerial()));
}

OpenRGBDeviceInfoPage::~OpenRGBDeviceInfoPage()
{
    delete ui;
}

void OpenRGBDeviceInfoPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

RGBController* OpenRGBDeviceInfoPage::GetController()
{
    return controller;
}
