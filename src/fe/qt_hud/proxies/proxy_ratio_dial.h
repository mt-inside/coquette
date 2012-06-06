/* Monitors an observer_ratio and renders to a Qdial. */

#ifndef _INCLUDED_PROXY_RATIO_DIAL_H
#define _INCLUDED_PROXY_RATIO_DIAL_H

#include <QtGui>
#include "widgets/dial.h"
#include "proxy_base.h"


class proxy_ratio_dial : public proxy_base
{
    Q_OBJECT

    public:
    proxy_ratio_dial( QObject *marshall_object, Dial *dial );
    virtual ~proxy_ratio_dial( void );

    signals:
    void update_dial( Dial *dial, float ratio );

    private:
    virtual void cb( observer_base_t *obs );

    Dial *_dial;
};

#endif /* defined _INCLUDED_PROXY_RATIO_DIAL_H */
