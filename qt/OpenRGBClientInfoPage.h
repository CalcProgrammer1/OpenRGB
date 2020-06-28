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
    explicit OpenRGBClientInfoPage(std::vector<RGBController *>& control, QWidget *parent = nullptr);
    ~OpenRGBClientInfoPage();

    void AddClient(NetworkClient* new_client);

public slots:
    void UpdateInfo();

private slots:
    void on_ClientConnectButton_clicked();
    void on_ClientDisconnectButton_clicked(QObject * arg);

private:
    Ui::OpenRGBClientInfoPageUi *ui;

    std::vector<RGBController *>&       controllers;
    std::vector<NetworkClient*> rgb_clients;

signals:
    void ClientListUpdated();
};

#endif // OPENRGBCLIENTINFOPAGE_H
