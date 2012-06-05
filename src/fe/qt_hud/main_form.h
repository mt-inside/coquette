#ifndef _INCLUDED_MAIN_FORM_H
#define _INCLUDED_MAIN_FORM_H

#include "ui_main_form.h"
#include "proxy_value_label.h"
#include "proxy_value_dial.h"

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
        proxy_value_label *_label_proxy;
        proxy_value_dial  *_dial_proxy;
};

#endif /* defined _INCLUDED_MAIN_FORM_H */
