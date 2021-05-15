#ifndef OPENRGBCLIENTINFOPAGE_H
#define OPENRGBCLIENTINFOPAGE_H

#include <QFrame>
#include "RGBController.h"
#include "ui_OpenRGBClientInfoPage.h"
#include "NetworkClient.h"

namespace Ui {
class OpenRGBClientInfoPage;
}

class Ui::OpenRGBClientInfoPage : public QFrame
{
    Q_OBJECT

public:
    explicit OpenRGBClientInfoPage(QWidget *parent = nullptr);
    ~OpenRGBClientInfoPage();

    void AddClient(NetworkClient* new_client);

public slots:
    void UpdateInfo();

private slots:
    void on_ClientConnectButton_clicked();
    void onClientDisconnectButton_clicked(QObject * arg);

private:
    Ui::OpenRGBClientInfoPageUi *ui;
};

#endif // OPENRGBCLIENTINFOPAGE_H
