/*---------------------------------------------------------*\
| AddDeviceDialog.h                                         |
|                                                           |
|   Device adding helper window & troubleshooting wizard    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QDialog>
#include <QSortFilterProxyModel>

namespace Ui {
class AddDeviceDialog;
}

class AddDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDeviceDialog(QWidget *parent = nullptr);
    ~AddDeviceDialog();

private slots:
    void on_ButtonSerial_clicked();
    void on_ButtonDMX_clicked();
    void on_ButtonQMK_clicked();
    void on_ButtonE131_clicked();
    void on_ButtonElgatoKeyLight_clicked();
    void on_ButtonElgatoLightStrip_clicked();
    void on_ButtonGovee_clicked();
    void on_ButtonKasaSmart_clicked();
    void on_ButtonLIFX_clicked();
    void on_ButtonPhilipsHue_clicked();
    void on_ButtonPhilipsWiz_clicked();
    void on_ButtonYeelight_clicked();
    void on_ButtonNanloeaf_clicked();

    void on_ButtonConfigureBack_clicked();
    void on_ButtonConfigureScan_clicked();

    void on_ButtonScanBack_clicked();
    void on_ButtonScanRescan_clicked();
    void on_ButtonScanNotFound_clicked();
    void on_ButtonScanBeginning_clicked();
    void on_ButtonScanClose_clicked();

    void on_ButtonHelpBack_clicked();
    void on_ButtonHelpIssue_clicked();
    void on_ButtonHelpBeginning_clicked();
    void on_LabelGitlab_linkActivated(const QString &link);

    void onDetectionProgressChanged();
    void onDetectionEnded();

private:
    Ui::AddDeviceDialog *ui;
    void home();
    void rescan();
    void help();
    void setScanButtons(bool scan);
    void configure(QWidget* entry);

    int scanFromPage = -1;
    int helpFromPage = -1;
    int configureFromPage = -1;

    //device_type filterDevType;
    //connection_type filterConnType;

    std::string title; // Cached device name & manufacturer from Hardware IDs
    std::vector<std::string> cachedDevices;
};
