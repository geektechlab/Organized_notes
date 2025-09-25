#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/delay.h>

#define PCI_INDEX_PORT	0xcf8
#define PCI_DATA_PORT	0xcfc


unsigned short read_pci_config_short(unsigned char bus, unsigned char slot, unsigned char func,
					unsigned char offset)
{
	outl(0x80000000 | (bus<<16) | (slot<<11) | (func<<8) | offset, 0xcf8);
	return inw(0xcfc + (offset&2));
}

unsigned int read_pci_config(unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset)
{
        unsigned int v;
        outl(0x80000000 | (bus<<16) | (slot<<11) | (func<<8) | offset, 0xcf8);
        v = inl(0xcfc);
        return v;
}

int __init  hello_init(void)
{	
	//02:01.0 Ethernet controller
	unsigned int bar_register = read_pci_config(2, 1, 0, 0x10);
	pr_info("bar register:%02x\n", bar_register);

	if (bar_register & 0x01)
		pr_info("I/O Mapped I/O\n");
	else {
		pr_info("Memory Mapped I/O\n");
		if (((bar_register >> 1) & 0x03) == 0x00)
			pr_info("32-BIT Address space\n");
		else
			pr_info("64-bit address space\n");
	}

	return 0;
}

void  __exit hello_exit(void)
{

}

MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
