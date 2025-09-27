#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioport.h>

#define PCI_INDEX_PORT	0xcf8
#define PCI_DATA_PORT	0xcfc

unsigned char bus_number = 0;
unsigned char slot_number = 0;
unsigned char function_number = 0;

module_param(bus_number, byte, 0);
module_param(slot_number, byte, 0);
module_param(function_number, byte, 0);

unsigned short read_pci_config_short(unsigned char bus, unsigned char slot, unsigned char func,
					unsigned char offset)
{
	outl(0x80000000 | (bus<<16) | (slot<<11) | (func<<8) | offset, 0xcf8);
	return inw(0xcfc + (offset&2));
}

unsigned char read_pci_config_byte(unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset)
{
        unsigned char v;
        outl(0x80000000 | (bus<<16) | (slot<<11) | (func<<8) | offset, 0xcf8);
        v = inb(0xcfc + (offset&3));
        return v;
}

void print_configuration_space(unsigned char bus, unsigned char slot, unsigned char func)
{
	unsigned char pci_config_space[256];
	unsigned short i;

	for (i = 0; i < 256; i++)
		pci_config_space[i] = read_pci_config_byte(bus, slot, func, i);
	
	print_hex_dump_bytes("", DUMP_PREFIX_NONE, pci_config_space, 256);

}

int __init  hello_init(void)
{
	pr_info("Printing Configuration space of :%d:%d:%d\n",
		bus_number, slot_number, function_number);
	print_configuration_space(bus_number, slot_number, function_number);

	return 0;
}

void  __exit hello_exit(void)
{

}

MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
