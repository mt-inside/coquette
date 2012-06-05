#include "proxy_value_dial.h"


proxy_value_dial::proxy_value_dial( QObject *mo, Dial *dial )
{
    _dial = dial;

    /* We use async Queued because BlockingQueued blocks the thread ad infinitum
     * if the other end dies (e.g. the window is closed). */
    connect( this, SIGNAL(update_dial(Dial *, float)),
             mo,   SLOT(  update_dial(Dial *, float)),
             Qt::QueuedConnection );
}

proxy_value_dial::~proxy_value_dial( void )
{
}

void proxy_value_dial::cb( observer_base_t *obs )
{
    float value = observer_base_get_value( obs );

    /* Becuase we use an async connection to mo it's not safe for _str to be on
     * our stack. */
    emit update_dial( _dial, value / 100 );
}
