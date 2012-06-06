#include "proxy_ratio_label.h"
extern "C" {
#include "observers/observer_ratio.h"
}


proxy_ratio_label::proxy_ratio_label( QObject *mo, QLabel *label )
{
    _label = label;

    /* We use async Queued because BlockingQueued blocks the thread ad infinitum
     * if the other end dies (e.g. the window is closed). */
    connect( this, SIGNAL(update_label(QLabel *, char *)),
             mo,   SLOT(  update_label(QLabel *, char *)),
             Qt::QueuedConnection );
}

proxy_ratio_label::~proxy_ratio_label( void )
{
}

void proxy_ratio_label::cb( observer_base_t *obs )
{
    float ratio;

    observer_ratio_get_ratio( obs, &ratio );

    sprintf( _str,
             "%.0f",
             ratio * 100
           );

    /* Becuase we use an async connection to mo it's not safe for _str to be on
     * our stack. */
    emit update_label( _label, _str );
}
