#include "proxy_integral_label.h"
extern "C" {
#include "observers/observer_integral.h"
}


proxy_integral_label::proxy_integral_label( QObject *mo, QLabel *label )
{
    _label = label;

    /* We use async Queued because BlockingQueued blocks the thread ad infinitum
     * if the other end dies (e.g. the window is closed). */
    connect( this, SIGNAL(update_label(QLabel *, char *)),
             mo,   SLOT(  update_label(QLabel *, char *)),
             Qt::QueuedConnection );
}

proxy_integral_label::~proxy_integral_label( void )
{
}

void proxy_integral_label::cb( observer_base_t *obs )
{
    int64_t distance;

    observer_integral_get_integral( obs, &distance );

    sprintf( _str,
            "%.3lfkm", /* TODO: proper unit support so that the observer can tell you this */
            (double)distance / 3600.0f /* TODO: proper unit support so that the observer can tell you this */
           );

    emit update_label( _label, _str );
}
