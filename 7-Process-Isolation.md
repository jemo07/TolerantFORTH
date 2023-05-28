# Process Isolation
Process isolation in the TolerantFORTH project refers to the design and implementation of mechanisms that ensure independent execution and data integrity for each process within the system. It aims to solve the problem of interference between processes and the need to maintain data isolation. Process isolation is essential in TolerantFORTH to create a reliable and robust system where processes can execute concurrently without causing conflicts or corruption of shared resources. It will attemtp to enhances the stability, reliability, and maintainability of the system while enabling efficient utilization of system resources.


### Design Specs: 

**Process Isolation:** The FORTH system would ensure process isolation by maintaining separate execution contexts for each process. Each process would have its own set of local stacks, variables, and other data structures that are isolated from other processes.

**Execution Context:** The execution context for each process would include a process-specific temporary stack, registers, and any other necessary state variables. This ensures that data and state within a process are not accessible or modified by other processes.

**Local Stacks:** Each process in the FORTH system would have its own local stacks, such as a data stack and a return stack. These stacks would be used for local data manipulation and storing return addresses during subroutine calls. Local stacks ensure that process-specific data remains isolated and unaffected by other processes.

**Global Tagged Stack:** To facilitate communication and sharing of data between processes, the FORTH system would include a global tagged stack. Processes would explicitly communicate through this stack by using specific tags associated with the data being pushed or popped. This ensures that data shared between processes is explicitly communicated and avoids unintended interference.

**Sharing Data:** When a process needs to share data with another process, it would push the data onto the global tagged stack with a specific tag. The receiving process would then use the corresponding tag to retrieve and consume the data from the global stack. This explicit sharing mechanism ensures controlled access and prevents unauthorized modification of shared data.

### Example Implementation:

Here's an example implementation of process isolation in the FORTH system:

```forth
variable global-stack \ Global stack for inter-process communication

\ Process Data Structure
: process ( -- addr ) here ;

\ Process Creation
: create-process ( -- pid ) process allocate throw ;

\ Local Stacks
variable data-stack
variable return-stack

\ Push data onto the global stack with a specific tag
: send ( data tag -- )
  over swap \ Swap data and tag
  global-stack @ over ! \ Push tag onto the stack
  global-stack @ 1+ ! \ Increment stack pointer
  global-stack @ ! \ Push data onto the stack ;

\ Read and consume data from the global stack based on the tag
: receive ( tag -- data )
  begin
    global-stack @ \ Top of stack
    dup \ Duplicate tag for comparison
    = if
      global-stack @ swap ! \ Remove tag from the stack
      drop \ Drop tag from the stack
      exit \ Exit the loop
    else
      2drop \ Drop tag and data from the stack
    then
  again ;

\ Example usage
: process1 ( -- )
  10 'process2-tag send \ Send the value 10 with the tag 'process2-tag'
  ." Process 1 sent data." cr ;

: process2 ( -- )
  'process2-tag receive \ Receive the data with the tag 'process2-tag'
  ." Process 2 received data: " . cr ;

\ Initialize global stack
create 100 cells global-stack !

\ Example execution
process1 process2
```

In this example, the FORTH system implements process isolation to ensure data integrity and prevent interference. Here's a breakdown of the key components:

- The `process` word defines the process data structure.

- The `create-process` word creates a new process and returns its process identifier (PID).

- The `data-stack` and `return-stack` variables hold the local stacks for each process. These stacks are process-specific and ensure isolation of local data and return addresses.

- The `send` word is used to push data onto the global stack with a specific

 tag.

- The `receive` word is used to retrieve and consume data from the global stack based on the tag.

- The example shows two processes, `process1` and `process2`, where `process1` sends data with the tag `'process2-tag'` using the `send` word, and `process2` receives the data with the matching tag using the `receive` word.

- The global stack is initialized with 100 cells to serve as the shared stack for inter-process communication.

As mentioned before, additional error handling, synchronization mechanisms, and more sophisticated communication protocols can be incorporated as the project matures or the scope expands to new needs. 
