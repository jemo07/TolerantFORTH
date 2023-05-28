#include "uart.h"
#include "forthvm.h"
#include "dictionary.h"

int main() {
    forthvm_init();
    
    printf("TolerantForth v0.01\n");
    printf("Type 'bye' to exit.\n\n");
    
    char input[256];
    
    // REPL (Read-Eval-Print Loop)
    while (1) {
        printf("> ");
        fgets(input, sizeof(input), stdin);
        
        // Remove trailing newline character
        input[strcspn(input, "\n")] = '\0';
        
        // Exit if the user types 'bye'
        if (strcmp(input, "bye") == 0) {
            break;
        }
        
        // Parse and execute the Forth command
        forthvm_interpret(input);
    }
    
    printf("\nGoodbye!\n");
    
    return 0;
}

