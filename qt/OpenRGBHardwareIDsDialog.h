#ifndef OPENRGBHARDWAREIDSDIALOG_H
#define OPENRGBHARDWAREIDSDIALOG_H

#include "ui_OpenRGBHardwareIDsDialog.h"

#include <QDialog>

namespace Ui
{
    class OpenRGBHardwareIDsDialog;
}

class Ui::OpenRGBHardwareIDsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBHardwareIDsDialog(QWidget *parent = nullptr);
    ~OpenRGBHardwareIDsDialog();

    int show();

private slots:
    void on_CopyToClipboardButton_clicked();

private:
    Ui::OpenRGBHardwareIDsDialogUi *ui;
    QStringList strings;
};

#endif // OPENRGBHARDWAREIDSDIALOG_H
