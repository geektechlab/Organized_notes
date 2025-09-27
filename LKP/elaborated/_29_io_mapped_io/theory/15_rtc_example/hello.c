#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/delay.h>

#define RTC_ADDRESS     0x70
#define RTC_DATA        0x71

#define SECONDS         0x00
#define MINUTES         0x02
#define HOURS           0x04
#define DAY_WEEK        0x06
#define DAY_MONTH       0x07
#define MONTH           0x08
#define YEAR            0x09

#define REGA            0x0A
#define REGB            0x0B
#define REGC            0x0C
#define REGD            0x0D 

unsigned char rtc_read(int address)
{
	outb_p(address, RTC_ADDRESS);
	return inb_p(RTC_DATA);
}

int __init  hello_init(void)
{
	unsigned char regb = rtc_read(REGB);

	pr_info("Register B:%02x\n", regb);	

	if (regb & 0x01) {
		pr_info("Daylight saving enabled\n");
	} else {
		pr_info("Daylight saving disabled\n");
	}

	if (regb & 0x02) {
		pr_info("24 Hour Mode\n");
	} else {
		pr_info("12 Hour Mode\n");
	}

	if (regb & 0x04) {
		pr_info("Data in Binary Format\n");
	} else {
		pr_info("Data in BCD Format\n");
	}

	return 0;
}

void  __exit hello_exit(void)
{

}

MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
