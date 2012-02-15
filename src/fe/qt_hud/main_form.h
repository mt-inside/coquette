#ifndef _INCLUDED_MAIN_FORM_H
#define _INCLUDED_MAIN_FORM_H

#include "ui_main_form.h"

class main_form : public QMainWindow, private Ui::main_form
{
    Q_OBJECT

    public:
        main_form( QWidget *parent = 0 );

        void update_stats( );

    public slots:
        void lol( );
};

#endif /* defined _INCLUDED_MAIN_FORM_H */
