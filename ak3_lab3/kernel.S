.syntax unified

.cpu cortex-m4
//.fpu softvfp
.thumb

#define A #4
#define B #16
#define C #3

// global memory locations
.global vtable_kernel
.global __kernel_reset__

.type vtable_kernel, %object
.type __kernel_reset__, %function

.section .interrupt_vector
vtable_kernel:
    .word __stack_start
    .word __kernel_reset__+1
    .size vtable_kernel, .-vtable_kernel

.section .rodata
    data: .asciz "kernel started!\n"
    final: .asciz "value in register #2: "
.section .text
__kernel_reset__:
    mov r0, #0
    ldr r0, =data
    bl dbgput_line
   
    // calculations from lab2 
    mov r0, A
    mov r1, B
    sub r2, r0, r1
    cmp r2, #0
    it ge
    addge r2, r1, r0
    mov r3, C
    sdiv r2, r3

    // output into console
    ldr r0, =final
    bl dbgput
    mov r0, r3
    bl dbgput_num

    end:
    b end
