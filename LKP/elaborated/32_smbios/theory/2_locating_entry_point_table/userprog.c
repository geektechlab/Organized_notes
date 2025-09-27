#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/** SMBIOS entry point scan region start address */
#define SMBIOS_ENTRY_START 0xf0000

/** SMBIOS entry point scan region length */
#define SMBIOS_ENTRY_LEN 0x10000

void find_smbios_entry(u_int8_t *mem_mapped)
{
	size_t fp;

	/* Try to find SMBIOS */
	for(fp=0; fp<=0xFFF0; fp+=16)
	{
		if(memcmp(mem_mapped+fp, "_SM_", 4)==0 && fp<=0xFFE0)
		{
			printf("smbios found at offset:%02x\n", SMBIOS_ENTRY_START + fp);
		}

	}
}

int main(int argc, char *argv[])
{
	int fd;
	u_int8_t *entry_mem;

	fd = open("/dev/mem", O_RDWR);

	if (fd < 0) {
		perror("open failed:");
		return -1;
	}

	/* Map the region potentially containing the SMBIOS entry point */
	entry_mem = mmap(NULL, SMBIOS_ENTRY_LEN, PROT_READ, MAP_SHARED, 
				fd, SMBIOS_ENTRY_START );	

	if (entry_mem == MAP_FAILED) {
		perror("mmap failed\n");
		close(fd);
		return -2;
	}

	/* Scan for the SMBIOS entry point */
        find_smbios_entry(entry_mem);	

	/* Unmap the entry point region (no longer required) */
	munmap(entry_mem, SMBIOS_ENTRY_LEN);

	close(fd);
	return 0;
}
