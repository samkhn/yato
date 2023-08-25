.extern	isr_handler

.global isr_common_stub
isr_common_stub:
	pusha
	mov		%ds,	%ax
	push	%eax
	mov		$0x10,	%ax
	mov		%ax,	%ds
	mov		%ax,	%es
	mov		%ax,	%fs
	mov		%ax,	%gs
	push	%esp
	call	isr_handler
	pop		%ebx
	pop		%ebx
	mov		%bx,	%ds
	mov		%bx,	%es
	mov		%bx,	%fs
	mov		%bx,	%gs
	popa
	add		$8,		%esp
	sti
	iret
