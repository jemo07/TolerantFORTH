#include "forthvm.h"
#include "dictionary.h"
#include "process.h"
#include "scheduler.h"
#include "interpreter.h"
#include "stack.h"
#include "io.h"

int main() {
    // Initialize Forth VM and components
    forthvm_init();
    dictionary_init();
    process_init();
    scheduler_init();
    interpreter_init();
    stack_init();
    io_init();

    // Define Forth words and add them to the dictionary
forthvm_define_word("BYE", &bye);
forthvm_define_word("?RX", &qrx);
forthvm_define_word("TX!", &txsto);
forthvm_define_word("NEXT", &next);
forthvm_define_word("DOVAR", &dovar);
forthvm_define_word("DOCON", &docon);
forthvm_define_word("DOLIT", &dolit);
forthvm_define_word("DOLIST", &dolist);
forthvm_define_word("EXIT", &exitt);
forthvm_define_word("EXECUTE", &execu);
forthvm_define_word("DOSTR", &donext);
forthvm_define_word("?BRANCH", &qbran);
forthvm_define_word("BRANCH", &bran);
forthvm_define_word("!", &store);
forthvm_define_word("@", &at);
forthvm_define_word("C!", &cstor);
forthvm_define_word("C@", &cat);
forthvm_define_word("R>", &rfrom);
forthvm_define_word("R@", &rat);
forthvm_define_word(">R", &tor);
forthvm_define_word("DROP", &drop);
forthvm_define_word("DUP", &dup);
forthvm_define_word("SWAP", &swap);
forthvm_define_word("OVER", &over);
forthvm_define_word("0<", &zless);
forthvm_define_word("=", &equals);
forthvm_define_word("<>", &notequ);
forthvm_define_word(">", &greater);
forthvm_define_word("<", &less);
forthvm_define_word("+", &plus);
forthvm_define_word("-", &minus);
forthvm_define_word("*", &multiply);
forthvm_define_word("/", &divide);
forthvm_define_word("MOD", &mod);
forthvm_define_word("AND", &andd);
forthvm_define_word("OR", &orr);
forthvm_define_word("XOR", &xorr);
forthvm_define_word("NOT", &nott);
forthvm_define_word("NEGATE", &negat);
forthvm_define_word("U<", &uless);
forthvm_define_word("UM+", &umplus);
forthvm_define_word("CELLS", &cells);
forthvm_define_word("ALLOT", &allot);
forthvm_define_word("HERE", &here);
forthvm_define_word("COMPILE,", &comma);
forthvm_define_word("CREATE", &create);
forthvm_define_word("CHARS", &chars);
forthvm_define_word("COUNT", &count);
forthvm_define_word("CMOVE", &cmove);
forthvm_define_word("MOVE", &move);
forthvm_define_word("FILL", &fill);
forthvm_define_word("DUP2", &dup2);
forthvm_define_word("SWAP2", &swap2);
forthvm_define_word("DROP2", &drop2);
forthvm_define_word("OVER2", &over2);
forthvm_define_word("PICK2", &pick2);
forthvm_define_word("ROT2", &rot2);
forthvm_define_word("MINUS", &minus);
forthvm_define_word("NEGATE", &negate);
forthvm_define_word("MAX2", &max2);
forthvm_define_word("MIN2", &min2);


    // Create initial process
    ProcessID pid = process_create(&initial_process);

    // Main loop
    while (!scheduler_is_empty()) {
        // Get next process to execute
        ProcessID nextProcess = scheduler_get_next_process();

        // Switch to next process
        process_switch_context(nextProcess);

        // Fetch and execute Forth words
        interpreter_execute();
    }

    // Cleanup and exit
    forthvm_reset();
    return 0;
}
