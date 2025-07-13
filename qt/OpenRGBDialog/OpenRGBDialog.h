/*---------------------------------------------------------*\
| OpenRGBDialog.h                                           |
|                                                           |
|   User interface for OpenRGB main window                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QSlider>

#include "OpenRGBClientInfoPage.h"
#include "OpenRGBPluginsPage/OpenRGBPluginsPage.h"
#include "OpenRGBSoftwareInfoPage.h"
#include "OpenRGBSystemInfoPage.h"
#include "OpenRGBSupportedDevicesPage.h"
#include "OpenRGBSettingsPage.h"
#include "ManualDevicesSettingsPage/ManualDevicesSettingsPage.h"

#include "PluginManager.h"
#include "SuspendResume.h"

#include "i2c_smbus.h"
#include "LogManager.h"
#include "RGBController.h"
#include "ProfileManager.h"
#include "NetworkClient.h"
#include "NetworkServer.h"

namespace Ui
{
    class OpenRGBDialog;
}

class OpenRGBDialog : public QMainWindow, private SuspendResumeListener
{
    Q_OBJECT

public:
    explicit OpenRGBDialog(QWidget *parent = 0);
    ~OpenRGBDialog();

    void AddClientTab();
    void AddI2CToolsPage();
    void AddServerTab();

    void AddPlugin(OpenRGBPluginEntry* plugin);
    void RemovePlugin(OpenRGBPluginEntry* plugin);

    void setMode(unsigned char mode_val);

    static bool IsMinimizeOnClose();

    void SetDialogMessage(PLogMessage msg);

    bool DontShowAgain;

signals:
    void ProfileListChanged();

public slots:
    void changeEvent(QEvent *event) override;
    void SetTrayIcon(bool tray_icon);
    void handleAboutToQuit();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    /*-----------------------------------------------------*\
    | Context string                                        |
    \*-----------------------------------------------------*/
    const char *                    context             = "OpenRGBDialog";

    /*-----------------------------------------------------*\
    | Page pointers                                         |
    \*-----------------------------------------------------*/
    OpenRGBClientInfoPage *         ClientInfoPage;
    OpenRGBPluginsPage *            PluginsPage;
    OpenRGBSystemInfoPage *         SMBusToolsPage;
    OpenRGBSoftwareInfoPage *       SoftInfoPage;
    OpenRGBSupportedDevicesPage *   SupportedPage;
    OpenRGBSettingsPage *           SettingsPage;

    ManualDevicesSettingsPage *     manualDevicesPage;

    PluginManager *                 plugin_manager      = nullptr;

    bool                            device_view_showing = false;
    bool                            ShowI2CTools        = false;
    bool                            plugins_loaded      = false;

    /*-----------------------------------------------------*\
    | System tray icon and menu                             |
    \*-----------------------------------------------------*/
    QSystemTrayIcon *               trayIcon;
    QMenu *                         trayIconMenu;
    QMenu *                         profileMenu;

    QAction *                       actionExit;
    QString                         dialog_message;

    /*-----------------------------------------------------*\
    | User interface                                        |
    \*-----------------------------------------------------*/
    Ui::OpenRGBDialog *ui;

    void AddSoftwareInfoPage();
    void AddSupportedDevicesPage();
    void AddSettingsPage();
    void AddPluginsPage();
    void AddConsolePage();
    void AddManualDevicesSettingsPage();

    void ClearDevicesList();
    void UpdateDevicesList();
    void UpdateProfileList();
    void closeEvent(QCloseEvent *event) override;
    bool SelectConfigProfile(const std::string name);

    void SetDetectionViewState(bool detection_showing);
    void SaveProfile();
    void SaveProfileAs();

    void TogglePluginsVisibility(int, QTabWidget*);

    void ShowLEDView();
    void HideLEDView();

    void OnSuspend() override;
    void OnResume() override;

private slots:
    void on_Exit();
    void on_LightsOff();
    void on_QuickRed();
    void on_QuickYellow();
    void on_QuickGreen();
    void on_QuickCyan();
    void on_QuickBlue();
    void on_QuickMagenta();
    void on_QuickWhite();
    void onDeviceListUpdated();
    void onDetectionProgressUpdated();
    void onDetectionStarted();
    void onDetectionEnded();
    void on_SetAllDevices(unsigned char red, unsigned char green, unsigned char blue);
    void on_SaveSizeProfile();
    void on_ShowHide();
    void onShowDialogMessage();
    void on_ReShow(QSystemTrayIcon::ActivationReason reason);
    void on_ProfileSelected();
    void on_ButtonLoadProfile_clicked();
    void on_ButtonDeleteProfile_clicked();
    void on_ButtonToggleDeviceView_clicked();
    void on_ButtonStopDetection_clicked();
    void on_ButtonRescan_clicked();
    void on_ActionSaveProfile_triggered();
    void on_ActionSaveProfileAs_triggered();
    void on_MainTabBar_currentChanged(int);
    void on_InformationTabBar_currentChanged(int);
    void on_DevicesTabBar_currentChanged(int);
    void on_SettingsTabBar_currentChanged(int);
};
