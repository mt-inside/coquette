#include <stdio.h>

#include "com/com.h"
#include "commands.h"


int main( int argc, char **argv )
{
    fault_report_t *faults, *fault;

    (void)argc;
    (void)argv;

    read_dtc( &faults );
    fault = faults;
    while( fault )
    {
        printf( "Fault code %#x, last seen %d engine starts ago\n",
                fault->code, fault->last_seen );

        fault = fault->next;
    }

    faults_free( faults );

    return 0;
}
