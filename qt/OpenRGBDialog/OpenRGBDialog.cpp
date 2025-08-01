/*---------------------------------------------------------*\
| OpenRGBDialog.cpp                                         |
|                                                           |
|   User interface for OpenRGB main window                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBDialog.h"
#include "LogManager.h"
#include "PluginManager.h"
#include "OpenRGBDevicePage.h"
#include "OpenRGBDeviceInfoPage.h"
#include "OpenRGBServerInfoPage.h"
#include "OpenRGBConsolePage.h"
#include "OpenRGBPluginContainer.h"
#include "OpenRGBProfileSaveDialog.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "TabLabel.h"
#include "OpenRGBZonesBulkResizer.h"
#include "OpenRGBThemeManager.h"
#include "OpenRGBFont.h"

#include "ui_OpenRGBDialog.h"

#include <QLabel>
#include <QTabBar>
#include <QMessageBox>
#include <QCloseEvent>
#include <QStyleFactory>
#include <QKeyEvent>
#include <QCheckBox>
#include <QSpinBox>

#include <string>
#include <functional>

#ifdef __APPLE__
#include "macutils.h"
#endif

static int GetIcon(device_type type)
{
    /*-----------------------------------------------------*\
    | Return the icon int value for the given device        |
    | type value                                            |
    \*-----------------------------------------------------*/
    int icon;

    switch(type)
    {
    case DEVICE_TYPE_ACCESSORY:
        icon = OpenRGBFont::usb;
        break;
    case DEVICE_TYPE_MOTHERBOARD:
        icon = OpenRGBFont::mainboard;
        break;
    case DEVICE_TYPE_DRAM:
        icon = OpenRGBFont::dram;
        break;
    case DEVICE_TYPE_GPU:
        icon = OpenRGBFont::gpu;
        break;
    case DEVICE_TYPE_COOLER:
        icon = OpenRGBFont::cooler;
        break;
    case DEVICE_TYPE_LEDSTRIP:
        icon = OpenRGBFont::ledstrip;
        break;
    case DEVICE_TYPE_KEYBOARD:
        icon = OpenRGBFont::keyboard;
        break;
    case DEVICE_TYPE_MICROPHONE:
        icon = OpenRGBFont::mic;
        break;
    case DEVICE_TYPE_MOUSE:
        icon = OpenRGBFont::mouse;
        break;
    case DEVICE_TYPE_MOUSEMAT:
        icon = OpenRGBFont::mousemat;
        break;
    case DEVICE_TYPE_HEADSET:
        icon = OpenRGBFont::headset;
        break;
    case DEVICE_TYPE_HEADSET_STAND:
        icon = OpenRGBFont::headsetstand;
        break;
    case DEVICE_TYPE_GAMEPAD:
        icon = OpenRGBFont::gamepad;
        break;
    case DEVICE_TYPE_LIGHT:
        icon = OpenRGBFont::bulb;
        break;
    case DEVICE_TYPE_SPEAKER:
        icon = OpenRGBFont::music_speaker;
        break;
    case DEVICE_TYPE_VIRTUAL:
        icon = OpenRGBFont::virtual_controller;
        break;
    case DEVICE_TYPE_STORAGE:
        icon = OpenRGBFont::drive;
        break;
    case DEVICE_TYPE_CASE:
        icon = OpenRGBFont::pc_case;
        break;
    case DEVICE_TYPE_KEYPAD:
        icon = OpenRGBFont::keypad;
        break;
    case DEVICE_TYPE_LAPTOP:
        icon = OpenRGBFont::laptop;
        break;
    case DEVICE_TYPE_MONITOR:
        icon = OpenRGBFont::monitor;
        break;
    default:
        icon = OpenRGBFont::unknown;
        break;
    }

    return icon;
}

static void UpdateDeviceListCallback(void * this_ptr)
{
    OpenRGBDialog * this_obj = (OpenRGBDialog *)this_ptr;

    QMetaObject::invokeMethod(this_obj, "onDeviceListUpdated", Qt::QueuedConnection);
}

static void UpdateDetectionProgressCallback(void * this_ptr)
{
    OpenRGBDialog * this_obj = (OpenRGBDialog *)this_ptr;

    QMetaObject::invokeMethod(this_obj, "onDetectionProgressUpdated", Qt::QueuedConnection);
}

static void CreatePluginCallback(void * this_ptr, OpenRGBPluginEntry* plugin)
{
    OpenRGBDialog * this_obj = (OpenRGBDialog *)this_ptr;

    this_obj->AddPlugin(plugin);
}

static void DeletePluginCallback(void * this_ptr, OpenRGBPluginEntry* plugin)
{
    OpenRGBDialog * this_obj = (OpenRGBDialog *)this_ptr;

    this_obj->RemovePlugin(plugin);
}

static void DetectionStartedCallback(void * this_ptr)
{
    OpenRGBDialog * this_obj = (OpenRGBDialog *)this_ptr;

    QMetaObject::invokeMethod(this_obj, "onDetectionStarted", Qt::QueuedConnection);
}

static void DetectionEndedCallback(void * this_ptr)
{
    OpenRGBDialog * this_obj = (OpenRGBDialog *)this_ptr;

    QMetaObject::invokeMethod(this_obj, "onDetectionEnded", Qt::QueuedConnection);
}

static void DialogShowCallback(void * this_ptr, PLogMessage msg)
{
    OpenRGBDialog * this_obj = (OpenRGBDialog *)this_ptr;

    this_obj->SetDialogMessage(msg);
    QMetaObject::invokeMethod(this_obj, "onShowDialogMessage", Qt::QueuedConnection);
}

bool OpenRGBDialog::IsMinimizeOnClose()
{
    json ui_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("UserInterface");

    if(ui_settings.contains("minimize_on_close"))
    {
        return ui_settings["minimize_on_close"];
    }

    return false;
}

