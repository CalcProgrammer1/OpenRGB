#include "OpenRGBHardwareIDsDialog.h"
#include "ui_OpenRGBHardwareIDsDialog.h"
#include <hidapi/hidapi.h>
#include "ResourceManager.h"
#include "StringUtils.h"
#include <QString>
#include <QClipboard>

#ifdef __FreeBSD__
#include <libusb.h>
#else
#include <libusb-1.0/libusb.h>
#endif

Ui::OpenRGBHardwareIDsDialog::OpenRGBHardwareIDsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenRGBHardwareIDsDialogUi)
{
    ui->setupUi(this);
}

Ui::OpenRGBHardwareIDsDialog::~OpenRGBHardwareIDsDialog()
{
    delete ui;
}

int Ui::OpenRGBHardwareIDsDialog::show()
{
    /*---------------------------------------------------------*\
    | Add i2c busses infos                                      |
    \*---------------------------------------------------------*/
    std::vector<i2c_smbus_interface*> i2CBusses = ResourceManager::get()->GetI2CBusses();

    strings.push_back("i2c busses");

    for(i2c_smbus_interface* bus : i2CBusses)
    {
        char line[550];
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

    strings.push_back("HID devices");

    while(current_hid_device)
    {
        const char* manu_name = StringUtils::wchar_to_char(current_hid_device->manufacturer_string);
        const char* prod_name = StringUtils::wchar_to_char(current_hid_device->product_string);

        char line[550];

        snprintf(line, 550, "[%04X:%04X U=%04X P=0x%04X I=%d] %s - %s", current_hid_device->vendor_id, current_hid_device->product_id, current_hid_device->usage, current_hid_device->usage_page, current_hid_device->interface_number, manu_name, prod_name);

        strings.push_back(line);
        current_hid_device = current_hid_device->next;
    }

    /*---------------------------------------------------------*\
    | Add LibUSB devices infos                                  |
    \*---------------------------------------------------------*/
    libusb_device** devices = nullptr;

    strings.push_back("LibUSB devices");

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
        sprintf(line, "%04X:%04X", descriptor.idVendor, descriptor.idProduct);
        strings.push_back(line);
    }

    if(devices != nullptr)
    {
        libusb_free_device_list(devices, 1);
    }

    ui->HardwareIdsList->addItems(strings);

    return this->exec();
}

void Ui::OpenRGBHardwareIDsDialog::on_CopyToClipboardButton_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(strings.join("\n"));
}
