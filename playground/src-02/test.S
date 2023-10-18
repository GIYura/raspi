/* Hello world program using assembly language */

.data
message:
	.ascii "Hello Raspberry Pi on Assembly Language\n"
	.set len, . - message
.text
.global _start

_start:
	mov r0, #1
	ldr r1, =message
	mov r2, #len
	mov r7, #4		@linux write system call
	svc 0			@call linux to print

	mov r0, #0
	mov r7, #1
	svc 0


