#ifndef OPENRGBDEVICEINFOPAGE_H
#define OPENRGBDEVICEINFOPAGE_H

#include <QFrame>
#include "RGBController.h"
#include "ui_OpenRGBDeviceInfoPage.h"

namespace Ui {
class OpenRGBDeviceInfoPage;
}

class Ui::OpenRGBDeviceInfoPage : public QFrame
{
    Q_OBJECT

public:
    explicit OpenRGBDeviceInfoPage(RGBController *dev, QWidget *parent = nullptr);
    ~OpenRGBDeviceInfoPage();

    RGBController* GetController();

private:
    RGBController*                  controller;
    Ui::OpenRGBDeviceInfoPageUi*    ui;

private slots:
    void changeEvent(QEvent *event);
};

#endif // OPENRGBDEVICEINFOPAGE_H
