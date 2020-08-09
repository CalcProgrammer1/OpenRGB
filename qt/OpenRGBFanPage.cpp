#include <QTimer>
#include "OpenRGBFanPage.h"

using namespace Ui;

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef __APPLE__
#include <unistd.h>

static void Sleep(unsigned int milliseconds)
{
    usleep(1000 * milliseconds);
}
#endif

#ifdef __linux__
#include <unistd.h>

static void Sleep(unsigned int milliseconds)
{
    usleep(1000 * milliseconds);
}
#endif

OpenRGBFanPage::OpenRGBFanPage(FanController *dev, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBFanPageUi)
{
    ui->setupUi(this);

    /*-----------------------------------------------------*\
    | Store device pointer                                  |
    \*-----------------------------------------------------*/
    device = dev;

    fan_thread_running = true;
    FanUpdateThread = new std::thread(&OpenRGBFanPage::FanUpdateThreadFunction, this);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&OpenRGBFanPage::UpdateRPM));
    timer->start(1000);

    /*-----------------------------------------------------*\
    | Fill in the fan box                                   |
    \*-----------------------------------------------------*/
    for (std::size_t i = 0; i < device->fans.size(); i++)
    {
        ui->FanBox->addItem(device->fans[i].name.c_str());
        ui->SpeedSlider->setMinimum(device->fans[i].speed_min);
        ui->SpeedSlider->setMaximum(device->fans[i].speed_max);
        ui->SpeedSlider->setValue(device->fans[i].speed_cmd);
        ui->RPMValue->setText(QString::number(device->fans[i].rpm_rdg));
    }
}

OpenRGBFanPage::~OpenRGBFanPage()
{
    fan_thread_running = false;
    FanUpdateThread->join();
    delete ui;
}

void OpenRGBFanPage::FanUpdateThreadFunction()
{
    while(fan_thread_running)
    {
        device->UpdateControl();
        device->UpdateReading();
        Sleep(1000);
    }
}

void OpenRGBFanPage::UpdateRPM()
{
    /*-----------------------------------------------------*\
    | Read selected fan                                     |
    \*-----------------------------------------------------*/
    unsigned int selected_fan   = (unsigned int)ui->FanBox->currentIndex();

    ui->RPMValue->setText(QString::number(device->fans[selected_fan].rpm_rdg));
}

void OpenRGBFanPage::on_FanBox_currentIndexChanged(int /*index*/)
{
    /*-----------------------------------------------------*\
    | Read selected fan                                     |
    \*-----------------------------------------------------*/
    unsigned int selected_fan   = (unsigned int)ui->FanBox->currentIndex();

    ui->SpeedSlider->setMinimum(device->fans[selected_fan].speed_min);
    ui->SpeedSlider->setMaximum(device->fans[selected_fan].speed_max);
    ui->SpeedSlider->setValue(device->fans[selected_fan].speed_cmd);

    UpdateRPM();
}

void OpenRGBFanPage::on_ModeBox_currentIndexChanged(int /*index*/)
{

}

void OpenRGBFanPage::on_SpeedSlider_valueChanged(int /* value */)
{
    /*-----------------------------------------------------*\
    | Read selected fan                                     |
    \*-----------------------------------------------------*/
    unsigned int selected_fan   = (unsigned int)ui->FanBox->currentIndex();
    unsigned int speed_cmd      = ui->SpeedSlider->value();

    device->fans[selected_fan].speed_cmd = speed_cmd;
}
