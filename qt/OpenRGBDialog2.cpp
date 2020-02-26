#include <fstream>
#include "OpenRGBDialog2.h"
#include "OpenRGBDevicePage.h"
#include "OpenRGBDeviceInfoPage.h"
#include "OpenRGBSystemInfoPage.h"
#include "OpenRGBProfileSaveDialog.h"
#include "RGBController_Dummy.h"
#include <QLabel>
#include <QTabBar>

using namespace Ui;

OpenRGBDialog2::OpenRGBDialog2(std::vector<i2c_smbus_interface *>& bus, std::vector<RGBController *>& control, QWidget *parent) : QMainWindow(parent), busses(bus), controllers (control), ui(new OpenRGBDialog2Ui)
{
    ui->setupUi(this);

    QIcon logo(":OpenRGB.png");
    setWindowIcon(logo);

    /*-----------------------------------------------------*\
    | Set up tray icon menu                                 |
    \*-----------------------------------------------------*/
    QMenu* trayIconMenu = new QMenu( this );

    trayIcon = new QSystemTrayIcon(this);

    QAction* actionShowHide = new QAction("Show/Hide", this);
    connect(actionShowHide, SIGNAL(triggered()), this, SLOT(on_ShowHide()));
    trayIconMenu->addAction(actionShowHide);

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

void OpenRGBDialog2::on_SetAllDevices(unsigned char red, unsigned char green, unsigned char blue)
{
    for(int device = 0; device < ui->DevicesTabBar->count(); device++)
    {
        qobject_cast<OpenRGBDevicePage *>(ui->DevicesTabBar->widget(device))->SetCustomMode();
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

void Ui::OpenRGBDialog2::on_ButtonSaveProfile_clicked()
{
    OpenRGBProfileSaveDialog dialog;

    /*---------------------------------------------------------*\
    | Open Profile Name Dialog                                  |
    \*---------------------------------------------------------*/
    std::string profile_name = dialog.show();

    /*---------------------------------------------------------*\
    | If a name was entered, save the profile file              |
    \*---------------------------------------------------------*/
    if(profile_name != "")
    {
        /*---------------------------------------------------------*\
        | Extension .orp - OpenRgb Profile                          |
        \*---------------------------------------------------------*/
        std::string filename = profile_name + ".orp";

        /*---------------------------------------------------------*\
        | Open an output file in binary mode                        |
        \*---------------------------------------------------------*/
        std::ofstream controller_file(filename, std::ios::out | std::ios::binary);

        /*---------------------------------------------------------*\
        | Write header                                              |
        | 16 bytes - "OPENRGB_PROFILE"                              |
        | 4 bytes - Version, unsigned int                           |
        \*---------------------------------------------------------*/
        unsigned int profile_version = 1;
        controller_file.write("OPENRGB_PROFILE", 16);
        controller_file.write((char *)&profile_version, sizeof(unsigned int));

        /*---------------------------------------------------------*\
        | Write controller data for each controller                 |
        \*---------------------------------------------------------*/
        for(int controller_index = 0; controller_index < controllers.size(); controller_index++)
        {
            unsigned char *controller_data = controllers[controller_index]->GetDeviceDescription();
            unsigned int controller_size;

            memcpy(&controller_size, controller_data, sizeof(controller_size));

            controller_file.write((const char *)controller_data, controller_size);
        }

        /*---------------------------------------------------------*\
        | Close the file when done                                  |
        \*---------------------------------------------------------*/
        controller_file.close();
    }
}

void Ui::OpenRGBDialog2::on_ButtonLoadProfile_clicked()
{
    std::vector<RGBController*> temp_controllers;
    unsigned int controller_size;
    unsigned int controller_offset = 0;

    /*---------------------------------------------------------*\
    | Get the profile filename from the profiles list           |
    \*---------------------------------------------------------*/
    std::string filename = "test123.orp";

    /*---------------------------------------------------------*\
    | Open input file in binary mode                            |
    \*---------------------------------------------------------*/
    std::ifstream controller_file(filename, std::ios::in | std::ios::binary);

    /*---------------------------------------------------------*\
    | Read and verify file header                               |
    \*---------------------------------------------------------*/
    char            header_string[16];
    unsigned int    header_version;

    controller_file.read(header_string, 16);
    controller_file.read((char *)&header_version, sizeof(unsigned int));

    controller_offset += 16 + sizeof(unsigned int);
    controller_file.seekg(controller_offset);

    if(strcmp(header_string, "OPENRGB_PROFILE") == 0)
    {
        if(header_version == 1)
        {
            /*---------------------------------------------------------*\
            | Read controller data from file until EOF                  |
            \*---------------------------------------------------------*/
            while(!(controller_file.peek() == EOF))
            {
                controller_file.read((char *)&controller_size, sizeof(controller_size));

                unsigned char *controller_data = new unsigned char[controller_size];

                controller_file.seekg(controller_offset);

                controller_file.read((char *)controller_data, controller_size);

                RGBController_Dummy *temp_controller = new RGBController_Dummy();

                temp_controller->ReadDeviceDescription(controller_data);

                temp_controllers.push_back(temp_controller);

                delete[] controller_data;

                controller_offset += controller_size;
                controller_file.seekg(controller_offset);
            }

            for(int controller_index = 0; controller_index < controllers.size(); controller_index++)
            {
                RGBController *temp_controller = temp_controllers[controller_index];
                RGBController *controller_ptr = controllers[controller_index];

                /*---------------------------------------------------------*\
                | Test if saved controller data matches this controller     |
                \*---------------------------------------------------------*/
                if((temp_controller->type        == controller_ptr->type       )
                 &&(temp_controller->name        == controller_ptr->name       )
                 &&(temp_controller->description == controller_ptr->description)
                 &&(temp_controller->version     == controller_ptr->version    )
                 &&(temp_controller->serial      == controller_ptr->serial     )
                 &&(temp_controller->location    == controller_ptr->location   ))
                {
                    /*---------------------------------------------------------*\
                    | Update all modes                                          |
                    \*---------------------------------------------------------*/
                    if(temp_controller->modes.size() == controller_ptr->modes.size())
                    {
                        for(int mode_index = 0; mode_index < temp_controller->modes.size(); mode_index++)
                        {
                            if((temp_controller->modes[mode_index].name       == controller_ptr->modes[mode_index].name      )
                             &&(temp_controller->modes[mode_index].value      == controller_ptr->modes[mode_index].value     )
                             &&(temp_controller->modes[mode_index].flags      == controller_ptr->modes[mode_index].flags     )
                             &&(temp_controller->modes[mode_index].speed_min  == controller_ptr->modes[mode_index].speed_min )
                             &&(temp_controller->modes[mode_index].speed_max  == controller_ptr->modes[mode_index].speed_max )
                             &&(temp_controller->modes[mode_index].colors_min == controller_ptr->modes[mode_index].colors_min)
                             &&(temp_controller->modes[mode_index].colors_max == controller_ptr->modes[mode_index].colors_max))
                            {
                                controller_ptr->modes[mode_index].speed      = temp_controller->modes[mode_index].speed;
                                controller_ptr->modes[mode_index].direction  = temp_controller->modes[mode_index].direction;
                                controller_ptr->modes[mode_index].color_mode = temp_controller->modes[mode_index].color_mode;

                                controller_ptr->modes[mode_index].colors.resize(temp_controller->modes[mode_index].colors.size());

                                for(int mode_color_index = 0; mode_color_index < temp_controller->modes[mode_index].colors.size(); mode_color_index++)
                                {
                                    controller_ptr->modes[mode_index].colors[mode_color_index] = temp_controller->modes[mode_index].colors[mode_color_index];
                                }
                            }

                        }

                        controller_ptr->active_mode = temp_controller->active_mode;
                    }

                    /*---------------------------------------------------------*\
                    | Update all colors                                         |
                    \*---------------------------------------------------------*/
                    if(temp_controller->colors.size() == controller_ptr->colors.size())
                    {
                        for(int color_index = 0; color_index < temp_controller->colors.size(); color_index++)
                        {
                            controller_ptr->colors[color_index] = temp_controller->colors[color_index];
                        }
                    }
                }
            }
        }
    }

    for(int device = 0; device < ui->DevicesTabBar->count(); device++)
    {
        qobject_cast<OpenRGBDevicePage *>(ui->DevicesTabBar->widget(device))->UpdateDevice();
    }
}
