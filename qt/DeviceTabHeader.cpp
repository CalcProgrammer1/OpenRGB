#include "DeviceTabHeader.h"

Ui::DeviceTabHeader::DeviceTabHeader(QString icon, QString device_name) :
    QWidget(nullptr),
    ui(new Ui::DeviceTabHeaderUi)
{
    ui->setupUi(this);
    ui->device_icon->setText("<img src=':/" + icon + "' height='16' width='16'  />");
    ui->device_name->setText(device_name);
}

Ui::DeviceTabHeader::~DeviceTabHeader()
{
    delete ui;
}
