/*---------------------------------------------------------*\
| OpenRGBDeviceInfoPage.cpp                                 |
|                                                           |
|   User interface for OpenRGB device information page      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBDeviceInfoPage.h"
#include "ui_OpenRGBDeviceInfoPage.h"

OpenRGBDeviceInfoPage::OpenRGBDeviceInfoPage(RGBController *dev, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBDeviceInfoPage)
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

    std::string flags_string = "";
    bool        need_separator = false;

    if(dev->flags & CONTROLLER_FLAG_LOCAL)
    {
        flags_string   += "Local";
        need_separator  = true;
    }
    if(dev->flags & CONTROLLER_FLAG_REMOTE)
    {
        if(need_separator)
        {
            flags_string += ", ";
        }
        flags_string   += "Remote";
        need_separator  = true;
    }
    if(dev->flags & CONTROLLER_FLAG_VIRTUAL)
    {
        if(need_separator)
        {
            flags_string += ", ";
        }
        flags_string   += "Virtual";
        need_separator  = true;
    }
    if(dev->flags & CONTROLLER_FLAG_RESET_BEFORE_UPDATE)
    {
        if(need_separator)
        {
            flags_string += ", ";
        }
        flags_string   += "Reset Before Update";
        need_separator  = true;
    }

    ui->FlagsValue->setText(QString::fromStdString(flags_string));
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
