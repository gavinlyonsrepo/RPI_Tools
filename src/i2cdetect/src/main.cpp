
/*!
	@file    main.cpp
	@author   Gavin Lyons
	@brief Sweep through all 7-bit I2C Address, to see if any slaves are present on the I2C bus.
	@note uses bcm2835 library
*/

/*!
 *  Print out a table that looks like this:
 I2C Bus Scan
	0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
 0 
 1       @
 2
 3             @
 4
 5
 6
 7

 E.g. if slave I2CAddress 0x12 and 0x34 were acknowledged.
 R = reserved address
 . = no device present
 @ = device present
*/

// === Includes ===
#include <stdio.h> // printf
#include <stdint.h>
#include <bcm2835.h>

// === Globals === 

const uint16_t _I2C_VERSION_NUMBER = 100; // Program version number 100 = 1.0.0
uint32_t I2Cbaudrate = 100; // I2C baudrate in kilohertz

// === Function prototypes ===
bool reservedI2CAddress(uint8_t I2CAddress);
bool Setup(void);
void PrintI2CScanTable(void);
void EndProgram(void);

// === Main ===
int main(int argc, char **argv) {
	
	if (!Setup()) return -1;
	PrintI2CScanTable();
	EndProgram();

	return 0;
}

// === Function Space ===

/*!
	@brief I2C reserves some I2CAddress for special purposes. We exclude these from the scan.
	These are any I2CAddress of the form 000 0xxx or 111 1xxx
	@return will return true if a reserved address found. 
*/
bool reservedI2CAddress(uint8_t I2CAddress)
{
	return (I2CAddress & 0x78) == 0 || (I2CAddress & 0x78) == 0x78;
}


/*!
  @brief sets up I2C bus with bcm2835 library
  @return Will return false if fails to init Bcm2835 library or start I2C
*/
bool Setup(void)
{
	printf("i2cDetectGL :: Start\n");
	
	if (!bcm2835_init())
	{
		printf("Error 1201 :: bcm2835_init failed. Are you running as root??\n");
		return false;
	}
	if (!bcm2835_i2c_begin())
	{
		printf("Error 1202 :: bcm2835_i2c_begin failed. Are you running as root??\n");
		return false;
	}
	
	printf("bcm2835 :: library Version Number :: %u\n", bcm2835_version());
	printf("i2cDetectGL :: Version Number :: %u\n", _I2C_VERSION_NUMBER);
	bcm2835_i2c_set_baudrate(I2Cbaudrate * 1000);
	return true;
}

/*!
  @brief  Print out a table of I2C bus scan
*/
void PrintI2CScanTable(void)
{
	printf("i2cDetectGL :: I2C Bus Scan\n");
	printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

	for (int I2CAddress = 0; I2CAddress < (1 << 7); ++I2CAddress)
	 {
		int returnValue;
		char rxDataBuffer[1];
		if (I2CAddress % 16 == 0) 
		{
			printf("%02x ", I2CAddress);
		}

		// Skip over any reserved I2C Address.
		if (reservedI2CAddress(I2CAddress))
		{
			returnValue = -1;
		}
		else
		{
			bcm2835_i2c_setSlaveAddress(I2CAddress);  
			// Perform a 1-byte dummy read from the probe I2C Address. If a slave
			// acknowledges this I2C Address, the function returns bcm2835 I2C reason code
			// 0x00 for success
			returnValue = bcm2835_i2c_read(rxDataBuffer, 1); // returns reason code , 0 success
		}
		switch (returnValue)
		{
			case 0: printf("@"); break;  // Device present
			case -1:printf("R"); break; // reserve address
			default:printf("."); break; // no device present
		}

		printf(I2CAddress % 16 == 15 ? "\n" : "  ");
	}
}

/*!
  @brief  Turns off I2C and closes bcm2835 library.
*/
void EndProgram(void)
{
	bcm2835_i2c_end(); //optional , switchs off I2C bus at end
	bcm2835_close(); // Close the library
	printf("i2cDetectGL :: Done.\n");
}
