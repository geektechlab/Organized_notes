What happens if I disable interrupt line shared among multiple interrupt handlers?
==================================================================================

Disabling the line disables interrupt delivery for all devices on the line.

Therefore, drivers for newer devices tend not to use these interfaces.

Because PCI devices have to support interrupt line sharing by specification, they should not use these interfaces at all.

Thus, disable_irq() and friends are found more often in drivers for older legacy devices, such as the PC parallel port.
