#include "proxy_zerosixty_label.h"
extern "C" {
#include "observers/observer_zerosixty.h"
}


proxy_zerosixty_label::proxy_zerosixty_label( QObject *mo, QLabel *label )
{
    _label = label;

    /* We use async Queued because BlockingQueued blocks the thread ad infinitum
     * if the other end dies (e.g. the window is closed). */
    connect( this, SIGNAL(update_label(QLabel *, char *)),
             mo,   SLOT(  update_label(QLabel *, char *)),
             Qt::QueuedConnection );
}

proxy_zerosixty_label::~proxy_zerosixty_label( void )
{
}

void proxy_zerosixty_label::cb( observer_base_t *obs )
{
    float zerosixty;

    observer_zerosixty_get_zerosixty( obs, &zerosixty );

    sprintf( _str,
            "%.2fs",
            zerosixty
           );

    emit update_label( _label, _str );
}