OpenRGBDialog::OpenRGBDialog(QWidget *parent) : QMainWindow(parent), ui(new Ui::OpenRGBDialog)
{
    ui->setupUi(this);

    /*-----------------------------------------------------*\
    | Set window icon                                       |
    \*-----------------------------------------------------*/
    QIcon logo(":org.openrgb.OpenRGB.png");
    setWindowIcon(logo);

    /*-----------------------------------------------------*\
    | Set window geometry from config (if available)        |
    \*-----------------------------------------------------*/
    SettingsManager*    settings_manager    = ResourceManager::get()->GetSettingsManager();
    std::string         ui_string           = "UserInterface";
    json                ui_settings;
    bool                new_settings_keys   = false;

    ui_settings = settings_manager->GetSettings(ui_string);

    if(ui_settings.contains("show_led_view") && ui_settings["show_led_view"])
    {
        ShowLEDView();
    }

    /*-----------------------------------------------------*\
    | If geometry info doesn't exist, write it to config    |
    \*-----------------------------------------------------*/
    if(!ui_settings.contains("geometry"))
    {
        json geometry_settings;

        geometry_settings["load_geometry"]  = false;
        geometry_settings["save_on_exit"]   = false;
        geometry_settings["x"]              = 0;
        geometry_settings["y"]              = 0;
        geometry_settings["width"]          = 0;
        geometry_settings["height"]         = 0;

        ui_settings["geometry"] = geometry_settings;
        new_settings_keys       = true;
    }

    /*-----------------------------------------------------*\
    | If geometry information exists in settings, apply it  |
    \*-----------------------------------------------------*/
    bool load_geometry = false;

    if(ui_settings["geometry"].contains("load_geometry"))
    {
        load_geometry = ui_settings["geometry"]["load_geometry"].get<bool>();
    }

    if(load_geometry)
    {
        QRect set_window;

        /*-------------------------------------------------*\
        | x and y can be set independent of width and       |
        | height.  QT attempts to clamp these values in     |
        | case the user enters invalid numbers.             |
        \*-------------------------------------------------*/
        if( ui_settings["geometry"].contains("x")
         && ui_settings["geometry"].contains("y"))
        {
            set_window.setX(ui_settings["geometry"]["x"].get<int>());
            set_window.setY(ui_settings["geometry"]["y"].get<int>());
        }

        if( ui_settings["geometry"].contains("width")
         && ui_settings["geometry"].contains("height"))
        {
            set_window.setWidth(ui_settings["geometry"]["width"].get<int>());
            set_window.setHeight(ui_settings["geometry"]["height"].get<int>());
        }

        setGeometry(set_window);
    }

    /*-----------------------------------------------------*\
    | If autoload_profiles doesn't exist or has missing     |
    | profiles, write it to config                          |
    \*-----------------------------------------------------*/
    json autoload_profiles;
    if(ui_settings.contains("autoload_profiles"))
    {
        autoload_profiles = ui_settings["autoload_profiles"];
    }
    else
    {
        new_settings_keys                = true;
    }

    if(!autoload_profiles.contains("exit_profile"))
    {
        json profile;
        profile["enabled"]                = false;
        profile["name"]                   = "";
        autoload_profiles["exit_profile"] = profile;
        new_settings_keys                 = true;
    }

    if(!autoload_profiles.contains("resume_profile"))
    {
        json profile;
        profile["enabled"]                  = false;
        profile["name"]                     = "";
        autoload_profiles["resume_profile"] = profile;
        new_settings_keys                   = true;
    }

    if(!autoload_profiles.contains("suspend_profile"))
    {
        json profile;
        profile["enabled"]                   = false;
        profile["name"]                      = "";
        autoload_profiles["suspend_profile"] = profile;
        new_settings_keys                    = true;
    }

    ui_settings["autoload_profiles"] = autoload_profiles;

    /*-----------------------------------------------------*\
    | Register detection progress callback with resource    |
    | manager                                               |
    \*-----------------------------------------------------*/
    ResourceManager::get()->RegisterDetectionProgressCallback(UpdateDetectionProgressCallback, this);
    ResourceManager::get()->RegisterDeviceListChangeCallback(UpdateDeviceListCallback, this);
    ResourceManager::get()->RegisterDetectionStartCallback(DetectionStartedCallback, this);
    ResourceManager::get()->RegisterDetectionEndCallback(DetectionEndedCallback, this);

    /*-----------------------------------------------------*\
    | Register dialog show callback with log manager        |
    \*-----------------------------------------------------*/
    LogManager::get()->RegisterDialogShowCallback(DialogShowCallback, this);

    /*-----------------------------------------------------*\
    | Initialize page pointers                              |
    \*-----------------------------------------------------*/
    ClientInfoPage  = NULL;
    SMBusToolsPage  = NULL;
    SoftInfoPage    = NULL;

    onDetectionProgressUpdated();

    ui->DetectionProgressBar->setRange(0, 100);
    ui->DetectionProgressBar->setValue(0);
    ui->DetectionProgressBar->setTextVisible(true);
    ui->DetectionProgressBar->setFormat("");
    ui->DetectionProgressBar->setAlignment(Qt::AlignCenter);

    /*-----------------------------------------------------*\
    | Set up Save Profile button action and menu            |
    \*-----------------------------------------------------*/
    QMenu* saveProfileMenu = new QMenu(this);
    saveProfileMenu->addAction(ui->ActionSaveProfileAs);

    ui->ButtonSaveProfile->setMenu(saveProfileMenu);
    ui->ButtonSaveProfile->setDefaultAction(ui->ActionSaveProfile);

    /*-----------------------------------------------------*\
    | Set up tray icon menu                                 |
    \*-----------------------------------------------------*/
    trayIconMenu = new QMenu( this );

    trayIcon = new QSystemTrayIcon(this);

    QAction* actionShowHide = new QAction(tr("Show/Hide"), this);
    connect(actionShowHide, SIGNAL(triggered()), this, SLOT(on_ShowHide()));
    trayIconMenu->addAction(actionShowHide);

    profileMenu = new QMenu(tr("Profiles"), this);

    trayIconMenu->addMenu(profileMenu);

    QMenu* quickColorsMenu = new QMenu(tr("Quick Colors"), this);

    QAction* actionQuickRed = new QAction(tr("Red"), this);
    connect(actionQuickRed, SIGNAL(triggered()), this, SLOT(on_QuickRed()));
    quickColorsMenu->addAction(actionQuickRed);

    QAction* actionQuickYellow = new QAction(tr("Yellow"), this);
    connect(actionQuickYellow, SIGNAL(triggered()), this, SLOT(on_QuickYellow()));
    quickColorsMenu->addAction(actionQuickYellow);

    QAction* actionQuickGreen = new QAction(tr("Green"), this);
    connect(actionQuickGreen, SIGNAL(triggered()), this, SLOT(on_QuickGreen()));
    quickColorsMenu->addAction(actionQuickGreen);

    QAction* actionQuickCyan = new QAction(tr("Cyan"), this);
    connect(actionQuickCyan, SIGNAL(triggered()), this, SLOT(on_QuickCyan()));
    quickColorsMenu->addAction(actionQuickCyan);

    QAction* actionQuickBlue = new QAction(tr("Blue"), this);
    connect(actionQuickBlue, SIGNAL(triggered()), this, SLOT(on_QuickBlue()));
    quickColorsMenu->addAction(actionQuickBlue);

    QAction* actionQuickMagenta = new QAction(tr("Magenta"), this);
    connect(actionQuickMagenta, SIGNAL(triggered()), this, SLOT(on_QuickMagenta()));
    quickColorsMenu->addAction(actionQuickMagenta);

    QAction* actionQuickWhite = new QAction(tr("White"), this);
    connect(actionQuickWhite, SIGNAL(triggered()), this, SLOT(on_QuickWhite()));
    quickColorsMenu->addAction(actionQuickWhite);

    trayIconMenu->addMenu(quickColorsMenu);

    QAction* actionLightsOff = new QAction(tr("Lights Off"), this);
    actionLightsOff->setObjectName("ActionLightsOff");
    connect(actionLightsOff, SIGNAL(triggered()), this, SLOT(on_LightsOff()));
    trayIconMenu->addAction(actionLightsOff);

    QAction* actionReScan = new QAction(tr("Rescan Devices"), this);
    connect(actionReScan, SIGNAL(triggered()), this, SLOT(on_ButtonRescan_clicked()));
    trayIconMenu->addAction(actionReScan);

    actionExit = new QAction(tr("Exit"), this );
    connect( actionExit, SIGNAL( triggered() ), this, SLOT( on_Exit() ));
    trayIconMenu->addAction(actionExit);

    /*-----------------------------------------------------*\
    | If tray minimize flag isn't in the config, set        |
    | default value to false                                |
    \*-----------------------------------------------------*/
    if(!ui_settings.contains("minimize_on_close"))
    {
        ui_settings["minimize_on_close"] = false;
        new_settings_keys                = true;
    }

    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_ReShow(QSystemTrayIcon::ActivationReason)));

    /*-----------------------------------------------------*\
    | If Greyscale Tray Icon flag is not set in config then |
    | set the default value to false                        |
    \*-----------------------------------------------------*/
    if(!ui_settings.contains("greyscale_tray_icon"))
    {
        ui_settings["greyscale_tray_icon"] = false;
        new_settings_keys                  = true;
    }

    /*-----------------------------------------------------*\
    | If greyscale tray icon exists in settings, apply it   |
    |   or else set the icon to the default window logo     |
    \*-----------------------------------------------------*/
    if(ui_settings.contains("greyscale_tray_icon"))
    {
        SetTrayIcon(ui_settings["greyscale_tray_icon"].get<bool>());
    }

    /*-----------------------------------------------------*\
    | Save the settings if new default values have been     |
    | inserted                                              |
    \*-----------------------------------------------------*/
    if(new_settings_keys)
    {
        settings_manager->SetSettings(ui_string, ui_settings);
        settings_manager->SaveSettings();
    }

    trayIcon->setToolTip("OpenRGB");
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();

    /*-----------------------------------------------------*\
    | Initialize the theme manager                          |
    \*-----------------------------------------------------*/
    OpenRGBThemeManager::Init();

    /*-----------------------------------------------------*\
    | Update the profile list                               |
    \*-----------------------------------------------------*/
    UpdateProfileList();

    /*-----------------------------------------------------*\
    | Update the device list and make sure the              |
    | ProgressBar gets a proper value                       |
    \*-----------------------------------------------------*/
    UpdateDevicesList();

    /*-----------------------------------------------------*\
    | Add Client Tab                                        |
    \*-----------------------------------------------------*/
    AddClientTab();

    /*-----------------------------------------------------*\
    | Add Server Tab                                        |
    \*-----------------------------------------------------*/
    AddServerTab();

    /*-----------------------------------------------------*\
    | Add the Software Info page                            |
    \*-----------------------------------------------------*/
    AddSoftwareInfoPage();

    /*-----------------------------------------------------*\
    | Add the settings page                                 |
    \*-----------------------------------------------------*/
    AddSettingsPage();

    /*-----------------------------------------------------*\
    | Add the Supported Devices page                        |
    \*-----------------------------------------------------*/
    AddSupportedDevicesPage();

    /*-----------------------------------------------------*\
    | Initialize the plugin manager                         |
    \*-----------------------------------------------------*/
    plugin_manager = new PluginManager();
    plugin_manager->RegisterAddPluginCallback(&CreatePluginCallback, this);
    plugin_manager->RegisterRemovePluginCallback(&DeletePluginCallback, this);

    /*-----------------------------------------------------*\
    | Add the Plugins page                                  |
    \*-----------------------------------------------------*/
    AddPluginsPage();

    /*-----------------------------------------------------*\
    | Add the Manually Added Devices settings page          |
    \*-----------------------------------------------------*/
    AddManualDevicesSettingsPage();

    /*-----------------------------------------------------*\
    | Add the SMBus Tools page if enabled                   |
    \*-----------------------------------------------------*/
    if(ShowI2CTools)
    {
        AddI2CToolsPage();
    }

    /*-----------------------------------------------------*\
    | If log console is enabled in settings, enable it      |
    \*-----------------------------------------------------*/
    json log_manager_settings = settings_manager->GetSettings("LogManager");

    bool log_console_enabled = false;
    if(log_manager_settings.contains("log_console"))
    {
        log_console_enabled = log_manager_settings["log_console"];
    }

    /*-----------------------------------------------------*\
    | Add the log console page                              |
    \*-----------------------------------------------------*/
    if(log_console_enabled)
    {
        AddConsolePage();
    }

    /*-----------------------------------------------------*\
    | Connect aboutToQuit signal to handleAboutToQuit       |
    \*-----------------------------------------------------*/
    connect(qApp, &QCoreApplication::aboutToQuit, this, &OpenRGBDialog::handleAboutToQuit);

    /*-----------------------------------------------------*\
    | Handle the condition where detection ended before we  |
    | finished initializing/registering detection callback  |
    \*-----------------------------------------------------*/
    if(ResourceManager::get()->GetDetectionPercent() >= 100)
    {
        onDetectionEnded();
    }
}

