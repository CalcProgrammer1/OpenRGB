#include "OpenAuraSDKQtDialog.h"
#include "OpenAuraSDK.h"

using namespace Ui;

OpenAuraSDKQtDialog::OpenAuraSDKQtDialog(std::vector<i2c_smbus_interface *>& bus, std::vector<AuraController *>& control, QWidget *parent) : QMainWindow(parent), busses(bus), controllers (control), ui(new OpenAuraSDKQtDialogUi)
{
    ui->setupUi(this);

    ui->ComboAuraDevices->addItem("All Devices");

    for (int i = 0; i < controllers.size(); i++)
    {
        ui->ComboAuraDevices->addItem(controllers[i]->GetDeviceName());
    }

    ui->ComboAuraDevices->setCurrentIndex(0);
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
    unsigned int aura_device = ui->ComboAuraDevices->currentIndex();

    if (aura_device == 0)
    {
        unsigned char direct = ui->RadioDirect->isChecked();

        if (direct == 0)
        {
            for (int i = 0; i < controllers.size(); i++)
            {
                controllers[i]->SetAllColorsEffect(ui->EditLED0R->text().toInt(),
                                                   ui->EditLED0G->text().toInt(),
                                                   ui->EditLED0B->text().toInt());

                controllers[i]->SetDirect(false);
            }
        }
        else
        {
            for (int i = 0; i < controllers.size(); i++)
            {
                controllers[i]->SetAllColorsDirect(ui->EditLED0R->text().toInt(),
                                                   ui->EditLED0G->text().toInt(),
                                                   ui->EditLED0B->text().toInt());

                controllers[i]->SetDirect(true);
            }
        }
    }
    else
    {
        unsigned char direct = controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_DIRECT);

        if (direct == 0)
        {
            controllers[aura_device - 1]->SetAllColorsEffect(ui->EditLED0R->text().toInt(),
                                                             ui->EditLED0G->text().toInt(),
                                                             ui->EditLED0B->text().toInt());
        }
        else
        {
            controllers[aura_device - 1]->SetAllColorsDirect(ui->EditLED0R->text().toInt(),
                                                             ui->EditLED0G->text().toInt(),
                                                             ui->EditLED0B->text().toInt());
        }
    }
}

