#ifndef QTOOLTIPEDSLIDER_H
#define QTOOLTIPEDSLIDER_H

#include <QSlider>

class QTooltipedSlider : public QSlider
{
    Q_OBJECT

public:
    explicit QTooltipedSlider(QWidget *parent = nullptr);
};

#endif // QTOOLTIPEDSLIDER_H
