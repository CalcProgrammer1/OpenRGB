/*---------------------------------------------------------*\
| OpenRGBServerInfoPage.h                                   |
|                                                           |
|   User interface for server information page              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QFrame>
#include "RGBController.h"
#include "NetworkServer.h"

namespace Ui
{
    class OpenRGBServerInfoPage;
}

class OpenRGBServerInfoPage : public QFrame
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
    Ui::OpenRGBServerInfoPage *ui;

    NetworkServer* network_server;
};
