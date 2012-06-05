/* Monitors an observer_value and renders to a Qdial. */

#ifndef _INCLUDED_PROXY_VALUE_DIAL_H
#define _INCLUDED_PROXY_VALUE_DIAL_H

#include <QtGui>
#include "widgets/dial.h"
#include "proxy_base.h"


class proxy_value_dial : public proxy_base
{
    Q_OBJECT

    public:
    proxy_value_dial( QObject *marshall_object, Dial *dial );
    virtual ~proxy_value_dial( void );

    signals:
    void update_dial( Dial *dial, float value );

    private:
    virtual void cb( observer_base_t *obs );

    Dial *_dial;
};

#endif /* defined _INCLUDED_PROXY_VALUE_DIAL_H */
