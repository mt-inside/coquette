#include <QtGui>
#include "main_form.h"

extern "C" {
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
    char str[256];
    unsigned warn_level;

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
    char str[256];
    unsigned zerosixty;
    double foo;

    observer_zerosixty_get_zerosixty( obs, &zerosixty );
    foo = (double)zerosixty / 1000000.0f;

    sprintf( str, "%0.00lfs", foo );

    label->setText( str );
}

main_form::main_form( QWidget *parent )
{
    (void)parent;

    setupUi( this );


    const unsigned stream_count = 4;
    stream_t **streams = (stream_t **)malloc( sizeof(stream_t *) * stream_count );
    unsigned i;


    for( i = 0; i < stream_count; ++i )
    {
        streams[i] = (stream_t *)malloc( sizeof(stream_t) );
    }


    (void)stats_label_cb;

    streams[0]->reg = reg_engine_COOLANT_TEMP;
    streams[0]->observers_len = 1;
    streams[0]->observers = (observer_base_t **)malloc( streams[0]->observers_len * sizeof(observer_base_t *) );
    streams[0]->observers[0] = (observer_base_t *)observer_value_new( &value_label_cb, (void *)labelCoolantTemp );

    streams[1]->reg = reg_TACHO;
    streams[1]->observers_len = 2;
    streams[1]->observers = (observer_base_t **)malloc( streams[1]->observers_len * sizeof(observer_base_t *) );
    streams[1]->observers[0] = (observer_base_t *)observer_value_new( &value_label_cb, (void *)labelEngineSpeed );
    streams[1]->observers[1] = (observer_base_t *)observer_shift_new( &shift_label_cb, (void *)labelShift, 6000, 7000 );

    streams[2]->reg = reg_ROAD_SPEED;
    streams[2]->observers_len = 2;
    streams[2]->observers = (observer_base_t **)malloc( streams[2]->observers_len * sizeof(observer_base_t *) );
    streams[2]->observers[0] = (observer_base_t *)observer_value_new( &value_label_cb, (void *)labelRoadSpeed );
    streams[2]->observers[1] = (observer_base_t *)observer_zerosixty_new( &zerosixty_label_cb, (void *)labelZerosixty, 60 );

    streams[3]->reg = reg_TPS;
    streams[3]->observers_len = 1;
    streams[3]->observers = (observer_base_t **)malloc( streams[3]->observers_len * sizeof(observer_base_t *) );
    streams[3]->observers[0] = (observer_base_t *)observer_ratio_new( &ratio_label_cb, (void *)labelTpsPc );


    stream_registers_start(
        streams,
        stream_count
    );
}
