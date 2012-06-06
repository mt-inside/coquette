/* Monitors an observer_flag and renders to a QLabel. */

#ifndef _INCLUDED_PROXY_FLAG_LABEL_H
#define _INCLUDED_PROXY_FLAG_LABEL_H

#include <QtGui>
#include "proxy_base.h"


class proxy_flag_label : public proxy_base
{
    Q_OBJECT

    public:
    proxy_flag_label( QObject *marshall_object, QLabel *label );
    virtual ~proxy_flag_label( void );

    signals:
    void update_label_style( QLabel *label, char *str );

    private:
    virtual void cb( observer_base_t *obs );

    QLabel *_label;
    char _str[256];
};

#endif /* defined _INCLUDED_PROXY_FLAG_LABEL_H */
