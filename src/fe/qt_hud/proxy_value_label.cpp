#include "proxy_value_label.h"


proxy_value_label::proxy_value_label( QObject *mo, QLabel *label )
{
    _label = label;

    /* We use async Queued because BlockingQueued blocks the thread ad infinitum
     * if the other end dies (e.g. the window is closed). */
    connect( this, SIGNAL(update_label(QLabel *, char *)),
             mo,   SLOT(  update_label(QLabel *, char *)),
             Qt::QueuedConnection );
}

proxy_value_label::~proxy_value_label( void )
{
}

void proxy_value_label::cb( observer_base_t *obs )
{
    sprintf( _str,
             "%d",
             observer_base_get_value( obs )
           );

    /* Becuase we use an async connection to mo it's not safe for _str to be on
     * our stack. */
    emit update_label( _label, _str );
}
