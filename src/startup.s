..section .text.boot

/* Reset Vector */
.globl _reset_vector
_reset_vector:
  ldr pc, _start

/* Interrupt Vector Table */
.section .vector_table
.globl _vector_table

_vector_table:
  /* Reset vector */
  .word _reset_vector
  /* Add other interrupt vectors as needed */

.section .text

/* Processor Modes and Execution Levels */
.equ ARM_MODE_USR, 0x10
.equ ARM_MODE_FIQ, 0x11
.equ ARM_MODE_IRQ, 0x12
.equ ARM_MODE_SVC, 0x13
.equ ARM_MODE_HYP, 0x1A
.equ ARM_MODE_UND, 0x1B
.equ ARM_MODE_SYS, 0x1F

.equ ARM_MODE_MASK, 0x1F
.equ ARM_I_BIT, 0x80
.equ ARM_F_BIT, 0x40

.equ CPSR_MODE_USER, 0x10
.equ CPSR_MODE_FIQ, 0x11
.equ CPSR_MODE_IRQ, 0x12
.equ CPSR_MODE_SVC, 0x13
.equ CPSR_MODE_ABORT, 0x17
.equ CPSR_MODE_UNDEFINED, 0x1B
.equ CPSR_MODE_SYSTEM, 0x1F

.equ CPSR_IRQ_INHIBIT, 0x80
.equ CPSR_FIQ_INHIBIT, 0x40
.equ CPSR_THUMB, 0x20

/* Stack Pointer Initialization */
.globl _get_stack_pointer
_get_stack_pointer:
  str sp, [sp]
  ldr r0, [sp]
  mov pc, lr

/* Memory Initialization */
/* Add your memory initialization code here */

/* _start label */
.globl _start
_start:
  /* Set the appropriate processor mode and execution level */
  mrs r0, CPSR
  orr r0, r0, #(ARM_I_BIT | ARM_F_BIT)
  and r11, r0, #ARM_MODE_MASK
  cmp r11, #ARM_MODE_HYP
  bne .NotInHypMode
  bic r0, r0, #ARM_MODE_MASK
  orr r0, r0, #ARM_MODE_SVC
  msr spsr_cxsf, r0
  add lr, pc, #4
  msr ELR_hyp, lr
  eret

.NotInHypMode:
  mov r0, #0x8000
  mov r1, #0x0000
  ldmia r0!, {r2-r9}
  stmia r1!, {r2-r9}
  ldmia r0!, {r2-r9}
  stmia r1!, {r2-r9}

  mov r0, #0xD2
  msr cpsr_c, r0
  ldr sp, =__irq_stack_base

  mov r0, #0xD3
  msr cpsr_c, r0
  ldr sp, =__svc_stack_base

  /* Enable Cache and Memory Subsystem */
  /* Add cache and memory subsystem configuration here */

  /* Enable Core Features */
  /* Add any necessary core feature enablement here */

  /* Call TolerantForth Entry Point */
  bl main

/* Interrupt Service Routines (ISRs) */
/* Add your ISR implementations here */

/* Halt */
_inf_loop:
  b _inf_loop

.section .
