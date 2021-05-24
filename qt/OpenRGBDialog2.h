#ifndef OPENRGBDIALOG2_H
#define OPENRGBDIALOG2_H

#include "ui_OpenRGBDialog2.h"

#include "OpenRGBClientInfoPage.h"
#include "OpenRGBSoftwareInfoPage.h"
#include "OpenRGBSystemInfoPage.h"
#include "OpenRGBSupportedDevicesPage.h"
#include "OpenRGBSettingsPage.h"
#include "PluginManager.h"

#include <vector>
#include "i2c_smbus.h"
#include "RGBController.h"
#include "ProfileManager.h"
#include "NetworkClient.h"
#include "NetworkServer.h"

#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>

namespace Ui
{
    class OpenRGBDialog2;
}

class Ui::OpenRGBDialog2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit OpenRGBDialog2(QWidget *parent = 0);
    ~OpenRGBDialog2();

    void AddClient(NetworkClient* new_client);
    void AddClientTab();
    void AddI2CToolsPage();
    void AddServerTab();

    void setMode(unsigned char mode_val);

    static bool IsDarkTheme();
    static bool IsMinimizeOnClose();

private:
    /*-------------------------------------*\
    | Page pointers                         |
    \*-------------------------------------*/
    OpenRGBClientInfoPage *ClientInfoPage;
    OpenRGBSystemInfoPage *SMBusToolsPage;
    OpenRGBSoftwareInfoPage *SoftInfoPage;
    OpenRGBSupportedDevicesPage *SupportedPage;
    OpenRGBSettingsPage *SettingsPage;

    bool ShowI2CTools = false;

    /*-------------------------------------*\
    | System tray icon and menu             |
    \*-------------------------------------*/
    QSystemTrayIcon* trayIcon;
    QMenu* profileMenu;

    /*-------------------------------------*\
    | User interface                        |
    \*-------------------------------------*/
    Ui::OpenRGBDialog2Ui *ui;

    void AddSoftwareInfoPage();
    void AddSupportedDevicesPage();
    void AddSettingsPage();
    void AddPluginTab(PluginManager* plugin_manager,int plugin_index);

    void ClearDevicesList();
    void UpdateDevicesList();
    void UpdateProfileList();
    void closeEvent(QCloseEvent *event);

    void SetDetectionViewState(bool detection_showing);
    void SaveProfile();
    void SaveProfileAs();

    void TogglePluginsVisibility(int, QTabWidget*);

    bool device_view_showing = false;

    PluginManager* plugin_manager = nullptr;

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
    void on_SetAllDevices(unsigned char red, unsigned char green, unsigned char blue);
    void on_SaveSizeProfile();
    void on_ShowHide();
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

#endif // OPENRGBDIALOG2_H
