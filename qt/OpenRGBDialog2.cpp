#include "OpenRGBDialog2.h"
#include "PluginManager.h"
#include "OpenRGBDevicePage.h"
#include "OpenRGBDeviceInfoPage.h"
#include "OpenRGBServerInfoPage.h"
#include "OpenRGBConsolePage.h"
#include "OpenRGBPluginContainer.h"
#include "OpenRGBProfileSaveDialog.h"
#include "ResourceManager.h"
#include "TabLabel.h"
#include "OpenRGBZonesBulkResizer.h"
#include <QLabel>
#include <QTabBar>
#include <QMessageBox>
#include <QCloseEvent>
#include <QStyleFactory>

#ifdef _WIN32
#include <QSettings>
#endif

using namespace Ui;

static QString GetIconString(device_type type, bool dark)
{
    /*-----------------------------------------------------*\
    | Return the icon filename string for the given device  |
    | type value                                            |
    \*-----------------------------------------------------*/
    QString filename;
    switch(type)
    {
    case DEVICE_TYPE_MOTHERBOARD:
        filename = "motherboard";
        break;
    case DEVICE_TYPE_DRAM:
        filename = "dram";
        break;
    case DEVICE_TYPE_GPU:
        filename = "gpu";
        break;
    case DEVICE_TYPE_COOLER:
        filename = "fan";
        break;
    case DEVICE_TYPE_LEDSTRIP:
        filename = "ledstrip";
        break;
    case DEVICE_TYPE_KEYBOARD:
        filename = "keyboard";
        break;
    case DEVICE_TYPE_MOUSE:
        filename = "mouse";
        break;
    case DEVICE_TYPE_MOUSEMAT:
        filename = "mousemat";
        break;
    case DEVICE_TYPE_HEADSET:
        filename = "headset";
        break;
    case DEVICE_TYPE_HEADSET_STAND:
        filename = "headsetstand";
        break;
    case DEVICE_TYPE_GAMEPAD:
        filename = "gamepad";
        break;
    case DEVICE_TYPE_LIGHT:
        filename = "light";
        break;
    case DEVICE_TYPE_SPEAKER:
        filename = "speaker";
        break;
    case DEVICE_TYPE_VIRTUAL:
        filename = "virtual";
        break;
    default:
        filename = "unknown";
        break;
    }
    if(dark)
    {
        filename += "_dark";
    }
    filename += ".png";
    return filename;
}

static void UpdateDeviceListCallback(void * this_ptr)
{
    OpenRGBDialog2 * this_obj = (OpenRGBDialog2 *)this_ptr;

    QMetaObject::invokeMethod(this_obj, "onDeviceListUpdated", Qt::QueuedConnection);
}

static void UpdateDetectionProgressCallback(void * this_ptr)
{
    OpenRGBDialog2 * this_obj = (OpenRGBDialog2 *)this_ptr;

    QMetaObject::invokeMethod(this_obj, "onDetectionProgressUpdated", Qt::QueuedConnection);
}

static void CreatePluginCallback(void * this_ptr, OpenRGBPluginEntry* plugin)
{
    OpenRGBDialog2 * this_obj = (OpenRGBDialog2 *)this_ptr;

    this_obj->AddPlugin(plugin);
}

static void DeletePluginCallback(void * this_ptr, OpenRGBPluginEntry* plugin)
{
    OpenRGBDialog2 * this_obj = (OpenRGBDialog2 *)this_ptr;

    this_obj->RemovePlugin(plugin);
}

static void DetectionEndedCallback(void * this_ptr)
{
    OpenRGBDialog2 * this_obj = (OpenRGBDialog2 *)this_ptr;

    QMetaObject::invokeMethod(this_obj, "onDetectionEnded", Qt::QueuedConnection);
}

bool OpenRGBDialog2::IsDarkTheme()
    {
#ifdef _WIN32
    /*-------------------------------------------------*\
    | Windows dark theme settings                       |
    \*-------------------------------------------------*/
    json            theme_settings;

    /*-------------------------------------------------*\
    | Get prefered theme from settings manager          |
    \*-------------------------------------------------*/
    theme_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Theme");

    /*-------------------------------------------------*\
    | Read the theme key and adjust accordingly         |
    \*-------------------------------------------------*/
    std::string current_theme = "light";

    if(theme_settings.contains("theme"))
    {
        current_theme = theme_settings["theme"];
    }

    if((current_theme == "auto") || (current_theme == "dark"))
    {
        if(current_theme == "auto")
        {
            QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", QSettings::NativeFormat);

            if(settings.value("AppsUseLightTheme") != 0)
            {
                return false;
            }
            else if(settings.value("AppsUseLightTheme") == 0)
            {
                return true;
            }
        }
        else if(current_theme == "dark")
        {
            return true;
        }
    }
    return false;

#else
    if(QPalette().window().color().value() < 127)
    {
        return true;
    }
#endif

    return false;
}

bool OpenRGBDialog2::IsMinimizeOnClose()
{
    json ui_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("UserInterface");

    if(ui_settings.contains("minimize_on_close"))
    {
        return ui_settings["minimize_on_close"];
    }

    return false;
}

