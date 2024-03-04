
/*!
	@file    main.cpp
	@author   Gavin Lyons
	@brief  user-space program to scan for I2C devices
		7-bit I2C Address, to see if any slaves are present on the I2C bus.
	@note uses bcm2835 library
*/

// === Includes ===
#include <cstdio> // printf
#include <cstdint>
#include <bcm2835.h>

// === Globals ===

const uint16_t _I2C_VERSION_NUMBER = 101; // Program version number 101 = 1.0.1
uint32_t I2Cbaudrate = 100; // I2C baudrate in kilohertz

// === Function prototypes ===
bool Setup(void);
void printHelp(void);
void printVersion(void);
void EndProgram(void);
bool reservedI2CAddress(uint8_t I2CAddress);
void PrintI2CScanTable(void);
void PrintI2CScanSuccess(void);
void PrintI2CAddress(void);
uint8_t ProbeAddress(char * i2cAddress);


// === Main ===
int main(int argc, char **argv) {

	if (!Setup()) return 2;
	if (argc < 2)
	{
		printHelp();
		return 3;
	}
	
	bool probeAddress = false;
	switch (argv[1][1])
	{
		case 'v': printVersion();        break;
		case 'h': printHelp();           break;
		case 'm': PrintI2CScanTable();   break;
		case 's': PrintI2CScanSuccess(); break;
		case 'a': probeAddress = true;   break;
		default:
			printf("Error 1203 :: Unsupported option ""\"%s\"\n", argv[1]);
			printHelp();
			EndProgram();
			return 4;
		break;
	}

	if(probeAddress == true)
	{
		if (argc < 3) {
			printf("Error 1204 :: No i2c address specified!\n");
			return 5;
		}else{
			ProbeAddress(argv[2]);
		}
	}

	EndProgram();
	return 0;
}
// === End of Main ===

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
	if (!bcm2835_init())
	{
		printf("Error 1201 :: bcm2835_init failed. Are you running as root??\n");
		return false;
	}
	if (!bcm2835_i2c_begin())
	{
		printf("Error 1202 :: bcm2835_i2c_begin failed. Are you running as root??\n");
		bcm2835_close();
		return false;
	}

	bcm2835_i2c_set_baudrate(I2Cbaudrate * 1000);
	return true;
}

/*!
  @brief Scans all non-reserved I2C address on I2C bus 
		and  Print out a table of I2C bus scan results
	@details  Print out a table that looks like this:
		I2C Bus Scan
		0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
		0
		1     @
		2
		3           @
		4
		5
		6
		7

		 E.g. if slave I2CAddress 0x12 and 0x34 were acknowledged.
		 R = reserved address
		 . = no device present
		 @ = device present
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
  @brief  Scans all non-reserved I2C address on I2C bus and print out a message
  on successful I2C device found. i.e I2C Device present at 0x3C
*/
void PrintI2CScanSuccess(void)
{
	printf("i2cDetectGL :: I2C Bus Scan\n");
	bool deviceDetected = false;
	for (int I2CAddress = 0; I2CAddress < (1 << 7); ++I2CAddress)
	 {
		int returnValue;
		char rxDataBuffer[1];

		if (reservedI2CAddress(I2CAddress)) // Skip over any reserved I2C Address.
		{
			returnValue = -1;
		}
		else
		{
			bcm2835_i2c_setSlaveAddress(I2CAddress);
			returnValue = bcm2835_i2c_read(rxDataBuffer, 1); // returns reason code,0 success
		}

		switch (returnValue)
		{
			case 0:
				printf("YES I2C Device present at 0x%02X \r\n", I2CAddress);
				deviceDetected = true;
			break;
			default: break;
		}
	}
	if (deviceDetected  != true)
	{
		printf("NO I2C Device Detected on Bus \r\n");
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

/*!
  @brief Prints help message
*/
void printHelp(void)
{
	printf("Usage: i2cDetectGL [-v] [-m] [-s] [-h] \r\n");
	printf("                   [-a] I2CADDRESS \r\n");
}

/*!
  @brief Prints version
*/
void printVersion(void)
{
	printf("bcm2835     :: Version Number :: %u\n", bcm2835_version());
	printf("i2cDetectGL :: Version Number :: %u\n", _I2C_VERSION_NUMBER);
}

/*!
  @brief Probes a passed address
  @param argumentData Argument 3 from commandline A 7-bit I2C address
  excepts  string in hexadecimal format in range 0x00 to 0x7F
  eg 0x3C 0x3c 3c 3C
  eg i2cDetectGl -a 0x3C
  @return 
	-# 0 success device present
	-# 2 Invalid hexadecmial address argument
	-# 3 Reserved address 
	-# 4. No device present at given I2c address
*/
uint8_t ProbeAddress(char * argumentData)
{
	int i2cReturnValue=0;
	char rxDataBuffer[1];
	int I2CAddress =0x00;
	int scanfreturnValue=0;
	
	// Convert hexadecimal string to integer eg "0x3C" to 60
	scanfreturnValue= sscanf(argumentData, "%x" , &I2CAddress);
	if (scanfreturnValue == 0 || I2CAddress < 0x00 || I2CAddress > 0X7F)
	{
		printf("Error 1205 :: Invalid hexadecimal 7-bit I2C address :: 0x00 to 0x7F :: %s\r\n", argumentData);
		return 2;
	}
	
	// Skip over any reserved I2C Address.
	if (reservedI2CAddress(I2CAddress))
	{
		i2cReturnValue = -1;
		printf("Error 1206 :: This is a Reserved address 0x%02X \r\n", I2CAddress);
		return 3;
	}
	else
	{
		bcm2835_i2c_setSlaveAddress(I2CAddress);
		i2cReturnValue = bcm2835_i2c_read(rxDataBuffer, 1); // returns reason code , 0 success
		if (i2cReturnValue == 0 ){
			printf("YES I2C Device present at 0x%02X \r\n", I2CAddress);
			return 0;
		}
		else
		{
			printf("NO I2C Device at 0x%02X \r\n", I2CAddress);
			return 4;
		}
	}
	return 0;
}
