/*
 *  MIT License
    Copyright (c) 2020 Tim Schneeberger (ThePBone) <tim.schneeberger(at)outlook.de>
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 *
 *  Copied from https://github.com/Qt-Widgets/SlidingStackedWidget-1
 */

#ifndef SLIDINGSTACKEDWIDGET_H
#define SLIDINGSTACKEDWIDGET_H
#include <QStackedWidget>
#include <QEasingCurve>
#include <QParallelAnimationGroup>

class SlidingStackedWidget : public QStackedWidget {

       Q_OBJECT
public:
       //! Animation direction
       enum t_direction {
               LEFT2RIGHT,
               RIGHT2LEFT,
               TOP2BOTTOM,
               BOTTOM2TOP,
               AUTOMATIC
       };
       SlidingStackedWidget(QWidget *parent);

public slots:
       //! Set animation speed
       void setSpeed(int speed);
       //! Set easing curve
       void setAnimation(enum QEasingCurve::Type animationtype);
       //! Change positioning mode
       void setVerticalMode(bool vertical = true);
       //! Enables page wrap for \c slideInNext and \c slideInPrev
       void setWrap(bool wrap);
       //! Slide to next page
       bool slideInNext();
       //! Slide to previous page
       bool slideInPrev();
       //! Slide to page x
       void slideInIdx(int idx, enum t_direction direction=AUTOMATIC);
        //! Slide to page with widget
       void slideInWgt(QWidget * widget, enum t_direction direction=AUTOMATIC);
signals:
       //! Animation is finished
       void animationFinished(void);

protected slots:
       void animationDoneSlot(void);

protected:
       QWidget *m_mainwindow;
       int m_speed;
       enum QEasingCurve::Type m_animationtype;
       bool m_vertical;
       int m_now;
       int m_next;
       bool m_wrap;
       QPoint m_pnow;
       bool m_active;
       QList<QWidget*> blockedPageList;
       QParallelAnimationGroup *animgroup;
};
#endif // SLIDINGSTACKEDWIDGET_H
