/*---------------------------------------------------------*\
| OpenRGBSystemInfoPage.cpp                                 |
|                                                           |
|   User interface entry for OpenRGB system information page|
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBSystemInfoPage.h"
#include "ui_OpenRGBSystemInfoPage.h"
#include "ResourceManager.h"
#include "i2c_tools.h"

static void UpdateBusListCallback(void * this_ptr)
{
    OpenRGBSystemInfoPage * this_obj = (OpenRGBSystemInfoPage *)this_ptr;

    QMetaObject::invokeMethod(this_obj, "UpdateBusList", Qt::QueuedConnection);
}

OpenRGBSystemInfoPage::OpenRGBSystemInfoPage(std::vector<i2c_smbus_interface *>& bus, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBSystemInfoPage),
    busses(bus)
{
    ui->setupUi(this);

    /*-----------------------------------------------------*\
    | Use a monospace font for the text box                 |
    \*-----------------------------------------------------*/
    QFont MonoFont("monospace");
    MonoFont.setStyleHint(QFont::Monospace);
    ui->SMBusDataText->setFont(MonoFont);

    /*-----------------------------------------------------*\
    | Register I2C bus list change callback                 |
    \*-----------------------------------------------------*/
    ResourceManager::get()->RegisterI2CBusListChangeCallback(UpdateBusListCallback, this);

    /*-----------------------------------------------------*\
    | Update the bus list                                   |
    \*-----------------------------------------------------*/
    UpdateBusList();

    ui->SMBusDetectionModeBox->addItem("Auto");
    ui->SMBusDetectionModeBox->addItem("Quick");
    ui->SMBusDetectionModeBox->addItem("Read");

    ui->SMBusDetectionModeBox->setCurrentIndex(0);
}

OpenRGBSystemInfoPage::~OpenRGBSystemInfoPage()
{
    delete ui;
}

void OpenRGBSystemInfoPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBSystemInfoPage::UpdateBusList()
{
    /*-----------------------------------------------------*\
    | Fill in the combo boxes with device information       |
    \*-----------------------------------------------------*/
    ui->SMBusAdaptersBox->clear();

    for (std::size_t i = 0; i < busses.size(); i++)
    {
        ui->SMBusAdaptersBox->addItem(busses[i]->device_name);
    }

    ui->SMBusAdaptersBox->setCurrentIndex(0);
}

void OpenRGBSystemInfoPage::on_DetectButton_clicked()
{
    int current_index = ui->SMBusAdaptersBox->currentIndex();

    if(current_index < 0)
    {
        current_index = 0;
    }

    if((int)(busses.size()) > current_index)
    {
        i2c_smbus_interface* bus = busses[current_index];

        switch(ui->SMBusDetectionModeBox->currentIndex())
        {
        case 0:
            ui->SMBusDataText->setPlainText(i2c_detect(bus, MODE_AUTO).c_str());
            break;

        case 1:
            ui->SMBusDataText->setPlainText(i2c_detect(bus, MODE_QUICK).c_str());
            break;

        case 2:
            ui->SMBusDataText->setPlainText(i2c_detect(bus, MODE_READ).c_str());
            break;
        }
    }
}

void OpenRGBSystemInfoPage::on_DumpButton_clicked()
{
    int current_index = ui->SMBusAdaptersBox->currentIndex();

    if(current_index < 0)
    {
        current_index = 0;
    }

    if((int)(busses.size()) > current_index)
    {
        i2c_smbus_interface* bus = busses[current_index];
        unsigned char address = ui->DumpAddressBox->value();

        ui->SMBusDataText->setPlainText(i2c_dump(bus, address).c_str());
    }
}

void OpenRGBSystemInfoPage::on_ReadButton_clicked()
{
    int current_index = ui->SMBusAdaptersBox->currentIndex();

    if(current_index < 0)
    {
        current_index = 0;
    }

    if((int)(busses.size()) > current_index)
    {
        i2c_smbus_interface* bus = busses[current_index];
        unsigned char address = ui->ReadAddressBox->value();
        unsigned char regaddr = ui->ReadRegisterBox->value();
        unsigned char size    = ui->ReadSizeBox->value();

        ui->SMBusDataText->setPlainText(i2c_read(bus, address, regaddr, size).c_str());
    }
}
