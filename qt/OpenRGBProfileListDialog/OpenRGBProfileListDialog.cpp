/*---------------------------------------------------------*\
| OpenRGBProfileListDialog.cpp                              |
|                                                           |
|   User interface entry for OpenRGB profile save dialog    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <QCloseEvent>
#include "ResourceManager.h"
#include "OpenRGBDialog.h"
#include "ProfileManager.h"
#include "OpenRGBProfileListDialog.h"
#include "ui_OpenRGBProfileListDialog.h"

#ifdef _WIN32
#include <QSettings>
#endif

OpenRGBProfileListDialog::OpenRGBProfileListDialog(bool create, QWidget *parent) :
    QDialog(parent), ui(new Ui::OpenRGBProfileListDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    std::vector<std::string> profiles = ResourceManager::get()->GetProfileManager()->GetProfileList();

    if(!create)
    {
        ui->new_label->setVisible(false);
        ui->new_edit->setVisible(false);
    }
    if(profiles.empty())
    {
        ui->select_label->setVisible(false);
        ui->select_list->setVisible(false);
    }
    else
    {
        for(const std::string& profile: profiles)
        {
            ui->select_list->addItem(QString::fromStdString(profile));
        }

        connect(ui->select_list, &QListWidget::currentItemChanged, [=](){
            ui->new_edit->setText(ui->select_list->currentItem()->text());
        });
    }
}

OpenRGBProfileListDialog::~OpenRGBProfileListDialog()
{
    delete ui;
}

void OpenRGBProfileListDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

std::string OpenRGBProfileListDialog::show()
{
    std::string return_string;

    int result = this->exec();

    if(result == QDialog::Rejected)
    {
        return_string = "";
    }
    else
    {
        return_string = ui->new_edit->text().toStdString();
    }

    return(return_string);
}
