// Sweep through all 7-bit I2C addresses, to see if any slaves are present on
// the I2C bus. Print out a table that looks like this:
//
// I2C Bus Scan
//   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
// 0
// 1       @
// 2
// 3             @
// 4
// 5
// 6
// 7
//
// E.g. if slave addresses 0x12 and 0x34 were acknowledged.

#include <stdio.h> // printf
#include <bcm2835.h>

bool reserved_addr(uint8_t addr);

int main(int argc, char **argv) {
	printf("Start\n");
	if (!bcm2835_init())
	{
		printf("bcm2835_init failed. Are you running as root??\n");
		return 1;
	}
	if (!bcm2835_i2c_begin())
	{
		printf("bcm2835_i2c_begin failed. Are you running as root??\n");
		return 1;
	}

	// This example will use I2C0 on the default SDA and SCL pins (GP4, GP5 on a Pico)
	bcm2835_i2c_set_baudrate(100000);

	printf("\nI2C Bus Scan\n");
	printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

	for (int addr = 0; addr < (1 << 7); ++addr)
	 {
		if (addr % 16 == 0) 
		{
			printf("%02x ", addr);
		}

		// Perform a 1-byte dummy read from the probe address. If a slave
		// acknowledges this address, the function returns the number of bytes
		// transferred. If the address byte is ignored, the function returns
		// -1.

		// Skip over any reserved addresses.
		int ret;
		char rxdata[1];
		if (reserved_addr(addr))
		{
			ret = -1;
		}
		else
		{
			bcm2835_i2c_setSlaveAddress(addr);  //i2c address
			ret = bcm2835_i2c_read(rxdata, 1); // returns reason code , 0 success
		}
		
			printf(ret != 0 ? "." : "@");
			printf(addr % 16 == 15 ? "\n" : "  ");
	}
	printf("Done.\n");
	return 0;

}


// I2C reserves some addresses for special purposes. We exclude these from the scan.
// These are any addresses of the form 000 0xxx or 111 1xxx
bool reserved_addr(uint8_t addr) {
	return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}
