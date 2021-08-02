#include "OpenRGBSerialSettingsEntry.h"
#include "ui_OpenRGBSerialSettingsEntry.h"

using namespace Ui;

OpenRGBSerialSettingsEntry::OpenRGBSerialSettingsEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBSerialSettingsEntryUi)
{
    ui->setupUi(this);

    ui->ProtocolComboBox->addItem("Keyboard Visualizer");
    ui->ProtocolComboBox->addItem("Adalight");
    ui->ProtocolComboBox->addItem("TPM2");
}

OpenRGBSerialSettingsEntry::~OpenRGBSerialSettingsEntry()
{
    delete ui;
}
