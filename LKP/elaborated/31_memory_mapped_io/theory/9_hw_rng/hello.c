#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioport.h>

#define INTEL_RANDOM_GENERATOR_ADDR	0xFFBC015F
#define LENGTH		3

#define INTEL_RNG_HW_STATUS                     0
#define INTEL_RNG_PRESENT               0x40
#define INTEL_RNG_ENABLED       	0x01
MODULE_LICENSE("GPL");

static inline unsigned char hwstatus_get(void __iomem *mem)
{
        return ioread8(mem + INTEL_RNG_HW_STATUS);
}

static int mod_init(void)
{
	if (!request_mem_region(INTEL_RANDOM_GENERATOR_ADDR, LENGTH, "myports")) {
		pr_info("request mem region failed for myports\n");
		return -1;
	} else {
		void __iomem *mem;
		unsigned char hw_status;

		pr_info("request mem region success for myports\n");

		mem = ioremap(INTEL_RANDOM_GENERATOR_ADDR, LENGTH);
		pr_info("ioremap returned:%px\n", mem);
		if (!mem) {
			pr_err("ioremap failed\n");
			return -1;
		}
		hw_status = hwstatus_get(mem);
		pr_info("Hardware Status:%02x\n", hw_status);	
		if ((hw_status & INTEL_RNG_PRESENT) == 0) {
			pr_warn("Random Number generator not present\n");
		} else {
			pr_info("Random Number generator present\n");
		}
		if ((hw_status & INTEL_RNG_ENABLED) == 0) {
			pr_warn("RNG is disabled\n");
		} else {
			pr_info("RNG is enabled\n");
		}
		
		iounmap(mem);	
	}
	return 0;
}

static void mod_exit(void)
{
	release_mem_region(INTEL_RANDOM_GENERATOR_ADDR, LENGTH);
}

module_init(mod_init);
module_exit(mod_exit);
