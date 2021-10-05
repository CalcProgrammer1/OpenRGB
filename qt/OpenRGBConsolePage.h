#ifndef OPENRGBCONSOLEPAGE_H
#define OPENRGBCONSOLEPAGE_H

#include <QFrame>
#include "ui_OpenRGBConsolePage.h"

namespace Ui {
class OpenRGBConsolePage;
}

class Ui::OpenRGBConsolePage : public QFrame
{
    Q_OBJECT

public:
    explicit OpenRGBConsolePage(QWidget *parent = nullptr);
    ~OpenRGBConsolePage();

private slots:
    void on_log_level_currentIndexChanged(int);
    void on_clear_clicked();
    void on_refresh_clicked();

private:
    Ui::OpenRGBConsolePageUi *ui;

    void Refresh();
};

#endif // OPENRGBCONSOLEPAGE_H
