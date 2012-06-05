#ifndef _INCLUDED_MAIN_FORM_H
#define _INCLUDED_MAIN_FORM_H

#include "ui_main_form.h"
#include "proxy_label.h"

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
        void update_label( QLabel *label, char *value );

    private:
        stream_t *_stream;
        proxy_label *_label_proxy;
};

#endif /* defined _INCLUDED_MAIN_FORM_H */
