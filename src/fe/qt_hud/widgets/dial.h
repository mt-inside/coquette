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
    Dial(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void setValue( float value );

protected:
    void paintEvent(QPaintEvent *event);

private:
    float value;
};

#endif /* _INCLUDED_DIAL_H */
