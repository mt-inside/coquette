#include <QtGui>

#include "dial.h"


Dial::Dial(QWidget *parent, int startAngle, int maxArcLength)
    : QWidget(parent)
{
    _start  = startAngle;
    _length = maxArcLength;
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

void Dial::startAngle( int value )
{
    _start = value;
    update( );
}

void Dial::maxArcLength( int value )
{
    _length = value;
    update( );
}

void Dial::value( float value )
{
    _value = value;
    update( );
}


static void drawArc(QPainter &painter, QRect &rect, int start, int length)
{
    painter.drawArc(rect, (-90 - start) * 16, length * 16 * -1);
}
void Dial::paintEvent(QPaintEvent *)
{
    QPen penValue = QPen( Qt::blue,  10, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin );
    QPen penMarks = QPen( Qt::black, 10, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin );
    QRect rect(10, 10, 100, 100);
    int arcLength = (int)(_value * (float)_length);
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(penMarks); drawArc(painter, rect, _start - 5, 5);
    painter.setPen(penValue); drawArc(painter, rect, _start, arcLength);
    painter.setPen(penMarks); drawArc(painter, rect, _start + _length, 5);

    /* Useful to see the extents of the widget for now */
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}