OpenRGBDialog2::OpenRGBDialog2(QWidget *parent) : QMainWindow(parent), ui(new OpenRGBDialog2Ui)
{
    ui->setupUi(this);

    /*-----------------------------------------------------*\
    | Set window icon                                       |
    \*-----------------------------------------------------*/
    QIcon logo(":OpenRGB.png");
    setWindowIcon(logo);

    /*-----------------------------------------------------*\
    | Set window geometry from config (if available)        |
    \*-----------------------------------------------------*/
    SettingsManager*    settings_manager    = ResourceManager::get()->GetSettingsManager();
    std::string         ui_string           = "UserInterface";
    json                ui_settings;

    ui_settings = settings_manager->GetSettings(ui_string);

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

        settings_manager->SetSettings(ui_string, ui_settings);
        settings_manager->SaveSettings();
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

        /*-----------------------------------------------------*\
        | x and y can be set independent of width and height    |
        | QT attempts to clamp these values in case the user    |
        | enters invalid numbers                                |
        \*-----------------------------------------------------*/
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
    | Register detection progress callback with resource    |
    | manager                                               |
    \*-----------------------------------------------------*/
    ResourceManager::get()->RegisterDetectionProgressCallback(UpdateDetectionProgressCallback, this);
    ResourceManager::get()->RegisterDeviceListChangeCallback(UpdateDeviceListCallback, this);
    ResourceManager::get()->RegisterDetectionEndCallback(DetectionEndedCallback, this);

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

    QAction* actionShowHide = new QAction("Show/Hide", this);
    connect(actionShowHide, SIGNAL(triggered()), this, SLOT(on_ShowHide()));
    trayIconMenu->addAction(actionShowHide);

    profileMenu = new QMenu("Profiles", this);

    trayIconMenu->addMenu(profileMenu);

    QMenu* quickColorsMenu = new QMenu("Quick Colors", this);

    QAction* actionQuickRed = new QAction("Red", this);
    connect(actionQuickRed, SIGNAL(triggered()), this, SLOT(on_QuickRed()));
    quickColorsMenu->addAction(actionQuickRed);

    QAction* actionQuickYellow = new QAction("Yellow", this);
    connect(actionQuickYellow, SIGNAL(triggered()), this, SLOT(on_QuickYellow()));
    quickColorsMenu->addAction(actionQuickYellow);

    QAction* actionQuickGreen = new QAction("Green", this);
    connect(actionQuickGreen, SIGNAL(triggered()), this, SLOT(on_QuickGreen()));
    quickColorsMenu->addAction(actionQuickGreen);

    QAction* actionQuickCyan = new QAction("Cyan", this);
    connect(actionQuickCyan, SIGNAL(triggered()), this, SLOT(on_QuickCyan()));
    quickColorsMenu->addAction(actionQuickCyan);

    QAction* actionQuickBlue = new QAction("Blue", this);
    connect(actionQuickBlue, SIGNAL(triggered()), this, SLOT(on_QuickBlue()));
    quickColorsMenu->addAction(actionQuickBlue);

    QAction* actionQuickMagenta = new QAction("Magenta", this);
    connect(actionQuickMagenta, SIGNAL(triggered()), this, SLOT(on_QuickMagenta()));
    quickColorsMenu->addAction(actionQuickMagenta);

    QAction* actionQuickWhite = new QAction("White", this);
    connect(actionQuickWhite, SIGNAL(triggered()), this, SLOT(on_QuickWhite()));
    quickColorsMenu->addAction(actionQuickWhite);

    trayIconMenu->addMenu(quickColorsMenu);

    QAction* actionLightsOff = new QAction("Lights Off", this);
    actionLightsOff->setObjectName("ActionLightsOff");
    connect(actionLightsOff, SIGNAL(triggered()), this, SLOT(on_LightsOff()));
    trayIconMenu->addAction(actionLightsOff);

    actionExit = new QAction( "Exit", this );
    connect( actionExit, SIGNAL( triggered() ), this, SLOT( on_Exit() ));
    trayIconMenu->addAction(actionExit);

    /*-------------------------------------------------*\
    | If tray minimize flag isn't in the config, set    |
    | default value to false                            |
    \*-------------------------------------------------*/
    if(!ui_settings.contains("minimize_on_close"))
    {
        ui_settings["minimize_on_close"] = false;

        settings_manager->SetSettings(ui_string, ui_settings);
        settings_manager->SaveSettings();
    }

    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_ReShow(QSystemTrayIcon::ActivationReason)));

    trayIcon->setIcon(logo);
    trayIcon->setToolTip("OpenRGB");
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();

#ifdef _WIN32
    /*-------------------------------------------------*\
    | Apply dark theme on Windows if configured         |
    \*-------------------------------------------------*/
    if(IsDarkTheme())
    {
        QPalette pal = palette();
        pal.setColor(QPalette::WindowText, Qt::white);
        QApplication::setPalette(pal);
        QFile darkTheme(":/windows_dark.qss");
        darkTheme.open(QFile::ReadOnly);
        setStyleSheet(darkTheme.readAll());
    }
#endif

#ifdef __APPLE__
    /*-------------------------------------------------*\
    | Apply Qt Fusion theme on MacOS, as the MacOS      |
    | default theme does not handle vertical tabs well  |
    \*-------------------------------------------------*/
    QApplication::setStyle(QStyleFactory::create("Fusion"));
#endif

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
    plugin_manager = new PluginManager(IsDarkTheme());
    plugin_manager->RegisterAddPluginCallback(&CreatePluginCallback, this);
    plugin_manager->RegisterRemovePluginCallback(&DeletePluginCallback, this);
    plugin_manager->ScanAndLoadPlugins();

    /*-----------------------------------------------------*\
    | Add the Plugins page                                  |
    \*-----------------------------------------------------*/
    AddPluginsPage();

    /*-----------------------------------------------------*\
    | Add the E1.31 settings page                           |
    \*-----------------------------------------------------*/
    AddE131SettingsPage();

    /*-----------------------------------------------------*\
    | Add the Serial settings page                          |
    \*-----------------------------------------------------*/
    AddSerialSettingsPage();

    /*-----------------------------------------------------*\
    | Add the QMK OpenRGB Protocol settings page            |
    \*-----------------------------------------------------*/
    AddQMKORGBSettingsPage();

    /*-----------------------------------------------------*\
    | Add the Philips Hue settings page                     |
    \*-----------------------------------------------------*/
    AddPhilipsHueSettingsPage();

    /*-----------------------------------------------------*\
    | Add the Philips Wiz settings page                     |
    \*-----------------------------------------------------*/
    AddPhilipsWizSettingsPage();

    /*-----------------------------------------------------*\
    | Add the Yeelight settings page                        |
    \*-----------------------------------------------------*/
    AddYeelightSettingsPage();

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

}