OpenRGBDialog::~OpenRGBDialog()
{
    delete ui;
}


void OpenRGBDialog::handleAboutToQuit()
{
    /*-----------------------------------------------------*\
    | Write window geometry to config (if enabled)          |
    \*-----------------------------------------------------*/
    SettingsManager*    settings_manager    = ResourceManager::get()->GetSettingsManager();
    std::string         ui_string           = "UserInterface";
    json                ui_settings;

    ui_settings = settings_manager->GetSettings(ui_string);

    if(ui_settings.contains("geometry"))
    {
        if( ui_settings["geometry"].contains("load_geometry")
         && ui_settings["geometry"].contains("save_on_exit"))
        {
            if( ui_settings["geometry"]["load_geometry"].get<bool>()
            && ui_settings["geometry"]["save_on_exit"].get<bool>())
            {
                ui_settings["geometry"]["x"]                = geometry().x();
                ui_settings["geometry"]["y"]                = geometry().y();
                ui_settings["geometry"]["width"]            = geometry().width();
                ui_settings["geometry"]["height"]           = geometry().height();

                settings_manager->SetSettings(ui_string, ui_settings);
                settings_manager->SaveSettings();
            }
        }
    }

    disconnect(qApp, &QCoreApplication::aboutToQuit, this, &OpenRGBDialog::handleAboutToQuit);
    QCloseEvent* closeEvent = new QCloseEvent;
    this->closeEvent(closeEvent);
    delete closeEvent;
}


void OpenRGBDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
        for(int i = 0; i < ui->MainTabBar->count(); i++)
        {
            /*---------------------------------------------*\
            | If the objectName is either of the SDK tabs   |
            | then translate tab text (TODO: improve        |
            | workaround)                                   |
            \*---------------------------------------------*/
            std::string label = ui->MainTabBar->widget(i)->objectName().toStdString();

            if(label.substr(0,3) == "SDK")
            {
                ui->MainTabBar->setTabText(i, tr(label.c_str()));
            }
        }
    }
}

void OpenRGBDialog::closeEvent(QCloseEvent *event)
{
    ResourceManager::get()->WaitForDeviceDetection();

    if (IsMinimizeOnClose() && !this->isHidden() && event->spontaneous())
    {
#ifdef __APPLE__
        MacUtils::ToggleApplicationDocklessState(false);
#endif
        hide();
        event->ignore();
    }
    else
    {
        plugin_manager->UnloadPlugins();

        if(SelectConfigProfile("exit_profile"))
        {
            on_ButtonLoadProfile_clicked();

            /*---------------------------------------------*\
            | Pause briefly to ensure that all profiles are |
            | loaded.                                       |
            \*---------------------------------------------*/
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }

        event->accept();
        QApplication::exit(0);
    }
}

void OpenRGBDialog::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F11)
    {
        if(isFullScreen())
        {
            showNormal();
        }
        else
        {
            showFullScreen();
        }
        event->accept();
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}

bool OpenRGBDialog::SelectConfigProfile(const std::string name)
{
    /*-----------------------------------------------------*\
    | Set automatic profile (if enabled and valid)          |
    \*-----------------------------------------------------*/
    json ui_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("UserInterface");

    if(ui_settings.contains("autoload_profiles"))
    {
        json autoload_profiles = ui_settings["autoload_profiles"];
        if(autoload_profiles.contains(name))
        {
            json profile = autoload_profiles[name];
            if (profile.contains("enabled") && profile["enabled"].get<bool>() && profile.contains("name"))
            {
                /*-----------------------------------------*\
                | Set the profile name from settings and    |
                | check the profile combobox for a match    |
                \*-----------------------------------------*/
                std::string profile_name = profile["name"].get<std::string>();
                int profile_index        = ui->ProfileBox->findText(QString::fromStdString(profile_name));

                if(profile_index > -1)
                {
                    ui->ProfileBox->setCurrentIndex(profile_index);
                    return true;
                }
            }
        }
    }
    return false;
}

