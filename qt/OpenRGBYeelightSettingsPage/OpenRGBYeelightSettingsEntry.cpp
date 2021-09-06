#include "OpenRGBYeelightSettingsEntry.h"
#include "ui_OpenRGBYeelightSettingsEntry.h"

using namespace Ui;

OpenRGBYeelightSettingsEntry::OpenRGBYeelightSettingsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBYeelightSettingsEntryUi)
{
    ui->setupUi(this);
}

OpenRGBYeelightSettingsEntry::~OpenRGBYeelightSettingsEntry()
{
    delete ui;
}
