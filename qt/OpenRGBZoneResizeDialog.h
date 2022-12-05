#ifndef OPENRGBZONERESIZEDIALOG_H
#define OPENRGBZONERESIZEDIALOG_H

#include <QDialog>
#include "ui_OpenRGBZoneResizeDialog.h"

#include "RGBController.h"

namespace Ui {
class OpenRGBZoneResizeDialog;
}

class Ui::OpenRGBZoneResizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBZoneResizeDialog(RGBController* edit_dev_ptr, unsigned int edit_zone_idx_val, QWidget *parent = nullptr);
    ~OpenRGBZoneResizeDialog();

    int show();

private slots:
    void changeEvent(QEvent *event);
    void on_ResizeSlider_valueChanged(int value);

    void on_ResizeBox_valueChanged(int arg1);

    void on_AddSegmentButton_clicked();

    void on_RemoveSegmentButton_clicked();

private:
    Ui::OpenRGBZoneResizeDialogUi *ui;
    RGBController* edit_dev;
    unsigned int edit_zone_idx;
};

#endif // OPENRGBZONERESIZEDIALOG_H