void OpenRGBDialog::AddPluginsPage()
{
    /*-----------------------------------------------------*\
    | Create the Plugins page                               |
    \*-----------------------------------------------------*/
    PluginsPage = new OpenRGBPluginsPage(plugin_manager);

    ui->SettingsTabBar->addTab(PluginsPage, "");

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* PluginTabLabel = new TabLabel(OpenRGBFont::extension, tr("Plugins"), (char *)"Plugins", (char *)context);

    ui->SettingsTabBar->tabBar()->setTabButton(ui->SettingsTabBar->tabBar()->count() - 1, QTabBar::LeftSide, PluginTabLabel);
}

void OpenRGBDialog::AddSoftwareInfoPage()
{
    /*-----------------------------------------------------*\
    | Create the Software Information page                  |
    \*-----------------------------------------------------*/
    SoftInfoPage = new OpenRGBSoftwareInfoPage();

    ui->InformationTabBar->addTab(SoftInfoPage, "");

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* SoftwareTabLabel = new TabLabel(OpenRGBFont::info, tr("About OpenRGB"), (char *)"About OpenRGB", (char *)context);

    ui->InformationTabBar->tabBar()->setTabButton(ui->InformationTabBar->tabBar()->count() - 1, QTabBar::LeftSide, SoftwareTabLabel);
}

void OpenRGBDialog::AddSupportedDevicesPage()
{
    /*-----------------------------------------------------*\
    | Create the Supported Devices page                     |
    \*-----------------------------------------------------*/
    SupportedPage = new OpenRGBSupportedDevicesPage();

    ui->SettingsTabBar->addTab(SupportedPage, "");

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* SupportedTabLabel = new TabLabel(OpenRGBFont::controller, tr("Supported Devices"), (char *)"Supported Devices", (char *)context);

    ui->SettingsTabBar->tabBar()->setTabButton(ui->SettingsTabBar->tabBar()->count() - 1, QTabBar::LeftSide, SupportedTabLabel);
}

void OpenRGBDialog::AddSettingsPage()
{
    /*-----------------------------------------------------*\
    | Create the Settings page                              |
    \*-----------------------------------------------------*/
    SettingsPage = new OpenRGBSettingsPage();

    ui->SettingsTabBar->addTab(SettingsPage, "");

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* SettingsTabLabel = new TabLabel(OpenRGBFont::options, tr("General Settings"), (char *)"General Settings", (char *)context);

    ui->SettingsTabBar->tabBar()->setTabButton(ui->SettingsTabBar->tabBar()->count() - 1, QTabBar::LeftSide, SettingsTabLabel);

    /*-----------------------------------------------------*\
    | Connect signals to slots                              |
    \*-----------------------------------------------------*/
    connect(SettingsPage, SIGNAL(TrayIconChanged(bool)), this, SLOT(SetTrayIcon(bool)));
    connect(this, SIGNAL(ProfileListChanged()), SettingsPage, SLOT(UpdateProfiles()));
}

void OpenRGBDialog::AddManualDevicesSettingsPage()
{
    /*-----------------------------------------------------*\
    | Create the Settings page                              |
    \*-----------------------------------------------------*/
    manualDevicesPage = new ManualDevicesSettingsPage();

    ui->SettingsTabBar->addTab(manualDevicesPage, "");

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* SettingsTabLabel = new TabLabel(OpenRGBFont::bulb, tr("Manually Added Devices"), (char *)"Manually Added Devices", (char *)context);

    ui->SettingsTabBar->tabBar()->setTabButton(ui->SettingsTabBar->tabBar()->count() - 1, QTabBar::LeftSide, SettingsTabLabel);
}

void OpenRGBDialog::AddPlugin(OpenRGBPluginEntry* plugin)
{
    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* PluginTabLabel = new TabLabel(OpenRGBFont::extension, QString::fromStdString(plugin->info.Label), (char *)plugin->info.Label.c_str(), (char *)context);

    /*-----------------------------------------------------*\
    | Place plugin as its own top level tab                 |
    \*-----------------------------------------------------*/
    if(plugin->info.Location == OPENRGB_PLUGIN_LOCATION_TOP)
    {
        QWidget* NewPluginTab = plugin->plugin->GetWidget();

        plugin->widget = NewPluginTab;

        OpenRGBPluginContainer* NewPluginContainer = new OpenRGBPluginContainer(NewPluginTab, this);

        ui->MainTabBar->addTab(NewPluginContainer,QString().fromStdString(plugin->info.Label));
    }
    /*-----------------------------------------------------*\
    | Place plugin in the Devices tab                       |
    \*-----------------------------------------------------*/
    else if(plugin->info.Location == OPENRGB_PLUGIN_LOCATION_DEVICES)
    {
        QWidget* NewPluginTab = plugin->plugin->GetWidget();

        plugin->widget = NewPluginTab;

        OpenRGBPluginContainer* NewPluginContainer = new OpenRGBPluginContainer(NewPluginTab, this);

        ui->DevicesTabBar->addTab(NewPluginContainer," ");

        ui->DevicesTabBar->tabBar()->setTabButton((ui->DevicesTabBar->count() - 1),QTabBar::LeftSide , PluginTabLabel);
    }
    /*-----------------------------------------------------*\
    | Place plugin in the Information tab                   |
    \*-----------------------------------------------------*/
    else if(plugin->info.Location == OPENRGB_PLUGIN_LOCATION_INFORMATION)
    {
        QWidget* NewPluginTab = plugin->plugin->GetWidget();

        plugin->widget = NewPluginTab;

        OpenRGBPluginContainer* NewPluginContainer = new OpenRGBPluginContainer(NewPluginTab, this);

        ui->InformationTabBar->addTab(NewPluginContainer," ");

        ui->InformationTabBar->tabBar()->setTabButton((ui->InformationTabBar->count() - 1),QTabBar::LeftSide , PluginTabLabel);
    }
    /*-----------------------------------------------------*\
    | Place plugin in the Settings tab                      |
    \*-----------------------------------------------------*/
    else if(plugin->info.Location == OPENRGB_PLUGIN_LOCATION_SETTINGS)
    {
        QWidget* NewPluginTab = plugin->plugin->GetWidget();

        plugin->widget = NewPluginTab;

        OpenRGBPluginContainer* NewPluginContainer = new OpenRGBPluginContainer(NewPluginTab, this);

        ui->SettingsTabBar->addTab(NewPluginContainer," ");

        ui->SettingsTabBar->tabBar()->setTabButton((ui->SettingsTabBar->count() - 1),QTabBar::LeftSide , PluginTabLabel);
    }
    /*-----------------------------------------------------*\
    | Display an error message if the plugin does not       |
    | specify a valid location                              |
    \*-----------------------------------------------------*/
    else
    {
        std::cout << ("Cannot load plugin '" + plugin->info.Name + "' as it does not specify a valid location.\n");
    }

    QMenu* NewTrayMenu = plugin->plugin->GetTrayMenu();

    plugin->traymenu = NewTrayMenu;

    if(NewTrayMenu)
    {
        trayIconMenu->insertMenu(actionExit, NewTrayMenu);
    }
}

