#include "OpenRGBLIFXSettingsEntry.h"
#include "ui_OpenRGBLIFXSettingsEntry.h"

using namespace Ui;

OpenRGBLIFXSettingsEntry::OpenRGBLIFXSettingsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBLIFXSettingsEntryUi)
{
    ui->setupUi(this);
}

OpenRGBLIFXSettingsEntry::~OpenRGBLIFXSettingsEntry()
{
    delete ui;
}

void OpenRGBLIFXSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}
