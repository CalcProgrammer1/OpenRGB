#include "OpenAuraSDKQtDialog.h"
#include "OpenAuraSDK.h"

using namespace Ui;

OpenAuraSDKQtDialog::OpenAuraSDKQtDialog(std::vector<i2c_smbus_interface *>& bus, std::vector<RGBController *>& control, QWidget *parent) : QMainWindow(parent), busses(bus), controllers (control), ui(new OpenAuraSDKQtDialogUi)
{
    ui->setupUi(this);

    for (int i = 0; i < controllers.size(); i++)
    {
        ui->ComboDevices->addItem(controllers[i]->name.c_str());
    }

    //Triggers on_ComboDevices_currentIndexChanged so we don't need to update the other fields here
    ui->ComboDevices->setCurrentIndex(0);
}

OpenAuraSDKQtDialog::~OpenAuraSDKQtDialog()
{
    delete ui;
}

void OpenAuraSDKQtDialog::show()
{
    QMainWindow::show();
}

void Ui::OpenAuraSDKQtDialog::on_ButtonSetAll_clicked()
{
    RGBColor color = ToRGBColor(
        ui->EditLED0R->text().toInt(),
        ui->EditLED0G->text().toInt(),
        ui->EditLED0B->text().toInt()
    );

    controllers[ui->ComboDevices->currentIndex()]->SetAllLEDs(color);
}

void Ui::OpenAuraSDKQtDialog::on_ButtonSetZone_clicked()
{
    RGBColor color = ToRGBColor(
        ui->EditLED0R->text().toInt(),
        ui->EditLED0G->text().toInt(),
        ui->EditLED0B->text().toInt()
    );

    controllers[ui->ComboDevices->currentIndex()]->SetAllZoneLEDs(ui->ComboZones->currentIndex(), color);
}

void Ui::OpenAuraSDKQtDialog::on_ButtonSetLED_clicked()
{
    RGBColor color = ToRGBColor(
        ui->EditLED0R->text().toInt(),
        ui->EditLED0G->text().toInt(),
        ui->EditLED0B->text().toInt()
    );

    controllers[ui->ComboDevices->currentIndex()]->SetLED(ui->ComboLEDs->currentIndex(), color);
}

void Ui::OpenAuraSDKQtDialog::on_ComboDevices_currentIndexChanged(int index)
{
    ui->ComboModes->clear();

    for (int i = 0; i < controllers[ui->ComboDevices->currentIndex()]->modes.size(); i++)
    {
        ui->ComboModes->addItem(controllers[ui->ComboDevices->currentIndex()]->modes[i].name.c_str());
    }

    ui->ComboModes->setCurrentIndex(controllers[ui->ComboDevices->currentIndex()]->GetMode());

    ui->ComboZones->clear();

    for (int i = 0; i < controllers[ui->ComboDevices->currentIndex()]->zones.size(); i++)
    {
        ui->ComboZones->addItem(controllers[ui->ComboDevices->currentIndex()]->zones[i].name.c_str());
    }

    ui->ComboZones->setCurrentIndex(0);

    ui->ComboLEDs->clear();

    for (int i = 0; i < controllers[ui->ComboDevices->currentIndex()]->leds.size(); i++)
    {
        ui->ComboLEDs->addItem(controllers[ui->ComboDevices->currentIndex()]->leds[i].name.c_str());
    }

    ui->ComboLEDs->setCurrentIndex(0);
}

void Ui::OpenAuraSDKQtDialog::on_ComboModes_currentIndexChanged(int index)
{
    controllers[ui->ComboDevices->currentIndex()]->SetMode(ui->ComboModes->currentIndex());
}
