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

int __init  hello_init(void)
{
	unsigned short vendor_id;
	unsigned short device_id;

	vendor_id = read_pci_config_short(3, 1, 0, 0);
	device_id = read_pci_config_short(3, 1, 0, 2);
	pr_info("Vendor id:%02x\t Device Id:%02x\n", vendor_id, device_id);

	return 0;
}

void  __exit hello_exit(void)
{

}

MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
