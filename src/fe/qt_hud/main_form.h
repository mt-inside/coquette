#ifndef _INCLUDED_MAIN_FORM_H
#define _INCLUDED_MAIN_FORM_H

#include "ui_main_form.h"
#include "boost/bind.hpp"
#include "boost/function.hpp"

extern "C" {
#include "stream.h"
}

typedef boost::function<void (observer_base_t *)> tramp_cb;
class main_form : public QMainWindow, private Ui::main_form
{
    Q_OBJECT

    public:
        main_form( QWidget *parent = 0 );
        virtual ~main_form( );

    private:
        stream_t *_stream;
        tramp_cb *_cb;
        void value_label_cb( observer_base_t *obs );
        static void trampoline( observer_base_t *obs, void *ctxt );
};

#endif /* defined _INCLUDED_MAIN_FORM_H */