OpenRGBDialog2::~OpenRGBDialog2()
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

    delete ui;
}

void OpenRGBDialog2::closeEvent(QCloseEvent *event)
{
    ResourceManager::get()->WaitForDeviceDetection();

    if (IsMinimizeOnClose() && !this->isHidden())
    {
        hide();
        event->ignore();
    }
    else
    {
        plugin_manager->UnloadPlugins();
        event->accept();
    }
}

void OpenRGBDialog2::AddPluginsPage()
{
    /*-----------------------------------------------------*\
    | Create the Plugins page                               |
    \*-----------------------------------------------------*/
    PluginsPage = new OpenRGBPluginsPage(plugin_manager);

    ui->SettingsTabBar->addTab(PluginsPage, "");

    QString PluginsLabelString;

    if(IsDarkTheme())
    {
        PluginsLabelString = "plugin_dark.png";
    }
    else
    {
        PluginsLabelString = "plugin.png";
    }

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* PluginTabLabel = new TabLabel(PluginsLabelString, "Plugins");

    ui->SettingsTabBar->tabBar()->setTabButton(ui->SettingsTabBar->tabBar()->count() - 1, QTabBar::LeftSide, PluginTabLabel);
}

void OpenRGBDialog2::AddSoftwareInfoPage()
{
    /*-----------------------------------------------------*\
    | Create the Software Information page                  |
    \*-----------------------------------------------------*/
    SoftInfoPage = new OpenRGBSoftwareInfoPage();

    ui->InformationTabBar->addTab(SoftInfoPage, "");

    QString SoftwareLabelString;

    if(IsDarkTheme())
    {
        SoftwareLabelString = "software_dark.png";
    }
    else
    {
        SoftwareLabelString = "software.png";
    }

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* SoftwareTabLabel = new TabLabel(SoftwareLabelString, "Software");

    ui->InformationTabBar->tabBar()->setTabButton(ui->InformationTabBar->tabBar()->count() - 1, QTabBar::LeftSide, SoftwareTabLabel);
}

void OpenRGBDialog2::AddSupportedDevicesPage()
{
    /*-----------------------------------------------------*\
    | Create the Supported Devices page                     |
    \*-----------------------------------------------------*/
    SupportedPage = new OpenRGBSupportedDevicesPage();

    ui->SettingsTabBar->addTab(SupportedPage, "");

    QString SettingsLabelString;

    if(IsDarkTheme())
    {
        SettingsLabelString = "software_dark.png";
    }
    else
    {
        SettingsLabelString = "software.png";
    }

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* SupportedTabLabel = new TabLabel(SettingsLabelString, "Supported Devices");

    ui->SettingsTabBar->tabBar()->setTabButton(ui->SettingsTabBar->tabBar()->count() - 1, QTabBar::LeftSide, SupportedTabLabel);
}


void OpenRGBDialog2::AddSettingsPage()
{
    /*-----------------------------------------------------*\
    | Create the Settings page                              |
    \*-----------------------------------------------------*/
    SettingsPage = new OpenRGBSettingsPage();

    ui->SettingsTabBar->addTab(SettingsPage, "");

    QString SettingsLabelString;

    if(IsDarkTheme())
    {
        SettingsLabelString = "settings_dark.png";
    }
    else
    {
        SettingsLabelString = "settings.png";
    }

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* SettingsTabLabel = new TabLabel(SettingsLabelString, "General Settings");

    ui->SettingsTabBar->tabBar()->setTabButton(ui->SettingsTabBar->tabBar()->count() - 1, QTabBar::LeftSide, SettingsTabLabel);
}

void OpenRGBDialog2::AddE131SettingsPage()
{
    /*-----------------------------------------------------*\
    | Create the Settings page                              |
    \*-----------------------------------------------------*/
    E131SettingsPage = new OpenRGBE131SettingsPage();

    ui->SettingsTabBar->addTab(E131SettingsPage, "");

    QString SettingsLabelString;

    if(IsDarkTheme())
    {
        SettingsLabelString = "wireless_dark.png";
    }
    else
    {
        SettingsLabelString = "wireless.png";
    }

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* SettingsTabLabel = new TabLabel(SettingsLabelString, "E1.31 Devices");

    ui->SettingsTabBar->tabBar()->setTabButton(ui->SettingsTabBar->tabBar()->count() - 1, QTabBar::LeftSide, SettingsTabLabel);
}

void OpenRGBDialog2::AddPhilipsHueSettingsPage()
{
    /*-----------------------------------------------------*\
    | Create the Settings page                              |
    \*-----------------------------------------------------*/
    PhilipsHueSettingsPage = new OpenRGBPhilipsHueSettingsPage();

    ui->SettingsTabBar->addTab(PhilipsHueSettingsPage, "");

    QString SettingsLabelString;

    if(IsDarkTheme())
    {
        SettingsLabelString = "light_dark.png";
    }
    else
    {
        SettingsLabelString = "light.png";
    }

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* SettingsTabLabel = new TabLabel(SettingsLabelString, "Philips Hue Devices");

    ui->SettingsTabBar->tabBar()->setTabButton(ui->SettingsTabBar->tabBar()->count() - 1, QTabBar::LeftSide, SettingsTabLabel);
}