void OpenRGBDialog::RemovePlugin(OpenRGBPluginEntry* plugin)
{
    /*-----------------------------------------------------*\
    | Remove plugin's tray menu                             |
    \*-----------------------------------------------------*/
    if(plugin->traymenu)
    {
        QWidget* plugin_tray_entry = trayIconMenu->find(plugin->traymenu->winId());

        if(plugin_tray_entry)
        {
            trayIconMenu->removeAction(plugin->traymenu->menuAction());
        }
    }

    /*-----------------------------------------------------*\
    | Find plugin's container                               |
    \*-----------------------------------------------------*/
    QTabWidget *plugin_parent_widget = nullptr;

    switch(plugin->info.Location)
    {
        case OPENRGB_PLUGIN_LOCATION_TOP:
            plugin_parent_widget = ui->MainTabBar;
            break;
        case OPENRGB_PLUGIN_LOCATION_DEVICES:
            plugin_parent_widget = ui->DevicesTabBar;
            break;
        case OPENRGB_PLUGIN_LOCATION_INFORMATION:
            plugin_parent_widget = ui->InformationTabBar;
            break;
        case OPENRGB_PLUGIN_LOCATION_SETTINGS:
            plugin_parent_widget = ui->InformationTabBar;
            break;
        default:
            break;
    }

    /*-----------------------------------------------------*\
    | Remove plugin from its container                      |
    \*-----------------------------------------------------*/
    if(plugin_parent_widget != nullptr)
    {
        for(int tab_idx = 0; tab_idx < plugin_parent_widget->count(); tab_idx++)
        {
            if(dynamic_cast<OpenRGBPluginContainer*>(plugin_parent_widget->widget(tab_idx)) != nullptr)
            {
                if(dynamic_cast<OpenRGBPluginContainer*>(plugin_parent_widget->widget(tab_idx))->plugin_widget == plugin->widget)
                {
                    plugin_parent_widget->removeTab(tab_idx);
                    break;
                }
            }
        }
    }
}

void OpenRGBDialog::AddI2CToolsPage()
{
    ShowI2CTools = true;

    /*-----------------------------------------------------*\
    | Create the I2C Tools page if it doesn't exist yet     |
    \*-----------------------------------------------------*/
    SMBusToolsPage = new OpenRGBSystemInfoPage(ResourceManager::get()->GetI2CBusses());

    /*-----------------------------------------------------*\
    | Create the I2C Tools tab in the Information bar       |
    \*-----------------------------------------------------*/
    ui->InformationTabBar->addTab(SMBusToolsPage, "");

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* SMBusToolsTabLabel = new TabLabel(OpenRGBFont::toolbox, tr("SMBus Tools"), (char *)"SMBus Tools", (char *)context);

    ui->InformationTabBar->tabBar()->setTabButton(ui->InformationTabBar->tabBar()->count() - 1, QTabBar::LeftSide, SMBusToolsTabLabel);
}

void OpenRGBDialog::AddClientTab()
{
    /*-----------------------------------------------------*\
    | Add client information tab if it doesn't exist yet    |
    \*-----------------------------------------------------*/
    if(ClientInfoPage == NULL)
    {
        ClientInfoPage = new OpenRGBClientInfoPage();
        ClientInfoPage->setObjectName(QString("SDK Client"));
        ui->MainTabBar->insertTab(2, ClientInfoPage, tr("SDK Client"));
    }
}

void OpenRGBDialog::AddServerTab()
{
    /*-----------------------------------------------------*\
    | Add server information tab if there is a server       |
    \*-----------------------------------------------------*/
    OpenRGBServerInfoPage *ServerInfoPage = new OpenRGBServerInfoPage(ResourceManager::get()->GetServer());
    ServerInfoPage->setObjectName(QString("SDK Server"));
    ui->MainTabBar->insertTab(2, ServerInfoPage, tr("SDK Server"));
}

void OpenRGBDialog::ClearDevicesList()
{
    for(int tab_idx = 0; tab_idx < ui->DevicesTabBar->count(); tab_idx++)
    {
        if(dynamic_cast<OpenRGBPluginContainer*>(ui->DevicesTabBar->widget(tab_idx)) == nullptr)
        {
            delete ui->DevicesTabBar->widget(tab_idx);
            ui->DevicesTabBar->removeTab(tab_idx);
        }
    }

    for(int tab_idx = 0; tab_idx < ui->InformationTabBar->count(); tab_idx++)
    {
        if(dynamic_cast<OpenRGBPluginContainer*>(ui->InformationTabBar->widget(tab_idx)) == nullptr)
        {
            delete ui->InformationTabBar->widget(tab_idx);
            ui->InformationTabBar->removeTab(tab_idx);
        }
    }
}

