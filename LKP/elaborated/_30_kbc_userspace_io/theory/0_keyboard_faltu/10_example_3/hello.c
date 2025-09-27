#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/delay.h>

#define I8042_COMMAND_REG       0x64
#define I8042_STATUS_REG        0x64
#define I8042_DATA_REG          0x60

static inline int i8042_read_data(void)
{
        return inb(I8042_DATA_REG);
}

static inline int i8042_read_status(void)
{
        return inb(I8042_STATUS_REG);
}

static inline void i8042_write_data(int val)
{
        outb(val, I8042_DATA_REG);
}

static inline void i8042_write_command(int val)
{
        outb(val, I8042_COMMAND_REG);
}

void print_cmd_byte(unsigned char cmd_byte)
{
        if (cmd_byte & 0x01)
                pr_info("keyboard interrupt enabled\n");
        else
                pr_info("keyboard interrupt disabled\n");

        if (cmd_byte & 0x02)
                pr_info("Mouse interrupt enabled\n");
        else
                pr_info("Mouse interrupt disabled\n");

        if (cmd_byte & 0x04)
                pr_info("System Flag self test passed\n");
        else
                pr_info("System Flag self test failed\n");

        if (cmd_byte & 0x80)
                pr_info("Inhibit switch enabled\n");
        else
                pr_info("Inhibit switch disabled\n");

	        if (cmd_byte & 0x10)
                pr_info("Keyboard interface disabled\n");
        else
                pr_info("All keyboard communication is enabled\n");

        if (cmd_byte & 0x20)
                pr_info("Auxillary PS/2 device interface disabled\n");
        else
                pr_info("Auxillary PS/2 device interface enabled\n");
}

int __init  hello_init(void)
{
	unsigned char cmd_byte;
	//0xAD (Disable keyboard interface) - Sets bit 4 of command byte and disables all communication with keyboard

	i8042_write_command(0xAD);

        //read command byte
        i8042_write_command(0x20);
        cmd_byte = i8042_read_data();

        pr_info("cmd_byte:%02x\n", cmd_byte);
        print_cmd_byte(cmd_byte);

	ssleep(5);

	//0xAE (Enable keyboard interface) - Clears bit 4 of command byte and re-enables communication with keyboard
	i8042_write_command(0xAE);
	i8042_write_command(0x20);
        cmd_byte = i8042_read_data();
        print_cmd_byte(cmd_byte);

	return 0;
}

void  __exit hello_exit(void)
{

}

MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
