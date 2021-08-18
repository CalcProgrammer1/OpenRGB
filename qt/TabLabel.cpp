#include "TabLabel.h"

Ui::TabLabel::TabLabel(QString icon, QString name) :
    QWidget(nullptr),
    ui(new Ui::TabLabelUi)
{
    ui->setupUi(this);
    ui->icon->setText("<img src=':/" + icon + "' height='16' width='16'  />");
    ui->name->setText(name);
    setMinimumHeight(ui->name->height());
}

Ui::TabLabel::~TabLabel()
{
    delete ui;
}
