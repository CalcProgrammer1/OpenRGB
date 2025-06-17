/*---------------------------------------------------------*\
| OpenRGBProfileSaveDialog.cpp                              |
|                                                           |
|   User interface entry for OpenRGB profile save dialog    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QCloseEvent>
#include "ResourceManager.h"
#include "OpenRGBDialog.h"
#include "ProfileManager.h"
#include "OpenRGBProfileSaveDialog.h"
#include "ui_OpenRGBProfileSaveDialog.h"

#ifdef _WIN32
#include <QSettings>
#endif

OpenRGBProfileSaveDialog::OpenRGBProfileSaveDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::OpenRGBProfileSaveDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    std::vector<std::string> filenames = ResourceManager::get()->GetProfileManager()->profile_list;

    if(filenames.empty())
    {
        ui->list_profile->setVisible(false);
        ui->existing->setVisible(false);
    }
    else
    {
        for(const std::string& f: filenames)
        {
            ui->list_profile->addItem(QString::fromStdString(f));
        }

        connect(ui->list_profile, &QListWidget::currentItemChanged, [=](){
            ui->lineEdit->setText(ui->list_profile->currentItem()->text());
        });
    }
}

OpenRGBProfileSaveDialog::~OpenRGBProfileSaveDialog()
{
    delete ui;
}

void OpenRGBProfileSaveDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

std::string OpenRGBProfileSaveDialog::show()
{
    std::string return_string;

    int result = this->exec();

    if(result == QDialog::Rejected)
    {
        return_string = "";
    }
    else
    {
        return_string = ui->lineEdit->text().toStdString();
    }

    return(return_string);
}