void OpenRGBDialog::UpdateDevicesList()
{
    std::vector<RGBController *> controllers = ResourceManager::get()->GetRGBControllers();

    /*-----------------------------------------------------*\
    | Loop through each controller in the list.             |
    \*-----------------------------------------------------*/
    for(unsigned int controller_idx = 0; controller_idx < controllers.size(); controller_idx++)
    {
        /*-------------------------------------------------*\
        | Loop through each tab in the devices tab bar      |
        \*-------------------------------------------------*/
        bool found = false;

        for(int tab_idx = 0; tab_idx < ui->DevicesTabBar->count(); tab_idx++)
        {
            QWidget* page = ui->DevicesTabBar->widget(tab_idx);

            if(dynamic_cast<OpenRGBDevicePage*>(page) != nullptr)
            {
                /*-----------------------------------------*\
                | If the current tab matches the current    |
                | controller, move the tab to the correct   |
                | position                                  |
                \*-----------------------------------------*/
                if(controllers[controller_idx] == ((OpenRGBDevicePage*)page)->GetController())
                {
                    found = true;
                    ui->DevicesTabBar->tabBar()->moveTab(tab_idx, controller_idx);
                    break;
                }
            }
        }

        if(!found)
        {
            /*---------------------------------------------*\
            | The controller does not have a tab already    |
            | created.  Create a new tab and move it to the |
            | correct position                              |
            \*---------------------------------------------*/
            OpenRGBDevicePage *NewPage = new OpenRGBDevicePage(controllers[controller_idx]);
            ui->DevicesTabBar->addTab(NewPage, "");

            /*---------------------------------------------*\
            | Connect the page's Set All button to the Set  |
            | All slot                                      |
            \*---------------------------------------------*/
            connect(NewPage,
                    SIGNAL(SetAllDevices(unsigned char, unsigned char, unsigned char)),
                    this,
                    SLOT(on_SetAllDevices(unsigned char, unsigned char, unsigned char)));

            /*---------------------------------------------*\
            | Connect the page's Resize signal to the Save  |
            | Size slot                                     |
            \*---------------------------------------------*/
            connect(NewPage,
                    SIGNAL(SaveSizeProfile()),
                    this,
                    SLOT(on_SaveSizeProfile()));

            /*---------------------------------------------*\
            | Create the tab label                          |
            \*---------------------------------------------*/
            TabLabel* NewTabLabel = new TabLabel(GetIcon(controllers[controller_idx]->type), QString::fromStdString(controllers[controller_idx]->GetName()), (char *)controllers[controller_idx]->GetName().c_str(), (char *)context);

            ui->DevicesTabBar->tabBar()->setTabButton(ui->DevicesTabBar->count() - 1, QTabBar::LeftSide, NewTabLabel);
            ui->DevicesTabBar->tabBar()->setTabToolTip(ui->DevicesTabBar->count() - 1, QString::fromStdString(controllers[controller_idx]->GetName()));

            /*---------------------------------------------*\
            | Now move the new tab to the correct position  |
            \*---------------------------------------------*/
            ui->DevicesTabBar->tabBar()->moveTab(ui->DevicesTabBar->count() - 1, controller_idx);
        }

        /*-------------------------------------------------*\
        | Loop through each tab in the information tab bar  |
        \*-------------------------------------------------*/
        found = false;

        for(int tab_idx = 0; tab_idx < ui->InformationTabBar->count(); tab_idx++)
        {
            /*---------------------------------------------*\
            | If type is a device info page, check it       |
            \*---------------------------------------------*/
            std::string type_str = ui->InformationTabBar->widget(tab_idx)->metaObject()->className();
            if(type_str == "OpenRGBDeviceInfoPage")
            {
                OpenRGBDeviceInfoPage* page = (OpenRGBDeviceInfoPage*) ui->InformationTabBar->widget(tab_idx);

                /*-----------------------------------------*\
                | If the current tab matches the current    |
                | controller, move the tab to the correct   |
                | position                                  |
                \*-----------------------------------------*/
                if(controllers[controller_idx] == page->GetController())
                {
                    found = true;
                    ui->InformationTabBar->tabBar()->moveTab(tab_idx, controller_idx);
                    break;
                }
            }
        }

        if(!found)
        {
            /*---------------------------------------------*\
            | The controller does not have a tab already    |
            | created.  Create a new tab and move it to     |
            | the correct position.                         |
            \*---------------------------------------------*/
            OpenRGBDeviceInfoPage *NewPage = new OpenRGBDeviceInfoPage(controllers[controller_idx]);
            ui->InformationTabBar->addTab(NewPage, "");

            /*---------------------------------------------*\
            | Create the tab label                          |
            \*---------------------------------------------*/
            TabLabel* NewTabLabel = new TabLabel(GetIcon(controllers[controller_idx]->type), QString::fromStdString(controllers[controller_idx]->GetName()), (char *)controllers[controller_idx]->GetName().c_str(), (char *)context);

            ui->InformationTabBar->tabBar()->setTabButton(ui->InformationTabBar->count() - 1, QTabBar::LeftSide, NewTabLabel);
            ui->InformationTabBar->tabBar()->setTabToolTip(ui->InformationTabBar->count() - 1, QString::fromStdString(controllers[controller_idx]->GetName()));

            /*---------------------------------------------*\
            | Now move the new tab to the correct position  |
            \*---------------------------------------------*/
            ui->InformationTabBar->tabBar()->moveTab(ui->InformationTabBar->count() - 1, controller_idx);
        }
    }

    /*-----------------------------------------------------*\
    | Remove all remaining device tabs                      |
    \*-----------------------------------------------------*/
    unsigned int tab_count = ui->DevicesTabBar->count();
    unsigned int base_tab = (unsigned int)controllers.size();

    for(std::size_t tab_idx = controllers.size(); tab_idx < tab_count; tab_idx++)
    {
        QWidget* tab_widget = ui->DevicesTabBar->widget(base_tab);

        if(dynamic_cast<OpenRGBPluginContainer*>(tab_widget) == nullptr)
        {
            ui->DevicesTabBar->removeTab(base_tab);
            delete tab_widget;
        }
    }

    bool found = true;

    while(found)
    {
        found = false;

        /*-------------------------------------------------*\
        | Remove all remaining device information tabs,     |
        | leaving other information tabs alone              |
        \*-------------------------------------------------*/
        for(std::size_t tab_idx = controllers.size(); tab_idx < (std::size_t)ui->InformationTabBar->count(); tab_idx++)
        {
            std::string type_str = ui->InformationTabBar->widget(base_tab)->metaObject()->className();
            if(type_str == "OpenRGBDeviceInfoPage")
            {
                found = true;
                QWidget* tab_widget = ui->InformationTabBar->widget(base_tab);

                ui->InformationTabBar->removeTab(base_tab);

                delete tab_widget;
                break;
            }
            base_tab += 1;
        }
    }

    if(device_view_showing)
    {
        ShowLEDView();
    }
}

void OpenRGBDialog::SetDialogMessage(PLogMessage msg)
{
    dialog_message = QString::fromStdString(msg->buffer);
}

void OpenRGBDialog::UpdateProfileList()
{
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    if(profile_manager != NULL)
    {
        /*-------------------------------------------------*\
        | Clear profile combo box and tray icon menu        |
        \*-------------------------------------------------*/
        ui->ProfileBox->clear();
        profileMenu->clear();

        for(std::size_t profile_index = 0; profile_index < profile_manager->profile_list.size(); profile_index++)
        {
            /*---------------------------------------------*\
            | Fill in profile combo box                     |
            \*---------------------------------------------*/
            ui->ProfileBox->addItem(profile_manager->profile_list[profile_index].c_str());

            /*---------------------------------------------*\
            | Fill in profile tray icon menu                |
            \*---------------------------------------------*/
            QAction* actionProfileSelected = new QAction(profile_manager->profile_list[profile_index].c_str(), this);
            actionProfileSelected->setObjectName(profile_manager->profile_list[profile_index].c_str());
            connect(actionProfileSelected, SIGNAL(triggered()), this, SLOT(on_ProfileSelected()));
            profileMenu->addAction(actionProfileSelected);
        }
    }

    emit ProfileListChanged();
}

void OpenRGBDialog::OnSuspend()
{
    if(SelectConfigProfile("suspend_profile"))
    {
        plugin_manager->UnloadPlugins();
        on_ButtonLoadProfile_clicked();
    }
}

void OpenRGBDialog::OnResume()
{
    if(SelectConfigProfile("resume_profile"))
    {
        on_ButtonLoadProfile_clicked();
    }
    plugin_manager->LoadPlugins();
}

void OpenRGBDialog::on_Exit()
{
    /*-----------------------------------------------------*\
    | This is the exit from the tray icon NOT the main exit |
    | button (top right on Windows)                         |
    |                                                       |
    | The hide is important, otherwise it won't close when  |
    | minimize on close is enabled                          |
    \*-----------------------------------------------------*/
    this->hide();
    trayIcon->hide();
    close();
}

void OpenRGBDialog::on_LightsOff()
{
    on_SetAllDevices(0x00, 0x00, 0x00);
}

void OpenRGBDialog::on_QuickRed()
{
    on_SetAllDevices(0xFF, 0x00, 0x00);
}

void OpenRGBDialog::on_QuickYellow()
{
    on_SetAllDevices(0xFF, 0xFF, 0x00);
}

void OpenRGBDialog::on_QuickGreen()
{
    on_SetAllDevices(0x00, 0xFF, 0x00);
}

void OpenRGBDialog::on_QuickCyan()
{
    on_SetAllDevices(0x00, 0xFF, 0xFF);
}

void OpenRGBDialog::on_QuickBlue()
{
    on_SetAllDevices(0x00, 0x00, 0xFF);
}

void OpenRGBDialog::on_QuickMagenta()
{
    on_SetAllDevices(0xFF, 0x00, 0xFF);
}

void OpenRGBDialog::on_QuickWhite()
{
    on_SetAllDevices(0xFF, 0xFF, 0xFF);
}

void OpenRGBDialog::onDeviceListUpdated()
{
    UpdateDevicesList();
}

void OpenRGBDialog::onDetectionProgressUpdated()
{
    ui->DetectionProgressBar->setValue(ResourceManager::get()->GetDetectionPercent());
    ui->DetectionProgressBar->setFormat(QString::fromStdString(ResourceManager::get()->GetDetectionString()));

    if(ResourceManager::get()->GetDetectionPercent() == 100)
    {
        SetDetectionViewState(false);
    }
    else
    {
        SetDetectionViewState(true);
    }
}

