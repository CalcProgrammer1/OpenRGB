/*---------------------------------------------------------*\
| QTooltipedSlider.h                                        |
|                                                           |
|   Qt widget for OpenRGB tooltiped slider                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QSlider>

class QTooltipedSlider : public QSlider
{
    Q_OBJECT

public:
    explicit QTooltipedSlider(QWidget *parent = nullptr);
};
