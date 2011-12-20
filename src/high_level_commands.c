#include <high_level_commands.h>
#include <commands.h>
#include <stdlib.h>


/* for a higher data rate:
 * - don't call the lower level for every register. This will result in one
 *   whole frame read, termination and wait per register. Instead read 20 regs
 *   at a time (will require knowing reg sizes)
 * - don't call this whole stack over and over, instead architect for
 *   streaming
 */
int read_registers( engine_reg_t *regs, unsigned no_regs, int **values )
{
    unsigned i;
    int rc = 0;


    *values = malloc( sizeof(int) * no_regs );

    for( i = 0; i < no_regs; ++i )
    {
        rc = read_register( regs[i], *values + i );
        if( rc ) break;
    }


    return rc;
}
