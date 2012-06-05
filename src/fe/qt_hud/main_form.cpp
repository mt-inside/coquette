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


main_form::main_form( QWidget *parent )
{
    (void)parent;

    setupUi( this );


    _label_proxy = new proxy_value_label( this, labelCoolantTemp );

    _stream = stream_new( 1,
        (observer_base_t *)observer_value_new( reg_engine_COOLANT_TEMP, &proxy_value_label::trampoline, _label_proxy->get_bound_cb( ) )
    );

    stream_registers_start( _stream );
}

main_form::~main_form()
{
    stream_registers_end( );
    stream_delete( _stream );
}

void main_form::update_label( QLabel *label, char *value )
{
    label->setText( value );
}
