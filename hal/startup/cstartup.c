
#include "../rpi_base.h"

// defined by linker
extern int __bss_start__;
extern int __bss_end__;

extern void application_main();

// registers are filled by gpu at startup
void _cstartup( hal_base_t r0, hal_base_t r1, hal_base_t r2 )
{
    int * pointer = &__bss_start__;
    int * bss_end = &__bss_end__;

    // clear bss section
    while( pointer < bss_end )
    {
        *pointer++ = 0;
    }

    // call application
    application_main();

    // catch if application_main returns
    while(1)
    {
        // nop
    }
}
