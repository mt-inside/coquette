#include <QtGui>
#include "main_form.h"

extern "C" {
#include "observers/observer_derivative.h"
#include "observers/observer_flag.h"
#include "observers/observer_integral.h"
#include "observers/observer_ratio.h"
#include "observers/observer_shift.h"
#include "observers/observer_stats.h"
#include "observers/observer_value.h"
#include "observers/observer_zerosixty.h"
#include "stream_frame.h"
}


void main_form::value_label_cb( observer_base_t *obs )
{
    QLabel *label = labelCoolantTemp;
    char str[256];

    sprintf( str,
             "%d",
             observer_base_get_value( obs )
           );

    label->setText( str );
}

void main_form::trampoline( observer_base_t *obs, void *ctxt )
{
    (* static_cast<tramp_cb *>(ctxt))(obs);
}

main_form::main_form( QWidget *parent )
{
    (void)parent;

    setupUi( this );


    /* This seems to have got us from the C cb to a member context.
     * Wrap this functionality up in a base class and derive things that are
     * constructed over the QLabel etc.
     * Then use the qt slots and signals to get from the proxy object into this
     * one on the right thread.
     */

    _cb = new tramp_cb(boost::bind(&main_form::value_label_cb, this, _1));

    _stream = stream_new( 1,
        (observer_base_t *)observer_value_new( reg_engine_COOLANT_TEMP, &trampoline, _cb )
    );

    stream_registers_start( _stream );
}

main_form::~main_form()
{
    stream_registers_end( );
    stream_delete( _stream );
}
