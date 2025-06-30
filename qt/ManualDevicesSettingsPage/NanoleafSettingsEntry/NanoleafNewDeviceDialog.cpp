/*---------------------------------------------------------*\
| NanoleafNewDeviceDialog.cpp                               |
|                                                           |
|   User interface for OpenRGB Nanoleaf dialog              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QCloseEvent>
#include "ResourceManager.h"
#include "NanoleafNewDeviceDialog.h"
#include "ui_NanoleafNewDeviceDialog.h"

#ifdef _WIN32
#include <QSettings>
#endif

NanoleafNewDeviceDialog::NanoleafNewDeviceDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::NanoleafNewDeviceDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->devicePortEdit->setText("16021");
}

NanoleafNewDeviceDialog::~NanoleafNewDeviceDialog()
{
    delete ui;
}

void NanoleafNewDeviceDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

NanoleafDevice NanoleafNewDeviceDialog::show()
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
