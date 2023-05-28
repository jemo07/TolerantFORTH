# Messaging Mechanism

The Messaging Mechanism in the TolerantFORTH project enables inter-process communication and messaging between lightweight processes. It allows processes to exchange data and information through a global tagged stack. This mechanism facilitates the development of concurrent and collaborative applications within the TolerantFORTH system.

### Design Specs: 

1. Messaging Mechanism: The FORTH system would include specific words to enable messaging between processes. The `send` word would be used to push data onto the global stack with a specific tag, while the `receive` word would read and consume data from the global stack based on the tag.

2. Sending Messages: The `send` word would take the message data and a tag as inputs. It would then push the message data onto the global stack along with the corresponding tag, effectively sending the message to be picked up by the intended recipient process.

3. Receiving Messages: The `receive` word would take the tag as input and search the global stack for messages with matching tags. Once a matching tag is found, the `receive` word would read and consume the corresponding message data from the global stack, allowing processes to retrieve and process the messages they are intended to receive.

4. Message Synchronization: To ensure proper synchronization and coordination, the FORTH system can implement mechanisms such as semaphores, mutexes, or other synchronization primitives. These mechanisms would help prevent race conditions when multiple processes try to send or receive messages simultaneously.

### Example Implementation:

Here's an example implementation of the messaging mechanism in the FORTH system for inter-process communication:

```forth
variable global-stack \ Global stack for inter-process communication

\ Push message data onto the global stack with a specific tag
: send ( data tag -- )
  over swap \ Swap data and tag
  global-stack @ over ! \ Push tag onto the stack
  global-stack @ 1+ ! \ Increment stack pointer
  global-stack @ ! \ Push data onto the stack ;

\ Read and consume message data from the global stack based on the tag
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
  42 'process2-tag send \ Send the message '42' with the tag 'process2-tag'
  ." Process 1 sent a message." cr ;

: process2 ( -- )
  'process2-tag receive \ Receive the message with the tag 'process2-tag'
  ." Process 2 received a message." cr ;

\ Initialize global stack
create 100 cells global-stack !

\ Example execution
process1 process2
```

Here, the FORTH system implements the messaging mechanism for inter-process communication with the following the key components:

- The `send` word takes the message data and tag as inputs. It swaps the data and tag on the stack and then pushes the tag onto the global stack. It increments the stack pointer and finally pushes the message data onto the stack.

- The `receive` word takes the tag as input and starts a loop to search for matching tags on the global stack. Once a matching tag is found, it swaps the data and the tag and consumes them by removing them from the stack.

- The example shows two processes, `process1` and `process2`, where `process1` sends a message with the tag `'process2-tag'` using the `send` word, and `process2` receives the message with the matching tag using the `receive` word.

- The global stack is initialized with 100 cells to serve as the shared stack for inter-process

 communication.

As the project matures, additional error handling, synchronization mechanisms, and more complex messaging protocols can be incorporated as needed. RDMA access will be improtant as complex implementations arrise, in particular, considering if the system communicates though a PCIe interface or a highspeed network outside the computer its running on or a possible cluster can be implemtend. 
