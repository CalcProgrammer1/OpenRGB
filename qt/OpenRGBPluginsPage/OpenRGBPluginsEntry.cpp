#include "OpenRGBPluginsEntry.h"
#include "ui_OpenRGBPluginsEntry.h"

using namespace Ui;

OpenRGBPluginsEntry::OpenRGBPluginsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBPluginsEntryUi)
{
    ui->setupUi(this);
}

OpenRGBPluginsEntry::~OpenRGBPluginsEntry()
{
    delete ui;
}
