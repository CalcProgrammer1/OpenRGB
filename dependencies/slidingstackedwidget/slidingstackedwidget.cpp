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
 */
#include "slidingstackedwidget.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>

SlidingStackedWidget::SlidingStackedWidget(QWidget *parent)

   : QStackedWidget(parent)
{

   if (parent!=nullptr) {
           m_mainwindow=parent;
       }
       else {
           m_mainwindow=this;
       }
       m_vertical=false;
       m_speed=300;
       m_animationtype = QEasingCurve::OutQuart;
       m_now=0;
       m_next=0;
       m_wrap=false;
       m_pnow=QPoint(0,0);
       m_active=false;
}

void SlidingStackedWidget::setVerticalMode(bool vertical) {

       m_vertical=vertical;
}

void SlidingStackedWidget::setSpeed(int speed) {

       m_speed = speed;
}

void SlidingStackedWidget::setAnimation(enum QEasingCurve::Type animationtype) {

       m_animationtype = animationtype;
}

void SlidingStackedWidget::setWrap(bool wrap) {

       m_wrap=wrap;
}

bool SlidingStackedWidget::slideInNext() {

       int now=currentIndex();
       if (m_wrap||(now<count()-1))
               slideInIdx(now+1);
       else
           return false;
       return true;
}

bool SlidingStackedWidget::slideInPrev() {
       int now=currentIndex();
       if (m_wrap||(now>0))
               slideInIdx(now-1);
       else
           return false;
       return true;
}

void SlidingStackedWidget::slideInIdx(int idx, enum t_direction direction) {

       if (idx>count()-1) {
           direction=m_vertical ? TOP2BOTTOM : RIGHT2LEFT;
           idx=(idx)%count();
       }
       else if (idx<0) {
           direction= m_vertical ? BOTTOM2TOP: LEFT2RIGHT;
           idx=(idx+count())%count();
       }
       slideInWgt(widget ( idx ),direction);
}

void SlidingStackedWidget::slideInWgt(QWidget * newwidget, enum t_direction direction) {

       if (m_active) {
           return;
       }
       else m_active=true;
       enum t_direction directionhint;
       int now = currentIndex();
       int next = indexOf(newwidget);
       if (now == next) {
               m_active=false;
               return;
       }
       else if (now<next){
               directionhint=m_vertical ? TOP2BOTTOM : RIGHT2LEFT;
       }
       else {
               directionhint=m_vertical ? BOTTOM2TOP : LEFT2RIGHT;
       }
       if (direction == AUTOMATIC) {
               direction=directionhint;
       }


       int offsetx=frameRect().width();
       int offsety=frameRect().height();


       widget(next)->setGeometry ( 0,  0, offsetx, offsety );
       if (direction==BOTTOM2TOP)  {
               offsetx=0;
               offsety=-offsety;
       }
       else if (direction==TOP2BOTTOM) {
               offsetx=0;
       }
       else if (direction==RIGHT2LEFT) {
               offsetx=-offsetx;
               offsety=0;
       }
       else if (direction==LEFT2RIGHT) {
               offsety=0;
       }

       QPoint pnext=widget(next)->pos();
       QPoint pnow=widget(now)->pos();
       m_pnow=pnow;
       widget(next)->move(pnext.x()-offsetx,pnext.y()-offsety);

       widget(next)->show();
       widget(next)->raise();

       QPropertyAnimation *animnow = new QPropertyAnimation(widget(now), "pos");
       animnow->setDuration(m_speed);
       animnow->setEasingCurve(m_animationtype);
       animnow->setStartValue(QPoint(pnow.x(), pnow.y()));
       animnow->setEndValue(QPoint(offsetx+pnow.x(), offsety+pnow.y()));

       QGraphicsOpacityEffect *animnow_op_eff = new QGraphicsOpacityEffect();
       widget(now)->setGraphicsEffect(animnow_op_eff);
       QPropertyAnimation *animnow_op = new QPropertyAnimation(animnow_op_eff, "opacity");
       animnow_op->setDuration(m_speed/2);
       animnow_op->setStartValue(1);
       animnow_op->setEndValue(0);
       connect(animnow_op,&QPropertyAnimation::finished,[=](){
           if(animnow_op_eff  != nullptr)
              animnow_op_eff->deleteLater();
       });

       QGraphicsOpacityEffect *animnext_op_eff = new QGraphicsOpacityEffect();
       animnext_op_eff->setOpacity(0);
       widget(next)->setGraphicsEffect(animnext_op_eff);
       QPropertyAnimation *animnext_op = new QPropertyAnimation(animnext_op_eff, "opacity");
       animnext_op->setDuration(m_speed/2);
       animnext_op->setStartValue(0);
       animnext_op->setEndValue(1);
       connect(animnext_op,&QPropertyAnimation::finished,[=](){
           if(animnext_op_eff != nullptr)
              animnext_op_eff->deleteLater();
       });

       QPropertyAnimation *animnext = new QPropertyAnimation(widget(next), "pos");
       animnext->setDuration(m_speed);
       animnext->setEasingCurve(m_animationtype);
       animnext->setStartValue(QPoint(-offsetx+pnext.x(), offsety+pnext.y()));
       animnext->setEndValue(QPoint(pnext.x(), pnext.y()));

       animgroup = new QParallelAnimationGroup;
       animgroup->addAnimation(animnow);
       animgroup->addAnimation(animnext);
       animgroup->addAnimation(animnow_op);
       animgroup->addAnimation(animnext_op);

       QObject::connect(animgroup, SIGNAL(finished()),this,SLOT(animationDoneSlot()));
       m_next=next;
       m_now=now;
       m_active=true;
       animgroup->start(QAbstractAnimation::DeleteWhenStopped);
}


void SlidingStackedWidget::animationDoneSlot()
{
   setCurrentIndex(m_next);
   widget(m_now)->hide();
   widget(m_now)->move(m_pnow);
   m_active=false;
   emit animationFinished();
}


