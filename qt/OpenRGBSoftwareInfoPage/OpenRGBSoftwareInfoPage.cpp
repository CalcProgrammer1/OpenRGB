/*---------------------------------------------------------*\
| OpenRGBSoftwareInfoPage.cpp                               |
|                                                           |
|   User interface for software information page            |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBSoftwareInfoPage.h"
#include "OpenRGBPluginInterface.h"
#include "NetworkProtocol.h"

#include "ui_OpenRGBSoftwareInfoPage.h"
#include <QSysInfo>

OpenRGBSoftwareInfoPage::OpenRGBSoftwareInfoPage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBSoftwareInfoPage)
{
    ui->setupUi(this);
    ui->SDKVersionValue->setText(QString::number(OPENRGB_SDK_PROTOCOL_VERSION));
    ui->PluginAPIVersionValue->setText(QString::number(OPENRGB_PLUGIN_API_VERSION));
    ui->QtVersionValue->setText(QT_VERSION_STR);
    ui->VersionValue->setText(VERSION_STRING);
    ui->BuildDateValue->setText(BUILDDATE_STRING);
    ui->GitCommitIDValue->setText(GIT_COMMIT_ID);
    ui->GitCommitDateValue->setText(GIT_COMMIT_DATE);
    ui->GitBranchValue->setText(GIT_BRANCH);
    ui->OsVersionValue->setText(QSysInfo::prettyProductName());
}

OpenRGBSoftwareInfoPage::~OpenRGBSoftwareInfoPage()
{
    delete ui;
}

void OpenRGBSoftwareInfoPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}
