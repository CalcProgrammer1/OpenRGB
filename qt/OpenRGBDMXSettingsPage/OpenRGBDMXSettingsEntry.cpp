#include "OpenRGBDMXSettingsEntry.h"
#include "ui_OpenRGBDMXSettingsEntry.h"

using namespace Ui;

OpenRGBDMXSettingsEntry::OpenRGBDMXSettingsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBDMXSettingsEntryUi)
{
    ui->setupUi(this);
}

OpenRGBDMXSettingsEntry::~OpenRGBDMXSettingsEntry()
{
    delete ui;
}

void OpenRGBDMXSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}
