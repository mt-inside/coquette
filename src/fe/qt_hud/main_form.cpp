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


    _coolant_value_label_proxy    = new proxy_value_label(      this, labelCoolantTemp  );
    _coolant_ratio_dial_proxy     = new proxy_ratio_dial(       this, dialCoolantTemp   );
    _rpm_value_label_proxy        = new proxy_value_label(      this, labelEngineSpeed  );
    _rpm_ratio_dial_proxy         = new proxy_ratio_dial(       this, dialRpm           );
    _rpm_shift_label_proxy        = new proxy_shift_label(      this, labelShift        );
    _speed_value_label_proxy      = new proxy_value_label(      this, labelRoadSpeed    );
    _speed_integral_label_proxy   = new proxy_integral_label(   this, labelDistance     );
    _speed_derivative_label_proxy = new proxy_derivative_label( this, labelAcceleration );
    _speed_zerosixty_label_proxy  = new proxy_zerosixty_label(  this, labelZerosixty    );
    _tps_ratio_label_proxy        = new proxy_ratio_label(      this, labelTpsPc        );
    _tps_ratio_dial_proxy         = new proxy_ratio_dial(       this, dialTps           );

    _stream = stream_new( 11,
        (observer_base_t *)observer_value_new( reg_engine_COOLANT_TEMP, &proxy_base::trampoline, _coolant_value_label_proxy->get_bound_cb( ) ),
        (observer_base_t *)observer_ratio_new( reg_engine_COOLANT_TEMP, &proxy_base::trampoline, _coolant_ratio_dial_proxy->get_bound_cb( ), 0, 100 ),
        (observer_base_t *)observer_value_new( reg_ENGINE_SPEED,        &proxy_base::trampoline, _rpm_value_label_proxy->get_bound_cb( ) ),
        (observer_base_t *)observer_ratio_new( reg_ENGINE_SPEED,        &proxy_base::trampoline, _rpm_ratio_dial_proxy->get_bound_cb( ), 0, 8000 ),
        (observer_base_t *)observer_shift_new( reg_ENGINE_SPEED,        &proxy_base::trampoline, _rpm_shift_label_proxy->get_bound_cb( ), 2000, 3000, 3 ),
        (observer_base_t *)observer_value_new( reg_ROAD_SPEED,          &proxy_base::trampoline, _speed_value_label_proxy->get_bound_cb( ) ),
        (observer_base_t *)observer_integral_new( reg_ROAD_SPEED,       &proxy_base::trampoline, _speed_integral_label_proxy->get_bound_cb( ) ),
        (observer_base_t *)observer_derivative_new( reg_ROAD_SPEED,     &proxy_base::trampoline, _speed_derivative_label_proxy->get_bound_cb( ), 1 ),
        (observer_base_t *)observer_zerosixty_new( reg_ROAD_SPEED,      &proxy_base::trampoline, _speed_zerosixty_label_proxy->get_bound_cb( ), 60 ),
        (observer_base_t *)observer_ratio_new( reg_TPS,                 &proxy_base::trampoline, _tps_ratio_label_proxy->get_bound_cb( ), INT_MAX, INT_MIN ),
        (observer_base_t *)observer_ratio_new( reg_TPS,                 &proxy_base::trampoline, _tps_ratio_dial_proxy->get_bound_cb( ), INT_MAX, INT_MIN )
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
