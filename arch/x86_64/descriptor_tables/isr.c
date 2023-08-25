#include "arch/x86_64/boot/console.h"
#include "arch/x86_64/descriptor_tables/isr.h"

void isr_handler(interrupt_state_t *registers)
{
	bprintf("received interrupts: %x\n", registers->interrupt_number);
}
