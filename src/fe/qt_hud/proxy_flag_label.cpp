#include "proxy_flag_label.h"
extern "C" {
#include "observers/observer_flag.h"
}


proxy_flag_label::proxy_flag_label( QObject *mo, QLabel *label )
{
    _label = label;

    /* We use async Queued because BlockingQueued blocks the thread ad infinitum
     * if the other end dies (e.g. the window is closed). */
    connect( this, SIGNAL(update_label_style(QLabel *, char *)),
             mo,   SLOT(  update_label_style(QLabel *, char *)),
             Qt::QueuedConnection );
}

proxy_flag_label::~proxy_flag_label( void )
{
}

void proxy_flag_label::cb( observer_base_t *obs )
{
    int flag;

    observer_flag_get_flag( obs, &flag );

    if( flag )
    {
        strcpy( _str, "background-color: yellow;" );
    }
    else
    {
        strcpy( _str, "" );
    }

    /* Becuase we use an async connection to mo it's not safe for _str to be on
     * our stack. */
    emit update_label_style( _label, _str );
}
