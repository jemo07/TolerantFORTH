.section .text
.global _start

/* Reset Vector */
_reset_vector:
  b _start

/* Interrupt Vector Table */
.section .vector_table
.global _vector_table

_vector_table:
  /* Reset vector */
  .word _reset_vector

  /* Interrupt Service Routines (ISRs) */
  .word _timer_isr
  .word _external_isr
  .word _exception_isr

.section .text
_start:
  /* Set the appropriate processor mode and execution level */
  mrs x0, CurrentEL
  and x0, x0, #0xC
  cmp x0, #0xC
  b.eq skip_el_change
  mov x0, #0xC
  msr DAIFSet, x0

skip_el_change:
  /* Initialize the stack pointer (SP) to a valid memory address */
  ldr sp, =_stack_top

  /* Memory Initialization */
  /* Perform any necessary initialization tasks for memory configuration and MMU */

  /* Peripheral Initialization */
  /* Perform initialization and configuration of peripherals */

  /* Enable Cache and Memory Subsystem */
  /* Configure cache subsystem and memory hierarchy if applicable */

  /* Enable Core Features */
  /* Enable any additional core features required */

  /* Call TolerantForth Entry Point */
  bl main

  /* Handle system shutdown or other actions if needed */

  /* Infinite loop to prevent program execution from falling through */
  b .

.section .data
.global _stack_top
_stack_top:
  /* Define the top of the stack memory address */
  /* Adjust the value to suit your memory layout and stack requirements */

.section .bss
/* Define any uninitialized data (if needed) */