void OpenRGBDialog2::AddPhilipsWizSettingsPage()
{
    /*-----------------------------------------------------*\
    | Create the Settings page                              |
    \*-----------------------------------------------------*/
    PhilipsWizSettingsPage = new OpenRGBPhilipsWizSettingsPage();

    ui->SettingsTabBar->addTab(PhilipsWizSettingsPage, "");

    QString SettingsLabelString;

    if(IsDarkTheme())
    {
        SettingsLabelString = "light_dark.png";
    }
    else
    {
        SettingsLabelString = "light.png";
    }

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* SettingsTabLabel = new TabLabel(SettingsLabelString, "Philips Wiz Devices");

    ui->SettingsTabBar->tabBar()->setTabButton(ui->SettingsTabBar->tabBar()->count() - 1, QTabBar::LeftSide, SettingsTabLabel);
}

void OpenRGBDialog2::AddQMKORGBSettingsPage()
{
    /*-----------------------------------------------------*\
    | Create the Settings page                              |
    \*-----------------------------------------------------*/
    QMKORGBSettingsPage = new OpenRGBQMKORGBSettingsPage();

    ui->SettingsTabBar->addTab(QMKORGBSettingsPage, "");

    QString SettingsLabelString;

    if(IsDarkTheme())
    {
        SettingsLabelString = "keyboard_dark.png";
    }
    else
    {
        SettingsLabelString = "keyboard.png";
    }

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* SettingsTabLabel = new TabLabel(SettingsLabelString, "OpenRGB QMK Protocol");

    ui->SettingsTabBar->tabBar()->setTabButton(ui->SettingsTabBar->tabBar()->count() - 1, QTabBar::LeftSide, SettingsTabLabel);
}

void OpenRGBDialog2::AddSerialSettingsPage()
{
    /*-----------------------------------------------------*\
    | Create the Settings page                              |
    \*-----------------------------------------------------*/
    SerialSettingsPage = new OpenRGBSerialSettingsPage();

    ui->SettingsTabBar->addTab(SerialSettingsPage, "");

    QString SettingsLabelString;

    if(IsDarkTheme())
    {
        SettingsLabelString = "serial_dark.png";
    }
    else
    {
        SettingsLabelString = "serial.png";
    }

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* SettingsTabLabel = new TabLabel(SettingsLabelString, "Serial Devices");

    ui->SettingsTabBar->tabBar()->setTabButton(ui->SettingsTabBar->tabBar()->count() - 1, QTabBar::LeftSide, SettingsTabLabel);
}

void OpenRGBDialog2::AddYeelightSettingsPage()
{
    /*-----------------------------------------------------*\
    | Create the Settings page                              |
    \*-----------------------------------------------------*/
    YeelightSettingsPage = new OpenRGBYeelightSettingsPage();

    ui->SettingsTabBar->addTab(YeelightSettingsPage, "");

    QString SettingsLabelString;

    if(IsDarkTheme())
    {
        SettingsLabelString = "light_dark.png";
    }
    else
    {
        SettingsLabelString = "light.png";
    }

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* SettingsTabLabel = new TabLabel(SettingsLabelString, "Yeelight Devices");

    ui->SettingsTabBar->tabBar()->setTabButton(ui->SettingsTabBar->tabBar()->count() - 1, QTabBar::LeftSide, SettingsTabLabel);
}

void OpenRGBDialog2::AddPlugin(OpenRGBPluginEntry* plugin)
{
    /*-----------------------------------------------------*\
    | Create Label for the Tab                              |
    \*-----------------------------------------------------*/
    QLabel* PluginTabLabel = new QLabel;

    /*-----------------------------------------------------*\
    | If the plugin has custom information, use it,         |
    | otherwise generate it                                 |
    \*-----------------------------------------------------*/
    QString PluginLabelString;

    if(IsDarkTheme())
    {
        PluginLabelString = "plugin_dark.png";
    }
    else
    {
        PluginLabelString = "plugin.png";
    }

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    PluginTabLabel = (QLabel*)new TabLabel(PluginLabelString, QString::fromStdString(plugin->info.Label));

    /*-----------------------------------------------------*\
    | Place plugin as its own top level tab                 |
    \*-----------------------------------------------------*/
    if(plugin->info.Location == OPENRGB_PLUGIN_LOCATION_TOP)
    {
        QWidget* NewPluginTab = plugin->plugin->GetWidget();

        plugin->widget = NewPluginTab;

        OpenRGBPluginContainer* NewPluginContainer = new OpenRGBPluginContainer(NewPluginTab);

        ui->MainTabBar->addTab(NewPluginContainer,QString().fromStdString(plugin->info.Label));
    }
    /*-----------------------------------------------------*\
    | Place plugin in the Devices tab                       |
    \*-----------------------------------------------------*/
    else if(plugin->info.Location == OPENRGB_PLUGIN_LOCATION_DEVICES)
    {
        QWidget* NewPluginTab = plugin->plugin->GetWidget();

        plugin->widget = NewPluginTab;

        OpenRGBPluginContainer* NewPluginContainer = new OpenRGBPluginContainer(NewPluginTab);

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

        OpenRGBPluginContainer* NewPluginContainer = new OpenRGBPluginContainer(NewPluginTab);

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

        OpenRGBPluginContainer* NewPluginContainer = new OpenRGBPluginContainer(NewPluginTab);

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
        NewTrayMenu->setParent(trayIconMenu);

        trayIconMenu->insertMenu(actionExit, NewTrayMenu);
    }
}

