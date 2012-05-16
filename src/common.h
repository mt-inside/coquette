#include <stdio.h>

#define LOG( x ) printf( x "\n" )

#ifdef WIN32
#include "Windows.h"
#define sleep(x) Sleep(1000 * (x))
#endif
