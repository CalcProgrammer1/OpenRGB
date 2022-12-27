#include "QTooltipedSlider.h"
#include <QToolTip>

QTooltipedSlider::QTooltipedSlider(QWidget *parent) :
    QSlider(parent)
{    
    connect(this, &QSlider::sliderMoved,[&](int value) {
        QToolTip::showText(QCursor::pos(), QString("%1").arg(value), nullptr);
    });
}
