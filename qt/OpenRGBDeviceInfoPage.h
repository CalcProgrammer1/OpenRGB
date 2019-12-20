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

private:
    Ui::OpenRGBDeviceInfoPageUi *ui;
};

#endif // OPENRGBDEVICEINFOPAGE_H
