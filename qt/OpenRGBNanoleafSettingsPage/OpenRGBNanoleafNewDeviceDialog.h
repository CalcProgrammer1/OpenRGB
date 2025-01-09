/*---------------------------------------------------------*\
| OpenRGBNanoleafNewDeviceDialog.h                          |
|                                                           |
|   User interface for OpenRGB Nanoleaf dialog              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QDialog>
#include "ui_OpenRGBNanoleafNewDeviceDialog.h"
#include "OpenRGBDialog.h"

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
