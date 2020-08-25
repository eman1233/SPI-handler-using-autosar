#include "Spi_MemMap.h"
#include "Spi.h"

//Enable interrupts
__attribute__((naked)) void irq_Enable(void)
{
	__asm(" CPSIE I ");
	__asm(" BX LR");
}

//Disable interrupts
__attribute__((naked)) void irq_Disable(void)
{
	__asm(" CPSID I ");
	__asm(" BX LR");
}