void Ui::OpenAuraSDKQtDialog::on_ButtonSetColors_clicked()
{
    unsigned int aura_device = ui->ComboAuraDevices->currentIndex();

    if (aura_device == 0)
    {
        unsigned char direct = ui->RadioDirect->isChecked();

        if (direct == 0)
        {
            for (int i = 0; i < controllers.size(); i++)
            {
                controllers[i]->SetLEDColorEffect( 0,
                                                   ui->EditLED0R->text().toInt(),
                                                   ui->EditLED0G->text().toInt(),
                                                   ui->EditLED0B->text().toInt() );
                controllers[i]->SetLEDColorEffect( 1,
                                                   ui->EditLED1R->text().toInt(),
                                                   ui->EditLED1G->text().toInt(),
                                                   ui->EditLED1B->text().toInt() );
                controllers[i]->SetLEDColorEffect( 2,
                                                   ui->EditLED2R->text().toInt(),
                                                   ui->EditLED2G->text().toInt(),
                                                   ui->EditLED2B->text().toInt() );
                controllers[i]->SetLEDColorEffect( 3,
                                                   ui->EditLED3R->text().toInt(),
                                                   ui->EditLED3G->text().toInt(),
                                                   ui->EditLED3B->text().toInt() );
                controllers[i]->SetLEDColorEffect( 4,
                                                   ui->EditLED4R->text().toInt(),
                                                   ui->EditLED4G->text().toInt(),
                                                   ui->EditLED4B->text().toInt() );

                controllers[i]->SetDirect(false);
            }
        }
        else
        {
            for (int i = 0; i < controllers.size(); i++)
            {
                controllers[i]->SetLEDColorDirect( 0,
                                                   ui->EditLED0R->text().toInt(),
                                                   ui->EditLED0G->text().toInt(),
                                                   ui->EditLED0B->text().toInt() );
                controllers[i]->SetLEDColorDirect( 1,
                                                   ui->EditLED1R->text().toInt(),
                                                   ui->EditLED1G->text().toInt(),
                                                   ui->EditLED1B->text().toInt() );
                controllers[i]->SetLEDColorDirect( 2,
                                                   ui->EditLED2R->text().toInt(),
                                                   ui->EditLED2G->text().toInt(),
                                                   ui->EditLED2B->text().toInt() );
                controllers[i]->SetLEDColorDirect( 3,
                                                   ui->EditLED3R->text().toInt(),
                                                   ui->EditLED3G->text().toInt(),
                                                   ui->EditLED3B->text().toInt() );
                controllers[i]->SetLEDColorDirect( 4,
                                                   ui->EditLED4R->text().toInt(),
                                                   ui->EditLED4G->text().toInt(),
                                                   ui->EditLED4B->text().toInt() );

                controllers[i]->SetDirect(true);
            }
        }
    }
    else
    {
        unsigned char direct = controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_DIRECT);

        if (direct == 0)
        {
            controllers[aura_device - 1]->SetLEDColorEffect( 0,
                                               ui->EditLED0R->text().toInt(),
                                               ui->EditLED0G->text().toInt(),
                                               ui->EditLED0B->text().toInt() );
            controllers[aura_device - 1]->SetLEDColorEffect( 1,
                                               ui->EditLED1R->text().toInt(),
                                               ui->EditLED1G->text().toInt(),
                                               ui->EditLED1B->text().toInt() );
            controllers[aura_device - 1]->SetLEDColorEffect( 2,
                                               ui->EditLED2R->text().toInt(),
                                               ui->EditLED2G->text().toInt(),
                                               ui->EditLED2B->text().toInt() );
            controllers[aura_device - 1]->SetLEDColorEffect( 3,
                                               ui->EditLED3R->text().toInt(),
                                               ui->EditLED3G->text().toInt(),
                                               ui->EditLED3B->text().toInt() );
            controllers[aura_device - 1]->SetLEDColorEffect( 4,
                                               ui->EditLED4R->text().toInt(),
                                               ui->EditLED4G->text().toInt(),
                                               ui->EditLED4B->text().toInt() );
        }
        else
        {
            controllers[aura_device - 1]->SetLEDColorDirect( 0,
                                               ui->EditLED0R->text().toInt(),
                                               ui->EditLED0G->text().toInt(),
                                               ui->EditLED0B->text().toInt() );
            controllers[aura_device - 1]->SetLEDColorDirect( 1,
                                               ui->EditLED1R->text().toInt(),
                                               ui->EditLED1G->text().toInt(),
                                               ui->EditLED1B->text().toInt() );
            controllers[aura_device - 1]->SetLEDColorDirect( 2,
                                               ui->EditLED2R->text().toInt(),
                                               ui->EditLED2G->text().toInt(),
                                               ui->EditLED2B->text().toInt() );
            controllers[aura_device - 1]->SetLEDColorDirect( 3,
                                               ui->EditLED3R->text().toInt(),
                                               ui->EditLED3G->text().toInt(),
                                               ui->EditLED3B->text().toInt() );
            controllers[aura_device - 1]->SetLEDColorDirect( 4,
                                               ui->EditLED4R->text().toInt(),
                                               ui->EditLED4G->text().toInt(),
                                               ui->EditLED4B->text().toInt() );
        }
    }
}

void Ui::OpenAuraSDKQtDialog::on_RadioDirect_clicked()
{
    unsigned int aura_device = ui->ComboAuraDevices->currentIndex();
    if ( aura_device == 0)
    {
        for (int i = 0; i < controllers.size(); i++)
        {
            controllers[i]->SetDirect(true);
        }
    }
    else
    {
        controllers[aura_device - 1]->SetDirect(true);
    }
}

void Ui::OpenAuraSDKQtDialog::on_RadioEffect_clicked()
{
    unsigned int aura_device = ui->ComboAuraDevices->currentIndex();
    if (aura_device == 0)
    {
        for (int i = 0; i < controllers.size(); i++)
        {
            controllers[i]->SetDirect(false);
        }
    }
    else
    {
        controllers[aura_device - 1]->SetDirect(false);
    }
}

