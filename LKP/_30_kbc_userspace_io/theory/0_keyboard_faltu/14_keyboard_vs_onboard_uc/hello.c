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

//ED followed by parameter bits
//bits 3-7: Must be zero.

//bit 2: Capslock LED (1 = on, 0 = off).

//bit 1: Numlock LED (1 = on, 0 = off).

//bit 0: Scroll lock LED (1 = on, 0 = off).

static void set_kbd_led_on(void)
{
	//wait till the input buffer is empty
        while ((i8042_read_status() & 0x02));

	//send 0xED as Data
	i8042_write_data(0xED);

	//wait till the input buffer is empty
        while ((i8042_read_status() & 0x02));

	//0x07 for all leds
	i8042_write_data(0x07);

	


}

static void set_kbd_led_off(void)
{
	//wait till the input buffer is empty
        while ((i8042_read_status() & 0x02));

	//send 0xED as Data
	i8042_write_data(0xED);

	//wait till the input buffer is empty
        while ((i8042_read_status() & 0x02));

	//0x00 for all leds
	i8042_write_data(0x00);


}


int __init  hello_init(void)
{
	set_kbd_led_on();
	ssleep(5);
	set_kbd_led_off();
	return 0;
}

void  __exit hello_exit(void)
{

}

MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
