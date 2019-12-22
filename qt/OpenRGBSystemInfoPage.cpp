#include "OpenRGBSystemInfoPage.h"
#include "i2c_tools.h"

using namespace Ui;

OpenRGBSystemInfoPage::OpenRGBSystemInfoPage(std::vector<i2c_smbus_interface *>& bus, QWidget *parent) :
    QFrame(parent),
    busses(bus),
    ui(new Ui::OpenRGBSystemInfoPageUi)
{
    ui->setupUi(this);

    /*-----------------------------------------------------*\
    | Use a monospace font for the text box                 |
    \*-----------------------------------------------------*/
    QFont MonoFont("monospace");
    MonoFont.setStyleHint(QFont::Monospace);
    ui->SMBusDataText->setFont(MonoFont);

    /*-----------------------------------------------------*\
    | Fill in the combo boxes with device information       |
    \*-----------------------------------------------------*/
    ui->SMBusAdaptersBox->clear();

    for (int i = 0; i < busses.size(); i++)
    {
        ui->SMBusAdaptersBox->addItem(busses[i]->device_name);
    }

    ui->SMBusAdaptersBox->setCurrentIndex(0);
}

OpenRGBSystemInfoPage::~OpenRGBSystemInfoPage()
{
    delete ui;
}

void Ui::OpenRGBSystemInfoPage::on_DetectButton_clicked()
{
    i2c_smbus_interface* bus = busses[ui->SMBusAdaptersBox->currentIndex()];

    ui->SMBusDataText->setPlainText(i2c_detect(bus, MODE_QUICK).c_str());
}

void Ui::OpenRGBSystemInfoPage::on_DumpButton_clicked()
{
    i2c_smbus_interface* bus = busses[ui->SMBusAdaptersBox->currentIndex()];
    unsigned char address = ui->DumpAddressBox->value();

    ui->SMBusDataText->setPlainText(i2c_dump(bus, address).c_str());
}
