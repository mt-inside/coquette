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


    _coolant_label_proxy = new proxy_value_label( this, labelCoolantTemp );
    _coolant_dial_proxy  = new proxy_ratio_dial(  this, dialCoolantTemp  );
    _rpm_label_proxy     = new proxy_value_label( this, labelEngineSpeed );
    _rpm_dial_proxy      = new proxy_ratio_dial(  this, dialRpm );
    _rpm_shift_proxy     = new proxy_shift_label( this, labelShift );

    _stream = stream_new( 5,
        (observer_base_t *)observer_value_new( reg_engine_COOLANT_TEMP, &proxy_base::trampoline, _coolant_label_proxy->get_bound_cb( ) ),
        (observer_base_t *)observer_ratio_new( reg_engine_COOLANT_TEMP, &proxy_base::trampoline, _coolant_dial_proxy->get_bound_cb( ), 0, 100 ),
        (observer_base_t *)observer_value_new( reg_ENGINE_SPEED,        &proxy_base::trampoline, _rpm_label_proxy->get_bound_cb( ) ),
        (observer_base_t *)observer_ratio_new( reg_ENGINE_SPEED,        &proxy_base::trampoline, _rpm_dial_proxy->get_bound_cb( ), 0, 8000 ),
        (observer_base_t *)observer_shift_new( reg_ENGINE_SPEED,        &proxy_base::trampoline, _rpm_shift_proxy, 2000, 3000, 3 )
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

void main_form::update_dial( Dial *dial, float value )
{
    dial->value( value );
}
