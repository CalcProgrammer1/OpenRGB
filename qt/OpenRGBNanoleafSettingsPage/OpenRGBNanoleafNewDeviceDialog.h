#ifndef OPENRGBNANOLEAFNEWDEVICEDIALOG_H
#define OPENRGBNANOLEAFNEWDEVICEDIALOG_H

#include <QDialog>
#include "ui_OpenRGBNanoleafNewDeviceDialog.h"
#include "OpenRGBDialog2.h"

struct NanoleafDevice
{
    std::string     ip;
    unsigned int    port;
};

namespace Ui
{
class OpenRGBNanoleafNewDeviceDialog;
}

class Ui::OpenRGBNanoleafNewDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBNanoleafNewDeviceDialog(QWidget *parent = nullptr);
    ~OpenRGBNanoleafNewDeviceDialog();

    NanoleafDevice show();

private:
    Ui::OpenRGBNanoleafNewDeviceDialogUi *ui;

private slots:
    void changeEvent(QEvent *event);
};

#endif // OPENRGBNANOLEAFNEWDEVICEDIALOG_H
