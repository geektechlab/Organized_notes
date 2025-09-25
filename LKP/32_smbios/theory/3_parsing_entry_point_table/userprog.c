#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * SMBIOS entry point
 *
 * This is the single table which describes the list of SMBIOS
 * structures.  It is located by scanning through the BIOS segment.
 */
struct smbios_entry {
	/** Signature
	 *
	 * Must be equal to SMBIOS_SIGNATURE
	 */
	u_int32_t signature;
	/** Checksum */
	u_int8_t checksum;
	/** Length */
	u_int8_t len;
	/** Major version */
	u_int8_t major;
	/** Minor version */
	u_int8_t minor;
	/** Maximum structure size */
	u_int16_t max;
	/** Entry point revision */
	u_int8_t revision;
	/** Formatted area */
	u_int8_t formatted[5];
	/** DMI Signature */
	u_int8_t dmi_signature[5];
	/** DMI checksum */
	u_int8_t dmi_checksum;
	/** Structure table length */
	u_int16_t smbios_len;
	/** Structure table address */
	u_int32_t smbios_address;
	/** Number of SMBIOS structures */
	u_int16_t smbios_count;
	/** BCD revision */
	u_int8_t bcd_revision;
} __attribute__ (( packed ));

/** SMBIOS entry point scan region start address */
#define SMBIOS_ENTRY_START 0xf0000

/** SMBIOS entry point scan region length */
#define SMBIOS_ENTRY_LEN 0x10000

void print_smbios_table(u_int8_t *addr)
{
	struct smbios_entry *entry = (struct smbios_entry *)addr;
	printf("smbios address:%02x\n", entry->smbios_address);
}

void find_smbios_entry(u_int8_t *mem_mapped)
{
	size_t fp;

	/* Try to find SMBIOS */
	for(fp=0; fp<=0xFFF0; fp+=16)
	{
		if(memcmp(mem_mapped+fp, "_SM_", 4)==0 && fp<=0xFFE0)
		{
			printf("smbios found at offset:%02x\n", SMBIOS_ENTRY_START + fp);
			print_smbios_table(mem_mapped+fp);
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
