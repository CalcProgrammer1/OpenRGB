/*---------------------------------------------------------*\
| OpenRGBNanoleafNewDeviceDialog.cpp                        |
|                                                           |
|   User interface for OpenRGB Nanoleaf dialog              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QCloseEvent>
#include "ResourceManager.h"
#include "OpenRGBNanoleafNewDeviceDialog.h"
#include "ui_OpenRGBNanoleafNewDeviceDialog.h"

#ifdef _WIN32
#include <QSettings>
#endif

OpenRGBNanoleafNewDeviceDialog::OpenRGBNanoleafNewDeviceDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::OpenRGBNanoleafNewDeviceDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->devicePortEdit->setText("16021");
}

OpenRGBNanoleafNewDeviceDialog::~OpenRGBNanoleafNewDeviceDialog()
{
    delete ui;
}

void OpenRGBNanoleafNewDeviceDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

NanoleafDevice OpenRGBNanoleafNewDeviceDialog::show()
{
    NanoleafDevice return_device;

    int result = this->exec();

    if(result != QDialog::Rejected)
    {
        return_device.ip = ui->deviceIPEdit->text().toStdString();
        return_device.port = ui->devicePortEdit->text().toInt();
    }
    else
    {
        return_device.ip = "";
        return_device.port = 0;
    }

    return(return_device);
}
