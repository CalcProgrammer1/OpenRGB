#include "OpenRGBDevicePage.h"
#include "hsv.h"

using namespace Ui;

OpenRGBDevicePage::OpenRGBDevicePage(RGBController *dev, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBDevicePageUi)
{
    ui->setupUi(this);

    /*-----------------------------------------------------*\
    | Store device pointer                                  |
    \*-----------------------------------------------------*/
    device = dev;

    /*-----------------------------------------------------*\
    | Set up the color palette buttons                      |
    \*-----------------------------------------------------*/
    QPalette pal;

    pal = ui->ButtonRed->palette();
    pal.setColor(QPalette::Button, QColor(255, 0, 0));
    ui->ButtonRed->setAutoFillBackground(true);
    ui->ButtonRed->setPalette(pal);
    ui->ButtonRed->setFlat(true);
    ui->ButtonRed->update();

    pal = ui->ButtonYellow->palette();
    pal.setColor(QPalette::Button, QColor(255, 255, 0));
    ui->ButtonYellow->setAutoFillBackground(true);
    ui->ButtonYellow->setPalette(pal);
    ui->ButtonYellow->setFlat(true);
    ui->ButtonYellow->update();

    pal = ui->ButtonGreen->palette();
    pal.setColor(QPalette::Button, QColor(0, 255, 0));
    ui->ButtonGreen->setAutoFillBackground(true);
    ui->ButtonGreen->setPalette(pal);
    ui->ButtonGreen->setFlat(true);
    ui->ButtonGreen->update();

    pal = ui->ButtonCyan->palette();
    pal.setColor(QPalette::Button, QColor(0, 255, 255));
    ui->ButtonCyan->setAutoFillBackground(true);
    ui->ButtonCyan->setPalette(pal);
    ui->ButtonCyan->setFlat(true);
    ui->ButtonCyan->update();

    pal = ui->ButtonBlue->palette();
    pal.setColor(QPalette::Button, QColor(0, 0, 255));
    ui->ButtonBlue->setAutoFillBackground(true);
    ui->ButtonBlue->setPalette(pal);
    ui->ButtonBlue->setFlat(true);
    ui->ButtonBlue->update();

    pal = ui->ButtonMagenta->palette();
    pal.setColor(QPalette::Button, QColor(255, 0, 255));
    ui->ButtonMagenta->setAutoFillBackground(true);
    ui->ButtonMagenta->setPalette(pal);
    ui->ButtonMagenta->setFlat(true);
    ui->ButtonMagenta->update();

    /*-----------------------------------------------------*\
    | Fill in the combo boxes with device information       |
    \*-----------------------------------------------------*/
    ui->ModeBox->clear();

    for (int i = 0; i < device->modes.size(); i++)
    {
        ui->ModeBox->addItem(device->modes[i].name.c_str());
    }

    ui->ModeBox->setCurrentIndex(device->GetMode());

    ui->ZoneBox->clear();

    for (int i = 0; i < device->zones.size(); i++)
    {
        ui->ZoneBox->addItem(device->zones[i].name.c_str());
    }

    ui->ZoneBox->setCurrentIndex(0);

    ui->LEDBox->clear();

    for (int i = 0; i < device->leds.size(); i++)
    {
        ui->LEDBox->addItem(device->leds[i].name.c_str());
    }

    ui->LEDBox->setCurrentIndex(0);
}

OpenRGBDevicePage::~OpenRGBDevicePage()
{
    delete ui;
}

void Ui::OpenRGBDevicePage::on_ZoneBox_currentIndexChanged(int index)
{

}

void Ui::OpenRGBDevicePage::on_LEDBox_currentIndexChanged(int index)
{

}

void Ui::OpenRGBDevicePage::on_ModeBox_currentIndexChanged(int index)
{
    /*-----------------------------------------------------*\
    | Change device mode                                    |
    \*-----------------------------------------------------*/
    device->SetMode(ui->ModeBox->currentIndex());
}

void Ui::OpenRGBDevicePage::SetDevice(unsigned char red, unsigned char green, unsigned char blue)
{
    UpdatingColor = true;
    ui->RedSpinBox->setValue(red);
    ui->GreenSpinBox->setValue(green);
    ui->BlueSpinBox->setValue(blue);
    UpdatingColor = false;
    updateHSV();

    on_SetDeviceButton_clicked();
}

void Ui::OpenRGBDevicePage::on_SetDeviceButton_clicked()
{
    /*-----------------------------------------------------*\
    | Set all device LEDs to the current color              |
    \*-----------------------------------------------------*/
    RGBColor color = ToRGBColor(
        ui->RedSpinBox->text().toInt(),
        ui->GreenSpinBox->text().toInt(),
        ui->BlueSpinBox->text().toInt()
    );

    device->SetAllLEDs(color);
}

