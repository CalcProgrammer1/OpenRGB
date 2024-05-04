/*---------------------------------------------------------*\
| QTooltipedSlider.cpp                                      |
|                                                           |
|   Qt widget for OpenRGB tooltiped slider                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QToolTip>
#include "QTooltipedSlider.h"

QTooltipedSlider::QTooltipedSlider(QWidget *parent) :
    QSlider(parent)
{
    connect(this, &QSlider::sliderMoved,[&](int value) {
        QToolTip::showText(QCursor::pos(), QString("%1").arg(value), nullptr);
    });
}
