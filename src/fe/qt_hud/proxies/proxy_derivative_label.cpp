#include "proxy_derivative_label.h"
extern "C" {
#include "observers/observer_derivative.h"
}


proxy_derivative_label::proxy_derivative_label( QObject *mo, QLabel *label )
{
    _label = label;

    /* We use async Queued because BlockingQueued blocks the thread ad infinitum
     * if the other end dies (e.g. the window is closed). */
    connect( this, SIGNAL(update_label(QLabel *, char *)),
             mo,   SLOT(  update_label(QLabel *, char *)),
             Qt::QueuedConnection );
}

proxy_derivative_label::~proxy_derivative_label( void )
{
}

void proxy_derivative_label::cb( observer_base_t *obs )
{
    float acceleration;

    observer_derivative_get_derivative( obs, &acceleration );

    sprintf( _str,
            "%.0fkm/h/s", /* TODO: proper unit support so that the observer can tell you this */
            acceleration
           );

    emit update_label( _label, _str );
}
