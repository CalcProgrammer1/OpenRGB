#include "TabLabel.h"
#include <QFontMetrics>

Ui::TabLabel::TabLabel(QString icon, QString name, char* original, char* context) :
    QWidget(nullptr),
    ui(new Ui::TabLabelUi)
{
    ui->setupUi(this);
    ui->icon->setText("<img src=':/" + icon + "' height='16' width='16'  />");
    ui->name->setText(name);
    label   = original;
    ctxt    = context;
}

Ui::TabLabel::~TabLabel()
{
    delete ui;
}

void Ui::TabLabel::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        /*-----------------------------------------------------*\
        | Storing the base string in label                      |
        |   enables switching between multiple languages        |
        | The context needs to be stored as the translation     |
        |   file requires the originating context               |
        \*-----------------------------------------------------*/
        QApplication* app       = static_cast<QApplication *>(QApplication::instance());

        ui->name->setText(app->translate(ctxt, label));
    }
}
