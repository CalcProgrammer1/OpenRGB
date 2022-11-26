#ifndef OPENRGBPROFILESAVEDIALOG_H
#define OPENRGBPROFILESAVEDIALOG_H

#include <QDialog>
#include "ui_OpenRGBProfileSaveDialog.h"
#include "OpenRGBDialog2.h"

namespace Ui
{
class OpenRGBProfileSaveDialog;
}

class Ui::OpenRGBProfileSaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRGBProfileSaveDialog(QWidget *parent = nullptr);
    ~OpenRGBProfileSaveDialog();

    std::string show();

private:
    Ui::OpenRGBProfileSaveDialogUi *ui;

private slots:
    void changeEvent(QEvent *event);
};

#endif // OPENRGBPROFILESAVEDIALOG_H
