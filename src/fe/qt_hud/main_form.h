#ifndef _INCLUDED_MAIN_FORM_H
#define _INCLUDED_MAIN_FORM_H

#include "ui_main_form.h"
#include "proxy_derivative_label.h"
#include "proxy_integral_label.h"
#include "proxy_ratio_dial.h"
#include "proxy_shift_label.h"
#include "proxy_value_label.h"
#include "proxy_zerosixty_label.h"

extern "C" {
#include "stream.h"
}

class main_form : public QMainWindow, private Ui::main_form
{
    Q_OBJECT

    public:
        main_form( QWidget *parent = 0 );
        virtual ~main_form( );

    public slots:
        void update_label( QLabel *label, char  *value );
        void update_dial(  Dial   *dial,  float  value );

    private:
        stream_t *_stream;
        proxy_value_label *_coolant_value_label_proxy;
        proxy_ratio_dial  *_coolant_ratio_dial_proxy;
        proxy_value_label *_rpm_value_label_proxy;
        proxy_ratio_dial  *_rpm_ratio_dial_proxy;
        proxy_shift_label *_rpm_shift_label_proxy;
        proxy_value_label *_speed_value_label_proxy;
        proxy_integral_label *_speed_integral_label_proxy;
        proxy_derivative_label *_speed_derivative_label_proxy;
        proxy_zerosixty_label *_speed_zerosixty_label_proxy;
};

#endif /* defined _INCLUDED_MAIN_FORM_H */
