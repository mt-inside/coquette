#include <QApplication>
#include <assert.h>

#include "main_form.h"

extern "C" {
#include "com/com.h"
#include "commands.h"
}

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    main_form *main_window;
    int rc;

    assert( argc == 2 );

    assert( !com_init( argv[1] ) );
    handshake( ecu_ENGINE );

    main_window = new main_form;
    main_window->setAttribute( Qt::WA_DeleteOnClose );
    main_window->show( );

    rc = app.exec( );

    com_finalise( );

    return rc;
}
