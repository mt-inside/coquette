#ifndef _INCLUDED_DIAL_H
#define _INCLUDED_DIAL_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>

class Dial : public QWidget
{
    Q_OBJECT

public:
    Dial(QWidget *parent = 0, int startAngle = 30, int maxArcLength = 300);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void startAngle( int value );
    void maxArcLength( int value );
    void value( float value );

protected:
    void paintEvent(QPaintEvent *event);

private:
    int _start;
    int _length;
    float _value;
};

#endif /* _INCLUDED_DIAL_H */
