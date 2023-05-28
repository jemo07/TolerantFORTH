# Scheduling and Preemption

In the TolerantFORTH project, scheduling and preemption are implemented to enable cooperative multitasking and fair execution of lightweight processes. TolerantFORTH would employ a cooperative multitasking approach similar to the BEAM VM. Each lightweight process is allocated a certain number of FORTH word executions, known as a quantum or time slice, before yielding control back to the scheduler. This allows processes to voluntarily yield control to other processes, promoting fairness and preventing one process from monopolizing system resources.

### Desing Spec: 
**Scheduling and Preemption:** The FORTH system's process management system would use a cooperative multitasking approach. Each process is allocated a certain number of FORTH word executions, known as "quantum" or "time slice," before yielding control back to the scheduler. The scheduler then selects the next process to execute based on a predefined scheduling algorithm.

**Quantum Counting:** The FORTH system would maintain a counter for each process to keep track of the remaining quantum. This counter would be decremented each time a FORTH word is executed by the process.

**Yielding Control:** When a process exhausts its quantum and reaches a predefined threshold, it voluntarily yields control back to the scheduler. The process would save its current state, including the program counter and other relevant data, before yielding.

**Scheduler Algorithm:** The scheduler in the FORTH system would determine the order in which processes are executed. It can use various scheduling algorithms, such as round-robin, priority-based, or fair-share scheduling. The specific algorithm chosen depends on the system's requirements and priorities.

**Context Switching:** When a process yields control, the FORTH system would perform a context switch. This involves saving the state of the yielding process and loading the state of the next scheduled process. The saved state would include the program counter, stack contents, and other relevant data.

### Example Implementation:

Here's an example implementation of scheduling and preemption in the FORTH system:

```forth
\ Process Data Structure
: process ( -- addr ) here ;

\ Process Creation
: create-process ( -- pid ) process allocate throw ;

\ Process Quantum Counter
: set-quantum ( pid quantum -- )
  \ Set the quantum counter for a process
  \ (This can be customized per process)
  swap process ! ;

\ Process Yielding
: should-yield? ( -- flag )
  \ Check if the process should yield
  \ (This can be customized per process based on the quantum counter)
  process @ \ Load the quantum counter
  0= ; \ Return true if the quantum counter reaches zero

\ Example usage
: process1 ( -- )
  10 20 + . cr ; \ Sample computation for process 1

: process2 ( -- )
  5 3 * . cr ; \ Sample computation for process 2

create-process constant pid1
create-process constant pid2

100 set-quantum pid1 \ Set a quantum of 100 for process 1
50 set-quantum pid2 \ Set a quantum of 50 for process 2

pid1 process1
pid2 process2

\ Main loop
: main-loop
  begin
    pid1 process @ dup if
      pid1 process @ execute \ Execute process 1
      should-yield? if
        \ Perform context switch if process 1 should yield
        context-save
        pid1 process @ process !
        context-restore
      then
    then

    pid2 process @ dup if
      pid2 process @ execute \ Execute process 2
      should-yield? if
        \ Perform context switch if process 2 should yield
        context-save
        pid2 process @ process !
        context-restore
      then
    then
  again ;

main-loop
```

The FORTH system would implement scheduling and preemption for cooperative multitasking, broken down into the following key components:

- The `process` word defines the process data structure.

- The `create-process` word creates a new process and returns its process identifier (PID).

- The `set-quantum` word sets the quantum counter for a process. The quantum counter determines the number of FORTH word executions before a process yields control.

- The `should-yield?` word checks whether a process should yield control based on the quantum counter.

- The example shows two processes, `process1` and `process2`, each with their respective computations.

- The `main-loop` repeatedly executes the processes in a round-robin fashion. Each process is executed until it reaches its quantum threshold and voluntarily yields control.

Additionally, synchronization mechanisms can be incorporated to ensure proper coordination and avoid race conditions in accessing shared resources.
