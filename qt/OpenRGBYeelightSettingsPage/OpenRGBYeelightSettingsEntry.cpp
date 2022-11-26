#include "OpenRGBYeelightSettingsEntry.h"
#include "ui_OpenRGBYeelightSettingsEntry.h"
#include "net_port.h"
#include <QInputDialog>

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

void OpenRGBYeelightSettingsEntry::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBYeelightSettingsEntry::on_HostIPChooserButton_clicked()
{
    char hostname[256];
    gethostname(hostname, 256);

    char **in_addrs = gethostbyname(hostname)->h_addr_list;

    QStringList in_addr_list;

    while (*in_addrs != NULL)
    {
        in_addr_list << inet_ntoa(*((struct in_addr*) *in_addrs));
        in_addrs++;
    }

    QInputDialog inp;

    inp.setOptions(QInputDialog::UseListViewForComboBoxItems);
    inp.setComboBoxItems(in_addr_list);
    inp.setWindowTitle(tr("Choose an IP..."));
    inp.setLabelText(tr("Choose the correct IP for the host"));

    if(!inp.exec())
    {
        return;
    }

    ui->HostIPEdit->setText(inp.textValue());
}

