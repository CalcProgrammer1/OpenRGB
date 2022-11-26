#ifndef OPENRGBSERVERINFOPAGE_H
#define OPENRGBSERVERINFOPAGE_H

#include <QFrame>
#include "RGBController.h"
#include "ui_OpenRGBServerInfoPage.h"
#include "NetworkServer.h"

namespace Ui {
class OpenRGBServerInfoPage;
}

class Ui::OpenRGBServerInfoPage : public QFrame
{
    Q_OBJECT

public:
    explicit OpenRGBServerInfoPage(NetworkServer * server, QWidget *parent = nullptr);
    ~OpenRGBServerInfoPage();

public slots:
    void UpdateInfo();

private slots:
    void changeEvent(QEvent *event);
    void on_ServerStartButton_clicked();
    void on_ServerStopButton_clicked();

private:
    Ui::OpenRGBServerInfoPageUi *ui;

    NetworkServer* network_server;
};

#endif // OPENRGBSERVERINFOPAGE_H
