/*---------------------------------------------------------*\
| NanoleafNewDeviceDialog.h                                 |
|                                                           |
|   User interface for OpenRGB Nanoleaf dialog              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QDialog>

struct NanoleafDevice
{
    std::string     ip;
    unsigned int    port;
};

namespace Ui
{
	class NanoleafNewDeviceDialog;
}

class NanoleafNewDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NanoleafNewDeviceDialog(QWidget *parent = nullptr);
    ~NanoleafNewDeviceDialog();

    NanoleafDevice show();

private:
    Ui::NanoleafNewDeviceDialog *ui;

private slots:
    void changeEvent(QEvent *event);
};