void OpenRGBDialog2::RemovePlugin(OpenRGBPluginEntry* plugin)
{
    /*-----------------------------------------------------*\
    | Place plugin as its own top level tab                 |
    \*-----------------------------------------------------*/
    if(plugin->info.Location == OPENRGB_PLUGIN_LOCATION_TOP)
    {
        for(int tab_idx = 0; tab_idx < ui->MainTabBar->count(); tab_idx++)
        {
            if(dynamic_cast<OpenRGBPluginContainer*>(ui->MainTabBar->widget(tab_idx)) != nullptr)
            {
                if(dynamic_cast<OpenRGBPluginContainer*>(ui->MainTabBar->widget(tab_idx))->plugin_widget == plugin->widget)
                {
                    ui->MainTabBar->removeTab(tab_idx);
                    delete plugin->widget;
                }
            }
        }
    }
    /*-----------------------------------------------------*\
    | Place plugin in the Devices tab                       |
    \*-----------------------------------------------------*/
    else if(plugin->info.Location == OPENRGB_PLUGIN_LOCATION_DEVICES)
    {
        for(int tab_idx = 0; tab_idx < ui->DevicesTabBar->count(); tab_idx++)
        {
            if(dynamic_cast<OpenRGBPluginContainer*>(ui->DevicesTabBar->widget(tab_idx)) != nullptr)
            {
                if(dynamic_cast<OpenRGBPluginContainer*>(ui->DevicesTabBar->widget(tab_idx))->plugin_widget == plugin->widget)
                {
                    ui->DevicesTabBar->removeTab(tab_idx);
                    delete plugin->widget;
                }
            }
        }
    }
    /*-----------------------------------------------------*\
    | Place plugin in the Information tab                   |
    \*-----------------------------------------------------*/
    else if(plugin->info.Location == OPENRGB_PLUGIN_LOCATION_INFORMATION)
    {
        for(int tab_idx = 0; tab_idx < ui->InformationTabBar->count(); tab_idx++)
        {
            if(dynamic_cast<OpenRGBPluginContainer*>(ui->InformationTabBar->widget(tab_idx)) != nullptr)
            {
                if(dynamic_cast<OpenRGBPluginContainer*>(ui->InformationTabBar->widget(tab_idx))->plugin_widget == plugin->widget)
                {
                    ui->InformationTabBar->removeTab(tab_idx);
                    delete plugin->widget;
                }
            }
        }
    }
    /*-----------------------------------------------------*\
    | Place plugin in the Settings tab                      |
    \*-----------------------------------------------------*/
    else if(plugin->info.Location == OPENRGB_PLUGIN_LOCATION_SETTINGS)
    {
        for(int tab_idx = 0; tab_idx < ui->SettingsTabBar->count(); tab_idx++)
        {
            if(dynamic_cast<OpenRGBPluginContainer*>(ui->SettingsTabBar->widget(tab_idx)) != nullptr)
            {
                if(dynamic_cast<OpenRGBPluginContainer*>(ui->SettingsTabBar->widget(tab_idx))->plugin_widget == plugin->widget)
                {
                    ui->SettingsTabBar->removeTab(tab_idx);
                    delete plugin->widget;
                }
            }
        }
    }

    if(plugin->traymenu)
    {
        trayIconMenu->removeAction(plugin->traymenu->menuAction());
    }
}

void OpenRGBDialog2::AddI2CToolsPage()
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

    QString SMBusToolsLabelString;

    if(IsDarkTheme())
    {
        SMBusToolsLabelString = "tools_dark.png";
    }
    else
    {
        SMBusToolsLabelString = "tools.png";
    }

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* SMBusToolsTabLabel = new TabLabel(SMBusToolsLabelString, "SMBus Tools");

    ui->InformationTabBar->tabBar()->setTabButton(ui->InformationTabBar->tabBar()->count() - 1, QTabBar::LeftSide, SMBusToolsTabLabel);
}

void OpenRGBDialog2::AddClientTab()
{
    /*-----------------------------------------------------*\
    | Add client information tab if it doesn't exist yet    |
    \*-----------------------------------------------------*/
    if(ClientInfoPage == NULL)
    {
        ClientInfoPage = new OpenRGBClientInfoPage();
        ui->MainTabBar->insertTab(2, ClientInfoPage, "SDK Client");
    }
}

void OpenRGBDialog2::AddClient(NetworkClient* new_client)
{
    /*-----------------------------------------------------*\
    | Add a client to the client information page           |
    \*-----------------------------------------------------*/
    if(ClientInfoPage != NULL)
    {
        ClientInfoPage->AddClient(new_client);
    }
}

void OpenRGBDialog2::AddServerTab()
{
    /*-----------------------------------------------------*\
    | Add server information tab if there is a server       |
    \*-----------------------------------------------------*/
    OpenRGBServerInfoPage *ServerInfoPage = new OpenRGBServerInfoPage(ResourceManager::get()->GetServer());
    ui->MainTabBar->insertTab(2, ServerInfoPage, "SDK Server");
}

void OpenRGBDialog2::ClearDevicesList()
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

