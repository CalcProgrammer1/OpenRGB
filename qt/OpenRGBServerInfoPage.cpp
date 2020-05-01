#include "OpenRGBServerInfoPage.h"

using namespace Ui;

OpenRGBServerInfoPage::OpenRGBServerInfoPage(RGBController *dev, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBServerInfoPageUi)
{
    ui->setupUi(this);
}

OpenRGBServerInfoPage::~OpenRGBServerInfoPage()
{
    delete ui;
}
