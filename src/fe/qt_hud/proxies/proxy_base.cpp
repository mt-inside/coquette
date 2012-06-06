#include "proxy_base.h"


proxy_base::proxy_base( )
{
    _cb = new bound_cb_t(boost::bind(&proxy_base::cb, this, _1));
}

proxy_base::~proxy_base( void )
{
    delete _cb;
}

bound_cb_t *proxy_base::get_bound_cb( void )
{
    return _cb;
}

/* It's nice that currently the observers' ctors take the same function pointer
 * type; they can all use this trampoline function */
/*static*/ void proxy_base::trampoline( observer_base_t *obs, void *ctxt )
{
    (* static_cast<bound_cb_t *>(ctxt))(obs);
}
