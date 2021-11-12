#include "TabLabel.h"
#include <QFontMetrics>

Ui::TabLabel::TabLabel(QString icon, QString name) :
    QWidget(nullptr),
    ui(new Ui::TabLabelUi)
{
    ui->setupUi(this);
    ui->icon->setText("<img src=':/" + icon + "' height='16' width='16'  />");
    ui->name->setText(name);

    /*---------------------------------------------------------*\
    | Make sure to properly set the tab label height depending  |
    | on the font in use.                                       |
    \*---------------------------------------------------------*/
    QFontMetrics fontMetrics(font());
    QRect rect = fontMetrics.boundingRect(0, 0, width(), height(), Qt::TextWordWrap | Qt::TextExpandTabs, name);
    setFixedHeight(std::max(rect.height(), ui->icon->height()));
}

Ui::TabLabel::~TabLabel()
{
    delete ui;
}
