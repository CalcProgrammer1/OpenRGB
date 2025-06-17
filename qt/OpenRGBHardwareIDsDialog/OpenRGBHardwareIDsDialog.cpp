/*---------------------------------------------------------*\
| OpenRGBHardwareIDsDialog.cpp                              |
|                                                           |
|   User interface for OpenRGB Hardware IDs dialog          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    /*---------------------------------------------------------*\
    | Add i2c busses infos                                      |
    \*---------------------------------------------------------*/
    std::vector<i2c_smbus_interface*> i2CBusses = ResourceManager::get()->GetI2CBusses();

    // The widget takes control over items after creation
    QTreeWidgetItem* i2c_top = new QTreeWidgetItem(ui->HardwareIdsList, {"i2c busses"});
    strings.push_back("[ i2c busses ]");

    for(i2c_smbus_interface* bus : i2CBusses)
    {
        char line[550];
        snprintf(line, 550, "%04X:%04X %04X:%04X", bus->pci_vendor, bus->pci_device, bus->pci_subsystem_vendor, bus->pci_subsystem_device);
        new QTreeWidgetItem(i2c_top, {line, bus->device_name});

        snprintf(line, 550, "%04X:%04X %04X:%04X - %s", bus->pci_vendor, bus->pci_device, bus->pci_subsystem_vendor, bus->pci_subsystem_device, bus->device_name);
        strings.push_back(line);
    }

    /*---------------------------------------------------------*\
    | Add HID devices infos                                     |
    \*---------------------------------------------------------*/
    hid_device_info*    hid_devices         = NULL;
    hid_devices = hid_enumerate(0,0);

    hid_device_info*    current_hid_device;
    current_hid_device  = hid_devices;

    QTreeWidgetItem* hid_top = new QTreeWidgetItem(ui->HardwareIdsList, {"HID devices"});
    strings.push_back("\n[ HID devices ]");

    while(current_hid_device)
    {
        const char* manu_name = StringUtils::wchar_to_char(current_hid_device->manufacturer_string);
        const char* prod_name = StringUtils::wchar_to_char(current_hid_device->product_string);

        char line[550];

        snprintf(line, 550, "[%04X:%04X U=%04X P=0x%04X I=%d]", current_hid_device->vendor_id, current_hid_device->product_id, current_hid_device->usage, current_hid_device->usage_page, current_hid_device->interface_number);
        new QTreeWidgetItem(hid_top, {line, prod_name, manu_name});

        snprintf(line, 550, "[%04X:%04X U=%04X P=0x%04X I=%d] %s - %s", current_hid_device->vendor_id, current_hid_device->product_id, current_hid_device->usage, current_hid_device->usage_page, current_hid_device->interface_number, manu_name, prod_name);
        strings.push_back(line);

        current_hid_device = current_hid_device->next;
    }

    /*---------------------------------------------------------*\
    | Add LibUSB devices infos                                  |
    \*---------------------------------------------------------*/
    libusb_device** devices = nullptr;

    QTreeWidgetItem* libusb_top = new QTreeWidgetItem(ui->HardwareIdsList, {"LibUSB devices"});
    strings.push_back("\n[ LibUSB devices ]");

    int ret;

    ret = libusb_init(NULL);

    if(ret < 0)
    {
        return 0;
    }

    ret = libusb_get_device_list(NULL, &devices);

    if(ret < 0)
    {
        return 0;
    }

    int deviceCount = ret;

    for(int i = 0; i < deviceCount; i++)
    {
        libusb_device* device = devices[i];
        libusb_device_descriptor descriptor;

        ret = libusb_get_device_descriptor(device, &descriptor);

        if(ret < 0)
        {
            continue;
        }

        char line[512];
        snprintf(line, 512, "%04X:%04X", descriptor.idVendor, descriptor.idProduct);
        new QTreeWidgetItem(libusb_top, {line});
        strings.push_back(line);
    }

    if(devices != nullptr)
    {
        libusb_free_device_list(devices, 1);
    }

    i2c_top->setExpanded(true);
    hid_top->setExpanded(true);
    libusb_top->setExpanded(true);

    return this->exec();
}

void OpenRGBHardwareIDsDialog::on_CopyToClipboardButton_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(strings.join("\n"));
}
