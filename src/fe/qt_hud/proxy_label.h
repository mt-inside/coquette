/* Class that wraps a QLabel and provides a function that can be passed to
 * things expecting a C-style function pointer */

#ifndef _INCLUDED_PROXY_LABEL_H
#define _INCLUDED_PROXY_LABEL_H

#include <QtGui>

#include "boost/bind.hpp"
#include "boost/function.hpp"

extern "C" {
#include "observers/observer_base.h"
}

typedef boost::function<void (observer_base_t *)> bound_cb_t;

class proxy_label
{
    public:
    proxy_label( QLabel *label );
    virtual ~proxy_label( void );
    bound_cb_t *get_bound_cb( void );

    private:
    void cb( observer_base_t *obs );

    QLabel *_label;
    bound_cb_t *_cb;
};

#endif /* defined _INCLUDED_PROXY_LABEL_H */
