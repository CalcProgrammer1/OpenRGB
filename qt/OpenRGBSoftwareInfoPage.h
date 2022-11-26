#ifndef OPENRGBSOFTWAREINFOPAGE_H
#define OPENRGBSOFTWAREINFOPAGE_H

#include <QFrame>
#include "ui_OpenRGBSoftwareInfoPage.h"

namespace Ui {
class OpenRGBSoftwareInfoPage;
}

class Ui::OpenRGBSoftwareInfoPage : public QFrame
{
    Q_OBJECT

public:
    explicit OpenRGBSoftwareInfoPage(QWidget *parent = nullptr);
    ~OpenRGBSoftwareInfoPage();

private:
    Ui::OpenRGBSoftwareInfoPageUi *ui;

private slots:
    void changeEvent(QEvent *event);
};

#endif // OPENRGBSOFTWAREINFOPAGE_H
