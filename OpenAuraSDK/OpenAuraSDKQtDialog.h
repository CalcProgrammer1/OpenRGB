#ifndef OPENAURASDKQTDIALOG_H
#define OPENAURASDKQTDIALOG_H

#include "ui_openaurasdk.h"

#include <vector>
#include "i2c_smbus.h"
#include "RGBController.h"

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
    explicit OpenAuraSDKQtDialog(std::vector<i2c_smbus_interface *>& bus, std::vector<RGBController *>& control, QWidget *parent = 0);
    ~OpenAuraSDKQtDialog();

    void show();
    void setMode(unsigned char mode_val);

protected:
    std::vector<i2c_smbus_interface *>& busses;
    std::vector<RGBController *>& controllers;

private slots:
    void on_ButtonSetAll_clicked();

    void on_ButtonSetDevice_clicked();

    void on_ButtonSetZone_clicked();

    void on_ButtonSetLED_clicked();

    void on_ComboDevices_currentIndexChanged(int index);

    void on_ComboModes_currentIndexChanged(int index);

private:
    Ui::OpenAuraSDKQtDialogUi *ui;
};

#endif // OPENAURASDKQTDIALOG_H
