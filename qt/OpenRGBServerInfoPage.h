#ifndef OPENRGBSERVERINFOPAGE_H
#define OPENRGBSERVERINFOPAGE_H

#include <QFrame>
#include "RGBController.h"
#include "ui_OpenRGBServerInfoPage.h"

namespace Ui {
class OpenRGBServerInfoPage;
}

class Ui::OpenRGBServerInfoPage : public QFrame
{
    Q_OBJECT

public:
    explicit OpenRGBServerInfoPage(RGBController *dev, QWidget *parent = nullptr);
    ~OpenRGBServerInfoPage();

private:
    Ui::OpenRGBServerInfoPageUi *ui;
};

#endif // OPENRGBSERVERINFOPAGE_H