void OpenRGBDialog::onDetectionStarted()
{
    /*-----------------------------------------------------*\
    | Hide devices view on rescan so it stops handling      |
    | paint events.                                         |
    | Memorize previous value of device_view_showing and    |
    | restore it.                                           |
    \*-----------------------------------------------------*/
    bool device_view_showing_prev = device_view_showing;

    HideLEDView();

    device_view_showing = device_view_showing_prev;

    /*-----------------------------------------------------*\
    | Show the detection progress bar.                      |
    \*-----------------------------------------------------*/
    SetDetectionViewState(true);
}

void OpenRGBDialog::onDetectionEnded()
{
    /*-----------------------------------------------------*\
    | Detect unconfigured zones and prompt for resizing     |
    \*-----------------------------------------------------*/
    OpenRGBZonesBulkResizer::RunChecks(this);

    /*-----------------------------------------------------*\
    | Load plugins after the first detection (ONLY the      |
    | first)                                                |
    \*-----------------------------------------------------*/
    if(!plugins_loaded)
    {
        plugin_manager->ScanAndLoadPlugins();
        plugins_loaded = true;
        PluginsPage->RefreshList();
    }

    if(device_view_showing)
    {
        ShowLEDView();
    }
}

void OpenRGBDialog::on_SetAllDevices(unsigned char red, unsigned char green, unsigned char blue)
{
    for(int device = 0; device < ui->DevicesTabBar->count(); device++)
    {
        qobject_cast<OpenRGBDevicePage *>(ui->DevicesTabBar->widget(device))->SetCustomMode(red, green, blue);
    }
}

void OpenRGBDialog::on_SaveSizeProfile()
{
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    if(profile_manager != NULL)
    {
        /*-------------------------------------------------*\
        | Save the profile                                  |
        \*-------------------------------------------------*/
        profile_manager->SaveProfile("sizes", true);
    }
}

void OpenRGBDialog::on_ShowHide()
{
    if(isHidden())
    {
#ifdef __APPLE__
        MacUtils::ToggleApplicationDocklessState(true);
#endif
        show();
        if(isMinimized())
        {
            bool maximize = isMaximized();
            showNormal();
            if(maximize)
            {
                showMaximized();
            }
        }
    }
    else
    {
#ifdef __APPLE__
        MacUtils::ToggleApplicationDocklessState(false);
#endif
        hide();
    }
}

void OpenRGBDialog::onShowDialogMessage()
{
    std::size_t hash = std::hash<std::string>{}(dialog_message.toStdString());

    /*-----------------------------------------------------*\
    | Load the LogManager settings and check if the hash of |
    | this message is in the no-show list                   |
    \*-----------------------------------------------------*/
    SettingsManager*    settings_manager    = ResourceManager::get()->GetSettingsManager();
    std::string         log_manager_string  = "LogManager";
    json                log_manager_settings;

    log_manager_settings = settings_manager->GetSettings(log_manager_string);

    /*-----------------------------------------------------*\
    | If in the no-show list, clear the message string and  |
    | return without displaying message box                 |
    \*-----------------------------------------------------*/
    if(log_manager_settings.contains("dialog_no_show_hashes"))
    {
        for(unsigned int list_idx = 0; list_idx < log_manager_settings["dialog_no_show_hashes"].size(); list_idx++)
        {
            if(log_manager_settings["dialog_no_show_hashes"][list_idx] == hash)
            {
                dialog_message.clear();
                return;
            }
        }
    }

    QMessageBox box;

    box.setInformativeText(QCoreApplication::translate("ResourceManager", dialog_message.toUtf8()));

    QCheckBox* CheckBox_DontShowAgain = new QCheckBox("Don't show this message again");

    DontShowAgain = false;

    #if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
        QObject::connect(CheckBox_DontShowAgain, &QCheckBox::checkStateChanged, [this](Qt::CheckState state)
    #else
        QObject::connect(CheckBox_DontShowAgain, &QCheckBox::stateChanged, [this](int state)
    #endif
    {
        if(static_cast<Qt::CheckState>(state) == Qt::CheckState::Checked)
        {
            this->DontShowAgain = true;
        }
    });

    box.setCheckBox(CheckBox_DontShowAgain);
    box.setTextFormat(Qt::RichText);
    box.setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextBrowserInteraction);

    box.exec();

    if(DontShowAgain)
    {
        /*-------------------------------------------------*\
        | Add hash of dialog text to no-show list in        |
        | LogManager settings                               |
        \*-------------------------------------------------*/
        log_manager_settings["dialog_no_show_hashes"].push_back(hash);

        settings_manager->SetSettings(log_manager_string, log_manager_settings);
        settings_manager->SaveSettings();
    }

    DontShowAgain = false;

    dialog_message.clear();
}

void OpenRGBDialog::on_ReShow(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
    {
        if (isHidden())
        {
            show();
            if(isMinimized())
            {
                bool maximize = isMaximized();
                showNormal();
                if(maximize)
                {
                    showMaximized();
                }
            }
        }
    }
}

void OpenRGBDialog::on_ProfileSelected()
{
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    if(profile_manager != NULL)
    {
        /*-------------------------------------------------*\
        | Get the profile filename from the selected object |
        \*-------------------------------------------------*/
        std::string profile_name = QObject::sender()->objectName().toStdString();

        /*-------------------------------------------------*\
        | Load the profile                                  |
        \*-------------------------------------------------*/
        if(profile_manager->LoadProfile(profile_name))
        {
            for(int device = 0; device < ui->DevicesTabBar->count(); device++)
            {
                qobject_cast<OpenRGBDevicePage *>(ui->DevicesTabBar->widget(device))->UpdateDevice();
            }
        }

        ui->ProfileBox->setCurrentIndex(ui->ProfileBox->findText(QString::fromStdString(profile_name)));
    }
}

void OpenRGBDialog::on_ButtonLoadProfile_clicked()
{
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    if(profile_manager != NULL)
    {
        /*-------------------------------------------------*\
        | Get the profile filename from the profiles list   |
        \*-------------------------------------------------*/
        std::string profile_name = ui->ProfileBox->currentText().toStdString();

        /*-------------------------------------------------*\
        | Load the profile                                  |
        \*-------------------------------------------------*/
        if(profile_manager->LoadProfile(profile_name))
        {
            for(int device = 0; device < ui->DevicesTabBar->count(); device++)
            {
                qobject_cast<OpenRGBDevicePage *>(ui->DevicesTabBar->widget(device))->UpdateDevice();
            }
        }
    }
}

void OpenRGBDialog::on_ButtonDeleteProfile_clicked()
{
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    if(profile_manager != NULL)
    {
        /*-------------------------------------------------*\
        | Get the profile filename from the profiles list   |
        \*-------------------------------------------------*/
        std::string profile_name = ui->ProfileBox->currentText().toStdString();

        /*-------------------------------------------------*\
        | Confirm we want to delete the profile             |
        \*-------------------------------------------------*/
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Delete Profile"), tr("Do you really want to delete this profile?"), QMessageBox::Yes|QMessageBox::No);

        /*-------------------------------------------------*\
        | Load the profile                                  |
        \*-------------------------------------------------*/
        if(reply == QMessageBox::Yes)
        {
            profile_manager->DeleteProfile(profile_name);

            UpdateProfileList();
        }
    }
}

