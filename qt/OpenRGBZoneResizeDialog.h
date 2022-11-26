#ifndef OPENRGBZONERESIZEDIALOG_H
#define OPENRGBZONERESIZEDIALOG_H

#include <QDialog>
#include "ui_OpenRGBZoneResizeDialog.h"

namespace Ui {
class OpenRGBZoneResizeDialog;
}

class Ui::OpenRGBZoneResizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBZoneResizeDialog(int size_min, int size_max, int size_current, QWidget *parent = nullptr);
    ~OpenRGBZoneResizeDialog();

    int show();

private slots:
    void changeEvent(QEvent *event);
    void on_ResizeSlider_valueChanged(int value);

    void on_ResizeBox_valueChanged(int arg1);

private:
    Ui::OpenRGBZoneResizeDialogUi *ui;
};

#endif // OPENRGBZONERESIZEDIALOG_H
