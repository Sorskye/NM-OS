#include "print.h"
#include "string.h"
#include "io.h"
#include "types.h"
#include "branch.h"
extern Bool InitInterrupts();

void kernel_main()
{
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str("NMOS Kernel loaded! \n");
    print_str("Kernel Version: 0.0.1A \n");
    print_str("\n");
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    
    //BRANCH(InitSerial(), print_str("SERIAL INIT OK\n");, print_str("SERIAL INIT FAIL\n"););
    BRANCH(InitInterrupts(), print_str("INTERRUPT INIT OK\n"); , print_str("INTERRUPT INIT FAIL\n"););
    
    print_str("IDLE\n");
    
    // Division by 0 is undefined behaviour in C/C++.
    // Use inline assembly to genarte one
    //int a = 1 / 0;
    asm ("div %b0" :: "a"(0));
    
}