void OpenRGBDialog::on_ButtonToggleDeviceView_clicked()
{
    if(device_view_showing)
    {
        HideLEDView();
    }
    else
    {
        ShowLEDView();
    }
}

void OpenRGBDialog::ShowLEDView()
{
    for(int device = 0; device < ui->DevicesTabBar->count(); device++)
    {
        OpenRGBDevicePage* device_page = qobject_cast<OpenRGBDevicePage *>(ui->DevicesTabBar->widget(device));

        /*-------------------------------------------------*\
        | Check the cast to make sure it is a device and    |
        | not plugin                                        |
        \*-------------------------------------------------*/
        if(device_page)
        {
            device_page->ShowDeviceView();
        }
    }
    device_view_showing = true;
}

void OpenRGBDialog::HideLEDView()
{
    for(int device = 0; device < ui->DevicesTabBar->count(); device++)
    {
        qobject_cast<OpenRGBDevicePage *>(ui->DevicesTabBar->widget(device))->HideDeviceView();
    }
    device_view_showing = false;
}


void OpenRGBDialog::on_ButtonStopDetection_clicked()
{
    /*-----------------------------------------------------*\
    | Notify the detection thread that it has to die        |
    \*-----------------------------------------------------*/
    ResourceManager::get()->StopDeviceDetection();

    /*-----------------------------------------------------*\
    | Pretend we're done already by hiding the progress bar |
    \*-----------------------------------------------------*/
    SetDetectionViewState(false);
}

void OpenRGBDialog::SetDetectionViewState(bool detection_showing)
{
    if(detection_showing)
    {
        /*-------------------------------------------------*\
        | Show the detection progress and hide the normal   |
        | buttons                                           |
        \*-------------------------------------------------*/
        ui->ButtonToggleDeviceView->setVisible(false);
        ui->ButtonRescan->setVisible(false);
        ui->ButtonLoadProfile->setVisible(false);
        ui->ButtonSaveProfile->setVisible(false);
        ui->ButtonDeleteProfile->setVisible(false);
        ui->ProfileBox->setVisible(false);

        ui->DetectionProgressBar->setVisible(true);
        ui->DetectionProgressLabel->setVisible(true);
        ui->ButtonStopDetection->setVisible(true);
    }
    else
    {
        /*-------------------------------------------------*\
        | Hide the detection progress and show the normal   |
        | buttons                                           |
        \*-------------------------------------------------*/
        ui->DetectionProgressBar->setVisible(false);
        ui->DetectionProgressLabel->setVisible(false);
        ui->ButtonStopDetection->setVisible(false);

        ui->ButtonToggleDeviceView->setVisible(true);
        ui->ButtonRescan->setVisible(true);
        ui->ButtonLoadProfile->setVisible(true);
        ui->ButtonSaveProfile->setVisible(true);
        ui->ButtonDeleteProfile->setVisible(true);
        ui->ProfileBox->setVisible(true);
    }
}

void OpenRGBDialog::SetTrayIcon(bool tray_icon)
{
    if(tray_icon)
    {
        trayIcon->setIcon(QIcon(":OpenRGBGreyscale.png"));
    }
    else
    {
        trayIcon->setIcon(QIcon(":org.openrgb.OpenRGB.png"));
    }
}

void OpenRGBDialog::SaveProfile()
{
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    if(profile_manager != NULL)
    {
        /*-------------------------------------------------*\
        | Get the profile filename from the profiles list   |
        \*-------------------------------------------------*/
        std::string filename = ui->ProfileBox->currentText().toStdString();

        /*-------------------------------------------------*\
        | Save the profile                                  |
        \*-------------------------------------------------*/
        profile_manager->SaveProfile(filename);
    }
}

void OpenRGBDialog::SaveProfileAs()
{
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    if(profile_manager != NULL)
    {
        OpenRGBProfileSaveDialog dialog;

        /*-------------------------------------------------*\
        | Open Profile Name Dialog                          |
        \*-------------------------------------------------*/
        std::string profile_name = dialog.show();

        if(!profile_name.empty())
        {
            /*---------------------------------------------*\
            | Extension .orp - OpenRgb Profile              |
            \*---------------------------------------------*/
            std::string filename = profile_name;

            /*---------------------------------------------*\
            | Save the profile                              |
            \*---------------------------------------------*/
            if(profile_manager->SaveProfile(filename))
            {
                UpdateProfileList();

                ui->ProfileBox->setCurrentIndex(ui->ProfileBox->findText(QString::fromStdString(profile_name)));
            }
        }
    }
}

void OpenRGBDialog::on_ButtonRescan_clicked()
{
    /*-----------------------------------------------------*\
    | Rescan devices in ResourceManager                     |
    \*-----------------------------------------------------*/
    ResourceManager::get()->RescanDevices();
}

void OpenRGBDialog::on_ActionSaveProfile_triggered()
{
    if(ui->ProfileBox->currentIndex() >= 0)
    {
        SaveProfile();
    }
    else
    {
        SaveProfileAs();
    }
}

void OpenRGBDialog::on_ActionSaveProfileAs_triggered()
{
    SaveProfileAs();
}


void OpenRGBDialog::on_InformationTabBar_currentChanged(int tab_idx)
{
    TogglePluginsVisibility(tab_idx, ui->InformationTabBar);
}

void OpenRGBDialog::on_DevicesTabBar_currentChanged(int tab_idx)
{
    TogglePluginsVisibility(tab_idx, ui->DevicesTabBar);
}

void OpenRGBDialog::on_MainTabBar_currentChanged(int tab_idx)
{
    TogglePluginsVisibility(tab_idx, ui->MainTabBar);
}

void OpenRGBDialog::on_SettingsTabBar_currentChanged(int tab_idx)
{
    TogglePluginsVisibility(tab_idx, ui->SettingsTabBar);
}

void OpenRGBDialog::TogglePluginsVisibility(int tab_idx, QTabWidget* tabBar)
{
    /*-----------------------------------------------------*\
    | Hide all plugins                                      |
    \*-----------------------------------------------------*/
    for(int i = 0; i < (tabBar->count()); i++)
    {
        QWidget* tab = tabBar->widget(i);

        /*-------------------------------------------------*\
        | Dynamic cast is essential in this check to ensure |
        | the tab is actually a plugin container            |
        \*-------------------------------------------------*/
        if((i != tab_idx) && (dynamic_cast<OpenRGBPluginContainer*>(tab) != nullptr))
        {
            ((OpenRGBPluginContainer*) tab)->Hide();
            ui->MainButtonsFrame->setVisible(true);
        }
    }

    /*-----------------------------------------------------*\
    | Show plugin if needed                                 |
    \*-----------------------------------------------------*/
    QWidget* tab = tabBar->widget(tab_idx);

    /*-----------------------------------------------------*\
    | Dynamic cast is essential in this check to ensure the |
    | tab is actually a plugin container                    |
    \*-----------------------------------------------------*/
    if(dynamic_cast<OpenRGBPluginContainer*>(tab) != nullptr)
    {
        ((OpenRGBPluginContainer*) tab)->Show();
        ui->MainButtonsFrame->setVisible(false);
    }
}

void OpenRGBDialog::AddConsolePage()
{
    OpenRGBConsolePage* page = new OpenRGBConsolePage();

    ui->InformationTabBar->addTab(page, "");

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* ConsoleTabLabel = new TabLabel(OpenRGBFont::terminal, tr("Log Console"), (char *)"Log Console", (char *)context);

    ui->InformationTabBar->tabBar()->setTabButton(ui->InformationTabBar->tabBar()->count() - 1, QTabBar::LeftSide, ConsoleTabLabel);
}
