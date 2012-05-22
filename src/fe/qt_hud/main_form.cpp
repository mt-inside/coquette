#include <QtGui>
#include "main_form.h"

extern "C" {
#include "observers/observer_derivative.h"
#include "observers/observer_integral.h"
#include "observers/observer_ratio.h"
#include "observers/observer_shift.h"
#include "observers/observer_stats.h"
#include "observers/observer_value.h"
#include "observers/observer_zerosixty.h"
#include "stream_frame.h"
}

static void value_label_cb( observer_base_t *obs, void *ctxt )
{
    QLabel *label = (QLabel *)ctxt;
    char str[256];

    sprintf( str,
             "%d",
             observer_base_get_value( obs )
           );

    label->setText( str );
}

static void distance_label_cb( observer_base_t *obs, void *ctxt )
{
    QLabel *label = (QLabel *)ctxt;
    int64_t distance;
    char str[256];

    observer_integral_get_integral( obs, &distance );
    sprintf( str,
             "%.3lfkm",
             (double)distance / 3600.0f
           );

    label->setText( str );
}

static void acceleration_label_cb( observer_base_t *obs, void *ctxt )
{
    QLabel *label = (QLabel *)ctxt;
    float acceleration;
    char str[256];

    observer_derivative_get_derivative( obs, &acceleration );
    sprintf( str,
             "%.0fkm/h/s",
             acceleration
           );

    label->setText( str );
}

static void ratio_label_cb( observer_base_t *obs, void *ctxt )
{
    QLabel *label = (QLabel *)ctxt;
    float ratio;
    char str[256];

    observer_ratio_get_ratio( obs, &ratio );

    sprintf( str,
             "%.0f",
             ratio * 100
           );

    label->setText( str );
}

static void coolant_dial_cb( observer_base_t *obs, void *ctxt )
{
    Dial *dial = (Dial *)ctxt;

    dial->value( (float)observer_base_get_value( obs ) / 100 );
}

static void rpm_dial_cb( observer_base_t *obs, void *ctxt )
{
    Dial *dial = (Dial *)ctxt;

    dial->value( (float)observer_base_get_value( obs ) / 8000 );
}

static void ratio_dial_cb( observer_base_t *obs, void *ctxt )
{
    Dial *dial = (Dial *)ctxt;
    float ratio;

    observer_ratio_get_ratio( obs, &ratio );

    dial->value( ratio );
}

static void stats_label_cb( observer_base_t *obs, void *ctxt )
{
    QLabel *label = (QLabel *)ctxt;
    int min, max, mean, stdev;
    char str[256];

    observer_stats_get_stats( obs, &min, &max, &mean, &stdev );

    sprintf( str,
             "current: %d\tmin: %d\tmean: %d\tmax: %d\tstdev: %d",
             observer_base_get_value( obs ),
             min,
             mean,
             max,
             stdev
           );

    label->setText( str );
}

static void shift_label_cb( observer_base_t *obs, void *ctxt )
{
    QLabel *label = (QLabel *)ctxt;
    unsigned warn_level;
    char str[256];

    observer_shift_get_shift( obs, &warn_level );

    switch( warn_level )
    {
         case 0: sprintf( str, "     " ); break;
         case 1: sprintf( str, "X    " ); break;
         case 2: sprintf( str, "X X  " ); break;
         case 3: sprintf( str, "X X X" ); break;
         case 4: sprintf( str, "SHIFT" ); break;
    }

    label->setText( str );
}

static void zerosixty_label_cb( observer_base_t *obs, void *ctxt )
{
    QLabel *label = (QLabel *)ctxt;
    float zerosixty;
    char str[256];

    observer_zerosixty_get_zerosixty( obs, &zerosixty );

    sprintf( str, "%.2fs", zerosixty );

    label->setText( str );
}

main_form::main_form( QWidget *parent )
{
    (void)parent;
    (void)stats_label_cb;

    setupUi( this );


    _stream = stream_new( 11,
        (observer_base_t *)observer_value_new( reg_engine_COOLANT_TEMP, &value_label_cb, (void *)labelCoolantTemp ),
        (observer_base_t *)observer_value_new( reg_engine_COOLANT_TEMP, &coolant_dial_cb, (void *)dialCoolantTemp ),
        (observer_base_t *)observer_value_new( reg_ENGINE_SPEED, &value_label_cb, (void *)labelEngineSpeed ),
        (observer_base_t *)observer_value_new( reg_ENGINE_SPEED, &rpm_dial_cb, (void *)dialRpm ),
        (observer_base_t *)observer_shift_new( reg_ENGINE_SPEED, &shift_label_cb, (void *)labelShift, 2000, 3000, 3 ),
        (observer_base_t *)observer_value_new( reg_ROAD_SPEED, &value_label_cb, (void *)labelRoadSpeed ),
        (observer_base_t *)observer_integral_new( reg_ROAD_SPEED, &distance_label_cb, (void *)labelDistance ),
        (observer_base_t *)observer_derivative_new( reg_ROAD_SPEED, &acceleration_label_cb, (void *)labelAcceleration, 1 ),
        (observer_base_t *)observer_zerosixty_new( reg_ROAD_SPEED, &zerosixty_label_cb, (void *)labelZerosixty, 60 ),
        (observer_base_t *)observer_ratio_new( reg_TPS, &ratio_label_cb, (void *)labelTpsPc ),
        (observer_base_t *)observer_ratio_new( reg_TPS, &ratio_dial_cb, (void *)dialTps )
    );

    stream_registers_start( _stream );
}

main_form::~main_form()
{
    stream_registers_end( );
    stream_delete( _stream );
}
