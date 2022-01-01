#include <QCloseEvent>
#include "ResourceManager.h"
#include "OpenRGBDialog2.h"
#include "OpenRGBProfileSaveDialog.h"
#include "ui_OpenRGBProfileSaveDialog.h"

#ifdef _WIN32
#include <QSettings>
#endif

Ui::OpenRGBProfileSaveDialog::OpenRGBProfileSaveDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::OpenRGBProfileSaveDialogUi)
{
    ui->setupUi(this);
}

Ui::OpenRGBProfileSaveDialog::~OpenRGBProfileSaveDialog()
{
    delete ui;
}

std::string Ui::OpenRGBProfileSaveDialog::show()
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
