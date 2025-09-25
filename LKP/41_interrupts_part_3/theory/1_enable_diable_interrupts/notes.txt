Enable/Disable Interrupt
=========================

Header File: <linux/irqflags.h>

local_irq_disable(); //disable all interrupts on the current processor

local_irq_enable(); //Enables all interrupts on the current processor

On x86, local_irq_disable() is a simple cli, and local_irq_enable() is a simple sti instruction

cli and sti are the assembly calls to clear and set the allow interrupts flag, respectively.
