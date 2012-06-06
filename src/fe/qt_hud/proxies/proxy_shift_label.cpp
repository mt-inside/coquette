#include "proxy_shift_label.h"
extern "C" {
#include "observers/observer_shift.h"
}


proxy_shift_label::proxy_shift_label( QObject *mo, QLabel *label )
{
    _label = label;

    /* We use async Queued because BlockingQueued blocks the thread ad infinitum
     * if the other end dies (e.g. the window is closed). */
    connect( this, SIGNAL(update_label(QLabel *, char *)),
             mo,   SLOT(  update_label(QLabel *, char *)),
             Qt::QueuedConnection );
}

proxy_shift_label::~proxy_shift_label( void )
{
}

void proxy_shift_label::cb( observer_base_t *obs )
{
    unsigned warn_level;

    observer_shift_get_shift( obs, &warn_level );

    switch( warn_level )
    {
        case 0: sprintf( _str, "     " ); break;
        case 1: sprintf( _str, "X    " ); break;
        case 2: sprintf( _str, "X X  " ); break;
        case 3: sprintf( _str, "X X X" ); break;
        case 4: sprintf( _str, "SHIFT" ); break;
    }

    emit update_label( _label, _str );
}
