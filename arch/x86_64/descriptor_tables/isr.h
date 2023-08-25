#ifndef YATO_ARCH_X86_64_DESCRIPTOR_TABLE_ISR_H
#define YATO_ARCH_X86_64_DESCRIPTOR_TABLE_ISR_H

struct interrupt_state {
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t interrupt_number, err_code;
	uint32_t eip, cs, eflags, useresponse, ss;
};
typedef struct interrupt_state interrupt_state_t;

void isr_handler(interrupt_state_t *registers);

#endif // YATO_ARCH_X86_64_DESCRIPTOR_TABLE_ISR_H
