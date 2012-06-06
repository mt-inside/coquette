/* Class that wraps a Q<foo> and provides a function that can be passed to
 * things expecting a C-style function pointer.
 * Performs 2 functions:
 * - Gets call back into member context using trampoline, boost::bind, etc
 * - Gets call back onto the thread that marshal_object lives on
 *
 * This is the base class. It has two attributes:
 * - It knows how to read from a particular type of observer.
 * - It knows how to update a particular type of widget.
 * However, the two are intertwined as different widgets will render different
 * types of info differently. Ergo, derived classes currently deal with both
 * aspects (rather than some kind of visitor pattern / AOP mash-up). It is not
 * expected to get a combinatorial explosion of subclasses as most widgets can't
 * render most kinds of observer (it's a sparse matrix).
 */

#ifndef _INCLUDED_PROXY_BASE_H
#define _INCLUDED_PROXY_BASE_H

#include <QtGui>

#include "boost/bind.hpp"
#include "boost/function.hpp"

extern "C" {
#include "observers/observer_base.h"
}

typedef boost::function<void (observer_base_t *)> bound_cb_t;

class proxy_base : public QObject
{
    Q_OBJECT

    public:
    /* marshal object must live on the thread that you want to be marshalled
     * back onto */
    proxy_base( );
    virtual ~proxy_base( void );
    bound_cb_t *get_bound_cb( void );
    static void trampoline( observer_base_t *obs, void *ctxt );

    private:
    /* Contains the actual logic for reading the particular observer type and
     * updating the particular control type. */
    virtual void cb( observer_base_t *obs ) = 0;

    bound_cb_t *_cb;
};

#endif /* defined _INCLUDED_PROXY_BASE_H */