void OpenRGBDialog2::UpdateDevicesList()
{
    std::vector<RGBController *> controllers = ResourceManager::get()->GetRGBControllers();

    /*-----------------------------------------------------*\
    | Loop through each controller in the list.             |
    \*-----------------------------------------------------*/
    for(unsigned int controller_idx = 0; controller_idx < controllers.size(); controller_idx++)
    {
        /*-----------------------------------------------------*\
        | Loop through each tab in the devices tab bar          |
        \*-----------------------------------------------------*/
        bool found = false;

        for(int tab_idx = 0; tab_idx < ui->DevicesTabBar->count(); tab_idx++)
        {
            QWidget* page = ui->DevicesTabBar->widget(tab_idx);

            if(dynamic_cast<OpenRGBDevicePage*>(page) != nullptr)
            {
                /*-----------------------------------------------------*\
                | If the current tab matches the current controller,    |
                | move the tab to the correct position                  |
                \*-----------------------------------------------------*/
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
            /*-----------------------------------------------------*\
            | The controller does not have a tab already created    |
            | Create a new tab and move it to the correct position  |
            \*-----------------------------------------------------*/
            OpenRGBDevicePage *NewPage = new OpenRGBDevicePage(controllers[controller_idx]);
            ui->DevicesTabBar->addTab(NewPage, "");

            /*-----------------------------------------------------*\
            | Connect the page's Set All button to the Set All slot |
            \*-----------------------------------------------------*/
            connect(NewPage,
                    SIGNAL(SetAllDevices(unsigned char, unsigned char, unsigned char)),
                    this,
                    SLOT(on_SetAllDevices(unsigned char, unsigned char, unsigned char)));

            /*-----------------------------------------------------*\
            | Connect the page's Resize signal to the Save Size slot|
            \*-----------------------------------------------------*/
            connect(NewPage,
                    SIGNAL(SaveSizeProfile()),
                    this,
                    SLOT(on_SaveSizeProfile()));

            /*-----------------------------------------------------*\
            | Create the tab label                                  |
            \*-----------------------------------------------------*/
            TabLabel* NewTabLabel = new TabLabel(GetIconString(controllers[controller_idx]->type, IsDarkTheme()), QString::fromStdString(controllers[controller_idx]->name));

            ui->DevicesTabBar->tabBar()->setTabButton(ui->DevicesTabBar->count() - 1, QTabBar::LeftSide, NewTabLabel);
            ui->DevicesTabBar->tabBar()->setTabToolTip(ui->DevicesTabBar->count() - 1, QString::fromStdString(controllers[controller_idx]->name));

            /*-----------------------------------------------------*\
            | Now move the new tab to the correct position          |
            \*-----------------------------------------------------*/
            ui->DevicesTabBar->tabBar()->moveTab(ui->DevicesTabBar->count() - 1, controller_idx);
        }

        /*-----------------------------------------------------*\
        | Loop through each tab in the information tab bar      |
        \*-----------------------------------------------------*/
        found = false;

        for(int tab_idx = 0; tab_idx < ui->InformationTabBar->count(); tab_idx++)
        {
            /*-----------------------------------------------------*\
            | If type is a device info page, check it               |
            \*-----------------------------------------------------*/
            std::string type_str = ui->InformationTabBar->widget(tab_idx)->metaObject()->className();
            if(type_str == "Ui::OpenRGBDeviceInfoPage")
            {
                OpenRGBDeviceInfoPage* page = (OpenRGBDeviceInfoPage*) ui->InformationTabBar->widget(tab_idx);

                /*-----------------------------------------------------*\
                | If the current tab matches the current controller,    |
                | move the tab to the correct position                  |
                \*-----------------------------------------------------*/
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
            /*-----------------------------------------------------*\
            | The controller does not have a tab already created    |
            | Create a new tab and move it to the correct position  |
            \*-----------------------------------------------------*/
            OpenRGBDeviceInfoPage *NewPage = new OpenRGBDeviceInfoPage(controllers[controller_idx]);
            ui->InformationTabBar->addTab(NewPage, "");

            /*-----------------------------------------------------*\
            | Create the tab label                                  |
            \*-----------------------------------------------------*/
            TabLabel* NewTabLabel = new TabLabel(GetIconString(controllers[controller_idx]->type, IsDarkTheme()), QString::fromStdString(controllers[controller_idx]->name));

            ui->InformationTabBar->tabBar()->setTabButton(ui->InformationTabBar->count() - 1, QTabBar::LeftSide, NewTabLabel);
            ui->InformationTabBar->tabBar()->setTabToolTip(ui->InformationTabBar->count() - 1, QString::fromStdString(controllers[controller_idx]->name));

            /*-----------------------------------------------------*\
            | Now move the new tab to the correct position          |
            \*-----------------------------------------------------*/
            ui->InformationTabBar->tabBar()->moveTab(ui->InformationTabBar->count() - 1, controller_idx);
        }
    }

    /*-----------------------------------------------------*\
    | Remove all remaining device tabs                      |
    \*-----------------------------------------------------*/
    unsigned int tab_count = ui->DevicesTabBar->count();

    for(unsigned int tab_idx = controllers.size(); tab_idx < tab_count; tab_idx++)
    {
        QWidget* tab_widget = ui->DevicesTabBar->widget(tab_idx);

        if(dynamic_cast<OpenRGBPluginContainer*>(tab_widget) == nullptr)
        {
            ui->DevicesTabBar->removeTab(tab_idx);
            delete tab_widget;
        }
    }

    bool found = true;

    while(found)
    {
        found = false;

        /*-----------------------------------------------------*\
        | Remove all remaining device information tabs, leaving |
        | other information tabs alone                          |
        \*-----------------------------------------------------*/
        for(int tab_idx = controllers.size(); tab_idx < ui->InformationTabBar->count(); tab_idx++)
        {
            std::string type_str = ui->InformationTabBar->widget(tab_idx)->metaObject()->className();
            if(type_str == "Ui::OpenRGBDeviceInfoPage")
            {
                found = true;
                QWidget* tab_widget = ui->InformationTabBar->widget(tab_idx);

                ui->InformationTabBar->removeTab(tab_idx);

                delete tab_widget;
                break;
            }
        }
    }
}

void OpenRGBDialog2::UpdateProfileList()
{
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    if(profile_manager != NULL)
    {
        /*-----------------------------------------------------*\
        | Clear profile combo box and tray icon menu            |
        \*-----------------------------------------------------*/
        ui->ProfileBox->clear();
        profileMenu->clear();

        for(std::size_t profile_index = 0; profile_index < profile_manager->profile_list.size(); profile_index++)
        {
            /*-----------------------------------------------------*\
            | Fill in profile combo box                             |
            \*-----------------------------------------------------*/
            ui->ProfileBox->addItem(profile_manager->profile_list[profile_index].c_str());

            /*-----------------------------------------------------*\
            | Fill in profile tray icon menu                        |
            \*-----------------------------------------------------*/
            QAction* actionProfileSelected = new QAction(profile_manager->profile_list[profile_index].c_str(), this);
            actionProfileSelected->setObjectName(profile_manager->profile_list[profile_index].c_str());
            connect(actionProfileSelected, SIGNAL(triggered()), this, SLOT(on_ProfileSelected()));
            profileMenu->addAction(actionProfileSelected);
        }
    }
}

void OpenRGBDialog2::on_Exit()
{
    /*-----------------------------------------------*\
    | This is the exit from the tray icon             |
    | NOT the main exit button (top right on Windows) |
    |                                                 |
    | The hide is important, otherwise it won't close |
    | when minimize on close is enabled               |
    \*-----------------------------------------------*/
    this->hide();
    trayIcon->hide();
    close();
}

void OpenRGBDialog2::on_LightsOff()
{
    on_SetAllDevices(0x00, 0x00, 0x00);
}

void OpenRGBDialog2::on_QuickRed()
{
    on_SetAllDevices(0xFF, 0x00, 0x00);
}

void OpenRGBDialog2::on_QuickYellow()
{
    on_SetAllDevices(0xFF, 0xFF, 0x00);
}

void OpenRGBDialog2::on_QuickGreen()
{
    on_SetAllDevices(0x00, 0xFF, 0x00);
}

void OpenRGBDialog2::on_QuickCyan()
{
    on_SetAllDevices(0x00, 0xFF, 0xFF);
}

void OpenRGBDialog2::on_QuickBlue()
{
    on_SetAllDevices(0x00, 0x00, 0xFF);
}

void OpenRGBDialog2::on_QuickMagenta()
{
    on_SetAllDevices(0xFF, 0x00, 0xFF);
}

void OpenRGBDialog2::on_QuickWhite()
{
    on_SetAllDevices(0xFF, 0xFF, 0xFF);
}

void OpenRGBDialog2::onDeviceListUpdated()
{
    UpdateDevicesList();
}

void OpenRGBDialog2::onDetectionProgressUpdated()
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

void OpenRGBDialog2::onDetectionEnded()
{
    /*-----------------------------------------------------*\
    | Detect unconfigured zones and prompt for resizing     |
    \*-----------------------------------------------------*/
    OpenRGBZonesBulkResizer::RunChecks(this);
}

void OpenRGBDialog2::on_SetAllDevices(unsigned char red, unsigned char green, unsigned char blue)
{
    for(int device = 0; device < ui->DevicesTabBar->count(); device++)
    {
        qobject_cast<OpenRGBDevicePage *>(ui->DevicesTabBar->widget(device))->SetCustomMode(red, green, blue);
    }
}

void OpenRGBDialog2::on_SaveSizeProfile()
{
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    if(profile_manager != NULL)
    {
        /*---------------------------------------------------------*\
        | Save the profile                                          |
        \*---------------------------------------------------------*/
        profile_manager->SaveProfile("sizes", true);
    }
}

void OpenRGBDialog2::on_ShowHide()
{
    if(isHidden())
    {
        show();
    }
    else
    {
        hide();
    }
}

void OpenRGBDialog2::on_ReShow(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
    {
        if (isHidden())
        {
            show();
        }
    }
}

void Ui::OpenRGBDialog2::on_ProfileSelected()
{
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    if(profile_manager != NULL)
    {
        /*---------------------------------------------------------*\
        | Get the profile filename from the selected object         |
        \*---------------------------------------------------------*/
        std::string profile_name = QObject::sender()->objectName().toStdString();

        /*---------------------------------------------------------*\
        | Load the profile                                          |
        \*---------------------------------------------------------*/
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

void Ui::OpenRGBDialog2::on_ButtonLoadProfile_clicked()
{
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    if(profile_manager != NULL)
    {
        /*---------------------------------------------------------*\
        | Get the profile filename from the profiles list           |
        \*---------------------------------------------------------*/
        std::string profile_name = ui->ProfileBox->currentText().toStdString();

        /*---------------------------------------------------------*\
        | Load the profile                                          |
        \*---------------------------------------------------------*/
        if(profile_manager->LoadProfile(profile_name))
        {
            for(int device = 0; device < ui->DevicesTabBar->count(); device++)
            {
                qobject_cast<OpenRGBDevicePage *>(ui->DevicesTabBar->widget(device))->UpdateDevice();
            }
        }
    }
}

void Ui::OpenRGBDialog2::on_ButtonDeleteProfile_clicked()
{
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    if(profile_manager != NULL)
    {
        /*---------------------------------------------------------*\
        | Get the profile filename from the profiles list           |
        \*---------------------------------------------------------*/
        std::string profile_name = ui->ProfileBox->currentText().toStdString();

        /*---------------------------------------------------------*\
        | Confirm we want to delete the profile                     |
        \*---------------------------------------------------------*/
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete Profile", "Do you really want to delete this profile?", QMessageBox::Yes|QMessageBox::No);

        /*---------------------------------------------------------*\
        | Load the profile                                          |
        \*---------------------------------------------------------*/
        if(reply == QMessageBox::Yes)
        {
            profile_manager->DeleteProfile(profile_name);

            UpdateProfileList();
        }
    }
}

void Ui::OpenRGBDialog2::on_ButtonToggleDeviceView_clicked()
{
    if(device_view_showing)
    {
        for(int device = 0; device < ui->DevicesTabBar->count(); device++)
        {
            qobject_cast<OpenRGBDevicePage *>(ui->DevicesTabBar->widget(device))->HideDeviceView();
        }
        device_view_showing = false;
    }
    else
    {
        for(int device = 0; device < ui->DevicesTabBar->count(); device++)
        {
            OpenRGBDevicePage* device_page = qobject_cast<OpenRGBDevicePage *>(ui->DevicesTabBar->widget(device));
            if(device_page) // Check the cast to make sure it is a device and not plugin
            {
                device_page->ShowDeviceView();
            }
        }
        device_view_showing = true;
    }
}

void Ui::OpenRGBDialog2::on_ButtonStopDetection_clicked()
{
    /*---------------------------------------------------------*\
    | Notify the detection thread that it has to die            |
    \*---------------------------------------------------------*/
    ResourceManager::get()->StopDeviceDetection();

    /*---------------------------------------------------------*\
    | Pretend we're done already by hiding the progress bar     |
    \*---------------------------------------------------------*/
    SetDetectionViewState(false);
}

void Ui::OpenRGBDialog2::SetDetectionViewState(bool detection_showing)
{
    if(detection_showing)
    {
        /*---------------------------------------------------------*\
        | Show the detection progress and hide the normal buttons   |
        \*---------------------------------------------------------*/
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
        /*---------------------------------------------------------*\
        | Hide the detection progress and show the normal buttons   |
        \*---------------------------------------------------------*/
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

void OpenRGBDialog2::SaveProfile()
{
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    if(profile_manager != NULL)
    {
        /*---------------------------------------------------------*\
        | Get the profile filename from the profiles list           |
        \*---------------------------------------------------------*/
        std::string filename = ui->ProfileBox->currentText().toStdString();

        /*---------------------------------------------------------*\
        | Save the profile                                          |
        \*---------------------------------------------------------*/
        profile_manager->SaveProfile(filename);
    }
}

void OpenRGBDialog2::SaveProfileAs()
{
    ProfileManager* profile_manager = ResourceManager::get()->GetProfileManager();

    if(profile_manager != NULL)
    {
        OpenRGBProfileSaveDialog dialog;

        /*---------------------------------------------------------*\
        | Open Profile Name Dialog                                  |
        \*---------------------------------------------------------*/
        std::string profile_name = dialog.show();

        if(!profile_name.empty())
        {
            /*---------------------------------------------------------*\
            | Extension .orp - OpenRgb Profile                          |
            \*---------------------------------------------------------*/
            std::string filename = profile_name;

            /*---------------------------------------------------------*\
            | Save the profile                                          |
            \*---------------------------------------------------------*/
            if(profile_manager->SaveProfile(filename))
            {
                UpdateProfileList();

                ui->ProfileBox->setCurrentIndex(ui->ProfileBox->findText(QString::fromStdString(profile_name)));
            }
        }
    }
}

void Ui::OpenRGBDialog2::on_ButtonRescan_clicked()
{
    SetDetectionViewState(true);

    ResourceManager::get()->DetectDevices();
}

void Ui::OpenRGBDialog2::on_ActionSaveProfile_triggered()
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

void Ui::OpenRGBDialog2::on_ActionSaveProfileAs_triggered()
{
    SaveProfileAs();
}


void Ui::OpenRGBDialog2::on_InformationTabBar_currentChanged(int tab_idx)
{
    TogglePluginsVisibility(tab_idx, ui->InformationTabBar);
}

void Ui::OpenRGBDialog2::on_DevicesTabBar_currentChanged(int tab_idx)
{
    TogglePluginsVisibility(tab_idx, ui->DevicesTabBar);
}

void Ui::OpenRGBDialog2::on_MainTabBar_currentChanged(int tab_idx)
{
    TogglePluginsVisibility(tab_idx, ui->MainTabBar);
}

void Ui::OpenRGBDialog2::on_SettingsTabBar_currentChanged(int tab_idx)
{
    TogglePluginsVisibility(tab_idx, ui->SettingsTabBar);
}

void Ui::OpenRGBDialog2::TogglePluginsVisibility(int tab_idx, QTabWidget* tabBar)
{
    /*---------------------------------------------------------*\
    | Hide all plugins                                          |
    \*---------------------------------------------------------*/
    for(int i = 0; i < (tabBar->count()); i++)
    {
        QWidget* tab = tabBar->widget(i);

        /*-----------------------------------------------------*\
        | Dynamic cast is essential in this check to ensure the |
        | tab is actually a plugin container                    |
        \*-----------------------------------------------------*/
        if((i != tab_idx) && (dynamic_cast<OpenRGBPluginContainer*>(tab) != nullptr))
        {
            ((OpenRGBPluginContainer*) tab)->Hide();
        }
    }

    /*---------------------------------------------------------*\
    | Show plugin if needed                                     |
    \*---------------------------------------------------------*/
    QWidget* tab = tabBar->widget(tab_idx);

    /*---------------------------------------------------------*\
    | Dynamic cast is essential in this check to ensure the tab |
    | is actually a plugin container                            |
    \*---------------------------------------------------------*/
    if(dynamic_cast<OpenRGBPluginContainer*>(tab) != nullptr)
    {
        ((OpenRGBPluginContainer*) tab)->Show();
    }
}

void Ui::OpenRGBDialog2::AddConsolePage()
{
    OpenRGBConsolePage* page = new OpenRGBConsolePage();

    ui->InformationTabBar->addTab(page, "");

    QString ConsoleLabelString;

    if(IsDarkTheme())
    {
        ConsoleLabelString = "console_dark.png";
    }
    else
    {
        ConsoleLabelString = "console.png";
    }

    /*-----------------------------------------------------*\
    | Create the tab label                                  |
    \*-----------------------------------------------------*/
    TabLabel* ConsoleTabLabel = new TabLabel(ConsoleLabelString, "Log Console");

    ui->InformationTabBar->tabBar()->setTabButton(ui->InformationTabBar->tabBar()->count() - 1, QTabBar::LeftSide, ConsoleTabLabel);
}
