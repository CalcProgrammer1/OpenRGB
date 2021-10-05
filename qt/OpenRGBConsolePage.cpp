#include "OpenRGBConsolePage.h"
#include "LogManager.h"
#include <stdio.h>

using namespace Ui;

OpenRGBConsolePage::OpenRGBConsolePage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBConsolePageUi)
{
    ui->setupUi(this);

    ui->log_level->blockSignals(true);
    ui->log_level->addItems({
                                "Fatal",
                                "Error",
                                "Warning",
                                "Info",
                                "Verbose",
                                "Debug",
                                "Trace"
                            });

    ui->log_level->setCurrentIndex(LogManager::get()->getLoglevel());
    ui->log_level->blockSignals(false);

    Refresh();
}

void OpenRGBConsolePage::Refresh()
{
    QString log;

    unsigned int current_level = LogManager::get()->getLoglevel();

    for(PLogMessage& message: LogManager::get()->messages())
    {
        unsigned int message_level = message.get()->level;

        if(message_level <= current_level)
        {
            log += "[";
            log += LogManager::log_codes[message_level];
            log += "] ";
            log += QString::fromStdString(message.get()->buffer);
            log += "\n";
        }
    }

    ui->logs->setText(log);
}

void OpenRGBConsolePage::on_log_level_currentIndexChanged(int index)
{
    LogManager::get()->setLoglevel(index);
}

void OpenRGBConsolePage::on_clear_clicked()
{
    LogManager::get()->clearMessages();
    ui->logs->clear();
}

void OpenRGBConsolePage::on_refresh_clicked()
{
    Refresh();
}

OpenRGBConsolePage::~OpenRGBConsolePage()
{
    delete ui;
}
