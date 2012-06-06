/* Monitors an observer_ratio and renders to a Qdial. */

#ifndef _INCLUDED_PROXY_ZEROSIXTY_LABEL_H
#define _INCLUDED_PROXY_ZEROSIXTY_LABEL_H

#include <QtGui>
#include "proxy_base.h"


class proxy_zerosixty_label : public proxy_base
{
    Q_OBJECT

    public:
    proxy_zerosixty_label( QObject *marshall_object, QLabel *label );
    virtual ~proxy_zerosixty_label( void );

    signals:
    void update_label( QLabel *label, char *str );

    private:
    virtual void cb( observer_base_t *obs );

    QLabel *_label;
    char _str[256];
};

#endif /* defined _INCLUDED_PROXY_ZEROSIXTY_LABEL_H */
