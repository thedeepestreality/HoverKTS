#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>

#include "customjoystick.h"

CustomJoystick::CustomJoystick(QWidget *parent)
    : QWidget(parent)
{
    state = false;
    setWindowTitle(tr("Custom Joystick"));
}

void CustomJoystick::resizeEvent(QResizeEvent *event)
{
    int minDim = std::min(event->size().width(),event->size().height());
    if (minDim%2 == 0)
        --minDim;
    this->jRad = 0.05*minDim;
    this->radius = minDim/2 - jRad;
    this->center = QPoint(minDim/2,minDim/2);
    jPos = center;
    //jRad =

}

void CustomJoystick::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << event->pos().x() << " " << event->pos().y();
    if (state)
    {

        QPoint tmpP = event->pos()-offset;
        if (dist(tmpP,center)<radius)
        {
            //qDebug() << tmpP;
            jPos = tmpP;

        }
        else
        {
            double angle = atan2(center.y()-tmpP.y(),center.x()-tmpP.x());

            jPos = center - QPoint(cos(angle)*radius,sin(angle)*radius);
            offset = QPoint(0,0);
            //qDebug()<<jPos;
        }
        this->repaint();
    }
}

void CustomJoystick::mousePressEvent(QMouseEvent *event)
{
    //qDebug() << event->pos().x() << " " << event->pos().y();
    //int mX = event->x();
    //int mY = event->y();
    //double dist = sqrt((mX-jPos.x())*(mX-jPos.x())+(mY-jPos.y())*(mY-jPos.y()));
    if (dist(jPos,event->pos()) <= 7)
    {
        state = true;
        offset = event->pos()-jPos;
    }
    else
        state = false;
}

void CustomJoystick::fillEllipse(QPainter* painter, QPoint center, int rad, QColor color) const
{
    painter->setBrush( QBrush( color ) );
    painter->setPen( Qt::NoPen );
    painter->drawEllipse(center,rad,rad);

    painter->setPen( QPen( Qt::black ) );
    painter->setBrush( Qt::NoBrush );
    painter->drawEllipse(center,rad,rad);
}

double CustomJoystick::dist(QPoint p1, QPoint p2) const
{
    return sqrt((p1.x()-p2.x())*(p1.x()-p2.x())+(p1.y()-p2.y())*(p1.y()-p2.y()));
}

void CustomJoystick::paintEvent(QPaintEvent *)
{
      QPainter painter(this);
      painter.setPen(QColor(0,0,0));

      fillEllipse(&painter,center,radius,QColor(164,164,164));

      painter.drawLine(jRad+radius,jRad,jRad+radius,jRad+2*radius);
      painter.drawLine(jRad,jRad+radius,jRad+2*radius,jRad+radius);

      fillEllipse(&painter,jPos,jRad,Qt::red);

}

