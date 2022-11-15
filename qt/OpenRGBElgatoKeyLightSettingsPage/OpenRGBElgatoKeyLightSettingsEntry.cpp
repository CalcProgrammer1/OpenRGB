#include "OpenRGBElgatoKeyLightSettingsEntry.h"
#include "ui_OpenRGBElgatoKeyLightSettingsEntry.h"

using namespace Ui;

OpenRGBElgatoKeyLightSettingsEntry::OpenRGBElgatoKeyLightSettingsEntry(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenRGBElgatoKeyLightSettingsEntryUi)
{
    ui->setupUi(this);
}

OpenRGBElgatoKeyLightSettingsEntry::~OpenRGBElgatoKeyLightSettingsEntry()
{
    delete ui;
}
