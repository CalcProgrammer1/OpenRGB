/*---------------------------------------------------------*\
| OpenRGBLogConsolePage.cpp                                 |
|                                                           |
|   User interface for OpenRGB console page                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "OpenRGBLogConsolePage.h"
#include "ui_OpenRGBLogConsolePage.h"

static void OpenRGBLogConsolePageLogManagerCallback(void* this_ptr, unsigned int update_reason, PLogMessage message)
{
    OpenRGBLogConsolePage* this_obj = (OpenRGBLogConsolePage*)this_ptr;

    switch(update_reason)
    {
        case LOGMANAGER_UPDATE_REASON_LOG_ENTRY:
            QMetaObject::invokeMethod(this_obj, "AddLogEntry", Qt::QueuedConnection, Q_ARG(PLogMessage, message));
            break;
    }
}

OpenRGBLogConsolePage::OpenRGBLogConsolePage(QWidget* parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBLogConsolePage)
{
    qRegisterMetaType<PLogMessage>("PLogMessage");

    ui->setupUi(this);

    ui->ComboBoxLogLevel->blockSignals(true);
    ui->ComboBoxLogLevel->addItems({"Fatal", "Error", "Warning", "Info", "Verbose", "Debug", "Trace"});

    ui->ComboBoxLogLevel->setCurrentIndex(LogManager::get()->GetLogLevel());
    ui->ComboBoxLogLevel->blockSignals(false);

    for(PLogMessage& message: LogManager::get()->GetLogBuffer())
    {
        AddLogEntry(message);
    }

    LogManager::get()->RegisterLogManagerCallback(OpenRGBLogConsolePageLogManagerCallback, this);
}

OpenRGBLogConsolePage::~OpenRGBLogConsolePage()
{
    LogManager::get()->UnregisterLogManagerCallback(OpenRGBLogConsolePageLogManagerCallback, this);
    delete ui;
}

void OpenRGBLogConsolePage::AddLogEntry(PLogMessage message)
{
    unsigned int current_level = LogManager::get()->GetLogLevel();
    unsigned int message_level = message->level;

    if(message_level <= current_level && message_level != LL_DIALOG)
    {
        ui->TextEditLog->append(QString::asprintf("[%-7s] %s", LogManager::LOG_CODES[message_level], message->text.c_str()));
    }
}

void OpenRGBLogConsolePage::on_ComboBoxLogLevel_currentIndexChanged(int index)
{
    LogManager::get()->SetLogLevel(index);
}

void OpenRGBLogConsolePage::on_ButtonClear_clicked()
{
    LogManager::get()->ClearLogBuffer();
    ui->TextEditLog->clear();
}

void OpenRGBLogConsolePage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}
