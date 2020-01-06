#include "OpenRGBDialog2.h"
#include "OpenRGBDevicePage.h"
#include "OpenRGBDeviceInfoPage.h"
#include "OpenRGBSystemInfoPage.h"
#include <QLabel>
#include <QTabBar>

using namespace Ui;

OpenRGBDialog2::OpenRGBDialog2(std::vector<i2c_smbus_interface *>& bus, std::vector<RGBController *>& control, QWidget *parent) : QMainWindow(parent), busses(bus), controllers (control), ui(new OpenRGBDialog2Ui)
{
    ui->setupUi(this);

    QIcon logo(":OpenRGB.png");
    setWindowIcon(logo);


    QMenu* trayIconMenu = new QMenu( this );

    trayIcon = new QSystemTrayIcon(this);

    QAction* actionShowHide = new QAction("Show/Hide", this);
    connect(actionShowHide, SIGNAL(triggered()), this, SLOT(on_ShowHide()));
    trayIconMenu->addAction(actionShowHide);

    QAction* actionLightsOff = new QAction("Lights Off", this);
    connect(actionLightsOff, SIGNAL(triggered()), this, SLOT(on_LightsOff()));
    trayIconMenu->addAction(actionLightsOff);

    QAction* actionExit = new QAction( "Exit", this );
    connect( actionExit, SIGNAL( triggered() ), this, SLOT( on_Exit() ));
    trayIconMenu->addAction(actionExit);

    trayIcon->setIcon(logo);
    trayIcon->setToolTip("OpenRGB");
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();

    /*-----------------------------------------------------*\
    | Set up list of devices                                |
    \*-----------------------------------------------------*/
    QTabBar *DevicesTabBar = ui->DevicesTabBar->tabBar();

    for(std::size_t dev_idx = 0; dev_idx < control.size(); dev_idx++)
    {
        OpenRGBDevicePage *NewPage = new OpenRGBDevicePage(control[dev_idx]);
        ui->DevicesTabBar->addTab(NewPage, "");

        /*-----------------------------------------------------*\
        | Connect the page's Set All button to the Set All slot |
        \*-----------------------------------------------------*/
        connect(NewPage,
                SIGNAL(SetAllDevices(unsigned char, unsigned char, unsigned char)),
                this,
                SLOT(on_SetAllDevices(unsigned char, unsigned char, unsigned char)));

        /*-----------------------------------------------------*\
        | Use Qt's HTML capabilities to display both icon and   |
        | text in the tab label.  Choose icon based on device   |
        | type and append device name string.                   |
        \*-----------------------------------------------------*/
        QString NewLabelString = "<html><table><tr><td width='30'><img src='";
        switch(control[dev_idx]->type)
        {
        case DEVICE_TYPE_MOTHERBOARD:
            NewLabelString += ":/motherboard.svg";
            break;
        case DEVICE_TYPE_DRAM:
            NewLabelString += ":/ram.svg";
            break;
        case DEVICE_TYPE_GPU:
            NewLabelString += ":/graphics-card.svg";
            break;
        case DEVICE_TYPE_COOLER:
            NewLabelString += ":/fan.svg";
            break;
        case DEVICE_TYPE_LEDSTRIP:
            //break;
        case DEVICE_TYPE_KEYBOARD:
            //break;
        case DEVICE_TYPE_MOUSE:
            //break;
        case DEVICE_TYPE_HEADSET:
            //break;
        case DEVICE_TYPE_UNKNOWN:
            NewLabelString += ":/keyboard.svg";
            break;
        }
        NewLabelString += "' height='15' width='15'></td><td>" + QString::fromStdString(control[dev_idx]->name) + "</td></tr></table></html>";

        QLabel *NewTabLabel = new QLabel();
        NewTabLabel->setText(NewLabelString);
        NewTabLabel->setIndent(20);
        NewTabLabel->setGeometry(0, 0, 200, 20);

        DevicesTabBar->setTabButton(dev_idx, QTabBar::LeftSide, NewTabLabel);
    }

    /*-----------------------------------------------------*\
    | Set up list of information                            |
    \*-----------------------------------------------------*/
    QTabBar *InformationTabBar = ui->InformationTabBar->tabBar();

    for(std::size_t dev_idx = 0; dev_idx < control.size(); dev_idx++)
    {
        OpenRGBDeviceInfoPage *NewPage = new OpenRGBDeviceInfoPage(control[dev_idx]);
        ui->InformationTabBar->addTab(NewPage, "");

        /*-----------------------------------------------------*\
        | Use Qt's HTML capabilities to display both icon and   |
        | text in the tab label.  Choose icon based on device   |
        | type and append device name string.                   |
        \*-----------------------------------------------------*/
        QString NewLabelString = "<html><table><tr><td width='30'><img src='";
        switch(control[dev_idx]->type)
        {
        case DEVICE_TYPE_MOTHERBOARD:
            NewLabelString += ":/motherboard.svg";
            break;
        case DEVICE_TYPE_DRAM:
            NewLabelString += ":/ram.svg";
            break;
        case DEVICE_TYPE_GPU:
            NewLabelString += ":/graphics-card.svg";
            break;
        case DEVICE_TYPE_COOLER:
            NewLabelString += ":/fan.svg";
            break;
        case DEVICE_TYPE_LEDSTRIP:
            //break;
        case DEVICE_TYPE_KEYBOARD:
            //break;
        case DEVICE_TYPE_MOUSE:
            //break;
        case DEVICE_TYPE_HEADSET:
            //break;
        case DEVICE_TYPE_UNKNOWN:
            NewLabelString += ":/keyboard.svg";
            break;
        }
        NewLabelString += "' height='15' width='15'></td><td>" + QString::fromStdString(control[dev_idx]->name) + "</td></tr></table></html>";

        QLabel *NewTabLabel = new QLabel();
        NewTabLabel->setText(NewLabelString);
        NewTabLabel->setIndent(20);
        NewTabLabel->setGeometry(0, 0, 200, 20);

        InformationTabBar->setTabButton(dev_idx, QTabBar::LeftSide, NewTabLabel);
    }

    OpenRGBSystemInfoPage *SysInfoPage = new OpenRGBSystemInfoPage(bus);
    ui->InformationTabBar->addTab(SysInfoPage, "");

    /*-----------------------------------------------------*\
    | Use Qt's HTML capabilities to display both icon and   |
    | text in the tab label.  Choose icon based on device   |
    | type and append device name string.                   |
    \*-----------------------------------------------------*/
    QString SystemLabelString = "<html><table><tr><td width='30'><img src='";
    SystemLabelString += ":/keyboard.svg";
    SystemLabelString += "' height='15' width='15'></td><td>System</td></tr></table></html>";

    QLabel *SystemTabLabel = new QLabel();
    SystemTabLabel->setText(SystemLabelString);
    SystemTabLabel->setIndent(20);
    SystemTabLabel->setGeometry(0, 0, 200, 20);

    InformationTabBar->setTabButton(control.size(), QTabBar::LeftSide, SystemTabLabel);
}

OpenRGBDialog2::~OpenRGBDialog2()
{
    delete ui;
}

void OpenRGBDialog2::show()
{
    QMainWindow::show();
}

void OpenRGBDialog2::on_Exit()
{
    close();
}

void OpenRGBDialog2::on_LightsOff()
{
    on_SetAllDevices(0, 0, 0);
}

void OpenRGBDialog2::on_SetAllDevices(unsigned char red, unsigned char green, unsigned char blue)
{
    for(int device = 0; device < ui->DevicesTabBar->count(); device++)
    {
        qobject_cast<OpenRGBDevicePage *>(ui->DevicesTabBar->widget(device))->SetDevice(red, green, blue);
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