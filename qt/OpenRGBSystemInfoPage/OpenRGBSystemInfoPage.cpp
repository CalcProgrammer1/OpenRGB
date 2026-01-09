/*---------------------------------------------------------*\
| OpenRGBSystemInfoPage.cpp                                 |
|                                                           |
|   User interface entry for OpenRGB system information page|
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "OpenRGBSystemInfoPage.h"
#include "ui_OpenRGBSystemInfoPage.h"
#include "ResourceManager.h"
#include "i2c_tools.h"

static void OpenRGBSystemInfoPageResourceManagerCallback(void * this_ptr, unsigned int update_reason)
{
    OpenRGBSystemInfoPage * this_obj = (OpenRGBSystemInfoPage *)this_ptr;

    switch(update_reason)
    {
        case RESOURCEMANAGER_UPDATE_REASON_I2C_BUS_LIST_UPDATED:
            QMetaObject::invokeMethod(this_obj, "UpdateBusList", Qt::QueuedConnection);
            break;
    }
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
    ResourceManager::get()->RegisterResourceManagerCallback(OpenRGBSystemInfoPageResourceManagerCallback, this);

    /*-----------------------------------------------------*\
    | Update the bus list                                   |
    \*-----------------------------------------------------*/
    UpdateBusList();

    ui->SMBusDetectionModeBox->addItem("Auto");
    ui->SMBusDetectionModeBox->addItem("Quick");
    ui->SMBusDetectionModeBox->addItem("Read");
    ui->SMBusDetectionModeBox->addItem("Read Data");

    ui->CommandModeComboBox->addItem("Read Byte");
    ui->CommandModeComboBox->addItem("Read Byte Data");
    ui->CommandModeComboBox->addItem("Read Word Data");
    ui->CommandModeComboBox->addItem("Write Quick");
    ui->CommandModeComboBox->addItem("Write Byte");
    ui->CommandModeComboBox->addItem("Write Byte Data");
    ui->CommandModeComboBox->addItem("Write Word Data");

    ui->SMBusDetectionModeBox->setCurrentIndex(0);
}

OpenRGBSystemInfoPage::~OpenRGBSystemInfoPage()
{
    /*-----------------------------------------------------*\
    | Unregister I2C bus list change callback               |
    \*-----------------------------------------------------*/
    ResourceManager::get()->UnregisterResourceManagerCallback(OpenRGBSystemInfoPageResourceManagerCallback, this);

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

        case 3:
            ui->SMBusDataText->setPlainText(i2c_detect(bus, MODE_READ_DATA).c_str());
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

void OpenRGBSystemInfoPage::on_CommandButton_clicked()
{
    int current_index = ui->SMBusAdaptersBox->currentIndex();

    if(current_index < 0)
    {
        current_index = 0;
    }

    if((int)(busses.size()) > current_index)
    {
        i2c_smbus_interface*    bus         = busses[current_index];
        unsigned char           address     = ui->CommandAddressBox->value();
        unsigned char           regaddr     = ui->CommandRegisterBox->value();
        unsigned short          write_data  = ui->CommandWriteDataLineEdit->text().toInt(nullptr, 0);
        unsigned short          read_data   = 0xFFFF;

        ui->SMBusDataText->clear();

        switch(ui->CommandModeComboBox->currentIndex())
        {
            case 0:
                read_data = bus->i2c_smbus_read_byte(address);
                ui->SMBusDataText->setPlainText(QString::number(read_data, 16));
                break;

            case 1:
                read_data = bus->i2c_smbus_read_byte_data(address, regaddr);
                ui->SMBusDataText->setPlainText(QString::number(read_data, 16));
                break;

            case 2:
                read_data = bus->i2c_smbus_read_word_data(address, regaddr);
                ui->SMBusDataText->setPlainText(QString::number(read_data, 16));
                break;

            case 3:
                bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);
                break;

            case 4:
                bus->i2c_smbus_write_byte(address, write_data);
                break;

            case 5:
                bus->i2c_smbus_write_byte_data(address, regaddr, write_data);
                break;

            case 6:
                bus->i2c_smbus_write_word_data(address, regaddr, write_data);
                break;
        }
    }
}
