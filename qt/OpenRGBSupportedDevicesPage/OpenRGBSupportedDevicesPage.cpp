/*---------------------------------------------------------*\
| OpenRGBSupportedDevicePage.cpp                            |
|                                                           |
|   User interface for enabling and disabling devices       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "OpenRGBSupportedDevicesPage.h"
#include "ui_OpenRGBSupportedDevicesPage.h"
#include "ResourceManager.h"
#include "OpenRGBHardwareIDsDialog.h"

OpenRGBSupportedDevicesPage::OpenRGBSupportedDevicesPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBSupportedDevicesPage)
{
    ui->setupUi(this);

    /*-----------------------------------------------------*\
    | Create a detector table model and a sort model and    |
    | set them                                              |
    \*-----------------------------------------------------*/
    detectorTableModel = new DetectorTableModel;
    detectorSortModel = new QSortFilterProxyModel;

    detectorSortModel->setSourceModel(detectorTableModel);
    ui->SupportedDevicesTable->setModel(detectorSortModel);

    /*-----------------------------------------------------*\
    | Disable header, enable sorting, and sort in ascending |
    | order                                                 |
    \*-----------------------------------------------------*/
    ui->SupportedDevicesTable->verticalHeader()->setVisible(0);
    ui->SupportedDevicesTable->setSortingEnabled(true);
    ui->SupportedDevicesTable->sortByColumn(0, Qt::AscendingOrder);

    /*-----------------------------------------------------*\
    | Resize columns to fit the contents                    |
    \*-----------------------------------------------------*/
    ui->SupportedDevicesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
}

OpenRGBSupportedDevicesPage::~OpenRGBSupportedDevicesPage()
{
    delete ui;
}

void OpenRGBSupportedDevicesPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void OpenRGBSupportedDevicesPage::on_SaveButton_clicked()
{
    detectorTableModel->applySettings();
}

void OpenRGBSupportedDevicesPage::on_GetHardwareIDsButton_clicked()
{
    OpenRGBHardwareIDsDialog dialog(this);
    dialog.show();
}

void OpenRGBSupportedDevicesPage::on_Filter_textChanged(const QString &arg1)
{
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    detectorSortModel->setFilterRegularExpression(QRegularExpression(arg1 , QRegularExpression::CaseInsensitiveOption));
#else
    detectorSortModel->setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive));
#endif
}

void OpenRGBSupportedDevicesPage::on_ToggleAllCheckbox_toggled(const bool checked)
{
    detectorTableModel->toggleAll(checked, detectorSortModel);
}
