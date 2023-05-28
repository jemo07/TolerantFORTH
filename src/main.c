#include "uart.h"
#include "forthvm.h"

int main()
{
    uart_init();

    // Initialize other components and peripherals if needed

    // Initialize and start TolerantForth system
    forthvm_init();

    // Enter the main loop of the TolerantForth system
    forthvm_run();

    // The program should not reach here
    while (1)
    {
        // Handle any error or recovery mechanism
    }

    return 0;
}
