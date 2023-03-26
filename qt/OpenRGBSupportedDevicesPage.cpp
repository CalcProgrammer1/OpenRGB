#include "OpenRGBSupportedDevicesPage.h"
#include "ui_OpenRGBSupportedDevicesPage.h"
#include "ResourceManager.h"
#include "OpenRGBHardwareIDsDialog.h"

using namespace Ui;

OpenRGBSupportedDevicesPage::OpenRGBSupportedDevicesPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenRGBSupportedDevicesPageUi)
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
#ifdef _QT6
    detectorSortModel->setFilterRegularExpression(QRegularExpression(arg1 , QRegularExpression::CaseInsensitiveOption));
#else
    detectorSortModel->setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive));
#endif
}

void OpenRGBSupportedDevicesPage::on_ToggleAllCheckbox_toggled(const bool checked)
{
    detectorTableModel->toggleAll(checked, detectorSortModel);
}
