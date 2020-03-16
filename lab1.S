.global lab1
.syntax unified
#define A #4
#define B #16
#define C #3

lab1:
	push {lr}
	mov r0, A
	mov r1, B
	sub r2, r0, r1
	cmp r2, #0
	it ge
	addge r2, r1, r0
	mov r3, C
	sdiv r2, r3
	pop {pc}

