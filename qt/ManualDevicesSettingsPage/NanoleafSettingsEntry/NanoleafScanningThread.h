/*---------------------------------------------------------*\
| NanoleafScanningThread.h                                  |
|                                                           |
|   OpenRGB Nanoleaf scanning thread                        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QThread>

class NanoleafScanningThread : public QThread
{
    Q_OBJECT

    void run();

    int SendMDNSQuery();

signals:
    void DeviceFound(QString address, int port);

public:
    void EmitDeviceFound(QString address, int port);
};
