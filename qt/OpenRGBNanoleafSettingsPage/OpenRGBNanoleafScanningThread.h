#ifndef OPENRGBNanoleafScanningThread_H
#define OPENRGBNanoleafScanningThread_H

#include <QThread>

class OpenRGBNanoleafScanningThread : public QThread
{
    Q_OBJECT

    void run();

    int SendMDNSQuery();

signals:
    void DeviceFound(QString address, int port);

public:
    void EmitDeviceFound(QString address, int port);
};

#endif // OPENRGBNanoleafScanningThread_H
