#include "OpenRGBElgatoLightStripSettingsEntry.h"
#include "ui_OpenRGBElgatoLightStripSettingsEntry.h"

using namespace Ui;

OpenRGBElgatoLightStripSettingsEntry::OpenRGBElgatoLightStripSettingsEntry(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenRGBElgatoLightStripSettingsEntryUi)
{
    ui->setupUi(this);
}

OpenRGBElgatoLightStripSettingsEntry::~OpenRGBElgatoLightStripSettingsEntry()
{
    delete ui;
}

void OpenRGBElgatoLightStripSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}