void Ui::OpenAuraSDKQtDialog::on_ComboAuraDevices_currentIndexChanged(int aura_device)
{
    if (aura_device != 0)
    {
        unsigned char direct = controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_DIRECT);

        ui->RadioDirect->setChecked(direct != 0);
        ui->RadioEffect->setChecked(direct == 0);

        unsigned char mode = controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_MODE);

        ui->RadioOff->setChecked(mode == AURA_MODE_OFF);
        ui->RadioStatic->setChecked(mode == AURA_MODE_STATIC);
        ui->RadioBreathing->setChecked(mode == AURA_MODE_BREATHING);
        ui->RadioFlashing->setChecked(mode == AURA_MODE_FLASHING);
        ui->RadioSpectrumCycle->setChecked(mode == AURA_MODE_SPECTRUM_CYCLE);
        ui->RadioRainbow->setChecked(mode == AURA_MODE_RAINBOW);
        ui->RadioBreathingSpectrum->setChecked(mode == AURA_MODE_SPECTRUM_CYCLE_BREATHING);
        ui->RadioChaseFade->setChecked(mode == AURA_MODE_CHASE_FADE);

        if (direct == 0)
        {
            ui->EditLED0R->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 0)));
            ui->EditLED0B->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 1)));
            ui->EditLED0G->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 2)));
            ui->EditLED1R->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 3)));
            ui->EditLED1B->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 4)));
            ui->EditLED1G->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 5)));
            ui->EditLED2R->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 6)));
            ui->EditLED2B->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 7)));
            ui->EditLED2G->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 8)));
            ui->EditLED3R->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 9)));
            ui->EditLED3B->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 10)));
            ui->EditLED3G->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 11)));
            ui->EditLED4R->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 12)));
            ui->EditLED4B->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 13)));
            ui->EditLED4G->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 14)));
        }
        else
        {
            ui->EditLED0R->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 0)));
            ui->EditLED0B->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 1)));
            ui->EditLED0G->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 2)));
            ui->EditLED1R->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 3)));
            ui->EditLED1B->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 4)));
            ui->EditLED1G->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 5)));
            ui->EditLED2R->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 6)));
            ui->EditLED2B->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 7)));
            ui->EditLED2G->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 8)));
            ui->EditLED3R->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 9)));
            ui->EditLED3B->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 10)));
            ui->EditLED3G->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 11)));
            ui->EditLED4R->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 12)));
            ui->EditLED4B->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 13)));
            ui->EditLED4G->setText(QString::number(controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 14)));
        }
    }
}

void OpenAuraSDKQtDialog::setMode(unsigned char mode_val)
{
    unsigned int aura_device = ui->ComboAuraDevices->currentIndex();

    if (aura_device == 0)
    {
        for (int i = 0; i < controllers.size(); i++)
        {
            controllers[i]->SetMode(mode_val);
        }
    }
    else
    {
        controllers[aura_device - 1]->SetMode(mode_val);
    }
}

void Ui::OpenAuraSDKQtDialog::on_RadioOff_clicked()
{
    setMode(AURA_MODE_OFF);
}

void Ui::OpenAuraSDKQtDialog::on_RadioStatic_clicked()
{
    setMode(AURA_MODE_STATIC);
}

void Ui::OpenAuraSDKQtDialog::on_RadioBreathing_clicked()
{
    setMode(AURA_MODE_BREATHING);
}

void Ui::OpenAuraSDKQtDialog::on_RadioFlashing_clicked()
{
    setMode(AURA_MODE_FLASHING);
}

void Ui::OpenAuraSDKQtDialog::on_RadioSpectrumCycle_clicked()
{
    setMode(AURA_MODE_SPECTRUM_CYCLE);
}

void Ui::OpenAuraSDKQtDialog::on_RadioRainbow_clicked()
{
    setMode(AURA_MODE_RAINBOW);
}

void Ui::OpenAuraSDKQtDialog::on_RadioBreathingSpectrum_clicked()
{
    setMode(AURA_MODE_SPECTRUM_CYCLE_BREATHING);
}

void Ui::OpenAuraSDKQtDialog::on_RadioChaseFade_clicked()
{
    setMode(AURA_MODE_CHASE_FADE);
}

void Ui::OpenAuraSDKQtDialog::on_ButtonDumpAura_clicked()
{
    unsigned int aura_device = ui->ComboAuraDevices->currentIndex();

    if (aura_device > 0)
    {
        ui->ButtonDumpAura->setEnabled(false);
        DumpAuraRegisters(controllers[aura_device - 1]);
        ui->ButtonDumpAura->setEnabled(true);
    }
}
