.macro	isr_noerrcode	n
.global	isr\n
isr\n:
	cli
	push	0
	push	\n
	jmp		isr_common_stub
.endm

isr_noerrcode	0
isr_noerrcode	1
isr_noerrcode	2
isr_noerrcode	3
isr_noerrcode	4
isr_noerrcode	5
isr_noerrcode	6
isr_noerrcode	7
isr_noerrcode	8
isr_noerrcode	9
isr_noerrcode	10
isr_noerrcode	11
isr_noerrcode	12
isr_noerrcode	13
isr_noerrcode	14
isr_noerrcode	15
isr_noerrcode	16
isr_noerrcode	17
isr_noerrcode	18
isr_noerrcode	19
isr_noerrcode	20
isr_noerrcode	21
isr_noerrcode	22
isr_noerrcode	23
isr_noerrcode	24
isr_noerrcode	25
isr_noerrcode	26
isr_noerrcode	27
isr_noerrcode	28
isr_noerrcode	29
isr_noerrcode	30
isr_noerrcode	31

