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

/* It's nice that currently the observers' ctors take the same function pointer
 * type; they can all use this trampoline function */
/*static*/ void proxy_label::trampoline( observer_base_t *obs, void *ctxt )
{
    (* static_cast<bound_cb_t *>(ctxt))(obs);
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
