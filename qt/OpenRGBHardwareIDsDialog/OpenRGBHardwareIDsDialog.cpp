/*---------------------------------------------------------*\
| OpenRGBHardwareIDsDialog.cpp                              |
|                                                           |
|   User interface for OpenRGB Hardware IDs dialog          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <QString>
#include <QClipboard>
#include <hidapi.h>
#include <libusb.h>
#include "OpenRGBHardwareIDsDialog.h"
#include "ui_OpenRGBHardwareIDsDialog.h"
#include "ResourceManager.h"
#include "StringUtils.h"

OpenRGBHardwareIDsDialog::OpenRGBHardwareIDsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenRGBHardwareIDsDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->HardwareIdsList->header()->resizeSection(0 /*column index*/, 300 /*width*/);
    ui->HardwareIdsList->header()->resizeSection(1 /*column index*/, 200 /*width*/);
    ui->HardwareIdsList->header()->resizeSection(2 /*column index*/, 100 /*width*/);
}

OpenRGBHardwareIDsDialog::~OpenRGBHardwareIDsDialog()
{
    delete ui;
}

int OpenRGBHardwareIDsDialog::show()
{
    /*-----------------------------------------------------*\
    | Add I2C Buses                                         |
    \*-----------------------------------------------------*/
    std::vector<i2c_smbus_info> i2c_bus_info    = ResourceManager::get()->GetI2CBusInfo();
    QTreeWidgetItem*            i2c_top         = new QTreeWidgetItem(ui->HardwareIdsList, {"I2C Buses"});

    strings.push_back("[ I2C Buses ]");

    for(i2c_smbus_info bus_info : i2c_bus_info)
    {
        char line[550];
        snprintf(line, 550, "%04X:%04X %04X:%04X", bus_info.pci_vendor, bus_info.pci_device, bus_info.pci_subsystem_vendor, bus_info.pci_subsystem_device);
        new QTreeWidgetItem(i2c_top, {line, bus_info.device_name});

        snprintf(line, 550, "%04X:%04X %04X:%04X - %s", bus_info.pci_vendor, bus_info.pci_device, bus_info.pci_subsystem_vendor, bus_info.pci_subsystem_device, bus_info.device_name);
        strings.push_back(line);
    }

    /*-----------------------------------------------------*\
    | Add HID Devices                                       |
    \*-----------------------------------------------------*/
    std::vector<HIDDeviceInfo>  hid_device_info = ResourceManager::get()->GetHIDDeviceInfo();
    QTreeWidgetItem*            hid_top         = new QTreeWidgetItem(ui->HardwareIdsList, {"HID Devices"});

    strings.push_back("\n[ HID Devices ]");

    for(HIDDeviceInfo device_info : hid_device_info)
    {
        char line[550];

        snprintf(line, 550, "[%04X:%04X U=%04X P=0x%04X I=%d]", device_info.vendor_id, device_info.product_id, device_info.usage, device_info.usage_page, device_info.interface_number);
        new QTreeWidgetItem(hid_top, {line, QString::fromStdString(device_info.product_string), QString::fromStdString(device_info.manufacturer_string)});

        snprintf(line, 550, "[%04X:%04X U=%04X P=0x%04X I=%d] %s - %s", device_info.vendor_id, device_info.product_id, device_info.usage, device_info.usage_page, device_info.interface_number, device_info.manufacturer_string.c_str(), device_info.product_string.c_str());
        strings.push_back(line);
    }

    /*-----------------------------------------------------*\
    | Add USB Devices                                       |
    \*-----------------------------------------------------*/
    std::vector<USBDeviceInfo>  usb_device_info = ResourceManager::get()->GetUSBDeviceInfo();
    QTreeWidgetItem*            usb_top         = new QTreeWidgetItem(ui->HardwareIdsList, {"USB devices"});

    strings.push_back("\n[ USB devices ]");

    for(USBDeviceInfo device_info : usb_device_info)
    {
        char line[550];

        snprintf(line, 550, "[%04X:%04X]", device_info.vendor_id, device_info.product_id);
        new QTreeWidgetItem(usb_top, {line, QString::fromStdString(device_info.product_string), QString::fromStdString(device_info.manufacturer_string)});

        snprintf(line, 550, "[%04X:%04X] %s - %s", device_info.vendor_id, device_info.product_id, device_info.manufacturer_string.c_str(), device_info.product_string.c_str());
        strings.push_back(line);
    }

    std::vector<SerialDeviceInfo>   serial_device_info = ResourceManager::get()->GetUSBSerialPorts();
    QTreeWidgetItem*                serial_top         = new QTreeWidgetItem(ui->HardwareIdsList, {"USB Serial devices"});

    strings.push_back("\n[ USB Serial devices ]");

    for(SerialDeviceInfo device_info : serial_device_info)
    {
        char line[550];

        snprintf(line, 550, "[%04X:%04X]", device_info.vendor_id, device_info.product_id);
        new QTreeWidgetItem(serial_top, {line, QString::fromStdString(device_info.port_path), QString::fromStdString(device_info.usb_path)});

        snprintf(line, 550, "[%04X:%04X] %s (%s)", device_info.vendor_id, device_info.product_id, device_info.port_path.c_str(), device_info.usb_path.c_str());
        strings.push_back(line);
    }

    i2c_top->setExpanded(true);
    hid_top->setExpanded(true);
    usb_top->setExpanded(true);
    serial_top->setExpanded(true);

    return this->exec();
}

void OpenRGBHardwareIDsDialog::on_CopyToClipboardButton_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(strings.join("\n"));
}
