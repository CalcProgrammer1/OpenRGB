#include "OpenRGBDeviceInfoPage.h"

using namespace Ui;

OpenRGBDeviceInfoPage::OpenRGBDeviceInfoPage(RGBController *dev, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBDeviceInfoPageUi)
{
    ui->setupUi(this);

    switch(dev->type)
    {
    case DEVICE_TYPE_MOTHERBOARD:
        ui->TypeValue->setText("Motherboard");
        break;
    case DEVICE_TYPE_DRAM:
        ui->TypeValue->setText("DRAM");
        break;
    case DEVICE_TYPE_GPU:
        ui->TypeValue->setText("GPU");
        break;
    case DEVICE_TYPE_COOLER:
        ui->TypeValue->setText("Cooler");
        break;
    case DEVICE_TYPE_LEDSTRIP:
        ui->TypeValue->setText("LED Strip");
        break;
    case DEVICE_TYPE_KEYBOARD:
        ui->TypeValue->setText("Keyboard");
        break;
    case DEVICE_TYPE_MOUSE:
        ui->TypeValue->setText("Mouse");
        break;
    case DEVICE_TYPE_HEADSET:
        ui->TypeValue->setText("Headset");
        break;
    default:
        ui->TypeValue->setText("Unknown");
    }

    ui->DescriptionValue->setText(QString::fromStdString(dev->description));
    ui->VersionValue->setText(QString::fromStdString(dev->version));
    ui->LocationValue->setText(QString::fromStdString(dev->location));
}

OpenRGBDeviceInfoPage::~OpenRGBDeviceInfoPage()
{
    delete ui;
}
