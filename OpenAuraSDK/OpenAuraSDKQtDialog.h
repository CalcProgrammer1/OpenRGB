#ifndef OPENAURASDKQTDIALOG_H
#define OPENAURASDKQTDIALOG_H

#include "ui_openaurasdk.h"

#include <vector>
#include "i2c_smbus.h"
#include "AuraController.h"

#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>

namespace Ui
{
    class OpenAuraSDKQtDialog;
}

class Ui::OpenAuraSDKQtDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit OpenAuraSDKQtDialog(std::vector<i2c_smbus_interface *>& bus, std::vector<AuraController *>& control, QWidget *parent = 0);
    ~OpenAuraSDKQtDialog();

    void show();
    void setMode(unsigned char mode_val);

protected:
    std::vector<i2c_smbus_interface *>& busses;
    std::vector<AuraController *>& controllers;

private slots:
    void on_ButtonSetAll_clicked();

    void on_ButtonSetColors_clicked();

    void on_RadioDirect_clicked();

    void on_RadioEffect_clicked();

    void on_ComboAuraDevices_currentIndexChanged(int index);

    void on_RadioOff_clicked();

    void on_RadioStatic_clicked();

    void on_RadioBreathing_clicked();

    void on_RadioFlashing_clicked();

    void on_RadioSpectrumCycle_clicked();

    void on_RadioRainbow_clicked();

    void on_RadioBreathingSpectrum_clicked();

    void on_RadioChaseFade_clicked();

private:
    Ui::OpenAuraSDKQtDialogUi *ui;
};

#endif // OPENAURASDKQTDIALOG_H
