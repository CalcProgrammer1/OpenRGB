/*---------------------------------------------------------*\
| OpenRGBSegmentExportDialog.cpp                            |
|                                                           |
|   User interface entry for OpenRGB segment export dialog  |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      22 Feb 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <QFileDialog>
#include "OpenRGBSegmentExportDialog.h"
#include "ui_OpenRGBSegmentExportDialog.h"

OpenRGBSegmentExportDialog::OpenRGBSegmentExportDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::OpenRGBSegmentExportDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

OpenRGBSegmentExportDialog::~OpenRGBSegmentExportDialog()
{
    delete ui;
}

void OpenRGBSegmentExportDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

bool OpenRGBSegmentExportDialog::show()
{
    int result = this->exec();

    if(result == QDialog::Rejected)
    {
        return(false);
    }
    else
    {
        filename    = ui->LineEditFile->text();
        vendor      = ui->LineEditVendor->text();
        device      = ui->LineEditDevice->text();
    }

    return(true);
}

QString OpenRGBSegmentExportDialog::GetDevice()
{
    return(device);
}

QString OpenRGBSegmentExportDialog::GetFilename()
{
    return(filename);
}

QString OpenRGBSegmentExportDialog::GetVendor()
{
    return(vendor);
}

void OpenRGBSegmentExportDialog::on_ButtonChooseFile_clicked()
{
    QFileDialog file_dialog(this);

    file_dialog.setFileMode(QFileDialog::AnyFile);
    file_dialog.setNameFilter("*.json");
    file_dialog.setDefaultSuffix(".json");
    file_dialog.setWindowTitle("Export Configuration");

    if(file_dialog.exec())
    {
        QStringList     selected_files  = file_dialog.selectedFiles();
        QString         filename        = selected_files[0];

        ui->LineEditFile->setText(filename);
    }
}