void Ui::OpenRGBDevicePage::on_SetZoneButton_clicked()
{
    /*-----------------------------------------------------*\
    | Set all LEDs in the selected zone to the current color|
    \*-----------------------------------------------------*/
    RGBColor color = ToRGBColor(
        ui->RedSpinBox->text().toInt(),
        ui->GreenSpinBox->text().toInt(),
        ui->BlueSpinBox->text().toInt()
    );

    device->SetAllZoneLEDs(ui->ZoneBox->currentIndex(), color);
}

void Ui::OpenRGBDevicePage::on_SetLEDButton_clicked()
{
    /*-----------------------------------------------------*\
    | Set the selected LED to the current color             |
    \*-----------------------------------------------------*/
    RGBColor color = ToRGBColor(
        ui->RedSpinBox->text().toInt(),
        ui->GreenSpinBox->text().toInt(),
        ui->BlueSpinBox->text().toInt()
    );

    device->SetLED(ui->LEDBox->currentIndex(), color);
}

void Ui::OpenRGBDevicePage::on_ButtonRed_clicked()
{
    UpdatingColor = true;
    ui->RedSpinBox->setValue(255);
    ui->GreenSpinBox->setValue(0);
    ui->BlueSpinBox->setValue(0);
    UpdatingColor = false;
    updateHSV();
}

void Ui::OpenRGBDevicePage::on_ButtonYellow_clicked()
{
    UpdatingColor = true;
    ui->RedSpinBox->setValue(255);
    ui->GreenSpinBox->setValue(255);
    ui->BlueSpinBox->setValue(0);
    UpdatingColor = false;
    updateHSV();
}

void Ui::OpenRGBDevicePage::on_ButtonGreen_clicked()
{
    UpdatingColor = true;
    ui->RedSpinBox->setValue(0);
    ui->GreenSpinBox->setValue(255);
    ui->BlueSpinBox->setValue(0);
    UpdatingColor = false;
    updateHSV();
}

void Ui::OpenRGBDevicePage::on_ButtonCyan_clicked()
{
    UpdatingColor = true;
    ui->RedSpinBox->setValue(0);
    ui->GreenSpinBox->setValue(255);
    ui->BlueSpinBox->setValue(255);
    UpdatingColor = false;
    updateHSV();
}

void Ui::OpenRGBDevicePage::on_ButtonBlue_clicked()
{
    UpdatingColor = true;
    ui->RedSpinBox->setValue(0);
    ui->GreenSpinBox->setValue(0);
    ui->BlueSpinBox->setValue(255);
    UpdatingColor = false;
    updateHSV();
}

void Ui::OpenRGBDevicePage::on_ButtonMagenta_clicked()
{
    UpdatingColor = true;
    ui->RedSpinBox->setValue(255);
    ui->GreenSpinBox->setValue(0);
    ui->BlueSpinBox->setValue(255);
    UpdatingColor = false;
    updateHSV();
}

void Ui::OpenRGBDevicePage::updateRGB()
{
    if(UpdatingColor)
    {
        return;
    }

    UpdatingColor = true;

    hsv_t hsv;
    hsv.hue = ui->HueSpinBox->value();
    hsv.saturation = ui->SatSpinBox->value();
    hsv.value = ui->ValSpinBox->value();

    RGBColor rgb = hsv2rgb(&hsv);

    ui->RedSpinBox->setValue(RGBGetRValue(rgb));
    ui->GreenSpinBox->setValue(RGBGetGValue(rgb));
    ui->BlueSpinBox->setValue(RGBGetBValue(rgb));

    UpdatingColor = false;
}

void Ui::OpenRGBDevicePage::updateHSV()
{
    if(UpdatingColor)
    {
        return;
    }

    UpdatingColor = true;

    RGBColor rgb = ToRGBColor(ui->RedSpinBox->value(), ui->GreenSpinBox->value(), ui->BlueSpinBox->value());

    hsv_t hsv;

    rgb2hsv(rgb, &hsv);

    ui->HueSpinBox->setValue(hsv.hue);
    ui->SatSpinBox->setValue(hsv.saturation);
    ui->ValSpinBox->setValue(hsv.value);

    UpdatingColor = false;
}

void Ui::OpenRGBDevicePage::on_RedSpinBox_valueChanged(int arg1)
{
    updateHSV();
}

void Ui::OpenRGBDevicePage::on_HueSpinBox_valueChanged(int arg1)
{
    updateRGB();
}

void Ui::OpenRGBDevicePage::on_GreenSpinBox_valueChanged(int arg1)
{
    updateHSV();
}

void Ui::OpenRGBDevicePage::on_SatSpinBox_valueChanged(int arg1)
{
    updateRGB();
}

void Ui::OpenRGBDevicePage::on_BlueSpinBox_valueChanged(int arg1)
{
    updateHSV();
}

void Ui::OpenRGBDevicePage::on_ValSpinBox_valueChanged(int arg1)
{
    updateRGB();
}

void Ui::OpenRGBDevicePage::on_SetAllButton_clicked()
{
    unsigned char red   = ui->RedSpinBox->value();
    unsigned char green = ui->GreenSpinBox->value();
    unsigned char blue  = ui->BlueSpinBox->value();

    emit SetAllDevices(red, green, blue);
}
