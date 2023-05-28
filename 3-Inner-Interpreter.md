# Inner Interpreter

The inner interpreter is a crucial component of this FORTH implementation, it's main function is to execute FORTH words within the context of a process. It interprets and executes the FORTH code, enabling the process to perform computations, manipulate data, and control the flow of execution. The inner interpreter maintains a separate execution context for each process, including a process-specific temporary stack.

Here's a description of the inner interpreter in the TolerantFORTH project:

**Execution Context:** The inner interpreter maintains a separate execution context for each process. This includes the process-specific temporary stack, which is used for local data manipulation during the execution of FORTH words.

**Fetch and Execute:** The inner interpreter fetches FORTH words from the process's input stream and executes them. It follows a cycle of fetching the next word, identifying its type (e.g., primitive word, user-defined word), and executing the corresponding behavior.

**Primitive Words:** The inner interpreter handles primitive words directly. These are predefined FORTH words that perform low-level operations and have a specific behavior associated with them. Examples of primitive words include arithmetic operations, stack manipulation, and control flow primitives.

**User-Defined Words:** The inner interpreter supports user-defined words, which are created by the programmer to encapsulate a sequence of FORTH words as a single word. When encountering a user-defined word, the inner interpreter recursively interprets the sequence of words it represents, executing the corresponding behaviors.

**Temporary Stack:** The inner interpreter utilizes a process-specific temporary stack for local data manipulation during word execution. The temporary stack is separate for each process, ensuring isolation and preventing interference between processes.

**Control Flow:** The inner interpreter handles control flow constructs such as conditionals (`IF`, `ELSE`, `THEN`) and loops (`DO`, `LOOP`, `BEGIN`, `UNTIL`, etc.). These constructs allow for conditional branching and iterative execution within the FORTH code.

**Word Look-Up:** When encountering a non-primitive word, the inner interpreter performs a word look-up to find the definition of the word. It searches the dictionary, which holds the definitions of all user-defined and primitive words, to determine the behavior associated with the word.

**Context Switching:** As the inner interpreter executes FORTH words within the context of a process, it also enables context switching when necessary. For example, when a process exhausts its allocated quantum, the inner interpreter triggers a context switch, allowing the scheduler to select the next process to execute.

Overall, the inner interpreter plays a critical role in the TolerantFORTH project by interpreting and executing FORTH words within the context of each process. It facilitates the execution of FORTH code, including both primitive and user-defined words, while maintaining the necessary execution context and supporting control flow constructs.

### Example Implementation

Here's an example implementation of the inner interpreter in the FORTH system:

```forth
variable pc-stack \ Stack to save and restore program counters
variable temp-stack \ Process-specific temporary stack

\ Fetch and execute the current word
: interpret-word ( -- )
  current @ execute ;

\ Save the state of the current process
: save-state ( -- )
  pc-stack @ swap ! \ Save the current PC onto the stack
  temp-stack @ swap ! \ Save the current temporary stack contents

\ Restore the state of the next process
: restore-state ( -- )
  swap ! temp-stack @ swap !
  pc-stack @ swap ! ;

\ Execute a process
: execute-process ( -- )
  save-state
  interpret-word
  restore-state ;

\ Example usage
: process1 ( -- ) 10 20 + . cr ;
: process2 ( -- ) 5 3 * . cr ;

\ Initialize temporary stack and PC stack
temp-stack constant process1-temp-stack
temp-stack constant process2-temp-stack
pc-stack constant process1-pc-stack
pc-stack constant process2-pc-stack

\ Main loop
: main-loop
  begin
    process1-pc-stack process1-pc-stack ! \ Set PC stack for process 1
    process1-temp-stack process1-temp-stack ! \ Set temporary stack for process 1
    execute-process

    process2-pc-stack process2-pc-stack ! \ Set PC stack for process 2
    process2-temp-stack process2-temp-stack ! \ Set temporary stack for process 2
    execute-process
  again ;

main-loop
```

In this example, the FORTH system implements an inner interpreter to execute FORTH words within the context of processes. Here's a breakdown of the key components:

- The `interpret-word` word fetches the current word from the PC and executes it using `execute`.

- The `save-state` word saves the state of the current process, including the PC and the temporary stack, onto the `pc-stack` and `temp-stack` variables, respectively.

- The `restore-state` word restores the state of the next process by swapping the saved PC and temporary stack
