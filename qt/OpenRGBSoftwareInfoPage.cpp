#include "OpenRGBSoftwareInfoPage.h"

using namespace Ui;

OpenRGBSoftwareInfoPage::OpenRGBSoftwareInfoPage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBSoftwareInfoPageUi)
{
    ui->setupUi(this);
}

OpenRGBSoftwareInfoPage::~OpenRGBSoftwareInfoPage()
{
    delete ui;
}
