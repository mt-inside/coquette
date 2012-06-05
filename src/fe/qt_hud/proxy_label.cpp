#include "proxy_label.h"


proxy_label::proxy_label( QLabel *label )
{
    _label = label;
    _cb = new bound_cb_t(boost::bind(&proxy_label::cb, this, _1));
}

proxy_label::~proxy_label( void )
{
    delete _cb;
}

bound_cb_t *proxy_label::get_bound_cb( void )
{
    return _cb;
}

void proxy_label::cb( observer_base_t *obs )
{
    char str[256];

    sprintf( str,
             "%d",
             observer_base_get_value( obs )
           );

    _label->setText( str );
}
