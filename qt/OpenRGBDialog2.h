#ifndef OPENRGBDIALOG2_H
#define OPENRGBDIALOG2_H

#include "ui_OpenRGBDialog2.h"

#include <vector>
#include "i2c_smbus.h"
#include "RGBController.h"

#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>

namespace Ui
{
    class OpenRGBDialog2;
}

class Ui::OpenRGBDialog2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit OpenRGBDialog2(std::vector<i2c_smbus_interface *>& bus, std::vector<RGBController *>& control, QWidget *parent = 0);
    ~OpenRGBDialog2();

    void show();
    void setMode(unsigned char mode_val);

protected:
    std::vector<i2c_smbus_interface *>& busses;
    std::vector<RGBController *>& controllers;

private:
    Ui::OpenRGBDialog2Ui *ui;

private slots:
    void on_SetAllDevices(unsigned char red, unsigned char green, unsigned char blue);
};

#endif // OPENRGBDIALOG2_H
