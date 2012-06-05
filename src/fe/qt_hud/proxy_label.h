/* Class that wraps a QLabel and provides a function that can be passed to
 * things expecting a C-style function pointer.
 * Performs 2 functions:
 * - Gets call back into member context using trampoline, boost::bind, etc
 * - Gets call back onto the thread that marshall_object lives on
 */

#ifndef _INCLUDED_PROXY_LABEL_H
#define _INCLUDED_PROXY_LABEL_H

#include <QtGui>

#include "boost/bind.hpp"
#include "boost/function.hpp"

extern "C" {
#include "observers/observer_base.h"
}

typedef boost::function<void (observer_base_t *)> bound_cb_t;

class proxy_label : public QObject
{
    Q_OBJECT

    public:
    /* masrshall object must live on the thread that you want to be marshalled
     * back onto */
    proxy_label( QObject *marshall_object, QLabel *label );
    virtual ~proxy_label( void );
    bound_cb_t *get_bound_cb( void );
    static void trampoline( observer_base_t *obs, void *ctxt );

    signals:
    void update_label( QLabel *label, char *str );

    private:
    void cb( observer_base_t *obs );

    QLabel *_label;
    bound_cb_t *_cb;
    char _str[256];
};

#endif /* defined _INCLUDED_PROXY_LABEL_H */
