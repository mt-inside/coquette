#include <QtGui>

#include "dial.h"


Dial::Dial(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
}

QSize Dial::minimumSizeHint() const
{
    /* TODO: don't understand what this does */
    return QSize(100, 100);
}

QSize Dial::sizeHint() const
{
    return QSize(400, 200);
}

void Dial::setValue( float value )
{
    this->value = value;
    update( );
}


void Dial::paintEvent(QPaintEvent *)
{
    QPen pen = QPen( Qt::blue, 10, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin );
    QRect rect(10, 10, 100, 100);
    int startAngle = (-120 + 10) * 16; /* 0 at 3 o'clock, -ve for clockwise fomr there */
    int arcLength = ((int)(this->value * 300.0f) + 10) * -1 * 16; /* -ve for clockwise drawing */
    QPainter painter(this);

    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.drawArc(rect, startAngle, arcLength);

    /* Useful to see the extents of the widget for now */
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}
