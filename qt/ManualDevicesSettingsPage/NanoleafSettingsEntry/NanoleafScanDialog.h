/*---------------------------------------------------------*\
| NanoleafScanDialog.h                                        |
|                                                           |
|   User interface for OpenRGB Nanoleaf scan & pairing page |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QDialog>
#include "NanoleafSettingsEntry.h"

namespace Ui
{
    class NanoleafScanDialog;
}

    class NanoleafScanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NanoleafScanDialog(QWidget *parent = nullptr);
    ~NanoleafScanDialog();

private slots:
    void changeEvent(QEvent *event);
    void on_AddNanoleafDeviceButton_clicked();
    void on_RemoveNanoleafDeviceButton_clicked();
    void on_ScanForNanoleafDevicesButton_clicked();
    void on_DeviceFound(QString address, int port);

    void on_NanoleafDeviceList_itemSelectionChanged();

private:
    Ui::NanoleafScanDialog *ui;
    std::map<std::string, NanoleafSettingsEntry*> entries;
};
