#include "OpenRGBZoneResizeDialog.h"

using namespace Ui;

OpenRGBZoneResizeDialog::OpenRGBZoneResizeDialog(int size_min, int size_max, int size_current, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenRGBZoneResizeDialogUi)
{
    ui->setupUi(this);

    ui->ResizeSlider->setRange(size_min, size_max);
    ui->ResizeBox->setRange(size_min, size_max);

    ui->ResizeSlider->setValue(size_current);
    ui->ResizeBox->setValue(size_current);
}

OpenRGBZoneResizeDialog::~OpenRGBZoneResizeDialog()
{
    delete ui;
}

void OpenRGBZoneResizeDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void Ui::OpenRGBZoneResizeDialog::on_ResizeSlider_valueChanged(int value)
{
    ui->ResizeBox->blockSignals(true);
    ui->ResizeBox->setValue(value);
    ui->ResizeBox->blockSignals(false);
}

void Ui::OpenRGBZoneResizeDialog::on_ResizeBox_valueChanged(int value)
{
    ui->ResizeSlider->blockSignals(true);
    ui->ResizeSlider->setValue(value);
    ui->ResizeSlider->blockSignals(false);
}

int Ui::OpenRGBZoneResizeDialog::show()
{
    int ret_val = 0;

    int result = this->exec();

    if(result == QDialog::Rejected)
    {
        ret_val = -1;
    }
    else
    {
        ret_val = ui->ResizeBox->value();
    }

    return(ret_val);
}
