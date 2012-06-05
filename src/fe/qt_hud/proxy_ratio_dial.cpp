#include "proxy_ratio_dial.h"
extern "C" {
#include "observers/observer_ratio.h"
}


proxy_ratio_dial::proxy_ratio_dial( QObject *mo, Dial *dial )
{
    _dial = dial;

    /* We use async Queued because BlockingQueued blocks the thread ad infinitum
     * if the other end dies (e.g. the window is closed). */
    connect( this, SIGNAL(update_dial(Dial *, float)),
             mo,   SLOT(  update_dial(Dial *, float)),
             Qt::QueuedConnection );
}

proxy_ratio_dial::~proxy_ratio_dial( void )
{
}

void proxy_ratio_dial::cb( observer_base_t *obs )
{
    float ratio;

    observer_ratio_get_ratio( obs, &ratio );

    emit update_dial( _dial, ratio );
}
