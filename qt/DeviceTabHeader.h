#ifndef DEVICETABHEADER_H
#define DEVICETABHEADER_H

#include <QWidget>
#include "ui_DeviceTabHeader.h"

namespace Ui {
class DeviceTabHeader;
}

class Ui::DeviceTabHeader : public QWidget
{
    Q_OBJECT

public:
    DeviceTabHeader(QString, QString);
    ~DeviceTabHeader();

private:
    Ui::DeviceTabHeaderUi *ui;
};

#endif // DEVICETABHEADER_H
